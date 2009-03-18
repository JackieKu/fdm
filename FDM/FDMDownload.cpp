/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "FdmApp.h"
#include "FDMDownload.h"        

STDMETHODIMP CFDMDownload::get_Url(BSTR *pVal)
{
	*pVal = SysAllocString (CA2W (m_strUrl, CP_UTF8));
	return S_OK;
}

STDMETHODIMP CFDMDownload::put_Url(BSTR newVal)
{
	m_strUrl = CW2A (newVal, CP_UTF8);
	return S_OK;
}

void CFDMDownload::ReadFromDownload(vmsDownloadSmartPtr dld)
{
	m_strUrl = dld->pMgr->get_URL ();
	
	for (int i = 0; i < 6; i++)
		m_astrDldTexts [i] = CDownloads_Tasks::GetDownloadText (dld, i);
}  

STDMETHODIMP CFDMDownload::get_DownloadText(long nTextIndex, BSTR *pVal)
{
	if (nTextIndex < 0 || nTextIndex > 5)
		return E_INVALIDARG;

	*pVal = SysAllocString (CA2W (m_astrDldTexts [nTextIndex], CP_UTF8));

	return S_OK;
}
