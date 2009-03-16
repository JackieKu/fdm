/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsVideoSiteHtmlCodeParser.h"
#include "inetutil.h"

_COM_SMARTPTR_TYPEDEF (IXMLDOMNamedNodeMap, __uuidof (IXMLDOMNamedNodeMap));

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define is_valid_char(c) (c >= 32 && c < 127)        

vmsVideoSiteHtmlCodeParser::vmsVideoSiteHtmlCodeParser()
{
	CoInitialize (NULL);
}

vmsVideoSiteHtmlCodeParser::~vmsVideoSiteHtmlCodeParser()
{
	CoUninitialize ();
}

BOOL vmsVideoSiteHtmlCodeParser::Parse(LPCTSTR pszSite, LPCTSTR pszHtml)
{
	switch (GetSupportedSiteIndex (pszSite))
	{
	case 0:
		return Parse_Youtube (pszHtml);

	case 1:
		return Parse_GoogleVideo (pszHtml);

	case 2:
		return Parse_LiveDigital (pszHtml);

	case 3:
		return Parse_MySpace (pszHtml);

	case 4:
		return Parse_Sharkle (pszHtml);

	case 5:
		return Parse_Blennus (pszHtml);

	case 6:
		return Parse_DailyMotion (pszHtml);

	case 7:
		return Parse_Grouper (pszHtml);

	default:
		return FALSE;
	}
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Youtube(LPCTSTR pszHtml)
{
	fsString strTitle, strUrl; 

	LPTSTR psz = (LPTSTR) _tcsstr (pszHtml, _T("<meta name=\"title\""));

	if (psz)
	{
		psz = _tcsstr (psz, _T("content="));
		if (psz)
		{
			psz += lstrlen (_T("content="));
			if (*psz == _T('"'))
			{
				psz++;
				while (*psz && *psz != _T('"'))
				{
					if (is_valid_char (*psz))
						strTitle += *psz++;
					else
						psz++;				
				}
			}
		}
	}

	psz = (LPTSTR) _tcsstr (pszHtml, _T("swfArgs ="));
	if (psz == NULL)
		return FALSE;
	psz = _tcschr (psz, _T('{'));
	if (psz == NULL)
		return FALSE;
	psz++;

	fsString strBase = "http://youtube.com/"; 
	fsString strParams;
	
	while (*psz != _T('}'))
	{
		while (*psz == _T(' ') || *psz == _T(','))
			psz++;

		fsString str;
	
		while (*psz && *psz != _T(':'))
		{
			if (*psz == _T(' '))
			{
				str = _T("");
				break;
			}
			str += *psz++;
		}

		if (str.IsEmpty ())
			break;

		if (*psz == _T(':'))
			psz++;
		while (*psz == _T(' '))
			psz++;

		if (str.Length () > 2 && str [0] == _T('"') && str [str.Length () - 1] == _T('"'))
		{
			_tcscpy (str, str.pszString+1);
			str [str.Length () - 1] = 0;
		}

		if (lstrcmpi (str, _T("BASE_YT_URL")) == 0)
		{
			strBase = ExtractValue (psz);
		}
		else
		{
			if (strParams.IsEmpty () == FALSE)
				strParams += _T('&');

			strParams += str; strParams += _T("="); strParams += ExtractValue (psz);
		}
	}

	strUrl = strBase;
	strUrl += _T("get_video?");
	strUrl += strParams;

	

	fsDecodeHtmlText (strTitle);

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("flv");
	m_bDirectLink   = TRUE;

	return TRUE;
}

LPCTSTR vmsVideoSiteHtmlCodeParser::get_VideoTitle()
{
	return m_strVideoTitle;
}

LPCTSTR vmsVideoSiteHtmlCodeParser::get_VideoUrl()
{
	return m_strVideoUrl;
}

LPCTSTR vmsVideoSiteHtmlCodeParser::get_VideoType()
{
	return m_strVideoType;
}

BOOL vmsVideoSiteHtmlCodeParser::IsSiteSupported(LPCTSTR pszHost)
{
	return GetSupportedSiteIndex (pszHost) != -1;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_GoogleVideo(LPCTSTR pszHtml)
{
	LPCTSTR psz = _tcsstr (pszHtml, _T("/googleplayer.swf?"));
	if (psz == NULL)
		return FALSE;
	psz = _tcsstr (psz, _T("videoUrl"));
	if (psz == NULL)
		return FALSE;
	psz = _tcsstr (psz, _T("http"));
	if (psz == NULL)
		return FALSE;

	fsString strUrl;
	while (*psz && *psz != _T('"'))
		strUrl += *psz++;
	if (strUrl [strUrl.GetLength () - 1] == _T('\\'))
		strUrl [strUrl.GetLength () - 1] = 0;
	fsDecodeHtmlUrl (strUrl);  

	fsString strTitle;
	psz = _tcsstr (pszHtml, _T("pvprogtitle"));
	if (psz)
	{
		while (*psz && *psz != _T('>'))
			psz++;
		if (*psz)
		{
			psz++;
			while (*psz == _T(' '))
				psz++;
			while (*psz && *psz != _T('<'))
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else 
					psz++;
			}
			while (strTitle.GetLength () && strTitle [strTitle.GetLength () - 1] == _T(' '))
				strTitle [strTitle.GetLength () - 1] = 0;
			fsDecodeHtmlText (strTitle);
		}
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("flv");
	m_bDirectLink   = TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Youtube_RootPage(LPCTSTR pszHtml)
{
	fsString strUrl; 

	LPCTSTR psz = _tcsstr (pszHtml, _T("/admp.swf"));
	if (psz == NULL)
		return FALSE;

	while (*psz && *psz != _T('='))
		psz++;
	if (*psz == 0)
		return FALSE;
	psz++;

	fsString strId;

	while (*psz && *psz != _T('&'))
		strId += *psz++;

	strUrl = "http://www.youtube.com/watch?v=";
	strUrl += strId;

	m_strVideoTitle = _T("");
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("");
	m_bDirectLink = FALSE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::get_IsVideoUrlDirectLink()
{
	return m_bDirectLink;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_LiveDigital(LPCTSTR pszHtml)
{
	LPCTSTR psz = _tcsstr (pszHtml, _T("flashvars"));
	if (psz == NULL)
		psz = _tcsstr (pszHtml, _T("flashVars"));
	
	fsString strC, strH;

	if (psz)
	{
		LPCTSTR pszC = _tcsstr (psz, _T("c=")), 
			pszH = _tcsstr (psz, _T("h="));

		if (pszC == NULL || pszH == NULL)
			return FALSE;

		pszC += 2;
		pszH += 2;

		while (*pszC && *pszC != _T('&') && *pszC != _T('"'))
			strC += *pszC++;

		while (*pszH && *pszH != _T('&') && *pszH != _T('"'))
			strH += *pszH++;
	}
	else
	{
		
		psz = _tcsstr (pszHtml, _T("content_id/"));
		if (psz)
		{
			psz += lstrlen (_T("content_id/"));
			while (isdigit (*psz))
				strC += *psz++;
		}

		if (strC.IsEmpty ())
		{
			psz = _tcsstr (pszHtml, _T("/content/"));
			if (psz)
			{
				psz += lstrlen (_T("/content/"));
				while (isdigit (*psz))
					strC += *psz++;
				if (strC.IsEmpty ())
					return FALSE;
			}
		}

		strH = _T("livedigital.com");
	}

	fsString strUrl = "http://";
	strUrl += strH;
	strUrl += _T("/content/flash_load_content/");
	strUrl += strC;

	m_strVideoTitle = _T("");
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("");
	m_bDirectLink = FALSE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Further(LPCTSTR pszSite, LPCTSTR pszHtml)
{
	switch (GetSupportedSiteIndex (pszSite))
	{
	case 2:
		return Parse_Further_LiveDigital (pszHtml);

	case 3:
		return Parse_Further_MySpace (pszHtml);

	default:
		return Parse (pszSite, pszHtml);
	}
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Further_LiveDigital(LPCTSTR pszTxt)
{
	LPCTSTR psz = _tcsstr (pszTxt, _T("content_url="));
	if (psz == NULL)
		return FALSE;

	fsString strUrl;
	psz += lstrlen (_T("content_url="));
	while (*psz && *psz != _T('&'))
		strUrl += *psz++;

	fsDecodeHtmlUrl (strUrl);

	fsString strTitle;

	psz = _tcsstr (pszTxt, _T("title="));
	if (psz)
	{
		psz += lstrlen (_T("title="));
		while (*psz && *psz != _T('&'))
		{
			if (is_valid_char (*psz))
				strTitle += *psz++;
			else
				psz++;
		}
		fsDecodeHtmlText (strTitle);
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = (LPCTSTR)strUrl + strUrl.GetLength () - 3;
	m_bDirectLink   = TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_MySpace(LPCTSTR pszHtml)
{
	CString str; 

	LPCTSTR psz = _tcsstr (pszHtml, _T("flashvars="));
	if (psz == NULL)
		psz = _tcsstr (pszHtml, _T("flashVars="));
	if (psz != NULL)
	{
		psz += 10;

		if (*psz == _T('"'))
			psz++;
		else if (strnicmp (psz, _T("&quot;"), 6) == 0)
			psz += 6;
		else
			return FALSE;

		while (*psz && *psz != _T('"') && strnicmp (psz, _T("&quot;"), 6))
			str += *psz++;
		if (str.IsEmpty ())
			return FALSE;
		str.Replace (_T("m="), _T("mediaID="));
	}
	else
	{
		psz = _tcsstr (pszHtml, _T("videoID ="));
		if (!psz)
		{
			psz = _tcsstr (pszHtml, _T("videoid="));
			if (!psz)
				return FALSE;
			else 
				psz += 8;
		}
		else
		{
			psz += 9;
		}
		while (*psz == _T(' '))
			psz++;
		str = _T("videoID=");
		while (isdigit (*psz))
			str += *psz++;
	}

	fsString strUrl;
	strUrl = "http://"; strUrl += "mediaservices.myspace.com/services/rss.ashx?";
	strUrl += str;

	m_strVideoTitle = _T("");
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("");
	m_bDirectLink = FALSE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Further_MySpace(LPCTSTR pszHtml)
{
	USES_CONVERSION;
	IXMLDOMDocumentPtr spXML;
	IXMLDOMNodePtr spNode, spNode2;

	while (*pszHtml && *pszHtml != _T('<'))
		pszHtml++;

	spXML.CreateInstance (__uuidof (DOMDocument));

	if (spXML == NULL)
		return FALSE;

	spXML->put_async (FALSE);

	VARIANT_BOOL bRes;
	spXML->loadXML (A2W (pszHtml), &bRes);
	if (bRes == FALSE)
		return FALSE;

	spXML->selectSingleNode (L"rss", &spNode);
	if (spNode == NULL)
		return FALSE;

	spNode->selectSingleNode (L"channel", &spNode2);
	if (spNode2 == NULL)
		return FALSE;

	spNode = NULL;
	spNode2->selectSingleNode (L"item", &spNode);
	if (spNode == NULL)
		return FALSE;

	spNode2 = NULL;
	spNode->selectSingleNode (L"title", &spNode2);
	if (spNode2 == NULL)
		return FALSE;

	CComBSTR bstrTitle;
	spNode2->get_text (&bstrTitle);

	spNode2 = NULL;
	spNode->selectSingleNode (L"media:content", &spNode2);
	if (spNode2 == NULL)
		return FALSE;
	IXMLDOMNamedNodeMapPtr spAttrs;
	spNode2->get_attributes (&spAttrs);
	if (spAttrs == NULL)
		return FALSE;
	IXMLDOMNodePtr spUrlValue;
	spAttrs->getNamedItem (L"url", &spUrlValue);
	if (spUrlValue == NULL)
		return FALSE;
	COleVariant vtUrl;
	spUrlValue->get_nodeValue (&vtUrl);
	ASSERT (vtUrl.vt == VT_BSTR);
	if (vtUrl.vt != VT_BSTR)
		return FALSE;

	m_strVideoTitle = W2A (bstrTitle);
	fsDecodeHtmlText (m_strVideoTitle);
	m_strVideoUrl   = W2A (vtUrl.bstrVal);
	m_strVideoType  = (LPCTSTR)m_strVideoUrl + m_strVideoUrl.GetLength () - 3;
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Sharkle(LPCTSTR pszHtml)
{
	LPCTSTR psz = _tcsstr (pszHtml, _T("splayer.swf?"));
	if (psz == NULL)
		return FALSE;

	psz = _tcsstr (psz, _T("rnd="));
	if (psz == NULL)
		return FALSE;

	fsString strRnd;

	psz += 4;
	while (*psz && *psz != _T('&'))
		strRnd += *psz++;
	if (strRnd.IsEmpty ())
		return FALSE;

	fsString strUrl;
	strUrl = "http://sharkle.com/inc/misc/about.php?rnd=";
	strUrl += strRnd;
	strUrl += _T("&ssd=ZeleninGalaburda");

	fsString strTitle;

	psz = _tcsstr (pszHtml, _T("blog_header"));
	if (psz != NULL)
	{
		while (*psz && *psz != _T('>'))
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == _T(' ') || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != _T('<'))
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			while (strTitle.GetLength () && strTitle [strTitle.GetLength () - 1] == _T(' '))
				strTitle [strTitle.GetLength () - 1] = 0;
		}
	}

	fsDecodeHtmlText (strTitle);

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("flv");
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Blennus(LPCTSTR pszHtml)
{
	LPCTSTR psz = _tcsstr (pszHtml, _T("embed"));
	if (psz == NULL)
		psz = _tcsstr (pszHtml, _T("EMBED"));
	if (psz == NULL)
		return FALSE;

	psz = _tcsstr (psz, _T("src="));
	if (psz == NULL)
		return FALSE;
	psz += 4;
	if (*psz++ != _T('"'))
		return FALSE;
	fsString strUrl;
	while (*psz && *psz != _T('"'))
		strUrl += *psz++;
	if (strUrl.IsEmpty ())
		return FALSE;

	fsString strTitle;
	psz = _tcsstr (pszHtml, _T("contentheading"));
	if (psz != NULL)
	{
		while (*psz && *psz != _T('>'))
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == _T(' ') || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != _T('<'))
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			fsDecodeHtmlText (strTitle);
			while (strTitle.GetLength () && strTitle [strTitle.GetLength () - 1] == _T(' '))
				strTitle [strTitle.GetLength () - 1] = 0;
		}
	}

	fsDecodeHtmlText (strTitle);

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("wmv");
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_DailyMotion(LPCTSTR pszHtml)
{
	LPCTSTR psz = _tcsstr (pszHtml, _T("flashvars="));
	if (psz == NULL)
		psz = _tcsstr (pszHtml, _T("flashVars="));
	if (psz == NULL)
		return FALSE;
	
_lSearchUrl:
	psz = _tcsstr (psz, _T("url="));
	if (psz == NULL)
		return FALSE;
	psz += 4;
	if (strncmp (psz, _T("rev="), 4) == 0)
		goto _lSearchUrl;

	fsString strUrl;
	while (*psz && *psz != _T('&'))
		strUrl += *psz++;
	fsDecodeHtmlUrl (strUrl);

	fsString strTitle;
	psz = _tcsstr (pszHtml, _T("<h1"));
	if (psz == NULL)
		psz = _tcsstr (pszHtml, _T("<H1"));
	if (psz != NULL)
	{
		while (*psz && *psz != _T('>'))
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == _T(' ') || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != _T('<'))
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			fsDecodeHtmlText (strTitle);
			while (strTitle.GetLength () && strTitle [strTitle.GetLength () - 1] == _T(' '))
				strTitle [strTitle.GetLength () - 1] = 0;
		}
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("flv");
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Grouper(LPCTSTR pszHtml)
{
	LPCTSTR psz = _tcsstr (pszHtml, _T("flvURL="));
	if (psz == NULL)
		return FALSE;
	psz += 7;

	fsString strUrl;
	while (*psz && *psz != _T('&'))
		strUrl += *psz++;

	fsString strTitle;
	psz = _tcsstr (pszHtml, _T("<h1"));
	if (psz == NULL)
		psz = _tcsstr (pszHtml, _T("<H1"));
	if (psz != NULL)
	{
		while (*psz && *psz != _T('>'))
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == _T(' ') || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != _T('<'))
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			fsDecodeHtmlText (strTitle);
			while (strTitle.GetLength () && strTitle [strTitle.GetLength () - 1] == _T(' '))
				strTitle [strTitle.GetLength () - 1] = 0;
		}
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = _T("flv");
	m_bDirectLink	= TRUE;	

	return TRUE;
}  

fsString vmsVideoSiteHtmlCodeParser::ExtractValue(LPTSTR &psz)
{
	TCHAR c;
	while (*psz == _T(' '))
		psz++;
	if (*psz == _T('"') || *psz == _T('\''))
		c = *psz++;
	else 
		c = _T(',');
	fsString strRes;
	while (*psz && *psz != c)
	{
		if (*psz == _T('}') && c == _T(','))
			break;
		strRes += *psz++;
	}
	if (*psz != _T('}'))
		psz++;
	return strRes;
}
