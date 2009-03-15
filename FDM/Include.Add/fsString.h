/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __FXS_STRING_H_
#define __FXS_STRING_H_

#include <tchar.h>
#include <stdio.h>

struct fsString
{
	LPTSTR pszString;

	fsString () 
	{
		pszString = NULL;
	}

	~fsString ()
	{
		if (pszString)
			delete [] pszString;
	}

	fsString (const fsString& str)
	{
		pszString = NULL;
		*this = str.pszString;
	}

	fsString (LPCTSTR str)
	{
		pszString = NULL;
		*this = str;
	}

	LPCTSTR operator = (LPCTSTR pszStr)
	{
		if (pszString)
		{
			delete [] pszString;
			pszString = NULL;
		}

		if (pszStr)
		{
           	pszString = new TCHAR [_tcslen (pszStr) + 1];
			if (pszString)
				_tcscpy ( pszString, pszStr );
		}

		return pszString;
	}

	fsString& operator = (const fsString& str)
	{
		*this = str.pszString;
		return *this;
	}

	LPCTSTR operator += (LPCTSTR pszStr)
	{
		if (pszStr == NULL)
			return pszString;

		if ( pszString )
		{
			LPTSTR pszOld = pszString;

			pszString = new TCHAR [ _tcslen (pszString) + _tcslen (pszStr) + 1 ];
			_tcscpy ( pszString, pszOld );
			_tcscat ( pszString, pszStr );

			delete [] pszOld;
		}
		else
		{
			*this = pszStr;
		}

		return pszString;
	}

	LPCTSTR  operator += (TCHAR c)
	{
			TCHAR sz [2];
			sz [0] = c; sz [1] = 0;
			return *this += sz;
	}

	fsString operator + (LPCTSTR psz) const
	{
		fsString str = *this;
		str += psz;
		return str;
	}

	friend BOOL operator == (const fsString& str1, const fsString& str2)
	{
		return str1 == (LPCTSTR) str2.pszString;
	}

	friend BOOL operator != (const fsString& str1, const fsString& str2)
	{
		return !(str1 == str2);
	}

	friend BOOL operator == (const fsString& str, LPCTSTR pszStr)
	{
		if (str.pszString == NULL || pszStr == NULL)
			return pszStr == str.pszString;

		return _tcscmp ( str.pszString, pszStr ) == 0;
	}

	void clear ()
	{
		if (pszString)
		{
			delete [] pszString;
			pszString = NULL;
		}
	}

	void ncpy (LPCTSTR pszStr, int nch)
	{
		alloc (nch);
		_tcsncpy (pszString, pszStr, nch);
	}

	void alloc (int nch)
	{
		clear ();
		pszString = new TCHAR [nch+1];
		pszString [nch] = 0;
	}

	friend BOOL operator != (const fsString& str, LPCTSTR pszStr)
	{
		return !(str == pszStr);
	}

	operator LPTSTR () const
	{
		return pszString;
	}

	int Length () const
	{
		if (pszString)
			return _tcslen (pszString);
		else
			return 0;
	}

	BOOL IsEmpty () const
	{
		return pszString == NULL || *pszString == 0;
	}

	TCHAR LastChar () const
	{
		return pszString ? pszString [GetLength () - 1] : (TCHAR)0;
	}

	int GetLength () const {return Length ();}

	void Replace (LPCTSTR , LPCTSTR ) {}

	void Format (LPCTSTR pszFormat ...)
	{
		LPTSTR psz = new TCHAR [100000];
		va_list ap;
		va_start (ap, pszFormat);
		_vstprintf (psz, pszFormat, ap);
		va_end (ap);
		*this = psz;
		delete [] psz;
	}
};

#endif