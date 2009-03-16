/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsLangMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsLangMgr::fsLangMgr()
{
	m_iCurLng = -1;
}

fsLangMgr::~fsLangMgr()
{

}  

#define LNG_COMMENT_CHAR	_T(';')

BOOL fsLangMgr::Initialize()
{
	LoadBuiltInLngStrings ();

	WIN32_FIND_DATA wfd;

	m_strLngFolder = ((CFdmApp*)AfxGetApp ())->m_strAppPath;
	m_strLngFolder += _T("Language\\");

	CString strMask = m_strLngFolder;
	strMask += _T("*.lng");

	HANDLE hFind = FindFirstFile (strMask, &wfd);
	BOOL bFind = hFind != INVALID_HANDLE_VALUE;

	while (bFind)
	{
		try {
		fsLngFileInfo info;
		info.strFileName = wfd.cFileName;

		CStdioFile file (m_strLngFolder + info.strFileName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone);

		
		while (file.ReadString (info.strLngName))
		{
			if (info.strLngName.GetLength () && info.strLngName [0] != LNG_COMMENT_CHAR)
			{
				AddLngFileInfo (info);
				break;
			}
		}
		}
		catch (...){}

		bFind = FindNextFile (hFind, &wfd);
	}

	if (hFind != INVALID_HANDLE_VALUE)
		FindClose (hFind);

	return m_vLngFiles.size () != 0;
}

int fsLangMgr::GetLngCount()
{
	return m_vLngFiles.size ();
}

LPCTSTR fsLangMgr::GetLngName(int iIndex)
{
	static TCHAR szBuiltIn [] = _T("Built In");
	if (iIndex < 0 || iIndex >= m_vLngFiles.size ())
		return szBuiltIn;
	return m_vLngFiles [iIndex].strLngName;
}

BOOL fsLangMgr::LoadLng(int iIndex)
{
	try {
	
	CStdioFile file (m_strLngFolder + m_vLngFiles [iIndex].strFileName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone);
	CString str;

	
	do
	{
		if (FALSE == file.ReadString (str))
			return FALSE;

		if (str.GetLength () && str [0] != LNG_COMMENT_CHAR)
			break;
	}
	while (TRUE);

	m_vStrings.clear ();

	
	while (file.ReadString (str))
	{
		if (str.GetLength () == 0 || str [0] == LNG_COMMENT_CHAR)
			continue; 

		PreprocessLanguageString (str);

		m_vStrings.add (str);
	}

	m_iCurLng = iIndex;

	return m_vStrings.size () != 0;

	}
	catch (...)
	{
		return FALSE;
	}
}

int fsLangMgr::FindLngByName(LPCTSTR pszName)
{
	for (int i = 0; i < m_vLngFiles.size (); i++)
	{
		if (m_vLngFiles [i].strLngName == pszName || 
				lstrcmpi (m_vLngFiles [i].strFileName, pszName) == 0)
		{
			return i;
		}
	}

	return -1;
}

int fsLangMgr::GetStringCount()
{
	return m_vStrings.size ();
}

LPCTSTR fsLangMgr::GetString(int iIndex)
{
	static TCHAR szNull [1] = {0};

	try{

	if (iIndex < m_vStrings.size ()) 
		return m_vStrings [iIndex];
	else if (iIndex < m_vBuiltInStrings.size ()) 
		return m_vBuiltInStrings [iIndex];
	else
		return szNull;

	}
	catch (...) {ASSERT (FALSE); return szNull;}
}

int fsLangMgr::GetCurLng()
{
	return m_iCurLng;
}

CString fsLangMgr::GetStringNP(int iIndex)
{
	CString str = GetString (iIndex);
	str.Remove (_T('&'));
	return str;
}

void fsLangMgr::AddLngFileInfo(fsLngFileInfo &info)
{
	for (int i = 0; i < m_vLngFiles.size (); i++)
	{
		if (stricmp (m_vLngFiles [i].strLngName, info.strLngName) > 0)
		{
			m_vLngFiles.insert (i, info);
			return;
		}
	}

	m_vLngFiles.add (info);
}

void fsLangMgr::LoadBuiltInLngStrings()
{
	HGLOBAL hResource;
	HRSRC hRes;
	LPBYTE pbRes;

	hRes = FindResource (NULL, MAKEINTRESOURCE (IDR_ENG_LNGSTRINGS), _T("RT_LNGSTRINGS"));
	if (hRes == NULL)
		return;

	hResource = LoadResource (NULL, hRes);
	pbRes = (LPBYTE) LockResource (hResource);
	LPBYTE pbResEnd = pbRes + SizeofResource (NULL, hRes);

	LPCTSTR psz = (LPCTSTR) pbRes;
	LPCTSTR pszE = (LPCTSTR) pbResEnd;
	bool bLngNamePassed = false;

	while (psz < pszE) 
	{
		CString str;
		while (psz < pszE && *psz != _T('\n') && *psz != _T('\r'))
			str += *psz++;
		while (psz < pszE && (*psz == _T('\n') || *psz == _T('\r')))
			psz++;

		if (str == _T("") || str [0] == LNG_COMMENT_CHAR)
			continue;

		if (bLngNamePassed == false) {
			bLngNamePassed = true;
			continue;
		}

		PreprocessLanguageString (str);
		m_vBuiltInStrings.add (str);
	}
}

void fsLangMgr::PreprocessLanguageString(CString &str)
{
	str.Replace (_T("\\n"), _T("\n"));
}
