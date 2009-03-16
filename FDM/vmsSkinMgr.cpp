/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsSkinMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsSkinMgr::vmsSkinMgr()
{
}

vmsSkinMgr::~vmsSkinMgr()
{

}

void vmsSkinMgr::Scan()
{
	m_vList.clear ();

	CString strSkinDir = _T("Skins");

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile (strSkinDir + _T("\\*.*"), &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	

	do 
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp (wfd.cFileName, _T(".")) && lstrcmp (wfd.cFileName, _T("..")))
			{
				
				Scan_TryFolder (strSkinDir + _T("\\") + wfd.cFileName);
			}
		}
	} 
	while (FindNextFile (hFind, &wfd));

	FindClose (hFind);
}

void vmsSkinMgr::Scan_TryFolder(LPCTSTR pszFolder)
{
	CFile file;
	CString strIni = pszFolder;
	strIni += _T("\\skin.ini");

	if (GetFileAttributes (strIni) == DWORD (-1))
		return; 

	TCHAR szValues [30000] = _T("");
	GetPrivateProfileSection (_T("Skin"), szValues, sizeof (szValues), strIni);
	if (*szValues == 0)
		return;

	vmsSkinInfo skin;
	skin.strSkinFolder = pszFolder;

	LPCTSTR pszValue = szValues;

	while (*pszValue)
	{
		LPTSTR pszVVal = (LPTSTR) _tcschr (pszValue, _T('='));
		*pszVVal = 0;
		pszVVal++;

		if (lstrcmpi (pszValue, _T("Name")) == 0)
			skin.strName = pszVVal;

		else if (lstrcmpi (pszValue, _T("tbSizeX")) == 0)
			skin.tbSizeX = _ttoi (pszVVal);

		else if (lstrcmpi (pszValue, _T("tbSizeY")) == 0)
			skin.tbSizeY = _ttoi (pszVVal);

		else if (lstrcmpi (pszValue, _T("tbUseAlpha")) == 0)
			skin.tbUseAlpha = lstrcmpi (pszVVal, _T("yes")) == 0;

		else if (lstrcmpi (pszValue, _T("fdmBuild")) == 0)
			skin.nFDMBuild = _ttoi (pszVVal);

		pszValue = pszVVal;
		while (*pszValue++);	
	}

	
	
	if (skin.nFDMBuild < 829 || skin.nFDMBuild > (int)vmsFdmAppMgr::getVersion ()->m_appVersion [2].dwVal)
		return;

	m_vList.add (skin);
}

void vmsSkinMgr::set_CurrentSkin(int iSkin)
{
	m_nCurrSkin = iSkin;
}

HBITMAP vmsSkinMgr::bmp_x(UINT nID, LPCTSTR pszName)
{
	if ((m_dwFlags & SM_ALLOWBITMAPS) == 0)
		pszName = NULL;

	if (m_nCurrSkin == -1 || pszName == NULL)
		return (HBITMAP)
			LoadImage (m_hInst, MAKEINTRESOURCE (nID), IMAGE_BITMAP, 
				0, 0, LR_CREATEDIBSECTION);

	HBITMAP hbm = (HBITMAP) 
		LoadImage (NULL, m_vList [m_nCurrSkin].strSkinFolder + _T("\\") + pszName + _T(".bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	return hbm ? hbm : bmp_x (nID, NULL);
}  

HBITMAP vmsSkinMgr::bmp(UINT nID)
{
	

	static UINT aIDs [] = {
		IDB_BACK, IDB_BACK_D, IDB_CHECKS, IDB_DOWNLOADSTASKS_SEL,
		IDB_DOWNLOADSTASKS, IDB_DOWNLOADSINFO, IDB_FILELIST,
		IDB_FILELIST_SEL, IDB_GO, IDB_GROUPS, IDB_GROUPSMENU_D,
		IDB_GROUPSMENU, IDB_LOGSTATES, IDB_MUTE, IDB_SCHEDULER_SEL,
		IDB_SHEDULER, IDB_SITELIST, IDB_SITELIST_SEL,
		IDB_TOOL0, IDB_TOOL0_16, IDB_TOOL0_16_D, IDB_TOOL0_D,
		IDB_TOOL_DLDS, IDB_TOOL_DLDS_D, IDB_TOOL_DLD_16, IDB_TOOL_DLD_16_D,
		IDB_TOOL_BT, IDB_TOOL_BT_D, IDB_TOOL_BT_16, IDB_TOOL_BT_16_D,
		IDB_TOOL_HFE, IDB_TOOL_HFE_D, IDB_TOOL_HFE_16, IDB_TOOL_HFE_16_D,
		IDB_TOOL_SCHEDULE, IDB_TOOL_SCHEDULE_D, IDB_TOOL_SCH_16, IDB_TOOL_SCH_16_D,
		IDB_TOOL_SITES, IDB_TOOL_SITES_D, IDB_TOOL_SITES_16, IDB_TOOL_SITES_16_D,
		IDB_TOOL_SPIDER, IDB_TOOL_SPIDER_D, IDB_TOOL_SPIDER_16, IDB_TOOL_SPIDER_16_D,
		IDB_VIDMAN, IDB_VIDMAN_D
	};

	static LPCTSTR ppszNames [] = {
		_T("back"), _T("back_d"), _T("checks"), _T("dldtasks_sel"), _T("dldtasks"),
		_T("dlinfo"), _T("filelist"), _T("filelist_sel"), _T("go"), _T("groups"),
		_T("groupsmenu_d"), _T("groupsmenu"), _T("logstat"), _T("mute"), 
		_T("scheduler_sel"), _T("scheduler"), _T("sitelist"), _T("sitelist_sel"),
		_T("tool0"), _T("tool0_small"), _T("tool0_small_d"), _T("tool0_d"),
		_T("tool_dld"), _T("tool_dld_d"), _T("tool_dld_small"), _T("tool_dld_small_d"),
        _T("tool_bt"), _T("tool_bt_d"), _T("tool_bt_small"), _T("tool_bt_small_d"),
		_T("tool_hfe"),	_T("tool_hfe_d"), _T("tool_hfe_small"), _T("tool_hfe_small_d"),
		_T("tool_sch"), _T("tool_sch_d"), _T("tool_sch_small"), _T("tool_sch_small_d"),
		_T("tool_sites"), _T("tool_sites_d"), _T("tool_sites_small"), _T("tool_sites_small_d"),
		_T("tool_spider"), _T("tool_spider_d"), _T("tool_spider_small"), _T("tool_spider_small_d"),
		_T("vidman"), _T("vidman_d")		
	};

	ASSERT (sizeof (aIDs)/sizeof (UINT) == sizeof (ppszNames)/sizeof (LPCTSTR));

	for (int i = 0; i < sizeof (aIDs) / sizeof (UINT); i++)
	{
		if (aIDs [i] == nID)
			return bmp_x (nID, ppszNames [i]);
	}

	
	return bmp_x (nID, NULL);
}

void vmsSkinMgr::Initialize()
{
	m_nCurrSkin = -1;
	m_hInst = AfxGetInstanceHandle ();
	m_dwFlags = _App.Skin_Flags ();
	Scan ();
	CString strSkin = _App.Skin_Current ();
	if (strSkin != _T(""))
	{
		for (int i = 0; i < m_vList.size (); i++)
		{
			if (m_vList [i].strName.CompareNoCase (strSkin) == 0)
			{
				m_nCurrSkin = i;
				break;
			}
		}
	}
}

int vmsSkinMgr::tbSizeX()
{
	return m_nCurrSkin == -1 || (m_dwFlags & SM_ALLOWBITMAPS) == 0 ? 32 : m_vList [m_nCurrSkin].tbSizeX;
}

int vmsSkinMgr::tbSizeY()
{
	return m_nCurrSkin == -1 || (m_dwFlags & SM_ALLOWBITMAPS) == 0 ? 32 : m_vList [m_nCurrSkin].tbSizeY;
}

BOOL vmsSkinMgr::tbUseAlpha()
{
	return m_nCurrSkin == -1 || (m_dwFlags & SM_ALLOWBITMAPS) == 0 ? TRUE : m_vList [m_nCurrSkin].tbUseAlpha;
}

HICON vmsSkinMgr::icon(UINT nID, int cx, int cy)
{
	static UINT aIDs [] = {
		IDI_CHOOSEFOLDER, IDI_CREATEGROUP, IDI_LOGO2, IDI_LOGIN,
		IDI_SETTIME, IDI_TOSEL, IDI_TOUNSEL,
		IDI_TRAY, IDI_TRAY_DOWNLOADING, IDI_TRAY_ERRORS,IDI_TRAY_UNKNOWN,
		IDI_TEST_OK,
	};

	static LPCTSTR ppszNames [] = {
		_T("choosefolder"), _T("creategroup"), _T("dropbox"), _T("login"), _T("settime"),
		_T("tosel"), _T("tounsel"), 
		_T("tray"),	_T("tray_down"), _T("tray_err"), _T("tray_starting"),
		_T("check_ok"),
	};

	ASSERT (sizeof (aIDs)/sizeof (UINT) == sizeof (ppszNames)/sizeof (LPCTSTR));

	for (int i = 0; i < sizeof (aIDs) / sizeof (UINT); i++)
	{
		if (aIDs [i] == nID)
			return icon_x (nID, ppszNames [i], cx, cy);
	}

	return icon_x (nID, NULL, cx, cy);
}

HICON vmsSkinMgr::icon_x(UINT nID, LPCTSTR pszName, int cx, int cy)
{
	if ((m_dwFlags & SM_ALLOWICONS) == 0)
		pszName = NULL;

	if (m_nCurrSkin == -1 || pszName == NULL)
		return (HICON)
			LoadIcon (m_hInst, MAKEINTRESOURCE (nID));
			

	HICON hico = (HICON) 
		LoadImage (NULL, m_vList [m_nCurrSkin].strSkinFolder + _T("\\") + pszName + _T(".ico"),
			IMAGE_ICON, cx, cy, LR_LOADFROMFILE);

	return hico ? hico : icon_x (nID, NULL, cx, cy);
}

int vmsSkinMgr::get_Count()
{
	return m_vList.size ();
}

int vmsSkinMgr::get_Current()
{
	return m_nCurrSkin;
}

vmsSkinInfo* vmsSkinMgr::get_Skin(int iIndex)
{
	return &m_vList [iIndex];
}

BOOL vmsSkinMgr::tbUseAlpha_ForAllPluginMenuImages()
{
	return m_nCurrSkin != -1 && tbUseAlpha ();
}
