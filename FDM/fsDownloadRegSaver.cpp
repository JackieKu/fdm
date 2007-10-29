/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsDownloadRegSaver.h"
#include "DownloadsWnd.h"
#include "fsDownloadMgr.h"
#include "system.h"
#include "mfchelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsDownloadRegSaver::fsDownloadRegSaver()
{
	
}

fsDownloadRegSaver::~fsDownloadRegSaver()
{

}

BOOL fsDownloadRegSaver::Save(t_downloads* vDownloads, LPCSTR pszFileName)
{
	

	m_bDontSaveLogs = _App.DontSaveLogs ();

	fsString strFileName = pszFileName;
	fsString strFileNameBak = pszFileName;

	strFileName += ".sav";
	strFileNameBak += ".bak";

	strFileName = fsGetDataFilePath (strFileName);
	strFileNameBak = fsGetDataFilePath (strFileNameBak);

	
	HANDLE hFile = CreateFile (strFileNameBak, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw;

	
	fsDLFileHdr hdr;
	if (FALSE == WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}
	
	for (size_t i = 0; i < vDownloads->size (); i++)
	{
		LPBYTE pBuffer = NULL;
		DWORD  dwSize;

		try {

		vmsDownloadSmartPtr dld = vDownloads->at (i);

		
		if (FALSE == SaveDownload (dld, NULL, &dwSize))
		{
			CloseHandle (hFile);
			return FALSE;
		}

		fsnew (pBuffer, BYTE, dwSize);

		
		if (FALSE == SaveDownload (dld, pBuffer, &dwSize))
		{
			delete [] pBuffer;
			CloseHandle (hFile);
			return FALSE;
		}

		}
		catch (...) {
			if (pBuffer)
				delete [] pBuffer;
			CloseHandle (hFile);
			return FALSE;
		}

		

		
		if (FALSE == WriteFile (hFile, &dwSize, sizeof (dwSize), &dw, NULL))
		{
			CloseHandle (hFile);
			return FALSE;
		}

		if (FALSE == WriteFile (hFile, pBuffer, dwSize, &dw, NULL))
		{
			CloseHandle (hFile);
			return FALSE;
		}

		delete [] pBuffer;
	}

	FlushFileBuffers (hFile);
	CloseHandle (hFile);

	if (GetFileAttributes (strFileName) != DWORD (-1))
		DeleteFile (strFileName);

	
	return MoveFile (strFileNameBak, strFileName);
}

fsDLLoadResult fsDownloadRegSaver::Load(t_downloads *vDownloads, LPCSTR pszFileName, BOOL bDontLoadIfTooLarge, fsDLLoadFromType lt, BOOL bErrIfNotExists)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	fsDLLoadFromType nlt = DLLFT_SAV; 
	fsDLFileHdr hdr;
	fsString strFileName = pszFileName;
	LPBYTE pBuffer = NULL;

	switch (lt)
	{
	case DLLFT_SAV:
		strFileName += ".sav";
		nlt = DLLFT_BAK;
		break;

	case DLLFT_BAK:
		strFileName += ".bak";
		nlt = DLLFT_SAV;
		break;
	}

	strFileName = fsGetDataFilePath (strFileName);

	if (GetFileAttributes (strFileName) == UINT (-1))
	{
		
		if (nlt == DLLFT_SAV)
			return bErrIfNotExists == FALSE ? DLLR_SUCCEEDED : DLLR_FAILED;
		else
			return Load (vDownloads, pszFileName, bDontLoadIfTooLarge, nlt, bErrIfNotExists);	

	}

	hFile = CreateFile (strFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_ALWAYS, 0, NULL);

	
	
	
	bErrIfNotExists = TRUE;
	

	if (hFile == INVALID_HANDLE_VALUE)
		goto _lErr;

	if (bDontLoadIfTooLarge && 5*1024*1024 < GetFileSize (hFile, NULL))
	{
		CloseHandle (hFile);
		return DLLR_TOOLARGESIZE;
	}

	DWORD dwFileSize;	
	DWORD dw;
	BOOL bUseOLD;
	WORD wVer;

	dwFileSize = GetFileSize (hFile, NULL);
	bUseOLD = FALSE;	
	wVer = 0;	

	
	if (FALSE == ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL) || 
			strnicmp (hdr.szSig, DLFILE_SIG, strlen (DLFILE_SIG)) )
	{
		
		SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
		bUseOLD = TRUE;
	}
	else
	{
		dwFileSize -= dw;  
		wVer = hdr.wVer;
		if (wVer > DLFILE_CURRENT_VERSION)
			goto _lErr;
	}

	vDownloads->clear ();

	
	while (dwFileSize)
	{
		DWORD dwSize;

		pBuffer = NULL;
		
		vmsDownloadSmartPtr dld;
		dld.CreateInstance ();
		dld->pMgr.CreateInstance ();
		

		
		if (!ReadFile (hFile, &dwSize, sizeof (dwSize), &dw, NULL) || dw != sizeof (dwSize))
			goto _lErr;

		if (dwFileSize < sizeof (dwSize) + dwSize)
			goto _lErr;

		
		dwFileSize -= sizeof (dwSize) + dwSize;

		fsnew (pBuffer, BYTE, dwSize);

		
		if (!ReadFile (hFile, pBuffer, dwSize, &dw, NULL) || dw != dwSize)
			goto _lErr;

		

		try{

		if (bUseOLD)
		{
			if (FALSE == OLD_LoadDownload (dld, pBuffer, &dwSize))
				goto _lErr;
		}
		else
		{
			if (FALSE == LoadDownload (dld, pBuffer, &dwSize, wVer))
				goto _lErr;
		}

		}catch (...) {goto _lErr;}

		delete [] pBuffer;

		vDownloads->push_back (dld);
	}

	CloseHandle (hFile);
	
	return DLLR_SUCCEEDED;

_lErr:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);

	if (pBuffer)
		delete [] pBuffer;

	DeleteFile (strFileName);	

	if (nlt == DLLFT_SAV)		
		return DLLR_FAILED;
		
	
	return Load (vDownloads, pszFileName, bDontLoadIfTooLarge, nlt, bErrIfNotExists);
}

BOOL fsDownloadRegSaver::SaveDownload(vmsDownloadSmartPtr dld, LPVOID pBuffer, LPDWORD pdwSize)
{
	DWORD dwNeedSize;

	if (FALSE == dld->pMgr->SaveState (NULL, &dwNeedSize))
		return FALSE;

	

	dwNeedSize += sizeof (dld->bAutoStart);
	
	int iReserved = 0;
	dwNeedSize += sizeof (iReserved);
	dwNeedSize += sizeof (dld->nID);
	dwNeedSize += dld->strComment.GetLength () + sizeof (DWORD);
	dwNeedSize += sizeof (UINT);	
	dwNeedSize += sizeof (dld->dwFlags);
	dwNeedSize += sizeof (dld->dwReserved);
	dwNeedSize += sizeof (dld->dateAdded);

	DWORD cEvents = dld->vEvents.size ();
	if (m_bDontSaveLogs)
		cEvents = 0;
	dwNeedSize += sizeof (DWORD);  

	for (DWORD i = 0; i < cEvents; i++)
		dwNeedSize += 2*sizeof (COLORREF) + sizeof (FILETIME) + sizeof (int) + dld->vEvents [i].strEvent.GetLength () + sizeof (DWORD);

	if (pBuffer == NULL)
	{
		*pdwSize = dwNeedSize;
		return TRUE;
	}

	if (*pdwSize < dwNeedSize)
	{
		*pdwSize = dwNeedSize;
		return FALSE;
	}

	LPBYTE pB = LPBYTE (pBuffer);

	
	DWORD dw = *pdwSize;
	if (FALSE == dld->pMgr->SaveState (pB, &dw))
		return FALSE;
	pB += dw;

	CopyMemory (pB, &dld->bAutoStart, sizeof (dld->bAutoStart));
	pB += sizeof (dld->bAutoStart);

	

	CopyMemory (pB, &iReserved, sizeof (iReserved));
	pB += sizeof (iReserved);

	CopyMemory (pB, &dld->nID, sizeof (dld->nID));
	pB += sizeof (dld->nID);

	dw = dld->strComment.GetLength ();
	CopyMemory (pB, &dw, sizeof (dw));
	pB += sizeof (dw);
	CopyMemory (pB, dld->strComment, dw);
	pB += dw;

	CopyMemory (pB, &dld->pGroup->nId, sizeof (UINT));
	pB += sizeof (UINT);

	CopyMemory (pB, &dld->dwFlags, sizeof (dld->dwFlags));
	pB += sizeof (dld->dwFlags);

	CopyMemory (pB, &dld->dwReserved, sizeof (dld->dwReserved));
	pB += sizeof (dld->dwReserved);

	CopyMemory (pB, &dld->dateAdded, sizeof (dld->dateAdded));
	pB += sizeof (dld->dateAdded);

	CopyMemory (pB, &cEvents, sizeof (DWORD));
	pB += sizeof (DWORD);

	

	for (i = 0; i < cEvents; i++)
	{
		fsDownloadEvents *ev = &dld->vEvents [i];
		
		CopyMemory (pB, &ev->clrBg, sizeof (ev->clrBg));
		pB += sizeof (ev->clrBg);

		CopyMemory (pB, &ev->clrText, sizeof (ev->clrText));
		pB += sizeof (ev->clrText);

		CopyMemory (pB, &ev->timeEvent, sizeof (ev->timeEvent));
		pB += sizeof (ev->timeEvent);

		CopyMemory (pB, &ev->iImage, sizeof (ev->iImage));
		pB += sizeof (ev->iImage);

		dw = ev->strEvent.GetLength ();
		CopyMemory (pB, &dw, sizeof (dw));
		pB += sizeof (dw);
		CopyMemory (pB, dld->vEvents [i].strEvent, dw);
		pB += dw;
	}

	*pdwSize = dwNeedSize;

	return TRUE;
}

BOOL fsDownloadRegSaver::LoadDownload(vmsDownloadSmartPtr dld, LPVOID lpBuffer, LPDWORD lpdwSize, WORD wVer)
{
#define CHECK_BOUNDS(need) if (need < 0 || need > int(*lpdwSize) - (pB - LPBYTE (lpBuffer))) return FALSE;
	
	LPBYTE pB = LPBYTE (lpBuffer);
	DWORD dw = *lpdwSize;
	LPSTR szStr;
	int iReserved;

	if (FALSE == dld->pMgr->LoadState (pB, &dw, wVer))
		return FALSE;
	pB += dw;

	CHECK_BOUNDS (sizeof (dld->bAutoStart));

	CopyMemory (&dld->bAutoStart, pB, sizeof (dld->bAutoStart));
	pB += sizeof (dld->bAutoStart);

	CHECK_BOUNDS (sizeof (iReserved));

	CopyMemory (&iReserved, pB, sizeof (iReserved));
	pB += sizeof (iReserved);

	CHECK_BOUNDS (sizeof (dld->nID));

	CopyMemory (&dld->nID, pB, sizeof (dld->nID));
	pB += sizeof (dld->nID);

	CHECK_BOUNDS (sizeof (dw));

	CopyMemory (&dw, pB, sizeof (dw));
	CHECK_BOUNDS (int (dw));
	pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
	CopyMemory (szStr, pB, dw);
	szStr [dw] = 0; dld->strComment = szStr; delete [] szStr;
	pB += dw;

	if (wVer < 9)
	{
		CHECK_BOUNDS (sizeof (dw));

		CopyMemory (&dw, pB, sizeof (dw));
		CHECK_BOUNDS (int (dw));
		pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
		CopyMemory (szStr, pB, dw);
		szStr [dw] = 0; 
		dld->pGroup = _DldsGrps.FindGroupByName (szStr);
		delete [] szStr;
		pB += dw;
	}
	else
	{
		UINT nGrpId;
		CHECK_BOUNDS (sizeof (UINT));
		CopyMemory (&nGrpId, pB, sizeof (UINT));
		pB += sizeof (UINT);
		dld->pGroup = _DldsGrps.FindGroup (nGrpId);
	}

	if (dld->pGroup == NULL)
		dld->pGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);

	CHECK_BOUNDS (sizeof (dld->dwFlags));
	CopyMemory (&dld->dwFlags, pB, sizeof (dld->dwFlags));
	pB += sizeof (dld->dwFlags);

	CHECK_BOUNDS (sizeof (dld->dwReserved));
	CopyMemory (&dld->dwReserved, pB, sizeof (dld->dwReserved));
	pB += sizeof (dld->dwReserved);

	CHECK_BOUNDS (sizeof (dld->dateAdded));
	CopyMemory (&dld->dateAdded, pB, sizeof (dld->dateAdded));
	pB += sizeof (dld->dateAdded);
	
	DWORD cEvents;
	CHECK_BOUNDS (sizeof (cEvents));
	CopyMemory (&cEvents, pB, sizeof (cEvents));
	pB += sizeof (cEvents);

	dld->vEvents.clear ();

	while (cEvents--)
	{
		fsDownloadEvents ev;

		CHECK_BOUNDS (sizeof (ev.clrBg));
		
		CopyMemory (&ev.clrBg, pB, sizeof (ev.clrBg));
		pB += sizeof (ev.clrBg);

		CHECK_BOUNDS (sizeof (ev.clrText));

		CopyMemory (&ev.clrText, pB, sizeof (ev.clrText));
		pB += sizeof (ev.clrText);

		CHECK_BOUNDS (sizeof (ev.timeEvent));

		CopyMemory (&ev.timeEvent, pB, sizeof (ev.timeEvent));
		pB += sizeof (ev.timeEvent);

		CHECK_BOUNDS (sizeof (ev.iImage));

		CopyMemory (&ev.iImage, pB, sizeof (ev.iImage));
		pB += sizeof (ev.iImage);

		CHECK_BOUNDS (sizeof (dw));

		CopyMemory (&dw, pB, sizeof (dw));
		CHECK_BOUNDS (int (dw));
		pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
		CopyMemory (szStr, pB, dw);
		szStr [dw] = 0; ev.strEvent = szStr; delete [] szStr;
		pB += dw;

		dld->vEvents.add (ev);
	}

	*lpdwSize = pB - LPBYTE (lpBuffer);
	
	return TRUE;
}

BOOL fsDownloadRegSaver::OLD_LoadDownload(vmsDownloadSmartPtr dld, LPVOID lpBuffer, LPDWORD lpdwSize)
{
	#define CHECK_BOUNDS(need) if (need < 0 || need > int(*lpdwSize) - (pB - LPBYTE (lpBuffer))) return FALSE;
	
	LPBYTE pB = LPBYTE (lpBuffer);
	DWORD dw = *lpdwSize;
	LPSTR szStr;
	int iReserved = 0;

	if (FALSE == dld->pMgr->LoadState (pB, &dw, 0))
		return FALSE;
	pB += dw;

	CHECK_BOUNDS (sizeof (dld->bAutoStart));

	CopyMemory (&dld->bAutoStart, pB, sizeof (dld->bAutoStart));
	pB += sizeof (dld->bAutoStart);

	CHECK_BOUNDS (sizeof (iReserved));

	CopyMemory (&iReserved, pB, sizeof (iReserved));
	pB += sizeof (iReserved);

	CHECK_BOUNDS (sizeof (dld->nID));

	CopyMemory (&dld->nID, pB, sizeof (dld->nID));
	pB += sizeof (dld->nID);

	CHECK_BOUNDS (sizeof (dw));

	CopyMemory (&dw, pB, sizeof (dw));
	CHECK_BOUNDS (int (dw));
	pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
	CopyMemory (szStr, pB, dw);
	szStr [dw] = 0; dld->strComment = szStr; delete [] szStr;
	pB += dw;

	CHECK_BOUNDS (sizeof (dw));

	CopyMemory (&dw, pB, sizeof (dw));
	CHECK_BOUNDS (int (dw));
	pB += sizeof (dw);  fsnew (szStr, CHAR, dw+1);
	CopyMemory (szStr, pB, dw);
	szStr [dw] = 0; 
	dld->pGroup = _DldsGrps.FindGroupByName (szStr);
	if (dld->pGroup == NULL)
		dld->pGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);
	delete [] szStr;
	pB += dw;

	dld->dwFlags = dld->dwReserved = 0;
	
	DWORD cEvents;
	CHECK_BOUNDS (sizeof (cEvents));
	CopyMemory (&cEvents, pB, sizeof (cEvents));
	pB += sizeof (cEvents);

	dld->vEvents.clear ();

	while (cEvents--)
	{
		fsDownloadEvents ev;

		CHECK_BOUNDS (sizeof (ev.clrBg));
		
		CopyMemory (&ev.clrBg, pB, sizeof (ev.clrBg));
		pB += sizeof (ev.clrBg);

		CHECK_BOUNDS (sizeof (ev.clrText));

		CopyMemory (&ev.clrText, pB, sizeof (ev.clrText));
		pB += sizeof (ev.clrText);

		CHECK_BOUNDS (sizeof (ev.timeEvent));

		CopyMemory (&ev.timeEvent, pB, sizeof (ev.timeEvent));
		pB += sizeof (ev.timeEvent);

		CHECK_BOUNDS (sizeof (ev.iImage));

		CopyMemory (&ev.iImage, pB, sizeof (ev.iImage));
		pB += sizeof (ev.iImage);

		CHECK_BOUNDS (sizeof (dw));

		CopyMemory (&dw, pB, sizeof (dw));
		CHECK_BOUNDS (int (dw));
		pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
		CopyMemory (szStr, pB, dw);
		szStr [dw] = 0; ev.strEvent = szStr; delete [] szStr;
		pB += dw;

		dld->vEvents.add (ev);
	}

	*lpdwSize = pB - LPBYTE (lpBuffer);
	
	return TRUE;
}
