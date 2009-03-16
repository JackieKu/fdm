/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSBATCHLIST_H__882E64F6_5B91_4027_9B79_EFBBA47FF941__INCLUDED_)
#define AFX_VMSBATCHLIST_H__882E64F6_5B91_4027_9B79_EFBBA47FF941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"

#define BATCHLIST_STEP_SYMBOL	_T('s')
#define BATCHLIST_WILDCARD_SYMBOL	_T('w')

class vmsBatchList  
{
public:
	
	void Clear();
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	bool Create (LPCTSTR pszTemplate, LPCTSTR pszNumbers, TCHAR chStart = 0, TCHAR chEnd = 0);

	int get_ResultCount();
	LPCTSTR get_Result (int nIndex);
	
	vmsBatchList();
	virtual ~vmsBatchList();

protected:
	
	bool CreateAZBatch (LPCTSTR pszTemplate, TCHAR chStart, TCHAR chEnd);
	fs::list <CString> m_v; 

};

#endif 
