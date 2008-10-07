/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSBTDOWNLOADMANAGER_H__8322A330_304D_470C_BCD6_15403F57E210__INCLUDED_)
#define AFX_VMSBTDOWNLOADMANAGER_H__8322A330_304D_470C_BCD6_15403F57E210__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "Bittorrent\fdmbtsupp\vmsBtDownload.h"
#include "fsTicksMgr.h"
#include "vmsDownloadMgrEx.h"
#include "tree.h"  

#define	BTDF_LAUNCH_WHEN_DONE					1
#define BTDF_LAUNCH_WHEN_DONE_NO_CONFIRM		(1 << 1)
#define BTDF_GENERATE_DESC_FILE					(1 << 2)
#define BTDF_RESERVE_DISK_SPACE					(1 << 3)
#define BTDF_IGNORE_ALL_RESTRICTIONS			(1 << 4)
#define BTDF_DISABLE_SEEDING					(1 << 5)
#define BTDF_NEVER_DELETE_FILES_ON_DISK			(1 << 6)

enum vmsBtDownloadManagerEvent
{
	BTDME_CHECKING_FILES,
	BTDME_ALLOCATING,
	BTDME_DOWNLOAD_STARTED,
	BTDME_DOWNLOADING,				
	BTDME_DOWNLOAD_STOPPED_OR_DONE,
	BTDME_ALLOCATION_FAILED,
	BTDME_SEEDING,
	BTDME_FATAL_ERROR,
	BTDME_STOP_SEEDING,
};

typedef DWORD (*fntBtDownloadManagerEventHandler)(class vmsBtDownloadManager*, vmsBtDownloadManagerEvent, DWORD, LPVOID);

enum vmsBtDownloadStateEx
{
	BTDSE_QUEUED = BTDS_QUEUED,
	BTDSE_CHECKING_FILES = BTDS_CHECKING_FILES,
	BTDSE_CONNECTING_TRACKER = BTDS_CONNECTING_TRACKER,
	BTDSE_DOWNLOADING = BTDS_DOWNLOADING,
	BTDSE_FINISHED = BTDS_FINISHED,
	BTDSE_SEEDING = BTDS_SEEDING,
	BTDSE_ALLOCATING = BTDS_ALLOCATING,
	BTDSE_STOPPED,
};  

class vmsBtDownloadManager  
{
public:
	static void Shutdown();
	float getRequiredRatio();
	void setRequiredRatio (float f);
	bool isSeedingEnabled () const;
	void setFilePriority (int nFileIndex, int iPriority, bool bApply = true);
	int getFilePriority (int nFileIndex) const;
	void getPartialDownloadProgress (UINT64 *pnBytesToDownload, UINT64 *pnBytesDownloaded);
	void PrioritizeFiles (int *piPriorities);
	BOOL LoadTorrentFile (LPCSTR pszFile);
	struct vmsFile {
		fsString strName;
		UINT64 nFileSize;
		int nIndex; 
		int iPriority;
	};
	void GetFilesTree (fs::ListTree <vmsFile> &tree);
	BOOL isSeeding();
	fsString get_RootFolderName();
	UINT64 get_SplittedByteCountAtBeginningOfFile();
	void GetSectionsInfo (std::vector <vmsSectionInfo> &v);
	int get_CurrentTaskProgress();
	std::wstring get_FileNameW(int nIndex);
	void StopSeeding();
	void EnableSeeding(BOOL bEnable);
	BOOL IsStoppedByUser();
	BOOL LoadState(LPBYTE lpBuffer, LPDWORD pdwSize, WORD wVer);
	BOOL SaveState(LPBYTE pb, LPDWORD pdwSize);
	int get_ConnectionCount();
	void UseDetailedLog(BOOL bUse);
	UINT GetTrafficLimit();
	void LimitTraffic(UINT uLimit);
	void StopDownloading();
	fsInternetResult StartDownloading();
	BOOL DeleteFile();
	fsInternetResult SetToRestartState();
	fsInternetResult RestartDownloading();
	BOOL IsBtDownloadRunning();
	BOOL IsCantStart();
	BOOL MoveToFolder(LPCSTR pszPath);
	UINT GetSpeed();
	int GetDownloadingSectionCount(bool bFromCache = true);
	void GetSectionInfo(int nIndex, vmsSectionInfo *sect);
	BOOL IsDownloading();
	UINT64 GetDownloadedBytesCount(bool bFromCache = true);
	UINT64 GetTotalFilesSize();
	int GetNumberOfSections();
	BOOL IsRunning();
	BOOL IsDone();
	float GetPercentDone();
	fsString get_OutputFilePathName();
	vmsBtDownload* get_BtDownload();
	int get_NextAnnounceInterval();
	vmsBtDownloadPeerInfoList* get_PeerInfoList();
	void get_PeersStat (int *pnPeersConnected, int *pnSeedsTotal, int *pnLeechersTotal, int *pnSeedsConnected);
	UINT64 get_WastedByteCount();
	double getRatio();
	UINT64 get_TotalUploadedByteCount();
	fsString m_strTrackerStatus;
	fsString get_CurrentTracker();
	int get_PieceSize();
	int get_PieceCount();
	int get_FilePercentDone(int nIndex);
	UINT64 get_FileSize(int nIndex);
	fsString get_FileName(int nIndex);
	fsString get_TorrentComment();
	void set_TrackerLogin(LPCSTR pszUser, LPCSTR pszPassword);
	void disable_Flags(DWORD dw);
	void enable_Flags(DWORD dw);
	DWORD get_Flags();
	fsString get_URL();
	void get_TrackerLogin (fsString &strUser, fsString &strPassword);
	LPCSTR get_TorrentUrl();
	fsString get_InfoHash();
	void SetEventsHandler (fntBtDownloadManagerEventHandler pfn, LPVOID pData);
	LPCSTR get_OutputPath();
	fsString get_OutputFilePathName (int nIndex);
	int get_FileCount();
	UINT GetUploadSpeed();
	fsString get_TorrentName();
	vmsBtDownloadStateEx get_State();
	BOOL CreateByTorrentFile (LPCSTR pszTorrentFile, LPCSTR pszOutputPath, LPCSTR pszTorrentUrl, BOOL bSeedOnly = FALSE);
	void DeleteBtDownload();

	vmsBtDownloadManager();
	virtual ~vmsBtDownloadManager();

protected:
	void AddToDldsList();
	void DeleteFromDldsList();
	static DWORD WINAPI _threadDlds (LPVOID lp);
	void ApplyNewFilesPriorities();
	void CalculateFilesPieces ();
	void calculateFoldersSizesInTree(fs::ListTree <vmsFile> *pTree);
	void addFileToTree (fs::ListTree <vmsFile> *pTree, LPCSTR pszFile, int nFileIndex);
	bool m_bWasFatalErrorInLoadState;
	
	struct _inc_CachedValues {
		int nDownloadingSectionCount;
		fsTicksMgr nDownloadingSectionCount_time;
		fsTicksMgr nDownloadedBytes_time;
		UINT64 nPartial_BytesDownloaded, nPartial_BytesToDownload;
		fsTicksMgr nPartial_time;
	}m_cache;

	void SaveBtDownloadState_Pieces();
	LONG m_nUsingBtDownload;
	static DWORD WINAPI _threadCheckStartSeeding (LPVOID lp);
	void RemoveBtDownloadDirectory();
	void RecursiveRemoveDirectory (LPCWSTR pwszPath);
	BOOL IsDownloadStatCanBeRead ();
	void PostCreateTorrentObject();
	DWORD RaiseEvent(vmsBtDownloadManagerEvent ev, DWORD dw = 0);
	void SaveBtDownloadState_FileProgress();
	void SaveBtDownloadState();
	void StopThread();
	static DWORD WINAPI _threadBtDownloadManager(LPVOID lp);
	BOOL m_bStoppedByUser;
	bool m_bDlThreadRunning, m_bDlThreadDoJob, m_bDlThreadNeedStop;
	void ProcessFilePathMacroses(CString &str);
	BOOL CreateBtDownload();
	
	vmsBtFile* m_pTorrent;
	vmsBtDownload *m_pDownload;

	
	fntBtDownloadManagerEventHandler m_pfnEvHandler;
	LPVOID m_lpEvParam;

	
	UINT m_uLowSpeedMaxTime; 
	UINT m_uTrafficLimit; 

	
	float m_fRequiredRatio;

	struct _inc_BtDownloadInfo
	{
		float *pfProgress;
		fsTicksMgr timeLastDataStatAccess;
		LPBYTE pbFastResumeData; DWORD dwFastResumeDataSize;
		fsString strOutputPath;
		fsString strTrackerUser, strTrackerPassword;
		fsString strTorrentUrl;
		DWORD dwFlags;
		fsString strCurrentTracker;
		UINT64 nUploadedBytes;
		double fShareRating;
		UINT64 nWastedBytes;
		float fPercentDone;
		BOOL bDone;
		std::vector <bool> vPieces; 
		UINT64 nDownloadedBytes;
		std::vector <BYTE> vFilesPriorities;
	};
	_inc_BtDownloadInfo m_info;

	struct _inc_FilePieces {
		int nStartPiece, nEndPiece;
		_inc_FilePieces (int start, int end) : nStartPiece (start), nEndPiece (end) {}
	};
	std::vector <_inc_FilePieces> m_vFilesPieces;

	static std::vector <vmsBtDownloadManager*> *m_pvpDlds;
	static LPCRITICAL_SECTION m_pcsvpDlds;
	static HANDLE m_htDlds;
	static LONG m_cStatDataRefs;
};

#endif 
