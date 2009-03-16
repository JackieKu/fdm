/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"

#include "FdmApp.h"
#include "dbghelp.h"

#include "MainFrm.h"
#include "UrlWnd.h"
#include <initguid.h>
#include "WGUrlReceiver.h"
#include "Fdm_i.c"
#include "UEDlg.h"
#include "fsIEUserAgent.h"
#include "WgUrlListReceiver.h"
#include "CFDM.h"
#include "fsFDMCmdLineParser.h"
#include "mfchelp.h"
#include "vmsFilesToDelete.h"
#include "vmsDLL.h"
#include "vmsFirefoxMonitoring.h"
#include "FDMDownloadsStat.h"
#include "FDMDownload.h"
#include "FDMUploader.h"
#include "FDMUploadPackage.h"
#include "vmsTorrentExtension.h"
#include "FdmTorrentFilesRcvr.h"
#include "FDMFlashVideoDownloads.h"
#include "vmsUploadsDllCaller.h"
#include "vistafx/vistafx.h"
#include "inetutil.h"
#include "vmsNotEverywhereSupportedFunctions.h"
#include "SpiderWnd.h"
extern CSpiderWnd *_pwndSpider;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

BEGIN_MESSAGE_MAP(CFdmApp, CWinApp)
	//{{AFX_MSG_MAP(CFdmApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

CFdmApp::CFdmApp()
{
	m_bCOMInited = m_bATLInited = m_bATLInited2 = FALSE;
	m_bSaveAllOnExit = FALSE;
	m_hAppMutex = NULL;
	m_bEmbedding = FALSE;
	m_bStarting = TRUE;

	SYSTEMTIME time;
	GetLocalTime (&time);
	SystemTimeToFileTime (&time, &_timeAppHasStarted);

	m_pModuleState;
}      

CFdmApp theApp;    

BOOL CFdmApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	SetUnhandledExceptionFilter (_UEF);

	AfxEnableControlContainer ();

	
	SetRegistryKey (IDS_COMPANY);

	CheckRegistry ();

	m_bSaveAllOnExit = FALSE;

	fsIECatchMgr::CleanIEPluginKey ();

	
	CString strPath = GetProfileString (_T(""), _T("Path"), _T(""));
	BOOL bNeedLocalRegister = FALSE;
	if (strPath == _T(""))
	{
		CRegKey key;
		if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), KEY_WRITE))
			vmsSHCopyKey (HKEY_LOCAL_MACHINE, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), key);
		strPath = GetProfileString (_T(""), _T("Path"), _T(""));
		bNeedLocalRegister = strPath != _T("");
	}

	if (GetFileAttributes (strPath + _T("\\fdm.exe")) == DWORD (-1))
	{
		strPath = _T("");
		bNeedLocalRegister = false;
	}

	
	
	if (strPath == _T("") || !SetCurrentDirectory (strPath))
		_dwAppState |= APPSTATE_PORTABLE_MODE;

	TCHAR szExeDir [MY_MAX_PATH], szExeFile [MY_MAX_PATH];
	GetModuleFileName (NULL, szExeFile, sizeof (szExeFile));
	fsGetPath (szExeFile, szExeDir);

	if (IS_PORTABLE_MODE)
	{
		strPath = szExeDir;
		SetCurrentDirectory (strPath);
	}

	m_strAppPath = strPath;
	if (!m_strAppPath.IsEmpty ())
	{
		if (m_strAppPath [m_strAppPath.GetLength ()-1] != _T('\\') &&
				m_strAppPath [m_strAppPath.GetLength ()-1] != _T('/'))
			m_strAppPath += _T('\\');
	}

	if (!IS_PORTABLE_MODE)
	{
		CString strDataFldr = szExeDir; strDataFldr += _T("Data");
		
		
		
		if (m_strAppPath.CompareNoCase (szExeDir) &&
			 DWORD (-1) != GetFileAttributes (strDataFldr))
		{
			
			_dwAppState |= APPSTATE_PORTABLE_MODE;
			_dwAppState |= APPSTATE_PORTABLE_MODE_NOREG;
			m_strAppPath = szExeDir;
		}
	}

	if (IS_PORTABLE_MODE)
	{
		
		vmsAppSettingsStore* pStgs = _App.get_SettingsStore ();
		CString strStgsFile = m_strAppPath + _T("Data\\settings.dat");
		fsBuildPathToFile (strStgsFile);
		pStgs->LoadSettingsFromFile (strStgsFile);
	}

	BOOL bNoLng = FALSE;

	if (FALSE == InitLanguage ())
		bNoLng = TRUE;

	if (strcmp (m_lpCmdLine, _T("-suis")) == 0 || 
			_tcscmp (m_lpCmdLine, _T("-euis")) == 0 ||
			_tcscmp (m_lpCmdLine, _T("-duis")) == 0)
	{
		IntegrationSettings ();
		return FALSE;
	}

	if (IS_PORTABLE_MODE)
	{
		
		
		
		TCHAR szTmpFile [MY_MAX_PATH];
		CString str = m_strAppPath; str += _T("Data");
		CreateDirectory (str, NULL);
		if (0 == GetTempFileName (str, _T("fdm"), 0, szTmpFile))
			MessageBox (NULL, LS (L_NOWRITEACCESSTODATAFOLDER), vmsFdmAppMgr::getAppName (), MB_ICONWARNING);
		else
			DeleteFile (szTmpFile);
	}

	_SkinMgr.Initialize ();

	_IECatchMgr.ReadSettings ();
	_NOMgr.Initialize ();
	_IECMM.ReadState ();

	
	HRESULT hRes = OleInitialize (NULL);
	
	if (FAILED(hRes))
		return FALSE;
	
	m_bCOMInited = TRUE;

	fsFDMCmdLineParser cmdline;
	cmdline.Parse ();
	m_bForceSilentSpecified = cmdline.is_ForceSilentSpecified ();

	
	if (GetVersion () & 0x80000000)
	{
		
		CFileFind f; 
		BOOL b = f.FindFile (m_strAppPath + _T("*.sav"));
		std::vector <CString> v;
		while (b)
		{
			b = f.FindNextFile ();
			v.push_back (f.GetFileName ());
		}
		for (size_t i = 0; i < v.size (); i++)
			MoveFile (m_strAppPath + v [i], fsGetDataFilePath (v [i]));
	}
	MoveFile (fsGetDataFilePath (_T("uploads.sav")), fsGetDataFilePath (_T("uploads.1.sav")));
	

	if (!InitATL())
		return FALSE;

	if (VistaFx::IsVista () && _tcsncmp (m_lpCmdLine, _T("-nelvcheck"), 10) && stricmp (m_lpCmdLine, _T("-autorun")))
	{
		VistaFx vf;
		vf.LoadDll (m_strAppPath + _T("vistafx.dll"));
		if (vf.IsProcessElevated ())
		{
			TCHAR sz [MAX_PATH];
			GetModuleFileName (NULL, sz, MAX_PATH);
			CString str = _T("-nelvcheck ");
			str += m_lpCmdLine;
			if (m_hAppMutex)
			{
				CloseHandle (m_hAppMutex);
				m_hAppMutex = NULL;
			}
			if (vf.RunNonElevatedProcess (sz, str, _T("")))
				return FALSE;
			m_hAppMutex = CreateMutex (NULL, TRUE, _pszAppMutex);
		}
	}

	
	
	if (IS_PORTABLE_MODE && (_dwAppState & APPSTATE_PORTABLE_MODE_NOREG) == 0)
		Install_RegisterServer ();

	
	
	
	
	vmsFilesToDelete::Process ();

	if (bNeedLocalRegister)
		RegisterServer (FALSE);

// Enable3dControls() is DEPRECATED
#if _MSC_VER < 1300
#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif
#endif

	CheckLocked ();

	m_bSaveAllOnExit = TRUE;

	_UpdateMgr.ReadSettings ();
	
	if (_UpdateMgr.IsStartUpdaterNeeded ())
	{
		if (_UpdateMgr.StartUpdater ())	
			return FALSE;	
		else
			::MessageBox (NULL, LS (L_CANTFINDUPDATER), LS (L_ERR), MB_ICONERROR);
	}

	LoadHistory ();

	_Snds.ReadSettings ();

	if (_App.ModifyIEUserAgent ())
	{
		fsIEUserAgent ua;
		ua.SetPP (IE_USERAGENT_ADDITION);
		ua.RemovePP (_T("Free Download Manager")); 
	}

	if (FALSE == GetProfileInt (_T("EnvCheck"), _T("848"), FALSE))
	{
		if (vmsBtSupport::getBtDllVersion () < 848)
			MessageBox (NULL, _T("Please update Bittorrent module.\nSome functions will be disabled until you update it."), _T("Warning"), MB_ICONEXCLAMATION);
		WriteProfileInt (_T("EnvCheck"), _T("848"), TRUE);
	}

	CMainFrame* pFrame = NULL;
	fsnew1 (pFrame, CMainFrame);
	m_pMainWnd = pFrame;

	
	if (FALSE == pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		NULL, NULL))
	{
		return FALSE;
	}

	
	BOOL bHidden = _tcscmp (m_lpCmdLine, _T ("-autorun")) == 0;

	_App.View_ReadWndPlacement (pFrame, _T("MainFrm"), bHidden);

	if (!bHidden)
	{
		if (_App.Prg_StartMinimized ())
		{
			if (IsWindowVisible (pFrame->m_hWnd))
				pFrame->ShowWindow (SW_MINIMIZE);
		}
		else
		{
			pFrame->UpdateWindow();
			if (pFrame->IsWindowVisible ())
				pFrame->SetForegroundWindow ();
		}
	}

	m_bStarting = FALSE;

	
	hRes = _Module.RegisterClassObjects (CLSCTX_LOCAL_SERVER, 
				REGCLS_MULTIPLEUSE);
	if (FAILED (hRes))
	{
		LPVOID lpMsgBuf;
		FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hRes,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			
			
			
			MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
			
			LocalFree( lpMsgBuf );
	}
	m_bATLInited2 = SUCCEEDED (hRes);

	return TRUE;
}            

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();  

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CUrlWnd	m_wndDLURL;
	CUrlWnd	m_wndFirm;
	//}}AFX_DATA

	
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL  

protected:
	CFont m_fntUnderline;
	CFont m_fontRegInfo;
	CFont m_fontWarn;
	CUrlWnd m_wndSupport;
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_fontRegInfo.CreateFont (10, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("MS Sans Serif"));
	m_fontWarn.CreateFont (12, 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Arial"));
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()  

void CFdmApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
   	_DlgMgr.OnDoModal (&aboutDlg);
	aboutDlg.DoModal();
    _DlgMgr.OnEndDialog (&aboutDlg);
}        

int CFdmApp::ExitInstance() 
{
	if (m_bATLInited2)
		_Module.RevokeClassObjects();

	if (IS_PORTABLE_MODE && (_dwAppState & APPSTATE_PORTABLE_MODE_NOREG) == 0)
		Install_UnregisterServer ();
	
	if (m_bATLInited)
		_Module.Term();

	
	if (m_bSaveAllOnExit)
	{
		SaveSettings ();
		SaveHistory ();
		_UpdateMgr.SaveSettings ();
	}

	

	
	
	return CWinApp::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CFont* fnt = GetFont ();
	LOGFONT lf;
	fnt->GetLogFont (&lf);
	lf.lfUnderline = TRUE;
	m_fntUnderline.CreateFontIndirect (&lf);
	
	m_wndDLURL.SubclassDlgItem (IDC__DOWNLOAD, this);
	m_wndDLURL.Init ();
	m_wndDLURL.SetUrl ("http://www.freedownloadmanager.org/download.htm");
	
	CString strVer;
	TCHAR szVer [] = _T("%s %s build %s");
	strVer.Format (szVer, LS (L_VERSION), vmsFdmAppMgr::getVersion ()->m_strProductVersion.c_str (), 
		vmsFdmAppMgr::getBuildNumber ());
	SetDlgItemText (IDC__VER, strVer);

	

	
	

	m_wndFirm.SubclassDlgItem (IDC__FIRM, this);
	m_wndFirm.Init ();
	m_wndFirm.SetUrl ("http://www.freedownloadmanager.org");

	SetDlgItemText (IDC__SUPPORT, LS (L_SUPPORTANDOTHER));
	m_wndSupport.SubclassDlgItem (IDC__SUPPORT, this);
	m_wndSupport.Init ();
	m_wndSupport.SetUrl ("http://www.freedownloadmanager.org/support.htm");

	SetDlgItemText (IDC__DOWNLOAD, LS (L_DLLATESTVERSION));

	SetDlgItemText (IDC__WARN, LS (L_LICWARN));

	SetWindowText (LS (L_ABOUT2));

	if (_AppMgr.IsBtInstalled () == FALSE || _AppMgr.IsMediaFeaturesInstalled () == FALSE)
	{
		CString str;
		GetDlgItemText (IDC__NAME, str);
		str += _T(" (Light)");
		SetDlgItemText (IDC__NAME, str);
	}

	return TRUE;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->m_hWnd == m_wndFirm.m_hWnd ||
		 pWnd->m_hWnd == m_wndDLURL.m_hWnd || 
		 pWnd->m_hWnd == m_wndSupport.m_hWnd)
	{
		pDC->SetTextColor (GetSysColor (26));
        pDC->SelectObject (&m_fntUnderline);
	}

	if (pWnd->m_hWnd == GetDlgItem (IDC__WARN)->m_hWnd)
		pDC->SelectObject (&m_fontWarn);
		
	return hbr;
}

void CFdmApp::LoadHistory()
{
	_HsMgr.ReadSettings ();

	
	HANDLE hFile = CreateFile (fsGetDataFilePath (_T("history.sav")), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (GetLastError () != ERROR_FILE_NOT_FOUND)
			goto _lErr;
		return;
	}

	
	if (!_LastUrlFiles.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastUrlPaths.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastFolders.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastBatchUrls.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastFind.ReadFromFile (hFile))
		goto _lErr;

	if (!_LastFlashVideoUrls.ReadFromFile (hFile))
		goto _lErr;

	CloseHandle (hFile);

	return;

_lErr:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);
	
}

void CFdmApp::SaveHistory()
{
	HANDLE hFile = CreateFile (fsGetDataFilePath (_T("history.sav")), GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto _lErr;
	}

	if (!_LastUrlFiles.SaveToFile (hFile))
		goto _lErr;

	if (!_LastUrlPaths.SaveToFile (hFile))
		goto _lErr;

	if (!_LastFolders.SaveToFile (hFile))
		goto _lErr;

	if (!_LastBatchUrls.SaveToFile (hFile))
		goto _lErr;

	if (!_LastFind.SaveToFile (hFile))
		goto _lErr;

	if (!_LastFlashVideoUrls.SaveToFile (hFile))
		goto _lErr;

	CloseHandle (hFile);

	return;

_lErr:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);
	
		DeleteFile (fsGetDataFilePath (_T("history.sav")));
}  

	
CFdmModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_WGUrlReceiver, CWGUrlReceiver)
OBJECT_ENTRY(CLSID_WGUrlListReceiver, CWgUrlListReceiver)
OBJECT_ENTRY(CLSID_FDM, CFDM)
OBJECT_ENTRY(CLSID_FDMDownloadsStat, CFDMDownloadsStat)
OBJECT_ENTRY(CLSID_FDMDownload, CFDMDownload)
OBJECT_ENTRY(CLSID_FDMUploader, CFDMUploader)
OBJECT_ENTRY(CLSID_FDMUploadPackage, CFDMUploadPackage)
OBJECT_ENTRY(CLSID_FdmTorrentFilesRcvr, CFdmTorrentFilesRcvr)
OBJECT_ENTRY(CLSID_FDMFlashVideoDownloads, CFDMFlashVideoDownloads)
END_OBJECT_MAP()

LONG CFdmModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CFdmModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CFdmModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

BOOL CFdmApp::InitATL()
{
	m_bEmbedding = FALSE;

	LPTSTR lpCmdLine = GetCommandLine(); 
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);

	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("Embedding"))==0)
		{
			
		}

		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			BOOL bWaitShutdown = FALSE;
			if (CheckFdmStartedAlready (FALSE))
			{
				UINT nMsg = RegisterWindowMessage (_T("FDM - shutdown"));
				if (nMsg)
				{
					PostMessage (HWND_BROADCAST, nMsg, 0, 0);
					bWaitShutdown = TRUE;
				}
				else
				{
					MessageBox (NULL, _T("Unable to shutdown Free Download Manager.\nPlease do that yourself."), _T("Error"), MB_ICONEXCLAMATION);
				}
			}

			HANDLE hMxWi = CreateMutex (NULL, FALSE, _T("FDM - remote control server"));
			if (GetLastError () == ERROR_ALREADY_EXISTS)
			{
				UINT nMsg = RegisterWindowMessage (_T("FDM - remote control server - shutdown"));
				if (nMsg)
					PostMessage (HWND_BROADCAST, nMsg, 0, 0);
			}
			if (hMxWi)
				CloseHandle (hMxWi);

			if (bWaitShutdown)
			{
				while (CheckFdmStartedAlready (FALSE))
				{
					Sleep (400);
				}
			}

			Install_UnregisterServer ();
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			Install_RegisterServer ();
			bRun = FALSE;
			break;
		}

		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (bRun && CheckFdmStartedAlready (m_bForceSilentSpecified == FALSE))
		bRun = FALSE;

	if (!bRun)
		return FALSE;

	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init (ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId ();
	}

	return TRUE;
}

BOOL CFdmApp::InitLanguage()
{
	m_nNoLngsErrReason = 0;
	
	
	if (FALSE == _LngMgr.Initialize ())
	{
		m_nNoLngsErrReason = 1;
		return FALSE;
	}

	
	int iLang = _LngMgr.FindLngByName (_App.View_Language ());
	if (iLang == -1) 
		iLang = 0;

	

	if (FALSE == _LngMgr.LoadLng (iLang))
	{
		
		if (iLang == 0 || FALSE == _LngMgr.LoadLng (0))
		{
			m_nNoLngsErrReason = 2;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFdmApp::CheckFdmStartedAlready(BOOL bSetForIfEx)
{
	LPCTSTR pszMainWnd = _T("Free Download Manager Main Window");

	m_hAppMutex = CreateMutex (NULL, TRUE, _pszAppMutex);
	
	
	if (GetLastError () == ERROR_ALREADY_EXISTS)
	{
		CloseHandle (m_hAppMutex);
		m_hAppMutex = NULL;

		if (bSetForIfEx)
		{
			

			HWND hWnd = FindWindow (pszMainWnd, NULL);

			if (IsIconic (hWnd))
				ShowWindow (hWnd, SW_RESTORE);
			else
			{
				WINDOWPLACEMENT wc;
				GetWindowPlacement (hWnd, &wc);
				if (wc.showCmd == SW_HIDE)
					ShowWindow (hWnd, SW_RESTORE);
			}

			SetForegroundWindow (hWnd);
			SetFocus (hWnd);
		}
		
		return TRUE;
	}

	return FALSE;
}  

CFdmApp::~CFdmApp()
{
	if (m_bCOMInited)
		CoUninitialize();
}

LONG CFdmApp::_UEF(_EXCEPTION_POINTERS *info)
{
	static BOOL _b = FALSE;

	if (_b)
		return EXCEPTION_EXECUTE_HANDLER;

	_b = TRUE;

	typedef BOOL (WINAPI *FNMDWD)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, 
		PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
	vmsDLL dll (_T("dbghelp.dll"));
	FNMDWD pfnMiniDumpWriteDump;
	pfnMiniDumpWriteDump = (FNMDWD) dll.GetProcAddress (_T("MiniDumpWriteDump"));
	if (pfnMiniDumpWriteDump == NULL)
		return EXCEPTION_EXECUTE_HANDLER;
	
	
	
	MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = info;
    eInfo.ClientPointers = FALSE;

	TCHAR szFile [MAX_PATH]; TCHAR szName [100];
	wsprintf (szName, _T("fdm%s.dmp"), vmsFdmAppMgr::getBuildNumber ());
	GetTempPath (MAX_PATH, szFile);
	_tcscat (szFile, szName);
	HANDLE hFile = CreateFile (szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		0, NULL);

    
    pfnMiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpNormal,
        &eInfo,
        NULL,
        NULL);

	CloseHandle (hFile);

	CUEDlg dlg;
	dlg.DoModal ();

	return EXCEPTION_EXECUTE_HANDLER;
}

#include "FDM.h"
#include "FDMDownloadsStat.h"
#include "FDMDownload.h"
#include "FDMUploader.h"
#include "FDMUploadPackage.h"

BOOL CFdmApp::Is_Starting()
{
	return m_bStarting;
}

void CFdmApp::CheckLocked()
{
	DWORD dwRes;

	do 
	{
		HANDLE hMx = CreateMutex (NULL, TRUE, _T("_mx_FDM_Lock_Start_"));
		dwRes = GetLastError ();
		CloseHandle (hMx);

		if (dwRes == ERROR_ALREADY_EXISTS)
			Sleep (100);
	}
	while (dwRes == ERROR_ALREADY_EXISTS);
}

void CFdmApp::UninstallCustomizations()
{
	CRegKey key;
	key.Open (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"));

	TCHAR sz [MY_MAX_PATH];
	DWORD dw = MY_MAX_PATH;
	
	
	key.QueryValue (sz, _T("CLSM_File"), &dw);
	DeleteFile (sz);

	
	dw = MY_MAX_PATH;
	key.QueryValue (sz, _T("CLFM_File"), &dw);
	DeleteFile (sz);

	

	

	

	TCHAR szIEOP [10000] = _T("about:blank");
	TCHAR szIECP [10000] = _T("about:blank");

	CRegKey fdmkey;
	fdmkey.Open (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"));

	dw = sizeof (szIECP);
	fdmkey.QueryValue (szIECP, _T("CustSite"), &dw);
	dw = sizeof (szIEOP);
	if (ERROR_SUCCESS == fdmkey.QueryValue (szIEOP, _T("CIEOP"), &dw))
	{
		key.Open (HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"));

		TCHAR sz2 [10000] = _T("about:blank");
		dw = sizeof (sz2);
		key.QueryValue (sz2, _T("Start Page"), &dw);

		if (lstrcmp (sz2, szIECP) == 0)
			key.SetValue (szIEOP, _T("Start Page"));
	}
}

BOOL CFdmApp::RegisterServer(BOOL bGlobal)
{
	LOG (_T("Registering server (%s)..."), bGlobal ? _T("global") : _T("local"));

	bool bRegisterForUserOnly = IS_PORTABLE_MODE && !Is9xME;

	if (bRegisterForUserOnly)
	{
		LOGsnl (_T("Portable mode: overriding HKCR..."));
		HKEY hKey;
		LONG lRes;
		lRes = RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hKey);
		LOGRESULT (_T(" open cu key"), lRes);
		lRes = vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, hKey);
		LOGRESULT (_T(" override HKCR key"), lRes);
		RegCloseKey (hKey);
	}

	if (_App.ModifyIEUserAgent ())
	{
		fsIEUserAgent ua;
		ua.SetPP (IE_USERAGENT_ADDITION);
        ua.RemovePP (_T("Free Download Manager")); 
	}

	if (bGlobal)
	{
		CRegKey key;
		
		
		
		if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), KEY_WRITE))
			vmsSHCopyKey (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), key);

		
		HRESULT hr = _Module.UpdateRegistryFromResource (IDR_FDM, TRUE);
		LOGRESULT (_T("UpdateRegistryFromResource"), hr);
	
		hr = _Module.RegisterServer (!bRegisterForUserOnly);
		LOGRESULT (_T("_Module.RegisterServer"), hr);

		if (bRegisterForUserOnly)
		{
			CComBSTR bstrPath;
			CComPtr<ITypeLib> pTypeLib;
			hr = AtlModuleLoadTypeLib (&_Module, NULL, &bstrPath, &pTypeLib);
			if (SUCCEEDED(hr))
			{
				OLECHAR szDir[_MAX_PATH];
				ocscpy(szDir, bstrPath);
				szDir[AtlGetDirLen(szDir)] = 0;
			
				typedef HRESULT (WINAPI *FNRTLFU)(ITypeLib*,OLECHAR*,OLECHAR*);
				FNRTLFU pfn = (FNRTLFU) GetProcAddress (GetModuleHandle (_T("oleaut32.dll")), _T("RegisterTypeLibForUser"));
				if (pfn)
					hr = pfn (pTypeLib, bstrPath, szDir);
				else
					hr = ::RegisterTypeLib(pTypeLib, bstrPath, szDir);
			}
			LOGRESULT (_T("register typelib"), hr);
		}
		
		
		ITypeLib *pLib = NULL;
		
		if (SUCCEEDED (hr = LoadTypeLibEx (L"fdm.tlb", REGKIND_REGISTER, &pLib)))
			pLib->Release ();
		else
			MessageBox (0, _T("Failed to load Free Download Manager type lib"),_T("Error"),0);
		LOGRESULT (_T("register fdm.tlb"), hr);

		if (_AppMgr.IsBtInstalled ())
		{
			
			if (vmsTorrentExtension::IsAssociatedWithUs () == FALSE)
			{
				if (_App.Bittorrent_Enable () || vmsTorrentExtension::IsAssociationExist () == FALSE)
				{
					_App.Bittorrent_OldTorrentAssociation (vmsTorrentExtension::GetCurrentAssociation ());
					vmsTorrentExtension::Associate ();
				}
			}
		}
	}

	WriteProfileInt (_T(""), _T("cverID"), 0);

	if (GetProfileInt (_T("Settings\\Monitor"), _T("IECMInited"), 0) == 0)
		WriteProfileInt (_T("Settings\\Monitor"), _T("IECMInited"), TRUE);
	_IECMM.DeleteIEMenus (); 
	fsIEContextMenuMgr::DeleteAllFDMsIEMenus ();
	_IECMM.AddIEMenus ();

	
	DWORD dwMUSO = _App.Monitor_UserSwitchedOn ();
				
	_IECatchMgr.ActivateIE2 ((dwMUSO & MONITOR_USERSWITCHEDON_IE) != 0);

	vmsFirefoxMonitoring::Install (true);
	if (vmsFirefoxMonitoring::IsInstalled () == false)
		_App.Monitor_Firefox (FALSE);
	else
		_App.Monitor_Firefox ((dwMUSO & MONITOR_USERSWITCHEDON_FIREFOX) != 0);
	
	_NOMgr.DeinstallFirefoxPlugin ();

	if (dwMUSO & MONITOR_USERSWITCHEDON_NETSCAPE)
		_NOMgr.InstallNetscapePlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_OPERA)
		_NOMgr.InstallOperaPlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_SEAMONKEY)
		_NOMgr.InstallMozillaSuitePlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_SAFARI)
		_NOMgr.InstallSafariPlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_CHROME)
		_NOMgr.InstallChromePlugin ();

	CRegKey key;
	if (ERROR_SUCCESS != key.Create (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Upload Manager")))
		key.Open (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Upload Manager"));
	key.SetValue (fsGetAppDataFolder (), _T("force_data_folder"));

	if (bRegisterForUserOnly)
		vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, NULL);

	return TRUE;
}

void CFdmApp::Install_RegisterServer()
{
	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init(ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId();
	}
	RegisterServer (TRUE);
}

void CFdmApp::Install_UnregisterServer()
{
	LOGsnl (_T("Unregister server..."));

	bool bUnregisterForUserOnly = IS_PORTABLE_MODE;
	
	if (bUnregisterForUserOnly)
	{
		HKEY hKeyCurrentUser;
		LONG l = RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hKeyCurrentUser);
		LOGRESULT (_T("open cu"), l);
		l = vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, hKeyCurrentUser);
		LOGRESULT (_T("override key"), l);
		RegCloseKey (hKeyCurrentUser);
	}

	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init(ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId();
	}
			
	
	HRESULT hr = _Module.UpdateRegistryFromResource(IDR_FDM, FALSE);
	LOGRESULT (_T("_Module.UpdateRegistryFromResource"), hr);
	hr = _Module.UnregisterServer(!bUnregisterForUserOnly); 
	LOGRESULT (_T("_Module.UnregisterServer"), hr);
	UnRegisterTypeLib (LIBID_FdmLib, 0, 0, LOCALE_SYSTEM_DEFAULT, SYS_WIN32);

	if (bUnregisterForUserOnly)
	{
		typedef HRESULT (WINAPI *PFNRTL)(REFGUID, WORD, WORD, LCID, SYSKIND);
		CComBSTR bstrPath;
		CComPtr<ITypeLib> pTypeLib;
		hr = AtlModuleLoadTypeLib(&_Module, NULL, &bstrPath, &pTypeLib);
		if (SUCCEEDED(hr))
		{
			TLIBATTR* ptla;
			HRESULT hr = pTypeLib->GetLibAttr(&ptla);
			if (SUCCEEDED(hr))
			{
				HINSTANCE h = LoadLibrary(_T("oleaut32.dll"));
				if (h != NULL)
				{
					PFNRTL pfn = (PFNRTL) GetProcAddress(h, _T("UnRegisterTypeLibForUser"));
					if (pfn == NULL)
						pfn = (PFNRTL) GetProcAddress(h, _T("UnRegisterTypeLib"));
					if (pfn != NULL)
						hr = pfn(ptla->guid, ptla->wMajorVerNum, ptla->wMinorVerNum, ptla->lcid, ptla->syskind);
					FreeLibrary (h);
				}
				pTypeLib->ReleaseTLibAttr(ptla);
			}
		}
		LOGRESULT (_T("unregister typelib"), hr);
	}
	
	
	
	if (_IECMM.IsIEMenusPresent ())
		WriteProfileInt (_T("Settings\\Monitor"), _T("IECMInited"), 0);

	fsIEContextMenuMgr::DeleteAllFDMsIEMenus ();
	
	_NOMgr.DeinstallNetscapePlugin ();
	_NOMgr.DeinstallOperaPlugin ();
    _NOMgr.DeinstallMozillaSuitePlugin ();
	_IECatchMgr.ActivateIE2 (FALSE);
	fsIEUserAgent ua;
	ua.RemovePP (IE_USERAGENT_ADDITION);
	UninstallCustomizations ();
	vmsFirefoxMonitoring::Install (false);

	
	if (vmsTorrentExtension::IsAssociatedWithUs ())
		vmsTorrentExtension::AssociateWith (_App.Bittorrent_OldTorrentAssociation ());

	if (bUnregisterForUserOnly)
		vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, NULL);
}

void CFdmApp::SaveSettings()
{
	if (IS_PORTABLE_MODE)
	{
		
		vmsAppSettingsStore* pStgs = _App.get_SettingsStore ();
		CString strStgsFile = fsGetDataFilePath (_T("settings.dat"));
		pStgs->SaveSettingsToFile (strStgsFile);
	}
}  

void CFdmApp::IntegrationSettings()
{
	vmsUploadsDllCaller udc;
	HMODULE hUploadsDll;

	#ifndef _DEBUG
	CString strFP = fsGetFumProgramFilesFolder ();
	hUploadsDll = LoadLibrary (strFP + _T("fumcore.dll"));
	#else
	hUploadsDll = LoadLibrary (_T("E:\\VCW\\FDM\\FDM\\Uploader\\CoreDll\\Debug\\fumcore.dll"));
	ASSERT (hUploadsDll != NULL);
	#endif

	if (hUploadsDll == NULL)
		return;

	vmsUploadsDll::FNSDC pfnSetCaller = (vmsUploadsDll::FNSDC) GetProcAddress (hUploadsDll, 
		_T("_SetDllCaller"));
	ASSERT (pfnSetCaller != NULL);
	pfnSetCaller (&udc);

	if (lstrcmpi (m_lpCmdLine, _T("-suis")) == 0)
	{
		vmsUploadsDll::FNSIS pfnShowIntegrationSettings = (vmsUploadsDll::FNSIS) GetProcAddress (hUploadsDll, 
			_T("_ShowIntegrationSettings"));
		ASSERT (pfnShowIntegrationSettings != NULL);
		pfnShowIntegrationSettings (NULL);
	}
	else
	{
		vmsUploadsDll::FNEI pfnEnableIntegration = (vmsUploadsDll::FNEI) GetProcAddress (hUploadsDll, 
			_T("_EnableIntegration"));			 
		ASSERT (pfnEnableIntegration != NULL);
		pfnEnableIntegration  (lstrcmpi (m_lpCmdLine, _T("-euis")) == 0, 0);
	}

	FreeLibrary (hUploadsDll);
}

DWORD WINAPI CFdmApp::_threadExitProcess(LPVOID lp)
{
	Sleep (((DWORD)lp) * 1000);
	ASSERT (FALSE);
	HANDLE hProcess = OpenProcess (PROCESS_TERMINATE, FALSE, GetCurrentProcessId ());
	TerminateProcess (hProcess, (DWORD)-1);
	return 0;
}

void CFdmApp::ScheduleExitProcess(DWORD dwSeconds)
{
	DWORD dw;
	CloseHandle (
		::CreateThread (NULL, 0, _threadExitProcess, (LPVOID)dwSeconds, 0, &dw));
}

void CFdmApp::CheckRegistry()
{
	fsString str = _T("%56%69%63%4D%61%6E%20%53%6F%66%74%77%61%72%65");
	fsDecodeHtmlUrl (str);
	CString strOldKey = _T("Software\\"); strOldKey += str; 
	CString strOldRKey = strOldKey;
	strOldKey += _T("\\Free Download Manager");

	
	CRegKey key;
	if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, strOldKey))
	{
		CRegKey key2;
		if (ERROR_SUCCESS != key2.Open (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\History")))
		{
			LONG lRes = key2.Create (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG"));
			if (lRes != ERROR_SUCCESS)
				lRes = key2.Open (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG"));

			if (ERROR_SUCCESS == lRes)
			{
				key.Open (HKEY_CURRENT_USER, strOldRKey);

				CString strPath = GetProfileString (_T(""), _T("Path"), _T(""));
				LONG nRes = fsCopyKey (key, key2, _T("Free Download Manager"), _T("Free Download Manager"));
				nRes; 
				WriteProfileString (_T(""), _T("Path"), strPath); 
				
				key.RecurseDeleteKey (_T("Free Download Manager"));
			}
		}
	}
}

AFX_MODULE_STATE* CFdmApp::GetModuleState()
{
	return m_pModuleState;
}
