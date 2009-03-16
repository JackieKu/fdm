/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsTorrentExtension.h"
#include "vmsFileExtensionInOs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsTorrentExtension::vmsTorrentExtension()
{

}

vmsTorrentExtension::~vmsTorrentExtension()
{

}

BOOL vmsTorrentExtension::Associate()
{
	return AssociateWith (get_ShellOpenCommandLine ());
}

fsString vmsTorrentExtension::get_ShellOpenCommandLine()
{
	fsString str = _T("\"");
	str += ((CFdmApp*)AfxGetApp ())->m_strAppPath;
	if (str [str.GetLength () - 1] != _T('\\'))
		str += _T('\\');
	str += _T("fdm.exe\" \"%1\"");
	return str;
}

BOOL vmsTorrentExtension::IsAssociationExist()
{
	return GetCurrentAssociation ().IsEmpty () == FALSE;
}

BOOL vmsTorrentExtension::IsAssociatedWithUs()
{
	return stricmp (vmsFileExtensionInOs::GetAssociation (_T("torrent"), _T("open")),
		get_ShellOpenCommandLine ()) == 0;
}

fsString vmsTorrentExtension::GetCurrentAssociation()
{
	return vmsFileExtensionInOs::GetAssociation (_T("torrent"), _T("open"));
}

BOOL vmsTorrentExtension::AssociateWith(LPCTSTR pszCmdLine)
{
	if (pszCmdLine && *pszCmdLine)
	{
		return vmsFileExtensionInOs::SetAssociation (_T("torrent"), _T("open"), pszCmdLine);
	}
	else
	{
		CRegKey key;
		if (ERROR_SUCCESS != key.Open (HKEY_CLASSES_ROOT, _T("")))
			return FALSE;
		return ERROR_SUCCESS == key.RecurseDeleteKey (_T(".torrent"));
	}
}

