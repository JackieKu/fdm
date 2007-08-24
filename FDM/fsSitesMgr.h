/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSSITESMGR_H__211935DF_F52C_4919_ADEC_506651483E6F__INCLUDED_)
#define AFX_FSSITESMGR_H__211935DF_F52C_4919_ADEC_506651483E6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <fsString.h>
#include "list.h"  

#define SITE_VALIDFOR_HTTP			1	
#define SITE_VALIDFOR_HTTPS			2	
#define SITE_VALIDFOR_FTP			4	
#define SITE_VALIDFOR_SUBDOMAINS	8	  

struct fsSiteInfo
{
	fsString strName;	
	fsString strUser;	
	fsString strPassword;	
	UINT cMaxConns;		
	UINT cConnsNow;		
	vmsDownloadsGroupSmartPtr pGroup;	
	DWORD dwFtpFlags;	
	BOOL bTemp;			
	DWORD dwValidFor;	
};

struct fsSiteInfo_v2
{
	fsString strName;	
	fsString strUser;	
	fsString strPassword;	
	UINT cMaxConns;		
	UINT cConnsNow;		
	fsString strGroup;	
	DWORD dwFtpFlags;	
	BOOL bTemp;			
	DWORD dwValidFor;	
};  

struct fsSiteInfo_v1
{
	fsString strName;
	fsString strUser;	
	fsString strPassword;
	UINT cMaxConns;
	UINT cConnsNow;
	fsString strGroup;	
	BOOL bFtpPassive;
	BOOL bTemp;	
};  

#define SITESFILE_CURRENT_VERSION	((WORD)3)
#define SITESFILE_SIG     "FDM Sites  "

struct fsSitesFileHdr
{
	char szSig [sizeof (SITESFILE_SIG) + 1];
	WORD wVer;

	fsSitesFileHdr ()
	{
		strcpy (szSig, SITESFILE_SIG);
		wVer = SITESFILE_CURRENT_VERSION;
	}
};  

enum fsSitesMgrEvent
{
	SM_SITEADDED,		
	SM_SITEDELETED,		
	SM_SITEUPDATED		
};  

typedef void (*fntSitesMgrEvents)(fsSitesMgrEvent, fsSiteInfo*, LPVOID);

class fsSitesMgr  
{
public:
	void CheckGroups();
	void DeleteAllPasswords();
	
	void SiteUpdated (fsSiteInfo* pSite);
	
	BOOL SaveToFile(HANDLE hFile);
	
	BOOL LoadFromFile(HANDLE hFile);

	
	int GetTempSiteCount();
	
	
	void SetEventsFunc (fntSitesMgrEvents pfn, LPVOID lpParam);

	
	int GetSiteCount();
	
	void DeleteAllTempSites();
	
	void DeleteSite (fsSiteInfo* pSite);
	
	
	
	fsSiteInfo* FindSite2 (LPCSTR pszSite, DWORD dwValidFor, BOOL bAllReq = TRUE);
	
	
	int FindSite (LPCSTR pszName, DWORD dwValidFor, BOOL bAllReq = TRUE);
	
	fsSiteInfo* GetSite (int iIndex);
	
	
	int AddSite (fsSiteInfo& site);

	fsSitesMgr();
	virtual ~fsSitesMgr();

protected:
	void Event (fsSitesMgrEvent ev, fsSiteInfo* site);
	fntSitesMgrEvents m_pfnEvents;
	LPVOID m_lpEventsParam;
	fs::list <fsSiteInfo> m_vSites;	
};

#endif 
