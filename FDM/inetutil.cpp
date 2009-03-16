/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "downloadproperties.h"
#include "misc.h"        

void DecodeURLPath (LPTSTR pszPath)
{
	
	TCHAR szURL [10000];
	_tcscpy (szURL, "ftp://f.c");
	if (*pszPath != _T('/') && *pszPath != _T('\\'))
		_tcscat (szURL, _T("/"));
	_tcscat (szURL, pszPath);

	

	fsURL url;
	if (IR_SUCCESS == url.Crack (szURL))
	{
		if (*pszPath != _T('/') && *pszPath != _T('\\'))		
			_tcscpy (pszPath, url.GetPath ()+1);
		else
			_tcscpy (pszPath, url.GetPath ());
	}
}

BOOL fsFilePathFromUrlPath (LPCTSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPTSTR pszBuf, UINT uSize)
{
	*pszBuf = 0;

	if (*pszUrl == 0)
		return TRUE;

	
	int end = bUsingFTP ? _tcslen (pszUrl)-1 : _tcscspn (pszUrl, _T("?=#")) - 1;

	if (end >= (int)uSize)
		return FALSE;

	

	CopyMemory (pszBuf, pszUrl, end+1);
	pszBuf [end+1] = 0;

	if (bUsingFTP == FALSE && bDecode)
		DecodeURLPath (pszBuf);

	return TRUE;
}

BOOL fsPathFromUrlPath (LPCTSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPTSTR pszBuf, UINT uSize)
{
	*pszBuf = 0;

	if (*pszUrl == 0)
		return TRUE;

	int end = bUsingFTP ? _tcslen (pszUrl)-1 : _tcscspn (pszUrl, _T("?#")) - 1;

	if (end >= (int)uSize)
		return FALSE;

	CopyMemory (pszBuf, pszUrl, end+1);
	pszBuf [end+1] = 0;

	

	LPTSTR pszEnd = max (strrchr (pszBuf, _T('/')), _tcsrchr (pszBuf, _T('\\')));

	if (pszEnd == NULL)
		pszEnd = pszBuf;

	*pszEnd = 0;

	if (bUsingFTP == FALSE && bDecode)
		DecodeURLPath (pszBuf);

	return TRUE;
}

BOOL fsIsAnchorInUrl (LPCTSTR pszFullUrl, LPTSTR* ppszWithoutAnchor, LPCTSTR* ppszAnchor = NULL)
{
	fsURL url;
	if (url.Crack (pszFullUrl) != IR_SUCCESS)
		return FALSE;

	if (url.GetInternetScheme () != INTERNET_SCHEME_HTTP && 
			url.GetInternetScheme () != INTERNET_SCHEME_FTP)
		return FALSE;

	LPCTSTR pszPath = url.GetPath ();
	LPCTSTR pszA = _tcschr (pszPath, _T('#'));

	if (pszA == NULL)
		return FALSE;

	
	int len = pszA - pszPath;

	TCHAR szUrl [10000]; DWORD dw = sizeof (szUrl);
	fsURL url2;
	url2.Create (url.GetInternetScheme (), url.GetHostName (), url.GetPort (),
		url.GetUserName (), url.GetPassword (), url.GetPath (), szUrl, &dw);

	if (ppszWithoutAnchor)
	{
		int oldlen = _tcslen (szUrl);
		int oldpathlen = _tcslen (pszPath);
		fsnew (*ppszWithoutAnchor, TCHAR, oldlen - oldpathlen + len + 1);
		CopyMemory (*ppszWithoutAnchor, szUrl, oldlen - oldpathlen + len);
		(*ppszWithoutAnchor) [oldlen - oldpathlen + len] = 0; 

		if (ppszAnchor)
			*ppszAnchor = szUrl + oldlen - oldpathlen + len;
	}

	return TRUE;
}

BOOL fsFileNameFromUrlPath (LPCTSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPTSTR pszBuf, UINT uSize)
{
	*pszBuf = 0;

	int len = _tcslen (pszUrl);
	int pos;

	if (len == 0)
		return TRUE;

	int end = bUsingFTP ? _tcslen (pszUrl)-1 : _tcscspn (pszUrl, _T("?#")) - 1;
	
	if (end < 0)
		return FALSE; 
	
	if (end >= len)
		end = len - 1; 

	pos = end; 

	
	while (pos && pszUrl [pos] != _T('/') && pszUrl [pos] != _T('\\'))
		pos--;

	if (pszUrl [pos] != _T('/') && pszUrl [pos] != _T('\\'))
		return FALSE; 

	if (UINT(len - pos) > uSize)
		return FALSE; 

	_tcscpy (pszBuf, pszUrl + pos + 1);

	pszBuf [end - pos] = 0; 

	if (bUsingFTP == FALSE && bDecode)
		DecodeURLPath (pszBuf);

	if (strchr (pszBuf, _T('\\')) || _tcschr (pszBuf, _T('/')))
	{
		
		TCHAR sz [1000] = _T("");
		fsGetFileName (pszBuf, sz);
		_tcscpy (pszBuf, sz);
	}

	return TRUE;
}

BOOL fsIRToStr (fsInternetResult ir, LPTSTR pszStr, UINT uMaxSize)
{
	LPCTSTR pszDesc;

	switch (ir)
	{
		case IR_WININETUNKERROR:
		case IR_ERROR:
			pszDesc = LS (L_UNKNETERR);
			break;

		case IR_LOGINFAILURE:
			pszDesc = LS (L_INVLOGIN);
			break;

		case IR_INVALIDPASSWORD:
			pszDesc = LS (L_INVPWD);
			break;

		case IR_SUCCESS:
			pszDesc = LS (L_SUCCESS);
			break;

		case IR_CANTCONNECT:
			pszDesc = LS (L_CANTCONNECT);
			break;

		case IR_FILENOTFOUND:
			pszDesc = LS (L_NOTFOUND);
			break;

		case IR_LOSTCONNECTION:
			pszDesc = LS (L_CONNWASLOST);
			break;

		case IR_TIMEOUT:
			pszDesc = LS (L_NOTRESPONSE);
			break;

		case IR_NAMENOTRESOLVED:
			pszDesc = LS (L_NOTRESOLVED);
			break;

		case IR_RANGESNOTAVAIL:
			pszDesc = LS (L_NORESUME);
			break;

		case IR_DOUBTFUL_RANGESRESPONSE:
			pszDesc = LS (L_DOUBTFULRANGESRESPONSE);
			break;

		case IR_PROXYAUTHREQ:
			pszDesc = LS (L_PROXYLOGINREQ);
			break;

		case IR_EXTERROR:
			pszDesc = LS (L_EXTERR);
			break;

		case IR_SERVERBADREQUEST:
			pszDesc = LS (L_BADREQ);
			break;

		case IR_SERVERUNKERROR:
			pszDesc = LS (L_UNKSERVERR);
			break;

		case IR_CONNECTIONABORTED:
			pszDesc = LS (L_CONNABORTED);
			break;

		case IR_BADURL:
			pszDesc = LS (L_BADURL);
			break;

		case IR_NOINTERNETCONNECTION:
			pszDesc = LS (L_NOINETCONN);
			break;

		case IR_HTTPVERNOTSUP:
			pszDesc = LS (L_HTTPNOTSUPP);
			break;

		default:
			pszDesc = LS (L_UNKERR);
	}

	if (strlen (pszDesc) >= uMaxSize)
		return FALSE;

	_tcscpy (pszStr, pszDesc);

	return TRUE;
}

#ifndef FDM_DLDR__RAWCODEONLY
DWORD fsNPToSiteValidFor (fsNetworkProtocol np)
{
	switch (np)
	{
		case NP_HTTP:
			return SITE_VALIDFOR_HTTP;

		case NP_HTTPS:
			return SITE_VALIDFOR_HTTPS;

		case NP_FTP:
			return SITE_VALIDFOR_FTP;

		default:
			return 0;
	}
}
#endif

ULONG fsGetSiteIp (LPCTSTR pszSite)
{
	hostent* he = gethostbyname (pszSite);

	if (he == NULL)
		return 0;

	return *((ULONG*) he->h_addr_list [0]);

	
}

void vmsMakeWinInetProxy (LPCTSTR pszProxy, fsNetworkProtocol npConnection, fsNetworkProtocol npProxy, LPTSTR pszWProxy)
{
	switch (npConnection)
	{
		case NP_FTP:
			_tcscpy (pszWProxy, _T("ftp="));
			break;

		case NP_HTTP:
		case NP_FILE:
			_tcscpy (pszWProxy, _T("http="));
			break;

		case NP_HTTPS:
			_tcscpy (pszWProxy, _T("https="));
			break;

		default:
			ASSERT (FALSE);
			return;
	}

	switch (npProxy)
	{
		case NP_FTP:
			_tcscat (pszWProxy, "ftp://");
			break;

		case NP_HTTP:
		case NP_FILE:
			_tcscat (pszWProxy, "http://");
			break;

		case NP_HTTPS:
			_tcscat (pszWProxy, "https://");
			break;

		default:
			ASSERT (FALSE);
			return;
	}

	_tcscat (pszWProxy, pszProxy);
}

void fsDecodeHtmlText (fsString &str)
{
	CString str2 = str;
	str2.Replace (_T("&amp;"), _T("&"));
	str2.Replace (_T("&lt;"), _T("<"));
	str2.Replace (_T("&gt;"), _T(">"));
	str2.Replace (_T("&quot;"), _T("\""));
	str2.Replace (_T("&nbsp;"), _T(" "));
	str = str2;
}

void fsDecodeHtmlUrl (fsString &str)
{
	CString str2;
	int len = str.GetLength ();
	for (int i = 0; i < len; )
	{
		if (str [i] == _T('\\') && str [i+1] == _T('u') && str.GetLength () - i >= 6)
		{
			
			TCHAR sz [5];
			sz [0] = str [i+2];
			sz [1] = str [i+3];
			sz [2] = str [i+4];
			sz [3] = str [i+5];
			sz [4] = 0;
			int c;
			_stscanf (sz, _T("%x"), &c);
			if (c < 127)
				str2 += (TCHAR)c; 
			i += 6;
		}
		else if (str [i] == _T('%') && str [i+1] != _T('%') && str.GetLength () - i >= 3)
		{
			
			TCHAR sz [3];
			sz [0] = str [i+1];
			sz [1] = str [i+2];
			sz [2] = 0;
			int c;
			_stscanf (sz, _T("%x"), &c);
			str2 += (TCHAR)c;
			i += 3;
		}
		else if (str [i] == _T('&') && str [i+1] == _T('#'))
		{
			CString strC;
			int j = i+2;
			while (isdigit (str [j]))
				strC += str [j++];
			if (str [j] == _T(';'))
			{
				
				int c = _ttoi (strC);
				str2 += (TCHAR)c;
				i = j + 1;
			}
			else
			{
				str2 += str [i++];
			}
		}
		else
		{
			str2 += str [i++];
		}
	}

	str = str2;

	fsDecodeHtmlText (str);
}
