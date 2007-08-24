/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSSCHEDULEMGR_H__D5B62542_F8EA_4F9C_BC88_C3E2BB4E1B9D__INCLUDED_)
#define AFX_FSSCHEDULEMGR_H__D5B62542_F8EA_4F9C_BC88_C3E2BB4E1B9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"  

enum fsWhatToStartType
{
	WTS_PROGRAM,			
	WTS_STARTDOWNLOAD,		
	WTS_STOPDOWNLOAD,		
	WTS_STARTALLDOWNLOADS,	
	WTS_STOPALLDOWNLOADS,	
	WTS_DIAL,				
	WTS_HANGUP,				
	WTS_CHANGE_TUM,			
	WTS_EXIT,				
	WTS_SHUTDOWN,			
	WTS_RESTRAINALLDLDS,	
};  

enum fsWhenToStartType
{
	HTS_ONCE,				
	HTS_DAILY,				
	HTS_CONTINUOUSLY,		
	HTS_WHENISTART,			
	HTS_WHENDIALINGSUCCESS,	
	HTS_WHENDIALINGFAILED,	
	HTS_WHENNODOWNLOADS,	
	HTS_WHENSPEEDLOW,		
	HTS_NOTHINGRECEIVED,	
	HTS_WHENDONE,			
};  

struct fsProgramInfo
{
	LPSTR pszName;		
	LPSTR pszArgs;		
};  

enum fsShutdownType
{
	ST_SHUTDOWN,
	ST_RESTART,
	ST_LOGOFF,
	ST_HIBERNATE,
	ST_STANDBY,
};  

struct fsShutdown
{
	fsShutdownType enShutdown;
	BOOL bForce;			
};  

struct fsDialInfo
{
	LPSTR pszConnection;	
	UINT  cAttempts;		
	UINT  cPauseBetween;	
	DWORD dwHangupLess;		
	UINT  cAgains;			
};

enum fsRestrainAllDlds
{
	RAD_ENABLE,
	RAD_DISABLE,
	RAD_INVERT,			
};  

struct fsWhatToStart
{
	fsWhatToStartType enType;

	union		
	{
		fsProgramInfo prog;		
		fs::list <UINT> *pvIDs; 
		fsTUM enTUM;			
		fsShutdown shutdown;	
		fsDialInfo dial;  
		
		
		LPSTR pszHangupConnection;
		fsRestrainAllDlds enRAD; 
	};
};  

struct fsSpeedLowInfo
{
	
	UINT uLowSpeed;	
	
	UINT uTimeInterval;	
};  

struct fsWhenToStart
{
	fsWhenToStartType enType;	
	
	union		
	{
		
		BOOL aWeeklyDays [7];
		
		UINT interval;  
		fsSpeedLowInfo speedLow;	
		
		
		UINT uTimeNothingReceived;	
	};

	
	
	
	FILETIME last;
	FILETIME next;			
};  

const UINT SCHEDULE_ID_INVALID = UINT_MAX;  

#define SCHEDULE_ENABLED		1	
#define SCHEDULE_AUTODEL		2	
#define SCHEDULE_AUTODIS		4	  

struct fsSchedule
{
	fsWhatToStart wts;	
	fsWhenToStart hts;	
	DWORD dwFlags;		
	UINT uWaitForConfirmation; 
};  

enum fsScheduleMgrEvent
{
	SME_TASK_STARTED,		
	SME_TASK_UPDATED,		
	SME_TASK_WILLBEDELETED	
};  

enum fsScheduleMgrEventType
{
	SMET_S,				
	SMET_E,				
	SMET_INPROGRESS		
};  

enum fsExternalEvent
{
	EV_STARTUP,				
	EV_DIALINGSUCCESS,		
	EV_DIALINGFAILED,		
	EV_NODOWNLOADS			
};  

typedef void (*fntScheduleMgrEvents)(fsSchedule *task, fsScheduleMgrEvent ev, LPVOID lp);
typedef void (*fntScheduleMgrEventDesc)(LPCSTR pszEvent, fsScheduleMgrEventType type, LPVOID lp);

class fsScheduleMgr
{
public:
	void RepairTasksNextTimes();
	
	void OnTaskUpdated (fsSchedule* task);
	
	
	
	
	int FindTask (fsWhatToStartType enType, int iStartPos = -1);
	
	void RepairNextTime (fsSchedule *task);
	
	
	fsSchedule* GetScheduleDLTask (DLDS_LIST &vDlds, BOOL bStartDL);
	
	BOOL IsDownloadScheduled (vmsDownloadSmartPtr dld);
	
	void Stop();
	
	void ExciteEvent (fsExternalEvent ev);
	
	void SetEventDescFunc (fntScheduleMgrEventDesc pfn, LPVOID lp);
	void SetEventFunc (fntScheduleMgrEvents pfn, LPVOID lp);
	

	
	int FindTask (fsSchedule* task);
	
	void DeleteTask (fsSchedule *task);
	
	int GetTaskCount();
	
	static void FreeTask (fsSchedule *task);
	
	BOOL LoadStateFromFile (HANDLE hFile);
	BOOL SaveStateToFile (HANDLE hFile);
	
	
	
	static CString LastTimeToStr(fsSchedule *task);
	
	static CString NextTimeToStr (fsSchedule *task);
	
	static CString HTSToStr (fsSchedule *task);
	
	static CString WTSToStr (fsSchedule *task);

	
	fsSchedule* GetTask (int i);
	
	int AddTask (fsSchedule *task);
	
	void CalculateStartTime (fsSchedule *task);

	fsScheduleMgr();
	virtual ~fsScheduleMgr();

protected:
	
	void Event (LPCSTR pszEvent, fsScheduleMgrEventType enType = SMET_S);
	void Event (fsSchedule* task, fsScheduleMgrEvent ev);
	

	
	void UpdateTaskDownloads (fsSchedule* task);
	
	void UpdateCurrentTime();
	
	UINT m_uID;
	
	
	static DWORD WINAPI _threadShutdown (LPVOID uFlags);
	
	
	void ManageHalfEvents();
	
	void ManageNotEvents();
	
	fsWhenToStartType ExternalEventToHTS (fsExternalEvent ev);
	
	void LaunchProgram (fsSchedule *task);
	
	
	static DWORD WINAPI _threadDial (LPVOID lp);
	
	void Shutdown (fsSchedule *task);
	
	void Hangup (fsSchedule *task);
	
	void Dial (fsSchedule *task);
	
	void StopDownloads (fsSchedule *task);
	void StartDownloads (fsSchedule *task);

	
	void FreeTasks ();
	
	
	BOOL IsTimeBased (fsSchedule* task);
	
	void GetNextTime (fsSchedule *task);
	
	static BOOL IsTimeUnknown (FILETIME& time);
	FILETIME m_curTime;	
	
	void StartTask (fsSchedule* task);
	
	BOOL IsNeedToStart (fsSchedule* task);

	
	fntScheduleMgrEventDesc m_pfnEventDesc;
	LPVOID m_lpEventDesc;
	fntScheduleMgrEvents m_pfnEvents;
	LPVOID m_lpEvents;
	

	
	static DWORD WINAPI _threadScheduleMgr (LPVOID lp);
	volatile BOOL m_bNeedExit;	

	fs::list <fsSchedule*> m_vTasks;	
};

#endif 
