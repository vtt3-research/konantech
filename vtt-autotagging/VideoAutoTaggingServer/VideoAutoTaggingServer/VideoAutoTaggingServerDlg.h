
// VideoAutoTaggingServerDlg.h : 헤더 파일
//

#pragma once

#include "ConnectCMS.h"
#include "FFMPEGProcessOperation.h"
#include "afxcmn.h"

#define CONTROL_MESSAGE WM_USER + 2
#define CONTROL_EDITMAM_MESSAGE WM_USER + 3
#define CONTROL_EDITJOB_MESSAGE WM_USER + 4
#define CONTROL_PROGRESS_MESSAGE WM_USER + 5

// CVideoAutoTaggingServerDlg 대화 상자
class CVideoAutoTaggingServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CVideoAutoTaggingServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DIALOG };

	protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	afx_msg LRESULT OnControlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnControlMAMMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnControlJobMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnControlProgressMessage(WPARAM wParam, LPARAM lParam);

public:

	static void ms_callbackConnect(bool bConnect, LPVOID lpvoid);
	static void ms_callbackDisconnect(LPVOID lpvoid);
	static void ms_callbackJobAlloc(bool bSuccess, LPVOID lpvoid);
	static void ms_callbackAbort(int nJobID, LPVOID lpvoid);

	void StartWork(bool bSuccess);
	void AbortWork(int nJobID);
	void ExtractionThread();

	void AddLabelDescriptionInfo(string stdstrPath, CString strFilePath);
	void AddLabelDescriptionInfoWithFriendsModule(string stdstrPath, CString strFilePath);

public:

	afx_msg void OnBnClickedConnectbtn();
	afx_msg void OnBnClickedDisconnectbtn();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	static UINT ms_threadShot(LPVOID lpvoid);
	
	void onNetworkConnectCS();

	bool getMediaInfo(CString& strFPS);
	bool makeFolder(CString strFolderPath);
	bool makeTempWorkinDir();
	bool changeFileName(CString strFileName, BOOL bShot);

	void threadShot();
	void extractFrame();
	
	void init();
	void cancle();
	void replaceLogFile();
	void writeLog(CString strMsg);

	bool GetShotInfoData(CString strStart, CString strEnd, CString strFilePath, int nStartIndex, int nEndIndex);
	// -------create shot data json file--------
	void GetShotInfoJsonData(string strStart, string strEnd, CString strFolerPath);

	bool getAppInfo(CString& strAppTitle);

	CString getTimecode(double dbTimecode);
	
private:

	CConnectCMS* m_pConnectCMS;

	CMS_INFO m_CMSInfo;
	JOB_INFO m_JobInfo;
	KVPAIR m_Kvpair;

	CWinThread* m_pShotThread;
	
	CFFMPEGProcessOperation m_ffmpeg;

	CString m_strCmd;
	CString m_strJobList;

	bool m_bStandAlone;
	bool m_bTaggingLabels;
	BOOL m_bConnectCMS;
	bool m_bAbort;
	BOOL m_bComplete;
	bool m_bShotThread;
	
	double m_dbFPS;
	CString m_strDuration;

	int m_nTotalFrameCount;
	int m_nWidth;
	int m_nHeight;
	int m_nFrameFFmpegProgress;
	int m_nFrameDataProgress;
	
	CProgressCtrl m_ProgressCtrl;

	CString m_strTempWorkinDir;

	CList<CFileItemInfo, CFileItemInfo &>* m_plistResultFile;
	vector<int> m_vecShotFrame;

	bool m_bWndCreated;
public:
	afx_msg void OnExitSizeMove();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
