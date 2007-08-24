/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#if !defined(AFX_FLASHVIDEODOWNLOADSWND_H__09228685_F3CC_4075_B8A1_430FAACBC81B__INCLUDED_)
#define AFX_FLASHVIDEODOWNLOADSWND_H__09228685_F3CC_4075_B8A1_430FAACBC81B__INCLUDED_

#include "FVDownloads_Tasks.h"	
#include "WndSplitter.h"	
#if _MSC_VER > 1000
#pragma once
#endif     

extern class CFlashVideoDownloadsWnd *_pwndFVDownloads;    

class CFlashVideoDownloadsWnd : public CWnd
{

public:
	CFlashVideoDownloadsWnd();  

public:  

public:
	
	static void Plugin_SetLanguage (wgLanguage, HMENU hMenuMain, HMENU);
	static void Plugin_GetPluginNames(LPCSTR *ppszLong, LPCSTR *ppszShort);
	static void Plugin_GetMenuViewItems(wgMenuViewItem **ppItems, int *cItems);
	static void Plugin_GetMenuImages(fsSetImage **ppImages, int *pcImages);
	static void Plugin_GetToolBarInfo (wgTButtonInfo **ppButtons, int *pcButtons);
	static HWND Plugin_CreateMainWindow (HWND hWndParent);
	  

	
	//{{AFX_VIRTUAL(CFlashVideoDownloadsWnd)
	//}}AFX_VIRTUAL  

public:
	BOOL CreateDownload (LPCSTR pszUrl = NULL);
	static HMENU Plugin_GetViewMenu();
	static HMENU Plugin_GetMainMenu();
	void SetActiveDownload (vmsDownloadSmartPtr dld);
	void SaveAll();
	void OnDownloadDone (vmsDownloadSmartPtr dld);
	void AddDownload (vmsDownloadSmartPtr dld);
	CDownloads_VideoPreview m_wndPreview;
	CFVDownloads_Tasks m_wndTasks;
	BOOL Create (CWnd *pwndParent);
	virtual ~CFlashVideoDownloadsWnd();

	
protected:
	CWndSplitter m_splitter;
	afx_msg void OnDLListShowCol (UINT nCmd);
	void ApplyLanguageToMenuView(CMenu *menu);
	afx_msg LRESULT OnUpdateToolBar (WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateMenuView (WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateMenu (WPARAM, LPARAM);
	afx_msg LRESULT OnAppQueryExit (WPARAM, LPARAM);
	afx_msg LRESULT OnAppExit (WPARAM, LPARAM);
	afx_msg LRESULT OnInsert (WPARAM, LPARAM);
	//{{AFX_MSG(CFlashVideoDownloadsWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFvdldCreate();
	afx_msg void OnFvdldAutostart();
	afx_msg void OnFvdldDelete();
	afx_msg void OnFvdldLaunch();
	afx_msg void OnFvdldOpenfolder();
	afx_msg void OnFvdldPasstodlds();
	afx_msg void OnFvdldProperties();
	afx_msg void OnFvdldStart();
	afx_msg void OnFvdldStop();
	afx_msg void OnFvdldConvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};    

//{{AFX_INSERT_LOCATION}//}}  

#endif 
