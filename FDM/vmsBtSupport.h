/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSBTSUPPORT_H__0E2B5DB4_43BC_4761_ABB5_28573A27F8F0__INCLUDED_)
#define AFX_VMSBTSUPPORT_H__0E2B5DB4_43BC_4761_ABB5_28573A27F8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "vmsDll.h"
#include "Bittorrent\fdmbtsupp\vmsBtSession.h"

class vmsBtSupport  
{
public:
	static int getBtDllVersion();
	vmsBtFile* CreateTorrentFileObject();
	
	void Shutdown();
	
	BOOL LoadState();
	BOOL SaveState();
	
	void ApplyDHTSettings();
	void ApplyListenPortSettings();
	void ApplyProxySettings();
	
	void ApplyRestrainAllDownloadsMode();
	
	BOOL is_Initialized();
	
	vmsBtSession* get_Session();
	
	BOOL Initialize();

	vmsBtSupport();
	virtual ~vmsBtSupport();

protected:
	bool LoadBtDll (vmsDLL& dll);
	CRITICAL_SECTION m_cs1;
	
	static void GetFirefoxProxySettings(fsString &strIp, fsString &strUser, fsString &strPwd, int &nPort);
	static void GetIeProxySettings (fsString& strIp, fsString& strUser, fsString& strPwd, int& nPort);
	
	LPBYTE m_pbDHTstate;
	DWORD m_dwDHTstateSize;
	
	vmsDLL m_dllBt, m_dllBtFile;
};

#endif 
