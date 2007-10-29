/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "fsSpeedMeter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsSpeedMeter::fsSpeedMeter()
{
	m_dwSpeed = 0;
	m_iCurPrevSpeed = 0;
	m_iMaxPrevSpeed = 0;
	m_cMeasures = 0;
	m_done = 0;
	SetInertness (5);
}

fsSpeedMeter::~fsSpeedMeter()
{

}

void fsSpeedMeter::Done(DWORD dwDone)
{
	UpdateSpeed ();
	m_done += dwDone;
}

DWORD fsSpeedMeter::GetSpeed()
{
	UpdateSpeed ();

	
	if (m_iCurPrevSpeed > 0)
		return m_dwSpeed;	
	else
	{
		m_now.Now ();
		if (m_now - m_past == 0)
			return 0;
		
		return (UINT) ((double) m_done / (m_now - m_past) * 1000.f);
	}
}

void fsSpeedMeter::Reset()
{
	m_past.Now ();
	m_done = 0;
	m_iCurPrevSpeed = 0;
	m_dwSpeed = 0;
	m_cMeasures = 0;
}

void fsSpeedMeter::UpdateSpeed()
{
	m_now.Now ();

	
	if (m_now - m_past > 1000)
	{
		
		if (m_iCurPrevSpeed > m_iMaxPrevSpeed)
		{
			
			BOOL bAllGreater = TRUE;	
			BOOL bAllLess = TRUE;		

			for (int i = 0; i < m_iMaxPrevSpeed; i++)
			{
				if (m_aPrevSpeed [i] < m_dwSpeed)
					bAllGreater = FALSE;

				if (m_aPrevSpeed [i] > m_dwSpeed)
					bAllLess = FALSE;
			}

			
			if (bAllGreater || bAllLess)
				m_cMeasures = 0;	

			m_iCurPrevSpeed = 0;
		}

		
		m_aPrevSpeed [m_iCurPrevSpeed++] = (UINT) ((double) m_done / (m_now - m_past) * 1000.f);

		if (m_cMeasures == 0)
			m_dwSpeed = m_aPrevSpeed [m_iCurPrevSpeed-1]; 

		m_cMeasures++;

		
		m_dwSpeed = DWORD (m_dwSpeed * ((double)(m_cMeasures-1) / m_cMeasures) + 
													(double) m_aPrevSpeed [m_iCurPrevSpeed-1] / m_cMeasures);

		m_past.Now ();
		m_done = 0;
	}
}

void fsSpeedMeter::SetInertness(int val)
{
	if (val < 10)
	{
		m_iMaxPrevSpeed = val;
		m_iCurPrevSpeed = 0;
		m_dwSpeed = 0;
	}
}
