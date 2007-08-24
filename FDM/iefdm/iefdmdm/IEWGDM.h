/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef __IEWGDM_H_
#define __IEWGDM_H_

#include "resource.h"       
#include "downloadmgr.h"    

class ATL_NO_VTABLE CIEWGDM : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIEWGDM, &CLSID_IEWGDM>,
	public IDownloadManager
{
public:
	CIEWGDM()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IEWGDM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIEWGDM)
	COM_INTERFACE_ENTRY(IDownloadManager)
END_COM_MAP()  

public:
protected:
	STDMETHOD (Download) (IMoniker *pmk, IBindCtx *pbc, DWORD dwBindVerb, LONG grfBINDF, BINDINFO *pBindInfo, LPCOLESTR pszHeaders, LPCOLESTR pszRedir, UINT uiCP);
};

#endif 
