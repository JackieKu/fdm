/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSFIREFOXMONITORING_H__5A4260CA_6142_4971_8707_4D8DAD852589__INCLUDED_)
#define AFX_VMSFIREFOXMONITORING_H__5A4260CA_6142_4971_8707_4D8DAD852589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsFirefoxMonitoring  
{
public:
	
	static bool Install (bool bInstall);
	
	static bool IsFlashGotInstalled();
	
	static bool IsInstalled();

	vmsFirefoxMonitoring();
	virtual ~vmsFirefoxMonitoring();

protected:
};

#endif 
