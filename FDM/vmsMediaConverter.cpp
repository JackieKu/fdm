/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsMediaConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsDLL vmsMediaConverter::m_dll;
long vmsMediaConverter::m_cDllRefs = 0;

vmsMediaConverter::vmsMediaConverter()
{
	
}

vmsMediaConverter::~vmsMediaConverter()
{

}

BOOL vmsMediaConverter::ConvertMedia(LPCTSTR pszSrcFile, LPCTSTR pszDstFile, LPCTSTR pszDstFormat,
						  LPCTSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCTSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel)
{
	Initialize ();
	
	typedef BOOL (*FNCM)(LPCTSTR pszSrcFile, LPCTSTR pszDstFile, LPCTSTR pszDstFormat,
						  LPCTSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCTSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel);

	FNCM pfn = (FNCM) m_dll.GetProcAddress (_T("ConvertMediaFile"));

	BOOL bRes = FALSE;

	if (pfn)
	{
		bRes = pfn (pszSrcFile, pszDstFile, pszDstFormat, pszAudioCodec, nAudioChannels,
			nAudioBitrate, nAudioRate, pszVideoCodec, nVideoBitrate, nVideoFrameRate,
			nVideoFrameWidth, nVideoFrameHeight, pnProgress, pbCancel);
	}

	Shutdown ();

	return bRes;
}

void vmsMediaConverter::Initialize()
{
	InterlockedIncrement (&m_cDllRefs);

	if (m_dll.is_Loaded () == false)
	{
		m_dll.Load (_T("mediaconverter.dll"));
		typedef void (*FNI)();
		FNI pfn = (FNI) m_dll.GetProcAddress (_T("Initialize"));
		if (pfn)
			pfn ();
	}
}

void vmsMediaConverter::Shutdown()
{
	InterlockedDecrement (&m_cDllRefs);

	if (m_cDllRefs == 0)
	{
		typedef void (*FNS)();
		FNS pfn = (FNS) m_dll.GetProcAddress (_T("Shutdown"));
		if (pfn)
			pfn ();
		m_dll.Free ();
	}
}
