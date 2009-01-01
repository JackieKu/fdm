/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#if !defined(AFX_DLG_OPTIONS_H__FB7201D6_472A_482C_BE35_8EEF034A8684__INCLUDED_)
#define AFX_DLG_OPTIONS_H__FB7201D6_472A_482C_BE35_8EEF034A8684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif     

#include "Dlg_Options_Page.h"
#include "resource.h"      

class CDlg_Options : public CDialog
{
protected:
	struct OptionsTreeItem
	{
		CDlg_Options_Page *page;
		CString strJustCaption; 

		OptionsTreeItem (CDlg_Options_Page* p) {page = p;}
		OptionsTreeItem (LPCSTR psz) {page = NULL; strJustCaption = psz;}
		OptionsTreeItem () {page = NULL;}
	};
	typedef fs::ListTree <OptionsTreeItem> OPTIONS_PAGES_TREE;  

public:
	void set_CurPage (OPTIONS_PAGES_TREE* ptPage);
	CDlg_Options(CWnd* pParent = NULL, LPCSTR pszStartPage = NULL);     

	//{{AFX_DATA(CDlg_Options)
	enum { IDD = IDD_OPTIONS };
	CTreeCtrl	m_wndPagesList;
	//}}AFX_DATA    

	
	//{{AFX_VIRTUAL(CDlg_Options)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL  

protected:
	BOOL OptionsTree_Apply (OPTIONS_PAGES_TREE* ptRoot = NULL);
	void FillPagesList(OPTIONS_PAGES_TREE* ptRoot = NULL, HTREEITEM htRoot = TVI_ROOT);
	virtual void InitializeData () = NULL;
	CImageList m_imgsPages;
	CFont m_fntCaption;
	void CreatePageDialog (CDlg_Options_Page *page);
	OPTIONS_PAGES_TREE m_tPages, *m_ptCurPage;
	CString m_strStartPage;

	
	//{{AFX_MSG(CDlg_Options)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnSelchangedPagelist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}//}}  

#endif 
