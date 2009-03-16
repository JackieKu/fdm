/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsClipboardMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsClipboardMgr::fsClipboardMgr()
{

}

fsClipboardMgr::~fsClipboardMgr()
{

}

void fsClipboardMgr::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
}

LPCTSTR fsClipboardMgr::Text()
{
	if (FALSE == OpenClipboard (m_hWnd))
		return NULL;

	HANDLE hMem = GetClipboardData (CF_TEXT);

	if (hMem == NULL)
	{
		CloseClipboard ();
		return FALSE;
	}

	LPCTSTR pszText = (LPCTSTR) GlobalLock (hMem);
	m_strText = pszText;
	GlobalUnlock (hMem);

	CloseClipboard ();

	return m_strText;
}

BOOL fsClipboardMgr::Text(LPCTSTR psz)
{
	if (FALSE == OpenClipboard (m_hWnd))
		return FALSE;

	EmptyClipboard ();

	HANDLE hText = GlobalAlloc (GMEM_MOVEABLE, _tcslen (psz)+1);
	if (hText == NULL)
	{
		CloseClipboard ();
		return FALSE;
	}

	LPTSTR pszText = (LPTSTR) GlobalLock (hText);
	_tcscpy (pszText, psz);
	GlobalUnlock (hText);

	SetClipboardData (CF_TEXT, hText);

	CloseClipboard ();

	return TRUE;
}
