/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsFirefoxUtil.h"
#include "vmsFileUtil.h"        

vmsFirefoxUtil::vmsFirefoxUtil()
{

}

vmsFirefoxUtil::~vmsFirefoxUtil()
{

}

void vmsFirefoxUtil::GetProfilesPath(LPTSTR pszPath)
{
	vmsFirefoxUtil::GetDataPath (pszPath);
	_tcscat (pszPath, _T("Profiles\\"));
}

void vmsFirefoxUtil::GetDataPath(LPTSTR pszPath)
{
	vmsFileUtil::GetAppDataPath (_T("Mozilla"), pszPath);
	vmsFileUtil::MakePathOK (pszPath, true);
	_tcscat (pszPath, _T("Firefox\\"));
}

bool vmsFirefoxUtil::GetProfilesPathes(FU_STRINGLIST &v, int &nDefaultProfile)
{
try {
	TCHAR szPath [MY_MAX_PATH];
	GetDataPath (szPath);

	

	TCHAR szProfilesIni [MY_MAX_PATH];
	_tcscpy (szProfilesIni, szPath);
	_tcscat (szProfilesIni, _T("profiles.ini"));

	if (GetFileAttributes (szProfilesIni) != DWORD (-1))
	{
		nDefaultProfile = -1;

		for (int i = 0;; i++)
		{
			TCHAR sz [100], szPP [MY_MAX_PATH] = _T(""); 
			_stprintf (sz, _T("Profile%d"), i);
			GetPrivateProfileString (sz, _T("Path"), _T(""), szPP, sizeof (szPP), szProfilesIni);
			if (*szPP == 0)
				break; 
			LPTSTR psz = szPP;
			while (*psz) {
				if (*psz == _T('/'))
					*psz = _T('\\');
				psz++;
			}
			if (szPP [1] == _T(':')) 
			{
				v.add (szPP);
			}
			else
			{
				
				TCHAR sz [MY_MAX_PATH];
				_tcscpy (sz, szPath);
				_tcscat (sz, szPP);
				v.add (sz);
			}

			if (nDefaultProfile == -1 &&
					GetPrivateProfileInt (sz, _T("Default"), 0, szProfilesIni))
				nDefaultProfile = i; 
		}
	}

	if (v.size () == 0)
	{
		

		

		TCHAR szProfiles [MY_MAX_PATH];
		vmsFirefoxUtil::GetProfilesPath (szProfiles);
		_tcscat (szProfiles, _T("*"));

		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile (szProfiles, &wfd);
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		do
		{
			if (0 == lstrcmp (wfd.cFileName, _T(".")) || 0 == lstrcmp (wfd.cFileName, _T("..")))
				continue;

			TCHAR sz [MY_MAX_PATH];
			_tcscpy (sz, szPath);
			_tcscat (sz, wfd.cFileName);

			DWORD dw = GetFileAttributes (sz);
			if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY))
			{
				TCHAR sz2 [MY_MAX_PATH];
				_tcscpy (sz2, sz);
				_tcscat (sz2, _T("\\extensions\\"));
				
				DWORD dw = GetFileAttributes (sz2);
				if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY))
					v.add (sz); 
			}
		}
		while (FindNextFile (hFind, &wfd));

		FindClose (hFind);
	}

	return true;

}catch (...) {return false;}
}

bool vmsFirefoxUtil::GetDefaultProfilePath(LPTSTR pszPath)
{
	FU_STRINGLIST v; int nDefaultProfile;

	if (false == GetProfilesPathes (v, nDefaultProfile))
		return false;

	if (nDefaultProfile == -1)
		nDefaultProfile = 0;

	_tcscpy (pszPath, v [nDefaultProfile]);

	return  true;
}
