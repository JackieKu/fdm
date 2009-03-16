/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsUnRARDLL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsUnRARDLL::vmsUnRARDLL(LPCTSTR pszDll) :
	vmsDLL (pszDll)
{
	m_pfnRAROpenArchiveEx = NULL;
	m_pfnRARSetCallback = NULL;
	m_pfnRARReadHeader = NULL;
	m_pfnRARProcessFile = NULL;
	m_pfnRARCloseArchive = NULL;
}

vmsUnRARDLL::~vmsUnRARDLL()
{

}

bool vmsUnRARDLL::Load(LPCTSTR pszDll)
{
	if (false == vmsDLL::Load (pszDll))
		return false;

	m_pfnRAROpenArchiveEx = (FNRAROAEX) GetProcAddress (_T("RAROpenArchiveEx"));
	m_pfnRARSetCallback = (FNRARSC) GetProcAddress (_T("RARSetCallback"));
	m_pfnRARReadHeader = (FNRARRH) GetProcAddress (_T("RARReadHeader"));
	m_pfnRARProcessFile = (FNRARPF) GetProcAddress (_T("RARProcessFile"));
	m_pfnRARCloseArchive = (FNRARCA) GetProcAddress (_T("RARCloseArchive"));

	return true;
}

void vmsUnRARDLL::Free()
{
	m_pfnRAROpenArchiveEx = NULL;
	m_pfnRARSetCallback = NULL;
	m_pfnRARReadHeader = NULL;
	m_pfnRARProcessFile = NULL;
	m_pfnRARCloseArchive = NULL;
	vmsDLL::Free ();
}

HANDLE vmsUnRARDLL::RAROpenArchiveEx(RAROpenArchiveDataEx *ArchiveData)
{
	if (m_pfnRAROpenArchiveEx == NULL)
		return NULL;

	return m_pfnRAROpenArchiveEx (ArchiveData);
}

void vmsUnRARDLL::RARSetCallback(HANDLE hArcData, UNRARCALLBACK Callback, LONG UserData)
{
	if (m_pfnRARSetCallback == NULL)
		return;

	m_pfnRARSetCallback (hArcData, Callback, UserData);
}

int vmsUnRARDLL::RARReadHeader(HANDLE hArcData, RARHeaderData *HeaderData)
{
	if (m_pfnRARReadHeader == NULL)
		return 0;

	return m_pfnRARReadHeader (hArcData, HeaderData);
}

int vmsUnRARDLL::RARProcessFile(HANDLE hArcData, int Operation, TCHAR *DestPath, TCHAR *DestName)
{
	if (m_pfnRARProcessFile == NULL)
		return 0;

	return m_pfnRARProcessFile (hArcData, Operation, DestPath, DestName);
}

int vmsUnRARDLL::RARCloseArchive(HANDLE hArcData)
{
	if (m_pfnRARCloseArchive == NULL)
		return 0;

	return m_pfnRARCloseArchive (hArcData);
}
