/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSVIDEOSITEHTMLCODEPARSER_H__36077242_C9A7_4188_8F8D_CFE99AF921C2__INCLUDED_)
#define AFX_VMSVIDEOSITEHTMLCODEPARSER_H__36077242_C9A7_4188_8F8D_CFE99AF921C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsVideoSiteHtmlCodeParser  
{
public:
	BOOL Parse_Further (LPCTSTR pszSite, LPCTSTR pszHtml);
	BOOL get_IsVideoUrlDirectLink();
	BOOL Parse_GoogleVideo (LPCTSTR pszHtml);
	static BOOL IsSiteSupported (LPCTSTR pszHost);
	LPCTSTR get_VideoType();
	LPCTSTR get_VideoUrl();
	LPCTSTR get_VideoTitle();
	BOOL Parse (LPCTSTR pszSite, LPCTSTR pszHtml);

	static int GetSupportedSiteIndex (LPCTSTR pszSite)
	{
		
		#define register_site_with_subdomains(s,n)\
			if (stricmp (pszSite, ##s) == 0)\
				return n;\
			if (lstrlen (pszSite) > lstrlen (_T(".")##s) && \
					strnicmp (pszSite + lstrlen (pszSite) - lstrlen (_T(".")##s), _T(".")##s, lstrlen (_T(".")##s)) == 0)\
				return n;
		

		if (strnicmp (pszSite, _T("www."), 4) == 0)
			pszSite += 4;	

		register_site_with_subdomains (_T("youtube.com"), 0);
		

		if (strnicmp (pszSite, _T("video.google."), 13) == 0)
			return 1;

		register_site_with_subdomains (_T("livedigital.com"), 2);

		register_site_with_subdomains (_T("myspace.com"), 3);

		if (stricmp (pszSite, _T("sharkle.com")) == 0)
			return 4;

		if (stricmp (pszSite, _T("blennus.com")) == 0)
			return 5;

		if (stricmp (pszSite, _T("dailymotion.com")) == 0)
			return 6;

		if (stricmp (pszSite, _T("grouper.com")) == 0)
			return 7;

		return -1;
	}

	vmsVideoSiteHtmlCodeParser();
	virtual ~vmsVideoSiteHtmlCodeParser();

protected:
	static fsString ExtractValue (LPTSTR &psz);
	BOOL Parse_Grouper (LPCTSTR pszHtml);
	BOOL Parse_DailyMotion (LPCTSTR pszHtml);
	BOOL Parse_Blennus (LPCTSTR pszHtml);
	BOOL Parse_Sharkle (LPCTSTR pszHtml);
	BOOL Parse_Further_MySpace (LPCTSTR pszHtml);
	BOOL Parse_MySpace (LPCTSTR pszHtml);
	BOOL Parse_Further_LiveDigital (LPCTSTR pszTxt);
	BOOL Parse_LiveDigital (LPCTSTR pszHtml);
	BOOL m_bDirectLink;
	BOOL Parse_Youtube_RootPage (LPCTSTR pszHtml);
	fsString m_strVideoTitle, m_strVideoUrl, m_strVideoType;
	BOOL Parse_Youtube (LPCTSTR pszHtml);
};

#endif 
