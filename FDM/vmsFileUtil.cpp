/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsFileUtil.h"
#include <shlobj.h>        

void vmsFileUtil::WriteString(vmsFile &file, LPCTSTR psz)
{
	int l = lstrlen (psz);
	file.Write (&l, sizeof (l));
	file.Write (psz, l);
}

void vmsFileUtil::ReadString(vmsFile &file, fsString &str)
{
	int l = 0;
	file.Read (&l, sizeof (l));

	if (l > 100 * 1024 * 1024)	
		throw ERROR_BAD_FORMAT;

	TCHAR* psz = new TCHAR [l+1];
	try {
		file.Read (psz, l);
		psz [l] = 0;
	} catch (DWORD dw) {
		delete [] psz;
		throw dw;
	}

	str = psz;
	delete [] psz;
}

void vmsFileUtil::WriteHeader(vmsFile& file, LPCTSTR pszDesc, WORD wVersion)
{
	WriteString (file, pszDesc);
	file.Write (&wVersion, sizeof (WORD));
}

void vmsFileUtil::ReadHeader(vmsFile& file, fsString& strDesc, WORD &wVersion)
{
	ReadString (file, strDesc);
	file.Read (&wVersion, sizeof (WORD));
}

void vmsFileUtil::MakePathOK(LPTSTR szPath, bool bNeedBackslashAtEnd)
{
	LPTSTR psz = szPath;
	while (*psz)
	{
		if (*psz == _T('/'))
			*psz = _T('\\');
		psz++;
	}

	if (bNeedBackslashAtEnd && psz [-1] != _T('\\'))
	{
		psz [0] = _T('\\');
		psz [1] = 0;
	}
}

void vmsFileUtil::GetAppDataPath(LPCTSTR pszAppName, LPTSTR szPath)
{
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation (NULL, CSIDL_APPDATA, &pidl);
	SHGetPathFromIDList (pidl, szPath);

	vmsFileUtil::MakePathOK (szPath);
	_tcscat (szPath, pszAppName);
	_tcscat (szPath, _T("\\"));
}

void vmsFileUtil::BuildPathToFile(LPCTSTR pszPathName)
{
	BuildPath (GetPathFromPathName (pszPathName));
}

fsString vmsFileUtil::GetPathFromPathName(LPCTSTR pszPathName)
{
	TCHAR szPath [MY_MAX_PATH];
	_tcscpy (szPath, pszPathName);
	MakePathOK (szPath, false);
	LPTSTR psz = _tcsrchr (szPath, _T('\\'));
	if (psz == NULL)
		return _T("");
	*psz = 0;
	return szPath;
}

void vmsFileUtil::BuildPath(LPCTSTR pszPath)
{
	TCHAR szPath [MY_MAX_PATH];
	_tcscpy (szPath, pszPath);
	MakePathOK (szPath, true);
	LPTSTR psz = szPath;
	if (psz [1] == _T(':'))
		psz += 3;

	while (*psz)
	{
		TCHAR szPathNow [MY_MAX_PATH];
		psz = _tcschr (psz, _T('\\')) + 1;
		_tcsncpy (szPathNow, szPath, psz - szPath);
		if (FALSE == CreateDirectory (szPathNow, NULL) && 
				ERROR_ALREADY_EXISTS != GetLastError ())
			throw GetLastError ();
	}
}
