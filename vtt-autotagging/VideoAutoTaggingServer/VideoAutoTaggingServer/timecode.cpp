#include "stdafx.h"
#include "timecode.h"

void FrameToTimecodeNTSC(LONG lFrames, int &h, int &m, int &s, int &f)
{
	LONG lReste;

	// one hour
	h = (int)(lFrames / FR_HOUR_DROP);
	lReste = lFrames % FR_HOUR_DROP;

	// 10 minutes
	m = (int)(10L * (lReste / FR_TENMIN_DROP));
	lReste %= FR_TENMIN_DROP;


	if (lReste >= FR_MINUTE_ND)
	{
		lReste -= FR_MINUTE_ND;
		// The first minute of a decade contains 1800 frames
		// and the 9 others contain 1798 frames 
		m += (int)(1 + lReste / FR_MINUTE_DROP);
		lReste %= FR_MINUTE_DROP;
		// two frames advance for the current minute
		lReste += 2;
	}

	s = (int)(lReste / FR_SECOND);
	f = (int)(lReste % FR_SECOND);
}

void FrameToTimecodePAL(LONG lFrames, int &h, int &m, int &s, int &f)
{
	LONG lReste;

	h = (BYTE)(lFrames / FR_HOUR_PAL);
	lReste = lFrames % FR_HOUR_PAL;

	m = (BYTE)(lReste / FR_MINUTE_PAL);
	lReste = lReste % FR_MINUTE_PAL;

	s = (BYTE)(lReste / FR_SECOND_PAL);

	f = (BYTE)(lReste % FR_SECOND_PAL);
}

void FrameToTimecode24Drop(LONG lFrames, int &h, int &m, int &s, int &frames)
{
	LONG lReste;

	// one hour
	h = (int)(lFrames / FR_HOUR_24_DROP);
	lReste = lFrames % FR_HOUR_24_DROP;

	// 10 minutes
	m = (int)(10L * (lReste / FR_TENMIN_24_DROP));
	lReste %= FR_TENMIN_24_DROP;


	if (lReste >= FR_MINUTE_24_ND)
	{
		lReste -= FR_MINUTE_24_ND;
		// The first minute of a decade contains 1440 frames
		// and the 9 others contain 1438 frames 
		m += (int)(1 + lReste / FR_MINUTE_24_DROP);
		lReste %= FR_MINUTE_24_DROP;
		// two frames advance for the current minute
		lReste += 2;
	}

	s = (int)(lReste / FR_SECOND_24);
	frames = (int)(lReste % FR_SECOND_24);
}

void FrameToTimecode60Drop(LONG lFrames, int &h, int &m, int &s, int &frames)
{
	LONG lReste;

	// one hour
	h = (int)(lFrames / (FR_HOUR_DROP * 2));
	lReste = lFrames % (FR_HOUR_DROP * 2);

	// 10 minutes
	m = (int)(10L * (lReste / (FR_TENMIN_DROP * 2)));
	lReste %= FR_TENMIN_DROP * 2;


	if (lReste >= (FR_MINUTE_ND * 2))
	{
		lReste -= FR_MINUTE_ND * 2;
		
		m += (int)(2 + lReste / (FR_MINUTE_DROP * 2));
		lReste %= FR_MINUTE_DROP * 2;
		
		lReste += 4;
	}

	s = (int)(lReste / (FR_SECOND * 2));
	frames = (int)(lReste % (FR_SECOND * 2));
}

void FrameToTimecodeETC(LONG lFrames, double dbFrameRate, int &h, int &m, int &s, int &f)
{
	// h
	h = lFrames / (LONG)(3600 * dbFrameRate);
	lFrames -= h * (LONG)(3600 * dbFrameRate);

	// m
	m = lFrames / (LONG)(60 * dbFrameRate);
	lFrames -= m * (LONG)(60 * dbFrameRate);

	// s
	s = lFrames / (LONG)dbFrameRate;
	lFrames -= s * (LONG)dbFrameRate;

	// f
	f = (int)lFrames;
}

CTimecodeFrameCountOperation::CTimecodeFrameCountOperation()
{
	m_nVideoStandard = NTSC;
	m_dbFrameRate = 29.97;
}

CTimecodeFrameCountOperation::CTimecodeFrameCountOperation(double dbFrameRate)
{
	SetFrameRate(dbFrameRate);
}
	
CTimecodeFrameCountOperation::~CTimecodeFrameCountOperation()
{

}

void CTimecodeFrameCountOperation::SetFrameRate(double dbFrameRate)
{
	m_nVideoStandard = ETC;
	m_dbFrameRate = dbFrameRate;

	int nFrameRate = (int)dbFrameRate;

	if (nFrameRate < dbFrameRate && nFrameRate == 29)
	{
		m_nVideoStandard = NTSC;
	}

	if (nFrameRate < dbFrameRate && nFrameRate == 23)
	{
		m_nVideoStandard = DROP24;
	}

	if (nFrameRate < dbFrameRate && nFrameRate == 59)
	{
		m_nVideoStandard = DROP60;
	}

	if (dbFrameRate == 25)
	{
		m_nVideoStandard = PAL;
	}
}

void CTimecodeFrameCountOperation::FrameToTimecode(LONG dwFrames, int &h, int &m, int &s, int &f)
{
	if (m_nVideoStandard == NTSC)
		FrameToTimecodeNTSC((LONG)dwFrames, h, m, s, f);

	if (m_nVideoStandard == PAL)
		FrameToTimecodePAL((LONG)dwFrames, h, m, s, f);

	if (m_nVideoStandard == DROP24)
		FrameToTimecode24Drop((LONG)dwFrames, h, m, s, f);

	if (m_nVideoStandard == DROP60)
		FrameToTimecode60Drop((LONG)dwFrames, h, m, s, f);

	if (m_nVideoStandard == ETC)
		FrameToTimecodeETC((LONG)dwFrames, m_dbFrameRate, h, m, s, f);
}

LONG CTimecodeFrameCountOperation::TimecodeToFrame(int h, int m, int s, int f)
{
	LONG lFrames;

	if (m_nVideoStandard == PAL)		// PAL
	{
		lFrames = (LONG)h * FR_HOUR_PAL + (LONG)m * FR_MINUTE_PAL + (LONG)s * FR_SECOND_PAL + (LONG)f;
	}

	if (m_nVideoStandard == NTSC)							// NTSC
	{
		// h
		lFrames = (LONG)h * FR_HOUR_DROP;

		// m
		LONG m10 = m / 10;
		LONG m01 = m % 10;

		lFrames += m10 * FR_TENMIN_DROP;
		lFrames += m01 * FR_MINUTE_DROP;

		// s
		lFrames += (LONG)s * FR_SECOND;

		// f
		lFrames += f;
	}

	if (m_nVideoStandard == DROP24)
	{
		// h
		lFrames = (LONG)h * FR_HOUR_24_DROP;

		// m
		LONG m10 = m / 10;
		LONG m01 = m % 10;

		lFrames += m10 * FR_TENMIN_24_DROP;
		lFrames += m01 * FR_MINUTE_24_DROP;

		// s
		lFrames += (LONG)s * FR_SECOND_24;

		// f
		lFrames += f;
	}

	if (m_nVideoStandard == DROP60)
	{
		// h
		lFrames = (LONG)h * (FR_HOUR_DROP * 2);

		// m
		LONG m10 = m / 10;
		LONG m01 = m % 10;

		lFrames += m10 * (FR_TENMIN_DROP * 2);
		lFrames += m01 * (FR_MINUTE_DROP * 2);

		// s
		lFrames += (LONG)s * (FR_SECOND * 2);

		// f
		lFrames += f;
	}

	if (m_nVideoStandard == ETC)
	{
		// h
		lFrames = (LONG)h * (LONG)(3600 * m_dbFrameRate);

		// m
		lFrames += (LONG)m * (LONG)(60 * m_dbFrameRate);

		// s
		lFrames += (LONG)s * (LONG)m_dbFrameRate;

		// f
		lFrames += f;
	}

	return lFrames;
}

void CTimecodeFrameCountOperation::SecondsToTimecode(int nSeconds, int &h, int &m, int &s, int &frames)
{
	int nFrame = 0;
	this->SecondsToFrame(nSeconds, nFrame);
	this->FrameToTimecode((DWORD)nFrame, h, m, s, frames);
}

void CTimecodeFrameCountOperation::SecondsToFrame(int nSeconds, int &frames)
{
	int nHour = nSeconds / 3600;

	int nMin = (nSeconds % 3600) / 60;

	int nSec = (nSeconds % 3600) % 60;

	int nFrame = 0;

	frames = this->TimecodeToFrame(nHour, nMin, nSec, nFrame);
}

int g_nVideoStandard = NTSC;

void SetVideoStandard(int nVideoStandard)
{
	g_nVideoStandard = nVideoStandard;
}

int GetVideoStandard()
{
	return g_nVideoStandard;
}


UINT FrameToTimecode(LONG lFrames)
{
	int h, m, s, f;
	FrameToTimecode(lFrames, h, m, s, f);
	return SET_TIMECODE_FRAME(h, m, s, f);
}

void FrameToTimecode(LONG lFrames, int &h, int &m, int &s, int &f)
{
	LONG lReste;

	if(g_nVideoStandard == PAL)		// PAL
	{
		h     = (BYTE)(lFrames / FR_HOUR_PAL);
		lReste       = lFrames % FR_HOUR_PAL;

		m   = (BYTE)(lReste / FR_MINUTE_PAL);
		lReste       = lReste % FR_MINUTE_PAL;

		s   = (BYTE)(lReste / FR_SECOND_PAL);

		f  = (BYTE)(lReste % FR_SECOND_PAL);
	}
	else				// NTSC
	{
		// one hour
		h    = (int)(lFrames / FR_HOUR_DROP);
		lReste       = lFrames % FR_HOUR_DROP;

		// 10 minutes
		m = (int)(10L * (lReste / FR_TENMIN_DROP));
		lReste  %= FR_TENMIN_DROP;


		if (lReste >= FR_MINUTE_ND)
		{
			lReste  -= FR_MINUTE_ND;
			// The first minute of a decade contains 1800 frames
			// and the 9 others contain 1798 frames 
			m += (int)(1 + lReste / FR_MINUTE_DROP);
			lReste  %= FR_MINUTE_DROP;
			// two frames advance for the current minute
			lReste  += 2;
		}

		s = (int)(lReste / FR_SECOND);
		f = (int)(lReste % FR_SECOND);
	}
}

void FrameToTimecode24DropFPS (LONG lFrames, int &h, int &m, int &s, int &frames)
{
	LONG lReste;

	// one hour
	h    = (int)(lFrames / FR_HOUR_24_DROP);
	lReste       = lFrames % FR_HOUR_24_DROP;

	// 10 minutes
	m = (int)(10L * (lReste / FR_TENMIN_24_DROP));
	lReste  %= FR_TENMIN_24_DROP;


	if (lReste >= FR_MINUTE_24_ND)
	{
		lReste  -= FR_MINUTE_24_ND;
		// The first minute of a decade contains 1440 frames
		// and the 9 others contain 1438 frames 
		m += (int)(1 + lReste / FR_MINUTE_24_DROP);
		lReste  %= FR_MINUTE_24_DROP;
		// two frames advance for the current minute
		lReste  += 2;
	}

	s = (int)(lReste / FR_SECOND_24);
	frames = (int)(lReste % FR_SECOND_24);

}

LONG TimecodeToFrame(UINT tc)
{
	return TimecodeToFrame(GET_TIMECODE_HOURS(tc),
		GET_TIMECODE_MINUTES(tc),
		GET_TIMECODE_SECONDS(tc),
		GET_TIMECODE_FRAMES(tc));
}

LONG TimecodeToFrame(UINT tc, UINT uTimerMode)
{
	return TimecodeToFrame(GET_TIMECODE_HOURS(tc),
		GET_TIMECODE_MINUTES(tc),
		GET_TIMECODE_SECONDS(tc),
		GET_TIMECODE_FRAMES(tc),
		uTimerMode);
}

LONG TimecodeToField(UINT tc)
{
	return TimecodeToField(GET_TIMECODE_HOURS(tc),
		GET_TIMECODE_MINUTES(tc),
		GET_TIMECODE_SECONDS(tc),
		GET_TIMECODE_FIELDS(tc));
}

LONG TimecodeToFrame(int h, int m, int s, int f)
{
	LONG lFrames;

	if(g_nVideoStandard == PAL)		// PAL
	{
		lFrames = (LONG)h * FR_HOUR_PAL + (LONG)m * FR_MINUTE_PAL + (LONG)s * FR_SECOND_PAL + (LONG)f;
	}
	else							// NTSC
	{
		// h
		lFrames = (LONG)h * FR_HOUR_DROP;

		// m
		LONG m10 = m/10;
		LONG m01 = m%10;

		lFrames += m10 * FR_TENMIN_DROP;
		lFrames += m01 * FR_MINUTE_DROP;

		// s
		lFrames += (LONG)s * FR_SECOND;
		
		// f
		lFrames += f;
	}

	return lFrames;
}

LONG TimecodeToFrame(int h, int m, int s, int f, UINT uTimerMode)
{
	if(uTimerMode == 1 && h>12)		// CTL sense
	{
		LONG lFrame = TimecodeToFrame(h, m, s, f);
		LONG l24 = TimecodeToFrame(24, 0, 0, 0);
		return lFrame - l24;
	}

	return TimecodeToFrame(h, m, s, f);
}

LONG TimecodeToField(int h, int m, int s, int f)
{
	LONG lFields;

	if(g_nVideoStandard == PAL)		// PAL
	{
		lFields = ((LONG)h   * FR_HOUR_PAL + (LONG)m * FR_MINUTE_PAL + (LONG)s * FR_SECOND_PAL + (LONG)f)<<1;
	}
	else							// NTSC
	{
		// h
		lFields = (LONG)h * FR_HOUR_DROP;

		// m
		LONG m10 = m/10;
		LONG m01 = m%10;

		lFields += m10 * FR_TENMIN_DROP;
		lFields += m01 * FR_MINUTE_DROP;

		// s
		lFields += (LONG)s * FR_SECOND;
		
		lFields <<= 1;

		// f
		lFields += f;

//		if(m01 != 0 && s == 0)
//			lFields -= 2;
	}

	return lFields;
}