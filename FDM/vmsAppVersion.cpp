/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsAppVersion.h"        

vmsAppVersion::vmsAppVersion()
{

}

vmsAppVersion::~vmsAppVersion()
{

}

BOOL vmsAppVersion::FromString(LPCTSTR ptszVersion)
{
	clear ();

	while (*ptszVersion == _T(' '))
		ptszVersion++;

	if (*ptszVersion == _T('v') || *ptszVersion == _T('V'))
		ptszVersion++;
	
	while (*ptszVersion)
	{
		while (*ptszVersion == _T(' '))
			ptszVersion++;
		std::string tstrVer;
		while (_istdigit (*ptszVersion))
			tstrVer += *ptszVersion++;
		if (tstrVer.empty ())
			break;
		while (*ptszVersion == _T(' '))
			ptszVersion++;
		if (*ptszVersion == _T('.') || *ptszVersion == _T(','))
			ptszVersion++;
		push_back (vmsAppVersionPart (tstrVer.c_str ()));
	}
	
	if (size () && *ptszVersion)
	{
		while (ptszVersion [-1] == _T(' '))
			ptszVersion--;
		m_strReleaseType = ptszVersion;
	}

	return size () != 0;
}

std::string vmsAppVersion::ToString() const
{
	std::string tstrVer;
	for (size_t i = 0; i < size (); i++)
	{
		if (!tstrVer.empty ())
			tstrVer += _T (".");
		tstrVer += at (i).toString ();
	}
	tstrVer += m_strReleaseType;
	return tstrVer;
}

void vmsAppVersion::clear()
{
	using namespace std;
	vector<vmsAppVersionPart>::clear ();
	m_strReleaseType = _T ("");
}
