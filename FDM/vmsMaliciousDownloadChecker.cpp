/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsMaliciousDownloadChecker.h"
#include "vmsSimpleFileDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsMaliciousDownloadChecker::vmsMaliciousDownloadChecker()
{
	m_dldr = NULL;
	m_bNeedStop = false;
}

vmsMaliciousDownloadChecker::~vmsMaliciousDownloadChecker()
{

}

fsInternetResult vmsMaliciousDownloadChecker::Check(LPCTSTR pszUrl)
{
	TCHAR szTmpPath [MY_MAX_PATH];
	TCHAR szTmpFile [MY_MAX_PATH];

	m_bNeedStop = false;

	GetTempPath (sizeof (szTmpPath), szTmpPath);
	GetTempFileName (szTmpPath, _T("fdm"), 0, szTmpFile);

	
	CString strUrl;
	strUrl.Format ("http://fdm.freedownloadmanager.org/fromfdm/url.php?url=%s", EncodeUrl (pszUrl));  

	
	vmsSimpleFileDownloader dldr;
	m_dldr = &dldr;
	if (m_bNeedStop) {
		DeleteFile (szTmpFile);
		return IR_S_FALSE;
	}
	dldr.Download (strUrl, szTmpFile);
	while (dldr.IsRunning ())
		Sleep (50);
	m_dldr = NULL;
	if (dldr.GetLastError () != IR_SUCCESS) {
		DeleteFile (szTmpFile);
		return dldr.GetLastError ();
	}
	if (m_bNeedStop) {
		DeleteFile (szTmpFile);
		return IR_S_FALSE;
	}

	
	HANDLE hFile = CreateFile (szTmpFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_FLAG_DELETE_ON_CLOSE, NULL);
	ASSERT (hFile != INVALID_HANDLE_VALUE);
	if (hFile == INVALID_HANDLE_VALUE) {
		DeleteFile (szTmpFile);
		return IR_ERROR;
	}

	TCHAR szBuf [1000];
	DWORD dwSize = 0;
	ReadFile (hFile, szBuf, sizeof (szBuf), &dwSize, NULL);
	CloseHandle (hFile);

	if (dwSize == 0)
	{
		
		
		m_cOpinions = 0;
		m_cMalOpinions = 0;
		m_fRating = 0;
		m_strVirusCheckResult = _T("");
	}
	else
	{
		
		

		szBuf [dwSize] = 0;

		TCHAR szVCR [10000];
		_stscanf (szBuf, _T("%d %f %d %s"), &m_cOpinions, &m_fRating, &m_cMalOpinions, szVCR);
		m_strVirusCheckResult = szVCR;
	}

	return IR_SUCCESS;
}

CString vmsMaliciousDownloadChecker::EncodeUrl(LPCTSTR pszUrl)
{
	CString str;

	while (*pszUrl)
	{
		TCHAR c = *pszUrl++;
		if ((c >= _T('a') && c <= _T('z')) ||
				(c >= _T('A') && c <=_T('Z')) ||
				(c >= _T('0') && c <= _T('9')))
		{
			str += c;
		}
		else
		{
			
			
			TCHAR szHex [10];
			_stprintf (szHex, _T("%%%x"), (int)(BYTE)c);
			str += szHex;
		}
	}

	return str;
}

int vmsMaliciousDownloadChecker::get_OpinionTotalCount()
{
	return m_cOpinions;
}

int vmsMaliciousDownloadChecker::get_MaliciousOpinionCount()
{
	return m_cMalOpinions;
}

float vmsMaliciousDownloadChecker::get_AverageRating()
{
	return m_fRating;
}

LPCTSTR vmsMaliciousDownloadChecker::get_VirusCheckResult()
{
	return m_strVirusCheckResult;
}

void vmsMaliciousDownloadChecker::Stop()
{
	m_bNeedStop = true;
	if (m_dldr)
		m_dldr->Stop ();
}
