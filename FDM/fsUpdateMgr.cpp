/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsUpdateMgr.h"
#include "MainFrm.h"
#include "mfchelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif            

fsUpdateMgr::fsUpdateMgr()
{
	m_bRunning = FALSE;
	m_pfnEvents = NULL;
	m_pfnDescEvents= NULL;
}

fsUpdateMgr::~fsUpdateMgr()
{
	SAFE_DELETE (m_dldr);
}

void fsUpdateMgr::ReadSettings()
{
	m_enAUT = _App.Update_AutoUpdateType ();
	m_strDlFullInstallPath = m_strDlUpgradesPath = m_strUpdateUrl = _App.Update_URL ();
}

void fsUpdateMgr::SaveSettings()
{
	_App.Update_AutoUpdateType (m_enAUT);
	if (m_strUpdateUrl.GetLength ())
		_App.Update_URL (m_strUpdateUrl);
}

BOOL fsUpdateMgr::StartUpdater()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory (&si, sizeof (si));
	si.cb = sizeof (si);
	ZeroMemory (&pi, sizeof (pi));

	CString strCmdLine;
	strCmdLine.Format (_T("\"%s\" \"%s\" \"%s\" \"%s\" \"/silent\" \"0\""),
		vmsGetAppFolder () + _T("updater.exe"), 
		((CFdmApp*)AfxGetApp ())->m_strAppPath + _T("fdm.exe"), 
		_pszAppMutex, 
		m_strUpdateFile);

	if (FALSE == CreateProcess (NULL, (LPTSTR)(LPCTSTR)strCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		return FALSE;

	return TRUE;
}

void fsUpdateMgr::CheckForUpdate(bool bByUser)
{
	if (m_bRunning || IS_PORTABLE_MODE)
		return;

	m_bCheckingByUser = bByUser;

	SAFE_DELETE (m_dldr);
	fsnew1 (m_dldr, fsDownloadMgr (NULL));
	m_dldr->SetEventFunc (_DownloadMgrEvents, this);
	m_dldr->SetEventDescFunc (_DownloadMgrDescEvents, this);

	m_bRunning = TRUE;
	m_bChecking = TRUE;

	CMainFrame* pFrm = (CMainFrame*) AfxGetApp ()->m_pMainWnd;

	CString strUrl = m_strUpdateUrl;

	strUrl += _T("proupd.lst");

	LPCTSTR pszCustomizer = pFrm->get_Customizations ()->get_Customizer ();
	if (pszCustomizer != NULL && lstrlen (pszCustomizer) != 0)
	{
		strUrl += _T("?edition=");
		strUrl += pszCustomizer;
	}

	m_dldr->CreateByUrl (strUrl);
	
	fsDownload_Properties *dp = m_dldr->GetDP ();
	fsDownload_NetworkProperties *dnp = m_dldr->GetDNP ();

	SAFE_DELETE_ARRAY (dp->pszFileName);
	CString strFile = fsGetDataFilePath (_T("Update\\"));	
	fsnew (dp->pszFileName, TCHAR, strFile.GetLength () + 1);
	_tcscpy (dp->pszFileName, strFile);
	dp->uMaxSections = 1;
	dp->uMaxAttempts = 1;
	dp->dwFlags |= DPF_DONTRESTARTIFNORESUME;
	dp->enSCR = SCR_STOP;
	dp->enAER = AER_REWRITE;
	*dp->pszAdditionalExt = 0;

	SAFE_DELETE_ARRAY (dnp->pszReferer);

	fsnew (dnp->pszReferer, TCHAR, 100);
	_stprintf (dnp->pszReferer, _T("FDM - Build number = %s"), vmsFdmAppMgr::getBuildNumber ());

	Event (UME_CONNECTING);

	m_dldr->StartDownloading ();
}

DWORD fsUpdateMgr::_DownloadMgrEvents(fsDownloadMgr* pMgr, fsDownloaderEvent ev, UINT uInfo, LPVOID lp)
{
	fsUpdateMgr *pThis = (fsUpdateMgr*) lp;

	switch (ev)
	{
		case DE_SECTIONSTARTED:
			if (pMgr->GetDownloader ()->GetRunningSectionCount ())
			{
				if (pThis->m_bChecking)
					pThis->Event (UME_RETREIVINGUPDLST);
				else
					pThis->Event (UME_RETREIVINGUPDATE);
			}
		break;

		case DE_EXTERROR:
			if (uInfo == DMEE_STOPPEDORDONE)
			{
				if (pMgr->IsDone ())
				{
					if (pThis->m_bChecking)
					{
						SYSTEMTIME time;
						GetLocalTime (&time);
						_App.NewVerExists (TRUE);
						_App.Update_LastCheck (time);

						pThis->Event (UME_UPDLSTDONE);
						pThis->m_bChecking = FALSE;
						pThis->ProcessUpdateLstFile ();
					}
					else
					{
						pThis->m_strUpdateFile = pThis->m_dldr->GetDP ()->pszFileName;
						pThis->Event (UME_UPDATEDONE);
						pThis->m_bRunning = FALSE;
					}
				}
				else
				{
					pThis->Event (UME_FATALERROR);
					pThis->m_bRunning = FALSE;
				}
			}
			else
			{
				pThis->Event (UME_ERROR);
			}
		break;

		case DE_ERRFROMSERVER:
		case DE_ERROROCCURED:
		case DE_FAILCONNECT:
		case DE_WRITEERROR:
			pThis->Event (UME_ERROR);
		break;
	}

	return TRUE;
}

void fsUpdateMgr::ProcessUpdateLstFile()
{	
	TCHAR szSections [10000];
	*szSections = 0;
	TCHAR szValues [10000];

	if (::GetVersion () & 0x80000000)
		FixIniFileFor9x (fsGetDataFilePath (_T("Update\\proupd.lst")));

	
	
	if (0 == GetPrivateProfileSectionNames (szSections, sizeof (szSections), 
		fsGetDataFilePath (_T("Update\\proupd.lst"))) || 
		_ttoi (szSections) <= (int)vmsFdmAppMgr::getVersion ()->m_appVersion [2].dwVal)
	{
		ASSERT (GetPrivateProfileSectionNames (szSections, sizeof (szSections), fsGetDataFilePath (_T("Update\\proupd.lst"))));
		
		_App.NewVerExists (FALSE);
		Event (UME_NEWVERSIONNOTAVAIL);
		m_bRunning = FALSE;
		return;
	}

	
	m_strBN = szSections;

	CString strCurBN;	
	strCurBN = vmsFdmAppMgr::getBuildNumber ();
	
	m_strUpgSize = _T("");
	m_strUpgFileName = _T("");
	m_vWN.clear ();

	LPCTSTR pszSect = szSections;

	while (*pszSect)
	{
		
		GetPrivateProfileSection (pszSect, szValues, sizeof (szValues), 
			fsGetDataFilePath (_T("Update\\proupd.lst")));
		LPTSTR pszValue = szValues;

		
		BOOL bCommon = stricmp (pszSect, _T("Common")) == 0;

		
		BOOL bNewBNNow = bCommon == FALSE && _tcscmp (pszSect, m_strBN) == 0;
		
		BOOL bBiggerBNNow = bCommon == FALSE && _ttoi (pszSect) > (int)vmsFdmAppMgr::getVersion ()->m_appVersion [2].dwVal;

		
		while (*pszValue)
		{
			
			LPTSTR pszVVal = _tcschr (pszValue, _T('='));
			*pszVVal = 0;
			pszVVal++;	

			if (bCommon)
			{
				
				if (stricmp (pszValue, _T("DownloadPathForFullInstall")) == 0)
				{
					m_strDlFullInstallPath = pszVVal;
					if (m_strDlFullInstallPath [m_strDlFullInstallPath.GetLength () - 1] != _T('\\') && 
							m_strDlFullInstallPath [m_strDlFullInstallPath.GetLength () - 1] != _T('/'))
						m_strDlFullInstallPath += _T('/');
				}

				if (stricmp (pszValue, _T("DownloadPathForUpgrades")) == 0)
				{
					m_strDlUpgradesPath = pszVVal;
					if (m_strDlUpgradesPath [m_strDlUpgradesPath.GetLength () - 1] != _T('\\') && 
							m_strDlUpgradesPath [m_strDlUpgradesPath.GetLength () - 1] != _T('/'))
						m_strDlUpgradesPath += _T('/');
				}
			}

			if (bNewBNNow)
			{
				if (stricmp (pszValue, _T("Version")) == 0)
					m_strVersion = pszVVal;	
				else if (stricmp (pszValue, _T("FullSize")) == 0)
					m_strFullSize = pszVVal;	
				else if (stricmp (pszValue, strCurBN) == 0)
					m_strUpgSize = pszVVal;	
				else if (strnicmp (pszValue, strCurBN, strCurBN.GetLength ()) == 0)
				{
					
					
					if (stricmp (pszValue + strCurBN.GetLength (), _T("-name")) == 0)
						m_strUpgFileName = pszVVal;
					
					
					else if (stricmp (pszValue + strCurBN.GetLength (), _T("-size")) == 0)
						m_strUpgSize = pszVVal;
				}
				else if (stricmp (pszValue, _T("FrmtVer")) == 0)
				{
					
					int nVer = _ttoi (pszVVal);
					if (nVer != 1)
					{
						
						_App.NewVerExists (FALSE);
						Event (UME_NEWVERSIONNOTAVAIL);
						m_bRunning = FALSE;
						return;
					}
				}
			}

			
			
			if (bBiggerBNNow && _tcsncmp (pszValue, _T("WN"), 2) == 0)
				m_vWN.add (pszVVal);

			pszValue = pszVVal;
			while (*pszValue++);	
		}

		while (*pszSect++); 
	}

	Event (UME_NEWVERSIONAVAIL);
}

void fsUpdateMgr::Event(fsUpdateMgrEvent ev)
{
	if (m_pfnEvents)
		m_pfnEvents (ev, m_lpEventsParam);
}

BOOL fsUpdateMgr::IsRunning()
{
	return m_bRunning;
}

void fsUpdateMgr::SetEventsFunc(fntUpdateMgrEventsFunc pfn, LPVOID lp)
{
	m_pfnEvents = pfn;
	m_lpEventsParam = lp;
}

LPCTSTR fsUpdateMgr::GetVersion()
{
	return m_strVersion;
}

LPCTSTR fsUpdateMgr::GetBuildNumber()
{
	return m_strBN;
}

void fsUpdateMgr::Stop()
{
	if (m_dldr && m_dldr->IsRunning ())
		m_dldr->StopDownloading ();
	else
		m_bRunning = FALSE;
}

LPCTSTR fsUpdateMgr::GetFullSize()
{
	return m_strFullSize;
}

LPCTSTR fsUpdateMgr::GetUpgSize()
{
	return m_strUpgSize;
}

fs::list <CString>* fsUpdateMgr::GetWhatNew()
{
	return &m_vWN;
}

void fsUpdateMgr::Update(BOOL bByFull)
{
	if (m_bRunning == FALSE || m_bChecking)
		return;

	SAFE_DELETE (m_dldr);
	fsnew1 (m_dldr, fsDownloadMgr (NULL));
	m_dldr->SetEventFunc (_DownloadMgrEvents, this);
	m_dldr->SetEventDescFunc (_DownloadMgrDescEvents, this);

	m_bChecking = FALSE;

	CString strUrl;	
	if (bByFull)
		strUrl.Format (_T("%sfdminst.exe"), m_strDlFullInstallPath);
	else
	{
		
		if (m_strUpgFileName.GetLength () == 0)
			
			strUrl.Format (_T("%sfdm%sto%supg.exe"), m_strDlUpgradesPath, vmsFdmAppMgr::getBuildNumber (), m_strBN);
		else
			
			strUrl.Format (_T("%s%s"), m_strDlUpgradesPath, m_strUpgFileName);
	}

	m_dldr->CreateByUrl (strUrl);
	
	fsDownload_Properties *dp = m_dldr->GetDP ();

	SAFE_DELETE_ARRAY (dp->pszFileName);
	CString strFile = fsGetDataFilePath (_T("Update\\"));
	fsnew (dp->pszFileName, TCHAR, strFile.GetLength () + 1);
	_tcscpy (dp->pszFileName, strFile);
	dp->enAER = AER_REWRITE;
	dp->uMaxAttempts = 1;
	*dp->pszAdditionalExt = 0;

	Event (UME_CONNECTING);

	m_dldr->StartDownloading ();
}

void fsUpdateMgr::_DownloadMgrDescEvents(fsDownloadMgr* , fsDownloadMgr_EventDescType , LPCTSTR pszDesc, LPVOID lp)
{
	fsUpdateMgr *pThis = (fsUpdateMgr*) lp;
	pThis->Event (pszDesc);
}

void fsUpdateMgr::SetDescEventsFunc(fntUpdateMgrDescEvents pfn, LPVOID lpParam)
{
	m_pfnDescEvents = pfn;
	m_lpDescEventsParam = lpParam;
}

void fsUpdateMgr::Event(LPCTSTR pszEvent)
{
	if (m_pfnDescEvents)
		m_pfnDescEvents (pszEvent, m_lpDescEventsParam);
}

void fsUpdateMgr::UpdateOnNextStart()
{
	AfxGetApp ()->WriteProfileString (_T("Settings\\Update"), _T("UpdateFile"), m_strUpdateFile);
}

BOOL fsUpdateMgr::IsStartUpdaterNeeded (BOOL bUpdaterWillBeLaunchedNow)
{
	m_strUpdateFile = AfxGetApp ()->GetProfileString (_T("Settings\\Update"), _T("UpdateFile"), _T(""));
	if (bUpdaterWillBeLaunchedNow)
		AfxGetApp ()->WriteProfileString (_T("Settings\\Update"), _T("UpdateFile"), _T(""));
	return m_strUpdateFile.GetLength () != 0;
}

void fsUpdateMgr::FixIniFileFor9x(LPCTSTR pszIni)
{
	HANDLE hFile = CreateFile (pszIni, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwLen = GetFileSize (hFile, NULL);
	LPTSTR psz = new TCHAR [dwLen + 1];
	DWORD dw;
	if (FALSE == ReadFile (hFile, psz, dwLen, &dw, NULL))
		return;
	psz [dwLen] = 0;

	CString str = psz;
	delete [] psz;

	str.Replace (_T("\r\n"), _T("\n"));
	str.Replace (_T("\n\r"), _T("\n"));
	str.Replace (_T("\n"), _T("\r\n"));

	SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
	WriteFile (hFile, (LPCTSTR)str, str.GetLength (), &dw, NULL);
	SetEndOfFile (hFile);

	CloseHandle (hFile);
}
