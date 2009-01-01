/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSWEBPAGEDOWNLOADSMGR_H__856D7E6C_DA34_4BA8_B335_8B32762296EA__INCLUDED_)
#define AFX_FSWEBPAGEDOWNLOADSMGR_H__856D7E6C_DA34_4BA8_B335_8B32762296EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"
#include "fsWebPageDownloader.h"  

#define SPIDERFILE_CURRENT_VERSION	(6)
#define SPIDERFILE_SIG     "FDM Web Pages  "
struct fsSpiderFileHdr
{
	char szSig [sizeof (SPIDERFILE_SIG) + 1];
	WORD wVer;

	fsSpiderFileHdr ()
	{
		strcpy (szSig, SPIDERFILE_SIG);
		wVer = SPIDERFILE_CURRENT_VERSION;
	}
};  

class fsWebPageDownloadsMgr  
{
public:
	bool IsDownloadsMgrRequired();
	
	void StopAll();
	
	BOOL OnDownloadRestored (vmsDownloadSmartPtr dld);
	
	BOOL Load();
	BOOL Save();
	
	void Delete (fsWebPageDownloader* wpd);
	
	fsWebPageDownloader* GetWPD (int iIndex);
	
	int GetWPDCount();
	
	void SetEventsFunc (fntWPDEvents pfn, LPVOID lp);
	
	void Add (fsWebPageDownloader* wpd);

	fsWebPageDownloadsMgr();
	virtual ~fsWebPageDownloadsMgr();

protected:
	fntWPDEvents m_pfnEvents;
	LPVOID m_lpEventsParam;
	static void _DownloaderEvents (fsWebPageDownloader* dldr, fsWPDEvent ev, int info, LPVOID lp);

	fs::list <fsWebPageDownloader*> m_vWPDs;	
	
};

#endif 
