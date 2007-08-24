/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include <windows.h>
#include "common.h"
#include "fsInternetFiles.h"        

fsInternetFiles::fsInternetFiles()
{
	m_strPath = "";
}

fsInternetFiles::~fsInternetFiles()
{

}

UINT fsInternetFiles::GetFileCount()
{
	return m_vFiles.size ();
}

fsFileInfo* fsInternetFiles::GetFileInfo(UINT uIndex)
{
	return &m_vFiles [uIndex];
}

LPCSTR fsInternetFiles::GetCurrentPath()
{
	return m_strPath;
}
