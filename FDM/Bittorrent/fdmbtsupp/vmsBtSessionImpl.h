/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsbtsession.h"
#include "vmsBtDownloadImpl.h"

class vmsBtSessionImpl : public vmsBtSession
{
public:
	void SetMaxHalfOpenConnections (int limit);
	void RestoreDownloadHandle (vmsBtDownloadImpl* dld);
	void ListenOn (int portFrom, int portTo);
	BOOL IsListening ();
	USHORT get_ListenPort ();
	vmsBtDownload* CreateDownload (vmsBtFile *torrent, LPCSTR pwszOutputPath, LPBYTE pbFastResumeData, DWORD dwFRDataSize, vmsBtStorageMode enStorageMode);
	void SetDownloadLimit (int limit);
	void SetUploadLimit (int limit);
	void SetMaxUploads (int limit);
	void DeleteDownload (vmsBtDownload*);
	void DHT_start (LPBYTE pbState, DWORD dwStateSize);
	void DHT_stop ();
	BOOL DHT_getState (LPBYTE pbBuffer, DWORD dwBufferSize, LPDWORD pdwDataSize);
	BOOL DHT_isStarted ();
	void set_EventsHandler (fntBtSessionEventsHandler pfn, LPVOID pData);
	void SetProxySettings (LPCSTR pszIp, int nPort, LPCSTR pszUser, LPCSTR pszPwd);
	int get_TotalDownloadConnectionCount ();
	int get_DownloadCount ();
	vmsBtDownload* get_Download (int nIndex);
	int get_TotalDownloadSpeed ();
	int get_TotalUploadSpeed ();
	UINT64 get_TotalDownloadedByteCount ();
	UINT64 get_TotalUploadedByteCount ();
	void SetUserAgent (LPCSTR pszUA);

	static vmsBtSessionImpl* Instance ();
	vmsBtSessionImpl(void);
public:
	virtual ~vmsBtSessionImpl(void);

public:
	session m_session;

protected:
	static DWORD WINAPI _threadSession (LPVOID lp);
	bool m_bThreadRunning, m_bNeedStop;
	BOOL m_bDHTstarted;
	fntBtSessionEventsHandler m_pfnEvHandler;
	LPVOID m_pEvData;
	int FindDownloadIndex (const torrent_handle &h);
	
	std::vector <vmsBtDownloadImpl*> m_vDownloads;
	CRITICAL_SECTION m_csRestoreTorrentHandle;
	CRITICAL_SECTION m_csDeleteDownload;
};
