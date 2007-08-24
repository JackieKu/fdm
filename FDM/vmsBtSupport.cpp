/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
#include "vmsBtSupport.h"
#include "mfchelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsBtSupport::vmsBtSupport()
{
	m_pbDHTstate = NULL;
	InitializeCriticalSection (&m_cs1);
}

vmsBtSupport::~vmsBtSupport()
{
	DeleteCriticalSection (&m_cs1);
	Shutdown ();
}

BOOL vmsBtSupport::Initialize()
{
	if (false == LoadBtDll (m_dllBt))
		return FALSE;
	return LoadState ();
}

vmsBtSession* vmsBtSupport::get_Session()
{
	EnterCriticalSection (&m_cs1);

	if (m_dllBt.is_Loaded () == false)
		Initialize ();

	typedef vmsBtSession* (WINAPI *FNS)();
	static FNS _pfnSession = NULL;
	
	if (_pfnSession == NULL)
	{
		_pfnSession = (FNS) m_dllBt.GetProcAddress ("vmsBt_getSession");
		if (_pfnSession)
		{
			_pfnSession ()->SetUserAgent (PRG_AGENT_NAME);
			ApplyListenPortSettings ();
			ApplyDHTSettings ();
			ApplyRestrainAllDownloadsMode ();
			ApplyProxySettings ();
			_DldsMgr.AttachToBtSession ();
			_DldsMgr.ApplyTrafficLimit ();
		}
	}

	LeaveCriticalSection (&m_cs1);

	if (_pfnSession)
		return _pfnSession ();
	
	return NULL;
}

BOOL vmsBtSupport::is_Initialized()
{
	return m_dllBt.is_Loaded ();
}

void vmsBtSupport::ApplyRestrainAllDownloadsMode()
{
	if (is_Initialized ())
	{
		vmsBtSession *pBtSession = get_Session ();
		
		int limit = fsInternetDownloader::is_PauseMode () ? 1 : -1;
		pBtSession->SetDownloadLimit (limit);
		pBtSession->SetUploadLimit (
			_App.Bittorrent_UploadTrafficLimit (_DldsMgr.GetTUM ()));
	}
}

void vmsBtSupport::ApplyListenPortSettings()
{
	if (is_Initialized ())
	{
		vmsBtSession *pBtSession = get_Session ();

		int portFrom = _App.Bittorrent_ListenPort_From (),
			portTo = _App.Bittorrent_ListenPort_To ();

		if (pBtSession->IsListening () == FALSE ||
				pBtSession->get_ListenPort () > portTo ||
				pBtSession->get_ListenPort () < portFrom)
			pBtSession->ListenOn (portFrom, portTo);
	}
}

void vmsBtSupport::ApplyDHTSettings()
{
	if (is_Initialized () == FALSE)
		return;

	vmsBtSession *pBtSession = get_Session ();

	if (_App.Bittorrent_EnableDHT ())
	{
		if (pBtSession->DHT_isStarted () == FALSE)
			pBtSession->DHT_start (m_pbDHTstate, m_dwDHTstateSize);
	}
	else
	{
		if (pBtSession->DHT_isStarted ())
			pBtSession->DHT_stop ();
	}
}

BOOL vmsBtSupport::SaveState()
{
	if (is_Initialized () == FALSE)
		return TRUE;

	vmsBtSession *pBtSession = get_Session ();

	if (pBtSession->DHT_isStarted ())
	{
		SAFE_DELETE_ARRAY (m_pbDHTstate);
	
		if (FALSE == pBtSession->DHT_getState (NULL, 0, &m_dwDHTstateSize))
			return FALSE;

		m_pbDHTstate = new BYTE [m_dwDHTstateSize];

		if (FALSE == pBtSession->DHT_getState (m_pbDHTstate, m_dwDHTstateSize, &m_dwDHTstateSize))
			return FALSE;
	}

	if (m_pbDHTstate == NULL)
		return TRUE;

	HANDLE hFile = CreateFile (fsGetDataFilePath ("btdht.sav"), GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw;
	WriteFile (hFile, m_pbDHTstate, m_dwDHTstateSize, &dw, NULL);

	CloseHandle (hFile);

	return TRUE;
}

BOOL vmsBtSupport::LoadState()
{
	if (GetFileAttributes (fsGetDataFilePath ("btdht.sav")) == DWORD (-1))
		return TRUE;

	HANDLE hFile = CreateFile (fsGetDataFilePath ("btdht.sav"), GENERIC_READ,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	SAFE_DELETE_ARRAY (m_pbDHTstate);
	m_dwDHTstateSize = GetFileSize (hFile, NULL);
	m_pbDHTstate = new BYTE [m_dwDHTstateSize];

	DWORD dw;
	ReadFile (hFile, m_pbDHTstate, m_dwDHTstateSize, &dw, NULL);

	CloseHandle (hFile);

	return TRUE;
}

void vmsBtSupport::Shutdown()
{
	if (is_Initialized () == FALSE)
		return;

	typedef void (WINAPI *FNS)();
	FNS pfn = (FNS) m_dllBt.GetProcAddress ("vmsBt_Shutdown");
	if (pfn)
		pfn ();

	m_dllBt.Free ();
}

void vmsBtSupport::ApplyProxySettings()
{
	if (is_Initialized () == FALSE)
		return;

	fsString strIp, strUser, strPwd;
	int nPort = 0;

	switch (_App.InternetAccessType ())
	{
	case IATE_PRECONFIGPROXY:
		GetIeProxySettings (strIp, strUser, strPwd, nPort);
		break;

	case IATE_NOPROXY:
		break;

	case IATE_MANUALPROXY:
		strIp = _App.HttpProxy_Name ();
		strUser = _App.HttpProxy_UserName ();
		strPwd = _App.HttpProxy_UserName ();
		if (strIp.IsEmpty () == FALSE)
		{
			char sz [1000];
			strcpy (sz, strIp);
			LPSTR pszPort = strrchr (sz, ':');
			if (pszPort)
			{
				*pszPort++ = 0;
				nPort = atoi (pszPort);
				strIp = sz;
			}
		}
		break;

	case IATE_FIREFOXPROXY:
		GetFirefoxProxySettings (strIp, strUser, strPwd, nPort);
		break;
	}

	get_Session ()->SetProxySettings (strIp, nPort, strUser, strPwd);
}

void vmsBtSupport::GetIeProxySettings(fsString &strIp, fsString &strUser, fsString &strPwd, int &nPort)
{
	strIp = strUser = strPwd = ""; 
	nPort = 0;

	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
			"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 
			KEY_READ))
		return;

	DWORD dw;
	if (ERROR_SUCCESS != key.QueryValue (dw, "ProxyEnable"))
		return;
	if (dw == FALSE)
		return;

	char szProxy [1000];
	dw = sizeof (szProxy);
	if (ERROR_SUCCESS != key.QueryValue (szProxy, "ProxyServer", &dw))
		return;

	LPSTR pszPort = strrchr (szProxy, ':');
	if (pszPort)
	{
		*pszPort = 0;
		pszPort++;
		nPort = atoi (pszPort);
	}

	strIp = szProxy;
}

void vmsBtSupport::GetFirefoxProxySettings(fsString &strIp, fsString &strUser, fsString &strPwd, int &nPort)
{
	strIp = strUser = strPwd = ""; 
	nPort = 0;

	if (1 != _App.FirefoxSettings_Proxy_Type ())
		return;

	strIp = _App.FirefoxSettings_Proxy_Addr ("http");
	nPort = _App.FirefoxSettings_Proxy_Port ("http");
}

vmsBtFile* vmsBtSupport::CreateTorrentFileObject()
{
	if (m_dllBtFile.is_Loaded () == false && 
			false == LoadBtDll (m_dllBtFile))
		return NULL;
	
	typedef vmsBtFile* (WINAPI *FNBTF)();
	static FNBTF _pfnCreateTorrentFileObject = NULL;
	
	if (_pfnCreateTorrentFileObject == NULL)
	{
		_pfnCreateTorrentFileObject = (FNBTF) m_dllBtFile.GetProcAddress ("vmsBt_CreateTorrentFileObject");
		if (_pfnCreateTorrentFileObject == NULL)
			return NULL;
	}

	return _pfnCreateTorrentFileObject ();
}

bool vmsBtSupport::LoadBtDll(vmsDLL &dll)
{
	CString str = ((CDataStretcherApp*)AfxGetApp ())->m_strAppPath;
	if (str [str.GetLength () - 1] != '\\')
		str += '\\';
	str += "fdmbtsupp.dll";
	return dll.Load (str);
}

