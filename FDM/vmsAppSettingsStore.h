/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSAPPSETTINGSSTORE_H__232450C9_092A_49EF_9B4E_3D2DA2B9E736__INCLUDED_)
#define AFX_VMSAPPSETTINGSSTORE_H__232450C9_092A_49EF_9B4E_3D2DA2B9E736__INCLUDED_

#include "vmsDataFile.h"	
#if _MSC_VER > 1000
#pragma once
#endif 

class vmsAppSettingsStore  
{
public:
	void SaveSettingsToFile(LPCTSTR pszFile);
	void LoadSettingsFromFile (LPCTSTR pszFile);

	BOOL WriteProfileBinary (LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE pbData, UINT nBytes);
	BOOL GetProfileBinary (LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE* ppData, UINT* pBytes);

	BOOL WriteProfileString(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue);
	CString GetProfileString(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault);

	BOOL WriteProfileInt (LPCTSTR pszSection, LPCTSTR pszEntry, int nValue);
	UINT GetProfileInt (LPCTSTR pszSection, LPCTSTR pszEntry, INT nDefault);

	vmsAppSettingsStore();
	virtual ~vmsAppSettingsStore();

protected:
	
	bool m_bUseRegistry;
	vmsDataFile m_file;
	CWinApp* m_app;
};

#endif 
