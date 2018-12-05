#pragma once
//#include "stdafx.h"

#define KI_BIN_COLOR	43
#define KI_BIN_EDGE	24


enum EShotKind
{
	eUnknownShot = 0,
	eGeneralShot,
	eFaceShot,
	eVideoQCErrorShot,
	eAudioQCErrorShot
};

enum EVideoQCErrorType
{
	eNormalShot = 0,
	eBlankShot = 1,
	eColorbarShot = 2,
	eStillShot = 3,
};

enum EAudioQCErrorType
{
	eAudioNormalShot = 0,
	eAudioSilenceShot = 1,
};

class CKShot 
{
public:
	CKShot(EShotKind eShot = eUnknownShot)
	{
		m_EShotKind = eShot;

		m_eVQCType = eNormalShot;
		m_eAQCType = eAudioNormalShot;
		m_strFilePathname = _T("");
		m_qwFileStartPos = 0;
		m_qwFileEndPos = 0;
		m_qwStartTc = 0;
		m_qwEndTc = 0;
		m_qwKeyframeTc = 0;
		m_nStartFrameIdx = 0;
		m_nEndFrameIdx = 0;
		m_nKeyframeIdx = 0;

		memset(m_pbColor, 0, KI_BIN_COLOR);
		memset(m_pbEdge, 0, KI_BIN_EDGE);

		m_listRect.RemoveAll();
	};
	virtual ~CKShot() {};

	CKShot::CKShot(CKShot& operand)
	{
		m_EShotKind = operand.m_EShotKind;

		m_eVQCType = operand.m_eVQCType;
		m_eAQCType = operand.m_eAQCType;
		m_strFilePathname = operand.m_strFilePathname;
		m_qwFileStartPos = operand.m_qwFileStartPos;
		m_qwFileEndPos = operand.m_qwFileEndPos;
		m_qwStartTc = operand.m_qwStartTc;
		m_qwEndTc = operand.m_qwEndTc;
		m_qwKeyframeTc = operand.m_qwKeyframeTc;
		m_nStartFrameIdx = operand.m_nStartFrameIdx;
		m_nEndFrameIdx = operand.m_nEndFrameIdx;
		m_nKeyframeIdx = operand.m_nKeyframeIdx;
		memcpy(operand.m_pbColor, m_pbColor, KI_BIN_COLOR);
		memcpy(operand.m_pbEdge, m_pbEdge, KI_BIN_EDGE);

		m_listRect.RemoveAll();
		POSITION pos = operand.m_listRect.GetHeadPosition();
		while (pos)
		{
			m_listRect.AddTail(m_listRect.GetNext(pos));
		}
	}

	CKShot& operator=(CKShot operand)
	{
		m_EShotKind = operand.m_EShotKind;

		m_eVQCType = operand.m_eVQCType;
		m_eAQCType = operand.m_eAQCType;
		m_strFilePathname = operand.m_strFilePathname;
		m_qwFileStartPos = operand.m_qwFileStartPos;
		m_qwFileEndPos = operand.m_qwFileEndPos;
		m_qwStartTc = operand.m_qwStartTc;
		m_qwEndTc = operand.m_qwEndTc;
		m_qwKeyframeTc = operand.m_qwKeyframeTc;
		m_nStartFrameIdx = operand.m_nStartFrameIdx;
		m_nEndFrameIdx = operand.m_nEndFrameIdx;
		m_nKeyframeIdx = operand.m_nKeyframeIdx;
		memcpy(operand.m_pbColor, m_pbColor, KI_BIN_COLOR);
		memcpy(operand.m_pbEdge, m_pbEdge, KI_BIN_EDGE);

		m_listRect.RemoveAll();
		POSITION pos = operand.m_listRect.GetHeadPosition();
		while (pos)
		{
			m_listRect.AddTail(operand.m_listRect.GetNext(pos));
		}
		operand.m_listRect.RemoveAll();

		return *this;
	}

	//2011.10.17 J.S.Lim : facelist 만 복사하기 위한 operator
	CKShot& operator<=(CKShot operand)
	{
		m_listRect.RemoveAll();
		POSITION pos = operand.m_listRect.GetHeadPosition();
		while (pos)
		{
			m_listRect.AddTail(operand.m_listRect.GetNext(pos));
		}

		return *this;
	}

	EShotKind	m_EShotKind;

	EVideoQCErrorType m_eVQCType;
	EAudioQCErrorType m_eAQCType;
	CString		m_strFilePathname;
	ULONGLONG	m_qwFileStartPos;
	ULONGLONG	m_qwFileEndPos;
	ULONGLONG	m_qwStartTc;
	ULONGLONG	m_qwEndTc;
	ULONGLONG	m_qwKeyframeTc;
	int			m_nStartFrameIdx;
	int			m_nEndFrameIdx;
	int			m_nKeyframeIdx;
	BYTE		m_pbColor[KI_BIN_COLOR];
	BYTE		m_pbEdge[KI_BIN_EDGE];

	CList<RECT, RECT&> m_listRect;
};
