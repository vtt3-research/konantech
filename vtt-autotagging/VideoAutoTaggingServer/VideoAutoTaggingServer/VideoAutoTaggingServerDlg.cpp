
// VideoAutoTaggingServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "VideoAutoTaggingServer.h"
#include "VideoAutoTaggingServerDlg.h"
#include "afxdialogex.h"
#include "mediainfodll.h"
#include "ConvertUTF8Operation.h"
#include "include\KRestSogangService.h"
#include "include\KRestRecognizeService.h"

#include "json.h"

#pragma warning( disable : 4996 )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SHOT_PROGRESS 20

using namespace MediaInfoDLL;

FILE* g_fpDecLog = NULL;

std::mutex g_mutexLog;

// CVideoAutoTaggingServerDlg 대화 상자

CVideoAutoTaggingServerDlg::CVideoAutoTaggingServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoAutoTaggingServerDlg::IDD, pParent)
	, m_pConnectCMS(NULL)
	, m_pShotThread(NULL)
	, m_bStandAlone(false)
	, m_bTaggingLabels(false)
	, m_bConnectCMS(false)
	, m_bAbort(false)
	, m_bComplete(false)
	, m_bShotThread(false)
	, m_strCmd(_T(""))
	, m_strJobList(_T(""))
	, m_strDuration(_T(""))
	, m_nTotalFrameCount(0)
	, m_dbFPS(0.0f)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nFrameFFmpegProgress(0)
	, m_nFrameDataProgress(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_plistResultFile = NULL;
}

void CVideoAutoTaggingServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CVideoAutoTaggingServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECTBTN, &CVideoAutoTaggingServerDlg::OnBnClickedConnectbtn)
	ON_BN_CLICKED(IDC_DISCONNECTBTN, &CVideoAutoTaggingServerDlg::OnBnClickedDisconnectbtn)
	ON_MESSAGE(CONTROL_MESSAGE, &CVideoAutoTaggingServerDlg::OnControlMessage)
	ON_MESSAGE(CONTROL_EDITMAM_MESSAGE, &CVideoAutoTaggingServerDlg::OnControlMAMMessage)
	ON_MESSAGE(CONTROL_EDITJOB_MESSAGE, &CVideoAutoTaggingServerDlg::OnControlJobMessage)
	ON_MESSAGE(CONTROL_PROGRESS_MESSAGE, &CVideoAutoTaggingServerDlg::OnControlProgressMessage)
	ON_WM_EXITSIZEMOVE()
	ON_WM_SYSCOMMAND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CVideoAutoTaggingServerDlg 메시지 처리기

void CVideoAutoTaggingServerDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	cancle();

	if (m_pConnectCMS)
	{
		m_pConnectCMS->DisconnectCMS();

		delete m_pConnectCMS;
		m_pConnectCMS = NULL;
	}

	CConnectCMS::CloseCompJobManager();

	if (g_fpDecLog != NULL)
	{
		fflush(g_fpDecLog);
		fclose(g_fpDecLog);
		g_fpDecLog = NULL;
	}

	if (m_plistResultFile)
	{
		m_plistResultFile->RemoveAll();

		delete m_plistResultFile;
		m_plistResultFile = NULL;
	}

	CDialogEx::OnClose();
}

BOOL CVideoAutoTaggingServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

#ifdef TEST_SAMPLE_FILE
	CSKMusicIDStreamQuery MID;
	//MID.SetLookupMode(true);
	MID.Initialize();
	MID.DoMusicIDStream();
	MID.DoMusicIDStream();
#endif

	makeTempWorkinDir();

	replaceLogFile();

	m_ProgressCtrl.SetRange(0, 100);

	CString strAppTitle;

	if (getAppInfo(strAppTitle))
	{
		m_pConnectCMS = new CConnectCMS;
		m_pConnectCMS->CallbackJobAlloc(ms_callbackJobAlloc, this);
		m_pConnectCMS->Callback_Connect(ms_callbackConnect, this);
		m_pConnectCMS->CallbackJobAbort(ms_callbackAbort, this);
		m_pConnectCMS->Callback_Disconnect(ms_callbackDisconnect, this);

		PostMessage(CONTROL_EDITMAM_MESSAGE, NULL, NULL);

		m_pConnectCMS->DisconnectCMS(m_CMSInfo, 1);

		if (m_CMSInfo.bAutoConnect)
			onNetworkConnectCS();

		GetDlgItem(IDC_CONNECTBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DISCONNECTBTN)->EnableWindow(FALSE);

		if (!strAppTitle.IsEmpty())
		{
			SetWindowText(strAppTitle);
		}
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CVideoAutoTaggingServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CVideoAutoTaggingServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CVideoAutoTaggingServerDlg::getAppInfo(CString& strAppTitle)
{
	int i;
	TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof path);
	CString strProfileName = path;
	CWinApp *pApp = AfxGetApp();
	CString strTemp = _T("");
	TCHAR sDir[MAX_PATH];

	i = strProfileName.ReverseFind('.');
	strProfileName = strProfileName.Left(i) + _T(".ini");

	free((void*)pApp->m_pszProfileName);
	pApp->m_pszProfileName = _tcsdup(strProfileName.GetString());

	GetPrivateProfileString(_T("Systemconfig"), _T("autoconn"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	if (!strTemp.Compare("true"))
		m_CMSInfo.bAutoConnect = true;
	else
		m_CMSInfo.bAutoConnect = false;

	GetPrivateProfileString(_T("Systemconfig"), _T("MAMUri"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strMamUri.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("MAMExUri"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strExUri.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("ReceiveJobUri"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strReceiveUri.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("RecognizeApiUri"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strRecognizeApiUri.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("password"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strPassword.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("user"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strUser.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("ffmpeg"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strFFmpegPath.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("Systemconfig"), _T("interval"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_CMSInfo.strInterval.Format(_T("%s"), sDir);

	GetPrivateProfileString(_T("Systemconfig"), _T("channel"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	m_CMSInfo.nChannel = atoi(strTemp);

	GetPrivateProfileString(_T("Systemconfig"), _T("pool"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	m_CMSInfo.nPool = atoi(strTemp);

	GetPrivateProfileString(_T("Systemconfig"), _T("standAlone"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	if (!strTemp.Compare("true"))
		m_bStandAlone = true;
	else
		m_bStandAlone = false;

	GetPrivateProfileString(_T("Systemconfig"), _T("taggingLabels"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	if (!strTemp.Compare("true"))
		m_bTaggingLabels = true;
	else
		m_bTaggingLabels = false;

	GetPrivateProfileString(_T("Systemconfig"), _T("ServiceType"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	m_CMSInfo.strServiceType.Format(_T("%s"), sDir);

	GetPrivateProfileString(_T("Systemconfig"), _T("SceneSensitivity"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strTemp.Format(_T("%s"), sDir);
	m_CMSInfo.strSceneSensitivity.Format(_T("%s"), sDir);

	GetPrivateProfileString(_T("AppInfo"), _T("apptitle"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	strAppTitle.Format(_T("%s"), sDir);
	GetPrivateProfileString(_T("AppInfo"), _T("jobname"), _T(""), sDir, MAX_PATH, pApp->m_pszProfileName);
	m_strJobList.Format(_T("%s"), sDir);

	return true;
}

void CVideoAutoTaggingServerDlg::replaceLogFile()
{
	CTime cTime = CTime::GetCurrentTime();
	CString strLogFile; 
	char strModuleFolder[MAX_PATH], *p;
	CString strLogFolder;

	GetModuleFileName(NULL, strModuleFolder, MAX_PATH);
	p = strrchr(strModuleFolder, '\\');
	if (p)
	{
		*(p + 1) = '\0';
	}

	strLogFolder.Format(_T("%sLog"), strModuleFolder);
	_mkdir(strLogFolder);

	strLogFile.Format(_T("%s/%04d%02d%02d_Log.txt"), strLogFolder, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());

	if (g_fpDecLog)
	{
		fclose(g_fpDecLog);
		g_fpDecLog = NULL;
	}

	g_fpDecLog = fopen(strLogFile, "a+t");
}

void CVideoAutoTaggingServerDlg::ms_callbackConnect(bool bConnect, LPVOID lpvoid)
{
	CVideoAutoTaggingServerDlg* pEF = (CVideoAutoTaggingServerDlg*)lpvoid;
	pEF->PostMessage(CONTROL_MESSAGE, (WPARAM)bConnect, NULL);
}

void CVideoAutoTaggingServerDlg::ms_callbackDisconnect(LPVOID lpvoid)
{
	CVideoAutoTaggingServerDlg* pEF = (CVideoAutoTaggingServerDlg*)lpvoid;
	pEF->PostMessage(CONTROL_MESSAGE, (WPARAM)false, NULL);
}

void CVideoAutoTaggingServerDlg::ms_callbackJobAlloc(bool bSuccess, LPVOID lpvoid)
{
	CVideoAutoTaggingServerDlg* pEF = (CVideoAutoTaggingServerDlg*)lpvoid;
	pEF->StartWork(bSuccess);
}

void CVideoAutoTaggingServerDlg::ms_callbackAbort(int nJobID, LPVOID lpvoid)
{
	CVideoAutoTaggingServerDlg* pDlg = (CVideoAutoTaggingServerDlg*)lpvoid;
	pDlg->AbortWork(nJobID);
}

void CVideoAutoTaggingServerDlg::init()
{
	m_bAbort = false;
	m_bComplete = false;

	m_strCmd = _T("");
	m_nTotalFrameCount = 0;

	m_pShotThread = NULL;
}

void CVideoAutoTaggingServerDlg::onNetworkConnectCS()
{
	int i = 0, j = 0;
	CString strMsg;

	try
	{
		//CKRestSogangService LabelDescriptionService;
		//LabelDescriptionService.SetServiceInfo("http://10.10.18.183:8000");

		//int nError = 0;
		//if (nError = LabelDescriptionService.LabelDetectionOnImageWithModuleName("f:/F0000370.jpg", "friends"))
		//{
		//	CString strMsg;

		//	string stdstrMsg = LabelDescriptionService.GetServiceMessage(nError);
		//	CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrMsg.c_str(), strMsg);
		//	writeLog(strMsg);

		//	strMsg.Format(_T("Label description failed"));
		//	writeLog(strMsg);
		//}

		//string strTaggingData = LabelDescriptionService.GetStrResponse();

		m_bConnectCMS = CConnectCMS::OpenCompJobManager(m_CMSInfo.strMamUri, m_CMSInfo.strExUri, m_CMSInfo.strReceiveUri, m_CMSInfo.strUser, m_CMSInfo.strPassword);

		if (m_bConnectCMS == TRUE)
		{
			strMsg.Format(_T("MAM uri : '%s'"), m_CMSInfo.strMamUri);
			writeLog(strMsg);
			strMsg.Format(_T("MAMEx uri : '%s'"), m_CMSInfo.strExUri);
			writeLog(strMsg);

			m_pConnectCMS->RegisterWorkerToMam(m_CMSInfo, m_strJobList, m_bStandAlone);
		}
		else
		{
			strMsg.Format(_T("MAM Connection Fail !!!"));
			writeLog(strMsg);
		}
	}
	catch (CKException *pEx)
	{
		strMsg.Format("Network 연결에 실패하였습니다.");
		writeLog(strMsg);

		if (m_bConnectCMS == TRUE)
		{
			CConnectCMS::CloseCompJobManager();
			m_bConnectCMS = FALSE;
		}

		delete pEx;
	}
}

void CVideoAutoTaggingServerDlg::StartWork(bool bSuccess)
{	
	CString strMsg = _T("");

	if (!bSuccess)
	{
		strMsg.Format(_T("작업정보 읽기를 실패했습니다."));
		writeLog(strMsg);
		m_pConnectCMS->ReportResult(CATALOGING_JOB_RESULT_ERROR, m_JobInfo.strJobInfo);
		return;
	}

	init();

	m_Kvpair = m_pConnectCMS->GetKvPair();
	m_JobInfo = m_pConnectCMS->GetJobInfo();
	
	CString* pstrCurrentJobName = new CString(m_Kvpair.m_strCurrentJobName);

	PostMessage(CONTROL_EDITJOB_MESSAGE, (WPARAM)pstrCurrentJobName, NULL);

	m_JobInfo.strSrcFullPath.Format(_T("%s%s"), m_JobInfo.strSrcStorage, m_JobInfo.strSrcFilepath);
	m_JobInfo.strDstFullPath.Format(_T("%s%s"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath);

	if (m_bShotThread == false)
	{
		m_bShotThread = true;
		m_pShotThread = AfxBeginThread(ms_threadShot, this);
	}
}

void CVideoAutoTaggingServerDlg::AbortWork(int nJobID)
{
	if (nJobID > 0)
	{
		CString strAbortMsg;
		strAbortMsg.Format(_T("Received abort job(ID: %d)"), nJobID);
		writeLog(strAbortMsg);
		m_pConnectCMS->ReportResult(CATALOGING_JOB_RESULT_ABORT, m_JobInfo.strJobInfo);
	}

	cancle();
}

UINT CVideoAutoTaggingServerDlg::ms_threadShot(LPVOID lpvoid)
{
	CVideoAutoTaggingServerDlg* pDlg = (CVideoAutoTaggingServerDlg*)lpvoid;
	pDlg->threadShot();

	return 0;
}

void CVideoAutoTaggingServerDlg::threadShot()
{
	CString strResult = _T("");
	CString strLastCmd = _T("");
	CString strMsg = _T("");
	CString strTempPath = _T("");
	std::string strXml = "";

	bool bSuccess = false;
	int nProgress = 0, nOldProgress = -1, nProgressWithInterval = 0;
	int nErr = 0;

	strTempPath.Format(_T("%s\\S%%05d.jpg"), m_strTempWorkinDir);

	CString strFPS;
	if (getMediaInfo(strFPS))
	{
		m_dbFPS = atof(strFPS);
	}
	else
		m_bAbort = true;

	strMsg.Format(_T("SRC File : %s\nDst Folder : %s"), m_JobInfo.strSrcFullPath, m_JobInfo.strDstFullPath);
	writeLog(strMsg);

	CUtil::DeleteFolder(m_JobInfo.strDstFullPath, FALSE);
	CUtil::DeleteFolder(m_strTempWorkinDir, FALSE);
	
	if (!m_plistResultFile)
		m_plistResultFile = new CList<CFileItemInfo, CFileItemInfo &>;
	else
		m_plistResultFile->RemoveAll();

	// 폴더없으면 생성
	if (!makeFolder(m_JobInfo.strDstFullPath))
	{
		strMsg.Format(_T("DST Folder Create Fail."));
		writeLog(strMsg);

		m_pConnectCMS->ReportResult(CATALOGING_JOB_RESULT_ERROR, m_JobInfo.strJobInfo);
		return;
	}

	m_strCmd.Format(_T(" -i %s -vsync 2 -filter:v select='gt(scene,%s)',showinfo -an -y %s"), m_JobInfo.strSrcFullPath, m_CMSInfo.strSceneSensitivity, strTempPath);

	strMsg.Format(_T("FFMPEG CMD : %s"), m_strCmd);
	writeLog(strMsg);

	bool bFailChangeFileName = false;

	m_ffmpeg.SetSrcVideoFrameRate(_ttof((LPCTSTR)strFPS));
	m_ffmpeg.initExePath(m_CMSInfo.strFFmpegPath, m_nTotalFrameCount);

	if (m_ffmpeg.StartFFMPEGProcess(m_strCmd))
	{
		CString* pstrCurrentJobState = new CString();
		pstrCurrentJobState->Format(_T("%s  Working"), m_Kvpair.m_strCurrentJobName);

		PostMessage(CONTROL_EDITJOB_MESSAGE, (WPARAM)pstrCurrentJobState, NULL);

		while (1)
		{
			Sleep(3000);

			if (m_bAbort)
			{
				m_ffmpeg.AbortFFMPEGProcess();
				m_bComplete = false;
				break;
			}

			bFailChangeFileName = !changeFileName(strTempPath, TRUE);

			//shot ffmpeg 완료 => 10%
			if (!m_ffmpeg.IsRunningFFMPEGProcessShot(&nProgress))
			{
				Sleep(1000);
				break;
			}
			else
			{
				nProgressWithInterval = (int)(nProgress / 10);
				if (nProgressWithInterval != nOldProgress)
				{
					//report progress
					if (!m_bStandAlone)
						m_pConnectCMS->ReportPregress(nProgressWithInterval);

					PostMessage(CONTROL_PROGRESS_MESSAGE, nProgressWithInterval, NULL);

					nOldProgress = nProgressWithInterval;
				}
			}
		}

		if (!m_bAbort)
		{
			nErr = m_ffmpeg.IsCompleteFFMPEGProcess(strResult, TRUE);
			m_strDuration = m_ffmpeg.GetDuration();
		}
	}
	else
		nErr = m_ffmpeg.GetErrorCode();

	strLastCmd = m_ffmpeg.GetLastCmd();
	writeLog(strLastCmd);

	if (m_bAbort || bFailChangeFileName)
		m_bComplete = false;
	else
	{
		if (nErr > 0)
		{
			m_bComplete = false;
		}
		else
		{
			m_bComplete = changeFileName(strTempPath, TRUE);
		}
	}

	//10 frame 마다 뽑기.
	if (m_bComplete && !m_bAbort)
	{
		if (!m_bStandAlone)
			m_pConnectCMS->ReportPregress(10);

		PostMessage(CONTROL_PROGRESS_MESSAGE, 10, NULL);

		extractFrame();
	}

	if (m_bComplete)
	{
		if (!m_bStandAlone)
			m_pConnectCMS->ReportPregress(100);

		PostMessage(CONTROL_PROGRESS_MESSAGE, 100, NULL);

		CString* pstrCurrentJobState = new CString();
		pstrCurrentJobState->Format(_T("%s  Success"), m_Kvpair.m_strCurrentJobName);

		PostMessage(CONTROL_EDITJOB_MESSAGE, (WPARAM)pstrCurrentJobState, NULL);
	}
	else
	{
		CString* pstrCurrentJobState = new CString();
		pstrCurrentJobState->Format(_T("%s  Fail"), m_Kvpair.m_strCurrentJobName);

		PostMessage(CONTROL_EDITJOB_MESSAGE, (WPARAM)pstrCurrentJobState, NULL);

		strMsg.Format(_T("프레임 이미지 추출 실패"));
		writeLog(strMsg);
	}

	if (m_bStandAlone)
		PostMessage(CONTROL_MESSAGE, TRUE, NULL);
	else
	{
		if (m_bComplete && !m_bAbort)
			m_pConnectCMS->ReportResult(CATALOGING_JOB_RESULT_COMPLETE, m_JobInfo.strJobInfo);
		else
			m_pConnectCMS->ReportResult(CATALOGING_JOB_RESULT_ERROR, m_JobInfo.strJobInfo);
	}

	strMsg.Format(_T("작업을 종료하였습니다."));
	writeLog(strMsg);

	if (m_plistResultFile)
	{
		m_plistResultFile->RemoveAll();

		delete m_plistResultFile;
		m_plistResultFile = NULL;
	}

	m_bShotThread = false;
}

void CVideoAutoTaggingServerDlg::extractFrame()
{
	bool bFailChangeFileName = false;
	CString strMsg = _T("");
	CString strFPS = _T("");
	CString strTempPath = _T("");
	CString strResult = _T("");
	CString strLastCmd = _T("");
	int nProgress = 0, nOldProgress = -1, nProgressWithInterval = 0;
	int nErr = 0;
	m_dbFPS = 0.0f;
	double dbPTSTime = 0.0f;
	int nNum = 0;
	int nFrame = 0;
	m_nFrameFFmpegProgress = 0;
	m_nFrameDataProgress = 0;

	m_vecShotFrame.clear();

	strTempPath.Format(_T("%s\\F%%07d.jpg"), m_strTempWorkinDir);

	if (getMediaInfo(strFPS))
	{
		m_dbFPS = atof(strFPS);

		strFPS.Format(_T("%f"), m_dbFPS);
		m_strCmd.Format(_T(" -i %s -qscale:v 1 -r %s/%s -y %s"), m_JobInfo.strSrcFullPath, strFPS, m_CMSInfo.strInterval, strTempPath);

		strMsg.Format(_T("FFMPEG CMD : %s"), m_strCmd);
		writeLog(strMsg);
	}
	else
		m_bAbort = true;

	int nStartIndex = 0;
	CString strFilePath = _T("");
	CString strStart = _T("00:00:00;00");
	CString strEnd = _T("00:00:00;00");
	CString strFolderPath = _T("");

	//report shot data 완료 => 총 20%

	strMsg.Format(_T("shot data update start"));
	writeLog(strMsg);

	while (1)
	{
		dbPTSTime = m_ffmpeg.GetPTSTime(nNum);

		strFilePath.Format(_T("%sS%05d.jpg"), m_JobInfo.strDstFilePath, nNum);
		strFolderPath.Format(_T("%s%sS%05d"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath, nNum);

		if (dbPTSTime == -1)
		{
			CString strTemp = _T("");
			CString strFrame = _T("");
			CString strTime = _T("");
			double dbTemp = 0.0f;
			double dbFrame = 0.0f;

			strTime = m_strDuration.Left(m_strDuration.Find('.'));
			strTemp = m_strDuration.Right(m_strDuration.GetLength() - m_strDuration.Find('.'));
			strFrame.Format(_T("0%s"), strTemp);
			dbTemp = atof(strFrame);
			dbFrame = dbTemp * m_dbFPS;

			strEnd.Format(_T("%s;%d"), strTime, (int)dbFrame);

			// -------shot data json--------
			GetShotInfoJsonData((LPTSTR)(LPCTSTR)strStart, (LPTSTR)(LPCTSTR)strEnd, strFolderPath);

			GetShotInfoData(strStart, strEnd, strFilePath, nStartIndex, m_nTotalFrameCount - 1);

			if (!m_bStandAlone)
				m_pConnectCMS->ReportPregress(20);

			PostMessage(CONTROL_PROGRESS_MESSAGE, 20, NULL);

			strMsg.Format(_T("shot data update end"));
			writeLog(strMsg);

			break;
		}

		nFrame = (int)(dbPTSTime * m_dbFPS);
		if (nFrame <= (int)(dbPTSTime * m_dbFPS))
			++nFrame;

		m_vecShotFrame.push_back(nFrame);

		++nNum;

		strEnd = getTimecode(dbPTSTime);
		GetShotInfoData(strStart, strEnd, strFilePath, nStartIndex, nFrame);

		// -------shot data json--------
		GetShotInfoJsonData((LPTSTR)(LPCTSTR)strStart, (LPTSTR)(LPCTSTR)strEnd, strFolderPath);

		nStartIndex = nFrame;
		strStart = strEnd;
	}

	if (!m_plistResultFile)
		m_plistResultFile = new CList<CFileItemInfo, CFileItemInfo &>;
	else
		m_plistResultFile->RemoveAll();

	m_ffmpeg.SetSrcVideoFrameRate(_ttof((LPCTSTR)strFPS));
	m_ffmpeg.initExePath(m_CMSInfo.strFFmpegPath, m_nTotalFrameCount);

	if (m_bAbort)
	{
		m_bComplete = false;
		return;
	}

	if (m_ffmpeg.StartFFMPEGProcess(m_strCmd))
	{
		CString* pstrCurrentJobState = new CString();
		pstrCurrentJobState->Format(_T("%s  Working"), m_Kvpair.m_strCurrentJobName);

		PostMessage(CONTROL_EDITJOB_MESSAGE, (WPARAM)pstrCurrentJobState, NULL);

		while (1)
		{
			Sleep(3000);

			if (m_bAbort)
			{
				m_ffmpeg.AbortFFMPEGProcess();
				m_bComplete = false;
				break;
			}

			bFailChangeFileName = !changeFileName(strTempPath, FALSE);

			if (!m_ffmpeg.IsRunningFFMPEGProcessFrame(&nProgress))
			{
				Sleep(1000);
				break;
			}
			else
			{
				m_nFrameFFmpegProgress = (int)((nProgress * 40) / 100);
				
				if (m_nFrameFFmpegProgress != nOldProgress)
				{
					//report progress
					nProgressWithInterval = m_nFrameFFmpegProgress + m_nFrameDataProgress + SHOT_PROGRESS;

					if (!m_bStandAlone)
						m_pConnectCMS->ReportPregress(nProgressWithInterval);

					PostMessage(CONTROL_PROGRESS_MESSAGE, nProgressWithInterval, NULL);

					nOldProgress = m_nFrameFFmpegProgress;
				}
			}
		}

		if (!m_bAbort)
		{
			nErr = m_ffmpeg.IsCompleteFFMPEGProcess(strResult);
		}
	}
	else
		nErr = m_ffmpeg.GetErrorCode();

	strLastCmd = m_ffmpeg.GetLastCmd();
	writeLog(strLastCmd);

	if (m_bAbort || bFailChangeFileName)
		m_bComplete = false;
	else
	{
		if (nErr > 0)
		{
			m_bComplete = false;
		}
		else
		{	
			if (!m_bStandAlone)
				m_pConnectCMS->ReportPregress(SHOT_PROGRESS + m_nFrameDataProgress + m_nFrameFFmpegProgress);

			PostMessage(CONTROL_PROGRESS_MESSAGE, SHOT_PROGRESS + m_nFrameDataProgress + m_nFrameFFmpegProgress, NULL);

			m_bComplete = changeFileName(strTempPath, FALSE);
		}
	}
}

LRESULT CVideoAutoTaggingServerDlg::OnControlMessage(WPARAM wParam, LPARAM lParam)
{
	bool bConnectBtn = false;

	if (wParam)
		bConnectBtn = true;

	if (!bConnectBtn)
	{
		GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DISCONNECTBTN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CONNECTBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DISCONNECTBTN)->EnableWindow(TRUE);
	}

	return 0;
}

LRESULT CVideoAutoTaggingServerDlg::OnControlMAMMessage(WPARAM wParam, LPARAM lParam)
{
	CString strPort = _T("");
	
	GetDlgItem(IDC_MAM_EDIT)->SetWindowTextA(m_CMSInfo.strMamUri);

	return 0;
}

LRESULT CVideoAutoTaggingServerDlg::OnControlJobMessage(WPARAM wParam, LPARAM lParam)
{
	CString* pstrJobName = (CString*)wParam;

	if (pstrJobName)
	{
		GetDlgItem(IDC_JOB_EDIT)->SetWindowTextA(*pstrJobName);
		delete pstrJobName;
		pstrJobName = NULL;
	}

	return 0;
}

LRESULT CVideoAutoTaggingServerDlg::OnControlProgressMessage(WPARAM wParam, LPARAM lParam)
{
	int nProgress = (int)wParam;

	m_ProgressCtrl.SetPos(nProgress);

	return 0;
}

bool CVideoAutoTaggingServerDlg::getMediaInfo(CString& strFPS)
{
	MediaInfo MI;

	if (MI.Open((LPSTR)(LPCTSTR)m_JobInfo.strSrcFullPath))
	{
		strFPS.Format(_T("%s"), MI.Get(Stream_Video, 0, _T("FrameRate"), Info_Text, Info_Name).c_str());
		m_nTotalFrameCount = atoi(MI.Get(Stream_Video, 0, _T("FrameCount"), Info_Text, Info_Name).c_str());
		m_nWidth = atoi(MI.Get(Stream_Video, 0, _T("Width"), Info_Text, Info_Name).c_str());
		m_nHeight = atoi(MI.Get(Stream_Video, 0, _T("Height"), Info_Text, Info_Name).c_str());

		int nFrameCount = ((int)(m_nTotalFrameCount / 10) - 1) * 10;
		CString strMsg = _T("");

		strMsg.Format(_T("Last Frame File Number = %d"), nFrameCount);
		writeLog(strMsg);
	}
	else
		return false;

	return true;
}

bool CVideoAutoTaggingServerDlg::makeFolder(CString strFolderPath)
{
	CString strTempPath = _T("");
	CString strDrive = _T("");
	CString strMsg = _T("");

	strTempPath.Format(_T("%s"), strFolderPath);
	
	int nFind = strTempPath.ReverseFind('\\');

	if (nFind >= 0)
		strTempPath = strTempPath.Mid(0, nFind + 1);

	nFind = strTempPath.Find(':');
	if (nFind >= 0)
	{
		strDrive = strTempPath.Mid(0, nFind + 1);
		strTempPath = strTempPath.Mid(nFind + 1);
		strTempPath.Replace(_T('/'), _T('\\'));
	}
	else
	{
		strMsg.Format("Don't find Drive letter %s.", strTempPath);
		writeLog(strMsg);
		return false;
	}

	int nError = 0;

	CString strPath;
	TCHAR *tokenStr = NULL;
	TCHAR* szNextToken = NULL;
	tokenStr = _tcstok_s((LPTSTR)(LPCTSTR)strTempPath, _T("\\"), &szNextToken);
	while (tokenStr)
	{
		strPath = strPath + CString(tokenStr) + _T("\\");

		CString strTempPath2 = strDrive + _T("\\") + strPath;
		strTempPath2.TrimRight(_T("\\"));

		DWORD dwExist = GetFileAttributes(strTempPath2);

		if (!((dwExist != INVALID_FILE_ATTRIBUTES) && (dwExist&FILE_ATTRIBUTE_DIRECTORY)))
		{
			if (nError = _tmkdir((LPCTSTR)(strDrive + _T("\\") + strPath)))
			{
				strMsg.Format("Can not make directory %s.", strPath);
				writeLog(strMsg);
				return false;
			}

			dwExist = GetFileAttributes(strTempPath2);

			if (!((dwExist != INVALID_FILE_ATTRIBUTES) && (dwExist&FILE_ATTRIBUTE_DIRECTORY)))
			{
				LOG1("Can not make directory %s.\n", strPath);
				return false;
			}
		}

		tokenStr = _tcstok_s(NULL, _T("\\"), &szNextToken);
	};

	return true;
}

bool CVideoAutoTaggingServerDlg::changeFileName(CString strFileName, BOOL bShot)
{
	CString strOriFilePath = _T("");
	CString strFilePath = _T("");
	CString strFolderPath = _T("");
	CString strJsonFilePath = _T("");
	CString strMsg = _T("");
	string strStart = "";
	string strEnd = "";

	int nNum = 0;
	int nCount = 0;
	int nShotFolder = 0;
	int nProgress = -1;
	__int64 lnTotalSize = 0;

	if (!m_plistResultFile)
		m_plistResultFile = new CList<CFileItemInfo, CFileItemInfo &>;

	if (m_plistResultFile)
	{
		if (!m_plistResultFile->IsEmpty())
		{
			nCount = m_plistResultFile->GetCount();

			for (int i = 0; i < nCount; i++)
			{
				POSITION pos = m_plistResultFile->FindIndex(i);
				CFileItemInfo FileInfo = m_plistResultFile->GetAt(pos);
				strOriFilePath = FileInfo.GetFullFilePath();

				CString strNumb = strOriFilePath.Right(strOriFilePath.GetLength() - strOriFilePath.ReverseFind('\\'));
				CString strLeft = _T("");

				strNumb.Replace(_T("\\"), _T(""));
				strNumb.Replace(_T("S"), _T(""));
				strNumb.Replace(_T("F"), _T(""));

				if (m_bAbort)
					return false;

				while (1)
				{
					strLeft = strNumb.Left(1);

					if (strLeft.Find('0') < 0)
						break;
					else
						strNumb = strNumb.Right(strNumb.GetLength() - strNumb.Find('0') - 1);
				}

				if (strNumb.Find(_T(".tmp")) >= 0)
					continue;

				strNumb.Replace(_T(".jpg"), _T(""));

				nNum = _ttoi((LPCTSTR)strNumb);

				if (bShot)
				{
					//shot folder create
					strFilePath.Format(_T("%s%s\\S%05d.jpg"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath, nNum);
					strFolderPath.Format(_T("%s%sS%05d"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath, nNum);
					if (makeFolder(strFolderPath) == 0)
						return false;
				}
				else
				{
					if (nNum == 2)
					{
						CUtil::DeleteFile((LPCTSTR)strOriFilePath);

						continue;
					}

					if (nNum > 2)
						--nNum;

					--nNum;

					if (nNum == 0)
					{
						strFilePath.Format(_T("%s%s\\S%05d.jpg"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath, nNum);
						CopyFile(strOriFilePath, strFilePath, TRUE);
						strFolderPath.Format(_T("%s%sS%05d"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath, nNum);
						if (makeFolder(strFolderPath) == 0)
							return false;
					}

					int j = 0;

					while (m_vecShotFrame.size() > 0)
					{
						if ((nNum * 10) > m_vecShotFrame[j])
						{
							++j;
							nShotFolder = j;

							if (j == m_vecShotFrame.size())
								break;
						}
						else
							break;
					}

					strFolderPath.Format(_T("%s%sS%05d"), m_JobInfo.strDstStorage, m_JobInfo.strDstFilePath, nShotFolder);
					strFilePath.Format(_T("%s/F%07d.jpg"), strFolderPath, nNum * 10);
					strJsonFilePath.Format(_T("%s/F%07d.json"), strFolderPath, nNum * 10);

					m_nFrameDataProgress = (int)((nNum * 400) / m_nTotalFrameCount);

					if (nProgress != m_nFrameDataProgress)
					{
						if (!m_bStandAlone)
							m_pConnectCMS->ReportPregress(SHOT_PROGRESS + m_nFrameDataProgress + m_nFrameFFmpegProgress);

						PostMessage(CONTROL_PROGRESS_MESSAGE, SHOT_PROGRESS + m_nFrameDataProgress + m_nFrameFFmpegProgress, NULL);
					
						nProgress = m_nFrameDataProgress;
					}
				}

				if (m_bAbort)
					return false;

				if (_trename(strOriFilePath, strFilePath))
				{
					strMsg.Format(_T("파일 이동 실패 (%s -> %s) - errno: %d"), strOriFilePath, strFilePath, errno);
					writeLog(strMsg);

					return false;
				}

				if (!bShot && m_bTaggingLabels)
				{
					if (!m_CMSInfo.strServiceType.CompareNoCase(_T("object")))
					{
						AddLabelDescriptionInfo((LPSTR)(LPCTSTR)strFilePath, strJsonFilePath);
					}
					else
					{
						//AddLabelDescriptionInfo((LPSTR)(LPCTSTR)strFilePath, strJsonFilePath);

						// 임시
						AddLabelDescriptionInfoWithFriendsModule((LPSTR)(LPCTSTR)strFilePath, strJsonFilePath);
					}
				}
			}

			m_plistResultFile->RemoveAll();
		}

		if (!CUtil::List((LPCTSTR)m_strTempWorkinDir, _T(""), m_plistResultFile, &lnTotalSize))
		{
			strMsg.Format(_T("파일 목록 획득 실패(%s)"), m_strTempWorkinDir);
			writeLog(strMsg);

			return false;
		}
	}

	return true;
}

void CVideoAutoTaggingServerDlg::OnBnClickedConnectbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pConnectCMS == NULL)
	{
		m_pConnectCMS = new CConnectCMS;
		m_pConnectCMS->CallbackJobAlloc(ms_callbackJobAlloc, this);
		m_pConnectCMS->CallbackJobAbort(ms_callbackAbort, this);
		m_pConnectCMS->Callback_Connect(ms_callbackConnect, this);
		m_pConnectCMS->Callback_Disconnect(ms_callbackDisconnect, this);
	}

	onNetworkConnectCS();
}


void CVideoAutoTaggingServerDlg::OnBnClickedDisconnectbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strMsg = _T("");
	cancle();

	if (m_pConnectCMS->DisconnectCMS())
	{
		CConnectCMS::CloseCompJobManager();

		strMsg.Format("check out");
		writeLog(strMsg);
	}
	else
	{
		strMsg.Format("disconnect fail!");
		writeLog(strMsg);
	}

	m_bConnectCMS = FALSE;

	PostMessage(CONTROL_MESSAGE, m_bConnectCMS, NULL);
}

void CVideoAutoTaggingServerDlg::writeLog(CString strMsg)
{
	CString		strLog;
	CString		strTime = CUtil::GetCurTime();

	strLog.Format("[%s] %s", strTime, strMsg);

	g_mutexLog.lock();

	try
	{
		LOG1("%s\n", (LPCSTR)strLog);

		_tprintf(_T("%s\n"), strLog);

		g_mutexLog.unlock();
	}
	catch (...)
	{
		g_mutexLog.unlock();
	}
}

void CVideoAutoTaggingServerDlg::cancle()
{
	m_bAbort = true;

	m_ffmpeg.AbortFFMPEGProcess();

	if (m_pShotThread)
	{
		while (m_bShotThread)
		{
			DWORD dwRet = WAIT_OBJECT_0;

			dwRet = WaitForSingleObject(m_pShotThread->m_hThread, INFINITE);
			if (dwRet == WAIT_OBJECT_0)
				break;
		}
	}

	m_pShotThread = NULL;
	m_bShotThread = false;
}

bool CVideoAutoTaggingServerDlg::makeTempWorkinDir()
{
	bool bRet = true;

	CString strMsg;

	CString strLogFile;
	TCHAR strModuleFolder[MAX_PATH], *p;
	CString strWorkingFolder;

	GetModuleFileName(NULL, strModuleFolder, MAX_PATH);
	p = strrchr(strModuleFolder, '\\');
	if (p)
	{
		*(p) = '\0';
	}

	strWorkingFolder.Format(_T("%s\\Temp"), strModuleFolder);
	int nError = _tmkdir((LPCTSTR)strWorkingFolder);
	if (nError && errno != EEXIST)
	{
		strMsg.Format(_T("Failed to create dir(%s) - errno: %d"), strModuleFolder, errno);
		writeLog(strMsg);
	}

	m_strTempWorkinDir = strWorkingFolder;

	return bRet;
}

void CVideoAutoTaggingServerDlg::AddLabelDescriptionInfo(string stdstrPath, CString strFilePath)
{
	string strTaggingData = "";
	FILE* pFile = NULL;

	char* pUri = CConvertUTF8Operation::ConvertStringToUTF8(m_CMSInfo.strRecognizeApiUri);

	string stdstrUri(pUri);

	delete[]pUri;

	int nError = 0;

	if (!m_CMSInfo.strServiceType.CompareNoCase(_T("object")))
	{
		CKRestRecognizeService LabelDescriptionService;
		LabelDescriptionService.SetServiceInfo(stdstrUri);

		if (nError = LabelDescriptionService.LabelDetectionOnImage(stdstrPath))
		{
			CString strMsg;

			string stdstrMsg = LabelDescriptionService.GetServiceMessage(nError);
			CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrMsg.c_str(), strMsg);
			writeLog(strMsg);

			strMsg.Format(_T("Label description failed"));
			writeLog(strMsg);
		}
		else
		{
			pFile = fopen(strFilePath, "a+t");

			strTaggingData = LabelDescriptionService.GetStrResponse();

			_ftprintf(pFile, strTaggingData.c_str()), fflush(pFile);

			fclose(pFile);
			pFile = NULL;
		}
	}
	else
	{
		CKRestSogangService LabelDescriptionService;
		LabelDescriptionService.SetServiceInfo(stdstrUri);

		if (nError = LabelDescriptionService.LabelDetectionOnImage(stdstrPath))
		{
			CString strMsg;

			string stdstrMsg = LabelDescriptionService.GetServiceMessage(nError);
			CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrMsg.c_str(), strMsg);
			writeLog(strMsg);

			strMsg.Format(_T("Label description failed"));
			writeLog(strMsg);
		}
		else
		{
			pFile = fopen(strFilePath, "a+t");

			strTaggingData = LabelDescriptionService.GetStrResponse();

			_ftprintf(pFile, strTaggingData.c_str()), fflush(pFile);

			fclose(pFile);
			pFile = NULL;
		}
	}
}

void CVideoAutoTaggingServerDlg::AddLabelDescriptionInfoWithFriendsModule(string stdstrPath, CString strFilePath)
{
	string strTaggingData = "";
	FILE* pFile = NULL;

	char* pUri = CConvertUTF8Operation::ConvertStringToUTF8(m_CMSInfo.strRecognizeApiUri);

	string stdstrUri(pUri);

	delete[]pUri;

	int nError = 0;

	CKRestSogangService LabelDescriptionService;
	LabelDescriptionService.SetServiceInfo(stdstrUri);

	if (nError = LabelDescriptionService.LabelDetectionOnImageWithModuleName(stdstrPath, "friends"))
	{
		CString strMsg;

		string stdstrMsg = LabelDescriptionService.GetServiceMessage(nError);
		CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrMsg.c_str(), strMsg);
		writeLog(strMsg);

		strMsg.Format(_T("Label description failed"));
		writeLog(strMsg);
	}
	else
	{
		pFile = fopen(strFilePath, "a+t");

		strTaggingData = LabelDescriptionService.GetStrResponse();

		_ftprintf(pFile, strTaggingData.c_str()), fflush(pFile);

		fclose(pFile);
		pFile = NULL;
	}
}

// -------shot data json--------
void CVideoAutoTaggingServerDlg::GetShotInfoJsonData(string strStart, string strEnd, CString strFolerPath)
{
	string stdstrRet;
	CString strFilePath = _T("");
	FILE* pFile = NULL;
	bool bComplete = false;

	Json::Value shotInfo;
	Json::Value section;

	section["end"] = strEnd;
	section["start"] = strStart;
	section["fps"] = m_dbFPS;
	shotInfo["ShotInfo"] = section;

	Json::StyledWriter writer;
	
	stdstrRet = writer.write(shotInfo);

	strFilePath.Format(_T("%s.json"), strFolerPath);
	pFile = fopen(strFilePath, "a+t");
	_ftprintf(pFile, stdstrRet.c_str()), fflush(pFile);
	fclose(pFile);
	pFile = NULL;
}

CString CVideoAutoTaggingServerDlg::getTimecode(double dbTimecode)
{
	CString strHour = _T("00");
	CString strMin = _T("00");
	CString strSec = _T("00");
	CString strFrame = _T("00");
	CString strTimecode = _T("00:00:00;00");

	int nSec = 0;
	double dbFrame = 0;
	int nFrame = 0;

	nSec = (int)(dbTimecode);
	dbFrame = dbTimecode - (double)nSec;
	nFrame = (int)(dbFrame * m_dbFPS);

	if (nSec / 60)
	{
		//min
		int nMin = nSec / 60;
		nSec = nSec % 60;

		if (nMin / 60)
		{
			//hour
			int nHour = nMin / 60;
			nMin = nMin % 60;
			nSec = nSec % 60;

			if (nHour < 10)
				strHour.Format(_T("0%d"), nHour);
			else
				strHour.Format(_T("%d"), nHour);
		}

		if (nMin < 10)
			strMin.Format(_T("0%d"), nMin);
		else
			strMin.Format(_T("%d"), nMin);
	}

	if (nSec <= 0)
		strSec.Format(_T("00"));
	else if (nSec < 10)
		strSec.Format(_T("0%d"), nSec);
	else
		strSec.Format(_T("%d"), nSec);

	if (nFrame < 10)
		strFrame.Format(_T("0%d"), nFrame);
	else
		strFrame.Format(_T("%d"), nFrame);

	strTimecode.Format(_T("%s:%s:%s;%s"), strHour, strMin, strSec, strFrame);

	return strTimecode;
}

bool CVideoAutoTaggingServerDlg::GetShotInfoData(CString strStart, CString strEnd, CString strFilePath, int nStartIndex, int nEndIndex)
{
	CKRestExUpdateShotData* pInfo = NULL;
	CString strMsg = _T("");
	char* szValue = NULL;

	pInfo = new CKRestExUpdateShotData;
	if (!pInfo)
	{
		strMsg.Format(_T("Memory allocation failure for shot data"));
		writeLog(strMsg);

		return false;
	}

	szValue = CConvertUTF8Operation::ConvertStringToUTF8(strStart);
	pInfo->m_stdstrStartTimecode.assign(strStart);
	delete[]szValue;
	szValue = NULL;

	szValue = CConvertUTF8Operation::ConvertStringToUTF8(strEnd);
	pInfo->m_stdstrEndTimecode.assign(strEnd);
	delete[]szValue;
	szValue = NULL;

	pInfo->m_nAssetid = m_JobInfo.nSrcAssetID;
	pInfo->m_nStartFrameIndex = nStartIndex;
	pInfo->m_nKeyPosFrameIndex = nStartIndex;
	pInfo->m_nEndFrameIndex = nEndIndex;

	if (nStartIndex == 0)
		pInfo->m_stdstrSequenceType = "start";

	if (nEndIndex == (m_nTotalFrameCount - 1))
		pInfo->m_stdstrSequenceType = "end";

	szValue = CConvertUTF8Operation::ConvertStringToUTF8(strFilePath);
	pInfo->m_FileInfo.m_stdstrPath.assign(szValue);
	delete[]szValue;
	szValue = NULL;

	m_pConnectCMS->UpdateShotInfoEx(m_nWidth, m_nHeight, m_dbFPS, m_strDuration, pInfo);

	return true;
}

void CVideoAutoTaggingServerDlg::OnExitSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnExitSizeMove();

	RECT RectClient;
	::GetWindowRect(this->m_hWnd, &RectClient);

	HWND consoleWindow = GetConsoleWindow();
	::SetWindowPos(consoleWindow, 0, RectClient.left, RectClient.bottom + 1, RectClient.right - RectClient.left, 500, SWP_NOZORDER);
}


void CVideoAutoTaggingServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	HWND consoleWindow = GetConsoleWindow();

	if ((nID & 0x0000FFF0) == SC_MINIMIZE)
	{
		// Minimizing, post to main dialogue also.
		AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);

		::ShowWindow(consoleWindow, SW_SHOWMINIMIZED);
	}
	else if ((nID & 0x0000FFF0) == SC_RESTORE && IsIconic())
	{
		// Restoring, post to main dialogue also.
		AfxGetMainWnd()->ShowWindow(SW_RESTORE);

		::ShowWindow(consoleWindow, SW_RESTORE);

		RECT RectClient;
		::GetWindowRect(this->m_hWnd, &RectClient);

		HWND consoleWindow = GetConsoleWindow();
		::SetWindowPos(consoleWindow, 0, RectClient.left, RectClient.bottom + 1, RectClient.right - RectClient.left, 500, SWP_NOZORDER);
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


void CVideoAutoTaggingServerDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (!m_bWndCreated)
	{
		m_bWndCreated = true;

		RECT RectClient;
		::GetWindowRect(this->m_hWnd, &RectClient);

		HWND consoleWindow = GetConsoleWindow();
		::SetWindowPos(consoleWindow, 0, RectClient.left, RectClient.bottom + 1, RectClient.right - RectClient.left, 500, SWP_NOZORDER);

		EnableMenuItem(::GetSystemMenu(consoleWindow, FALSE), SC_CLOSE, MF_GRAYED);
		::DrawMenuBar(consoleWindow);

	}
}

int CVideoAutoTaggingServerDlg::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (pDesktopWnd->m_hWnd == this->m_hWnd)
	{
		RECT RectClient;
		::GetWindowRect(this->m_hWnd, &RectClient);

		HWND consoleWindow = GetConsoleWindow();
		::SetWindowPos(consoleWindow, 0, RectClient.left, RectClient.bottom + 1, RectClient.right - RectClient.left, 500, SWP_NOZORDER);
	}

	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
