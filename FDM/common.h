/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __COMMON_H_
#define __COMMON_H_

#define SAFE_DELETE(p) do {if (p) {delete p; p = NULL;}} while (0)
#define SAFE_DELETE_ARRAY(a) do {if (a) {delete [] a; a = NULL;}} while (0)
#define SAFE_RELEASE(p) do {if (p) {p->Release (); p = NULL;}} while (0)

#define RIF(e) do {if (FAILED (hr=(e))) return hr;} while (0)
                 
#define Is9xME ((GetVersion () & 0x80000000) != 0)

inline int IsValidUTF8(LPCSTR str, DWORD cch = -1) { return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, cch, NULL, 0); }
inline int IsValidACP(LPCSTR str, DWORD cch = -1) { return MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str, cch, NULL, 0); }

#ifdef __ATLCONV_H__
class CU2W : public CA2W
{
public:
	CU2W(LPCSTR str) : CA2W((str), IsValidUTF8(str) ? CP_UTF8 : CP_ACP) {}
};

class CW2U : public CW2A
{
public:
	CW2U(LPCWSTR str) : CW2A((str), CP_UTF8) {}
};
#endif

#endif