/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsOpNetIntegrationMgr.h"
#include "FolderBrowser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsOpNetIntegrationMgr::fsOpNetIntegrationMgr()
{

}

fsOpNetIntegrationMgr::~fsOpNetIntegrationMgr()
{

}

void fsOpNetIntegrationMgr::Initialize()
{
	CRegKey key;
	
	CString strOpKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Opera.exe");
	
	CString strNetKey1 = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscp.exe");
	CString strNetKey2 = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscp6.exe");
	CString strNetKey3 = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscape.exe");
	CString strNetKey4 = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\navigator.exe");
	
	CString strFfKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Firefox.exe");
	
	CString strMozSKey1 = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\SeaMonkey.exe");
	CString strMozSKey2 = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\mozilla.exe");

	if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, _T("Software\\Opera Software"), KEY_READ))
	{
		TCHAR szPath [MY_MAX_PATH] = _T("");
		DWORD dw = sizeof (szPath);
		if (ERROR_SUCCESS == key.QueryValue (szPath, _T("Plugin Path"), &dw))
		{
			if (*szPath)
			{
				m_strOpPath = szPath;
				if (m_strOpPath [m_strOpPath.GetLength () - 1] != _T('\\') || m_strOpPath [m_strOpPath.GetLength () - 1] != _T('/'))
					m_strOpPath += _T('\\');
			}
		}
	}

	if (m_strOpPath == _T("") && ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strOpKey, KEY_READ))
	{
		

		TCHAR szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, _T("Path"), &dw);
		m_strOpPath = szPath;
		if (m_strOpPath [m_strOpPath.GetLength () - 1] != _T('\\') || m_strOpPath [m_strOpPath.GetLength () - 1] != _T('/'))
			m_strOpPath += _T('\\');
		if (GetFileAttributes (m_strOpPath + _T("program\\plugins")) != DWORD (-1))
			m_strOpPath += _T("program\\plugins\\");	
		else
			m_strOpPath += _T("Plugins\\");
		key.Close ();
	}
	
	if (m_strOpPath == _T(""))
	{
		
		
		
		m_strOpPath = _App.Monitor_OperaPDInstalledTo ();
	}

	
	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey1, KEY_READ) ||
		 ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey2, KEY_READ) || 
		 ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey3, KEY_READ) ||
		 ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey4, KEY_READ))
	{
		TCHAR szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, _T("Path"), &dw);
		m_strNetPath = szPath;
		if (m_strNetPath [m_strNetPath.GetLength () - 1] != _T('\\') || m_strNetPath [m_strNetPath.GetLength () - 1] != _T('/'))
			m_strNetPath += _T('\\');
		m_strNetPath += _T("Plugins\\");
		key.Close ();

		if (m_strNetPath == m_strOpPath)
			m_strNetPath = _App.Monitor_NetscapePDInstalledTo ();
	}
	else
		m_strNetPath = _App.Monitor_NetscapePDInstalledTo ();

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strFfKey, KEY_READ))
	{
		

		TCHAR szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, _T("Path"), &dw);
		m_strFfPath = szPath;
		if (m_strFfPath [m_strFfPath.GetLength () - 1] != _T('\\') || m_strFfPath [m_strFfPath.GetLength () - 1] != _T('/'))
			m_strFfPath += _T('\\');
		m_strFfPath += _T("Plugins\\");
		key.Close ();
	}
	else
		m_strFfPath = _App.Monitor_FirefoxPDInstalledTo ();

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strMozSKey1, KEY_READ) ||
			ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strMozSKey2, KEY_READ))
	{
		

		TCHAR szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, _T("Path"), &dw);
		m_strMozSPath = szPath;
		if (m_strMozSPath [m_strMozSPath.GetLength () - 1] != _T('\\') || m_strMozSPath [m_strMozSPath.GetLength () - 1] != _T('/'))
			m_strMozSPath += _T('\\');
		m_strMozSPath += _T("Plugins\\");
		key.Close ();
	}
	else
		m_strMozSPath = _App.Monitor_MozillaSuitePDInstalledTo ();

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, _T("Software\\Clients\\StartMenuInternet\\Safari.exe\\shell\\open\\command"), KEY_READ))
	{
		TCHAR szCmd [MY_MAX_PATH] = _T("");
		DWORD dw = sizeof (szCmd);
		key.QueryValue (szCmd, NULL, &dw);
		LPTSTR psz = szCmd;
		if (*psz == _T('"'))
		{
			LPTSTR psz2 = _tcschr (psz+1, _T('"'));
			if (psz2)
				*psz2 = 0;
			psz++;
		}
		else
		{
			LPTSTR psz2 = _tcschr (psz, _T(' '));
			if (psz2)
				*psz2 = 0;
		}
		LPTSTR psz3 = _tcsrchr (psz, _T('\\'));
		if (psz3)
			psz3 [1] = 0;
		_tcscat (psz, _T("Plugins\\"));
		m_strSafariPath = psz;
	}
	else
		m_strSafariPath = _App.Monitor_SafariPDInstalledTo ();

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, _T("Software\\Clients\\StartMenuInternet\\chrome.exe\\shell\\open\\command"), KEY_READ))
	{
		TCHAR szCmd [MY_MAX_PATH] = _T("");
		DWORD dw = sizeof (szCmd);
		key.QueryValue (szCmd, NULL, &dw);
		LPTSTR psz = szCmd;
		if (*psz == _T('"'))
		{
			LPTSTR psz2 = _tcschr (psz+1, _T('"'));
			if (psz2)
				*psz2 = 0;
			psz++;
		}
		else
		{
			LPTSTR psz2 = _tcschr (psz, _T(' '));
			if (psz2)
				*psz2 = 0;
		}
		LPTSTR psz3 = _tcsrchr (psz, _T('\\'));
		if (psz3)
			psz3 [1] = 0;
		_tcscat (psz, _T("Plugins\\"));
		m_strChromePath = psz;
	}
	else
		m_strChromePath = _App.Monitor_ChromePDInstalledTo ();
}

BOOL fsOpNetIntegrationMgr::IsOperaPluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strOpPath == _T(""))
	{
		Initialize ();
		if (m_strOpPath.GetLength ())
			return IsOperaPluginInstalled (bQueryPluginDirIfUnknown);

		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;

		if (MessageBox (NULL, LS (L_CANTFINDOPERADIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;

		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;

		m_strOpPath = fb->GetPath ();

		if (m_strOpPath [m_strOpPath.GetLength () - 1] != _T('\\') || m_strOpPath [m_strOpPath.GetLength () - 1] != _T('/'))
			m_strOpPath += _T('\\');

		
		
		if (strnicmp (LPCTSTR (m_strOpPath) + m_strOpPath.GetLength () - 8, _T("Plugins\\"), 8))
		{
			if (GetFileAttributes (m_strOpPath + _T("program\\plugins")) != DWORD (-1))
				m_strOpPath += _T("program\\plugins\\");	
			else
				m_strOpPath += _T("Plugins\\");
		}
	}

	CString str = m_strOpPath;
	str += _T("npfdm.dll");
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallOperaPlugin()
{
	if (m_strOpPath == _T(""))
		return FALSE;

	CString str = m_strOpPath;
	str += _T("npfdm.dll");
	fsBuildPathToFile (str);
	if (CopyFile (_T("npfdm.dll"), str, FALSE))
	{
		
		_App.Monitor_OperaPDInstalledTo (m_strOpPath);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallOperaPlugin()
{
	if (m_strOpPath == _T(""))
		return FALSE;

	_App.Monitor_OperaPDInstalledTo (_T(""));

	CString str = m_strOpPath;
	str += _T("npfdm.dll");

	m_strOpPath = _T("");

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsNetscapePluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strNetPath == _T(""))
	{
		Initialize ();
		if (m_strNetPath.GetLength ())
			return IsNetscapePluginInstalled (bQueryPluginDirIfUnknown);

		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;

		if (MessageBox (NULL, LS (L_CANTFINDNETSCAPEDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;

		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;

		m_strNetPath = fb->GetPath ();
		if (m_strNetPath [m_strNetPath.GetLength () - 1] != _T('\\') || m_strNetPath [m_strNetPath.GetLength () - 1] != _T('/'))
			m_strNetPath += _T('\\');
		if (strnicmp (LPCTSTR (m_strNetPath)  + m_strNetPath.GetLength () - 8, _T("Plugins\\"), 8))
			m_strNetPath += _T("Plugins\\");
	}

	CString str = m_strNetPath;
	str += _T("npfdm.dll");
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallNetscapePlugin()
{
	if (m_strNetPath == _T(""))
		return FALSE;

	CString str = m_strNetPath;
	str += _T("npfdm.dll");
	fsBuildPathToFile (str);
	if (CopyFile (_T("npfdm.dll"), str, FALSE))
	{
		_App.Monitor_NetscapePDInstalledTo (m_strNetPath);
		return TRUE;
	}

	return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallNetscapePlugin()
{
	if (m_strNetPath == _T(""))
		return FALSE;

	_App.Monitor_NetscapePDInstalledTo (_T(""));

	CString str = m_strNetPath;
	str += _T("npfdm.dll");

	m_strNetPath = _T("");

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsFirefoxPluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strFfPath == _T(""))
	{
		Initialize ();
		if (m_strFfPath.GetLength ())
			return IsFirefoxPluginInstalled (bQueryPluginDirIfUnknown);

		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;

		if (MessageBox (NULL, LS (L_CANTFINDFIREFOXDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;

		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;

		m_strFfPath = fb->GetPath ();

		if (m_strFfPath [m_strFfPath.GetLength () - 1] != _T('\\') || m_strFfPath [m_strFfPath.GetLength () - 1] != _T('/'))
			m_strFfPath += _T('\\');

		
		
		if (strnicmp (LPCTSTR (m_strFfPath) + m_strFfPath.GetLength () - 8, _T("Plugins\\"), 8))
			m_strFfPath += _T("Plugins\\");
	}

	CString str = m_strFfPath;
	str += _T("npfdm.dll");
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallFirefoxPlugin()
{
	if (m_strFfPath == _T(""))
		return FALSE;

	CString str = m_strFfPath;
	str += _T("npfdm.dll");
	fsBuildPathToFile (str);
	if (CopyFile (_T("npfdm.dll"), str, FALSE))
	{
		
		_App.Monitor_FirefoxPDInstalledTo (m_strFfPath);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallFirefoxPlugin()
{
	if (m_strFfPath == _T(""))
		return FALSE;

	_App.Monitor_FirefoxPDInstalledTo (_T(""));

	CString str = m_strFfPath;
	str += _T("npfdm.dll");

	m_strFfPath = _T("");

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsMozillaSuitePluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strMozSPath == _T(""))
	{
		Initialize ();
		if (m_strMozSPath.GetLength ())
			return IsMozillaSuitePluginInstalled (bQueryPluginDirIfUnknown);
		return FALSE;
	}

	CString str = m_strMozSPath;
	str += _T("npfdm.dll");
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallMozillaSuitePlugin()
{
	if (m_strMozSPath == _T(""))
		return FALSE;

	CString str = m_strMozSPath;
	str += _T("npfdm.dll");
	fsBuildPathToFile (str);
	if (CopyFile (_T("npfdm.dll"), str, FALSE))
	{
		
		_App.Monitor_MozillaSuitePDInstalledTo (m_strMozSPath);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallMozillaSuitePlugin()
{
	if (m_strMozSPath == _T(""))
		return FALSE;

	_App.Monitor_MozillaSuitePDInstalledTo (_T(""));

	CString str = m_strMozSPath;
	str += _T("npfdm.dll");

	m_strMozSPath = _T("");

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsSafariPluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strSafariPath == _T(""))
	{
		Initialize ();
		if (m_strSafariPath.GetLength ())
			return IsSafariPluginInstalled (bQueryPluginDirIfUnknown);
		
		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;
		
		if (MessageBox (NULL, LS (L_CANTFINDSAFARIDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;
		
		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;
		
		m_strSafariPath = fb->GetPath ();
		
		if (m_strSafariPath [m_strSafariPath.GetLength () - 1] != _T('\\') || m_strSafariPath [m_strSafariPath.GetLength () - 1] != _T('/'))
			m_strSafariPath += _T('\\');
		
		
		
		if (strnicmp (LPCTSTR (m_strSafariPath) + m_strSafariPath.GetLength () - 8, _T("Plugins\\"), 8))
				m_strSafariPath += _T("Plugins\\");
	}
	
	CString str = m_strSafariPath;
	str += _T("npfdm.dll");
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallSafariPlugin()
{
	if (m_strSafariPath == _T(""))
		return FALSE;
	
	CString str = m_strSafariPath;
	str += _T("npfdm.dll");
	fsBuildPathToFile (str);
	if (CopyFile (_T("npfdm.dll"), str, FALSE))
	{
		_App.Monitor_SafariPDInstalledTo (m_strSafariPath);
		return TRUE;
	}
	
	return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallSafariPlugin()
{
	if (m_strSafariPath == _T(""))
		return FALSE;
	
	_App.Monitor_SafariPDInstalledTo (_T(""));
	
	CString str = m_strSafariPath;
	str += _T("npfdm.dll");
	
	m_strSafariPath = _T("");
	
	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsChromePluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strChromePath == _T(""))
	{
		Initialize ();
		if (m_strChromePath.GetLength ())
			return IsChromePluginInstalled (bQueryPluginDirIfUnknown);
		
		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;
		
		if (MessageBox (NULL, LS (L_CANTFINDCHROMEDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;
		
		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;
		
		m_strChromePath = fb->GetPath ();
		
		if (m_strChromePath [m_strChromePath.GetLength () - 1] != _T('\\') || m_strChromePath [m_strChromePath.GetLength () - 1] != _T('/'))
			m_strChromePath += _T('\\');
		
		
		
		if (strnicmp (LPCTSTR (m_strChromePath) + m_strChromePath.GetLength () - 8, _T("Plugins\\"), 8))
			m_strChromePath += _T("Plugins\\");
	}
	
	CString str = m_strChromePath;
	str += _T("npfdm.dll");
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallChromePlugin()
{
	if (m_strChromePath == _T(""))
		return FALSE;
	
	CString str = m_strChromePath;
	str += _T("npfdm.dll");
	fsBuildPathToFile (str);
	if (CopyFile (_T("npfdm.dll"), str, FALSE))
	{
		_App.Monitor_ChromePDInstalledTo (m_strChromePath);
		return TRUE;
	}
	
	return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallChromePlugin()
{
	if (m_strChromePath == _T(""))
		return FALSE;
	
	_App.Monitor_ChromePDInstalledTo (_T(""));
	
	CString str = m_strChromePath;
	str += _T("npfdm.dll");
	
	m_strChromePath = _T("");
	
	return DeleteFile (str);
}

