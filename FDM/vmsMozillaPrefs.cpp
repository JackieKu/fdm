/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsMozillaPrefs.h"        

vmsMozillaPrefs::vmsMozillaPrefs()
{

}

vmsMozillaPrefs::~vmsMozillaPrefs()
{

}

bool vmsMozillaPrefs::LoadPrefs(LPCTSTR pszFile)
{
	Free ();

	

	HANDLE hFile = CreateFile (pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	
	int n = GetFileSize (hFile, NULL);
	m_strPrefs.pszString = new TCHAR [n+1];

	DWORD dw;
	ReadFile (hFile, m_strPrefs.pszString, n, &dw, NULL);
	m_strPrefs.pszString [n] = 0;

	CloseHandle (hFile);

	return true;
}

void vmsMozillaPrefs::Free()
{
	if (m_strPrefs.pszString) {
		delete [] m_strPrefs.pszString;
		m_strPrefs.pszString = 0;
	}
}

fsString vmsMozillaPrefs::get_Value(LPCTSTR pszPrefName) const
{
	LPCTSTR pszPrefs = m_strPrefs.pszString;

	fsString strPrefName; 
	strPrefName = _T("\""); 
	strPrefName += pszPrefName;
	strPrefName += _T("\""); 

	

	while (*pszPrefs && strnicmp (pszPrefs, strPrefName, strPrefName.Length ()))
		pszPrefs++;

	if (*pszPrefs == 0)
		return MOZILLAPREFS_VALUE_NOT_FOUND;

	pszPrefs += strPrefName.Length ();
	
	
	while (*pszPrefs && *pszPrefs != _T(','))
		pszPrefs++;

	if (*pszPrefs == 0)
		return MOZILLAPREFS_UNEXPECTED_ERROR;

	
	pszPrefs++;
	while (*pszPrefs == _T(' ') || *pszPrefs == _T('\t'))
		pszPrefs++;

	bool bInQ = false; 
	if (*pszPrefs == _T('"')) {
		pszPrefs++;
		bInQ = true;
	}

	fsString strValue;

	
	
	if (bInQ)
	{
		while (*pszPrefs && *pszPrefs != _T('"'))
		{
			TCHAR c = *pszPrefs++;

			
			if (c == _T('\\'))
			{
				switch (*pszPrefs)
				{
				case _T('n'): c = _T('\n'); break;
				case _T('t'): c = _T('\t'); break;
				case _T('r'): c = _T('\r'); break;
				default: c = *pszPrefs; break;
				}

				pszPrefs++;
			}

			strValue += c;
		}
	}
	else
	{
		while (IsCharAlphaNumeric (*pszPrefs))
			strValue += *pszPrefs++;
	}

	return strValue;
}
