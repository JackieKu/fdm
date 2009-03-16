/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsUploaderDllPersist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsUploaderDllPersist::vmsUploaderDllPersist()
{

}

vmsUploaderDllPersist::~vmsUploaderDllPersist()
{

}

BOOL vmsUploaderDllPersist::GetProfileBinary(LPCTSTR pszSection, LPCTSTR pszName, LPBYTE *ppData, UINT *pnDataSize)
{
	return _App.get_SettingsStore ()->GetProfileBinary (pszSection, pszName, ppData, pnDataSize);
}

void vmsUploaderDllPersist::WriteProfileBinary(LPCTSTR pszSection, LPCTSTR pszName, LPVOID pvData, UINT nDataSize)
{
	_App.get_SettingsStore ()->WriteProfileBinary (pszSection, pszName, (LPBYTE)pvData, nDataSize);
}

void vmsUploaderDllPersist::FreeBuffer(LPBYTE pb)
{
	delete [] pb;
}

UINT vmsUploaderDllPersist::GetProfileInt(LPCTSTR pszSection, LPCTSTR pszName, UINT nDefValue)
{
	return _App.get_SettingsStore ()->GetProfileInt (pszSection, pszName, nDefValue);
}

void vmsUploaderDllPersist::WriteProfileInt(LPCTSTR pszSection, LPCTSTR pszName, UINT nValue)
{
	_App.get_SettingsStore ()->WriteProfileInt (pszSection, pszName, nValue);
}

BOOL vmsUploaderDllPersist::GetProfileString (LPCTSTR pszSection, LPCTSTR pszName, LPCTSTR pszDefValue, LPTSTR pszValue, DWORD *pdwValueSize)
{
	CString str = _App.get_SettingsStore ()->GetProfileString (pszSection, pszName, pszDefValue);
	if (*pdwValueSize <= (DWORD)str.GetLength ())
	{
		*pdwValueSize = str.GetLength () + 1;
		return FALSE;
	}
	_tcscpy (pszValue, str);
	return TRUE;
}

void vmsUploaderDllPersist::WriteProfileString (LPCTSTR pszSection, LPCTSTR pszName, LPCTSTR pszValue)
{
	_App.get_SettingsStore ()->WriteProfileString (pszSection, pszName, pszValue);
}