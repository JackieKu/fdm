/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __INETUTIL_H_
#define __INETUTIL_H_

#include "downloadproperties.h"  

extern BOOL fsFileNameFromUrlPath (LPCTSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPTSTR pszBuf, UINT uSize);

extern BOOL fsIRToStr (fsInternetResult ir, LPTSTR pszStr, UINT uMaxSize);

extern BOOL fsFilePathFromUrlPath (LPCTSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPTSTR pszBuf, UINT uSize);

extern BOOL fsPathFromUrlPath (LPCTSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPTSTR pszBuf, UINT uSize);      

extern BOOL fsIsAnchorInUrl (LPCTSTR pszFullUrl, LPTSTR* ppszWithoutAnchor, LPCTSTR* ppszAnchor = NULL);

extern DWORD fsNPToSiteValidFor (fsNetworkProtocol np);

extern void vmsMakeWinInetProxy (LPCTSTR pszProxy, fsNetworkProtocol npConnection, fsNetworkProtocol npProxy, LPTSTR pszWProxy);  

extern ULONG fsGetSiteIp (LPCTSTR pszSite);  

extern void fsDecodeHtmlUrl (fsString &str);
extern void fsDecodeHtmlText (fsString &str);

#endif