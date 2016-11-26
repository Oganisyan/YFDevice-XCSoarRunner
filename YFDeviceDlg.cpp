// YFDeviceDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "YFDevice.h"
#include "YFDeviceDlg.h"
#include <Pm.h>
#include <TlHelp32.h>

#include <winsock2.h>
#include <ws2bth.h>
#include <Bt_api.h.>




extern "C" DWORD GetEventData(HANDLE hEvent );
extern "C" BOOL	SetEventData(HANDLE hEvent, DWORD dwData);
extern "C" BOOL KernelIoControl(DWORD dwIoControlCode,  LPVOID lpInBuf,  DWORD nInBufSize,
  LPVOID lpOutBuf,  DWORD nOutBufSize,  LPDWORD lpBytesReturned);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CYFDeviceDlg-Dialogfeld

CYFDeviceDlg::CYFDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYFDeviceDlg::IDD, pParent)
	, bScreanLocked(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hEvent = CreateEvent(NULL, TRUE, FALSE, _T("__YF_KEYPAD_EVENT__"));	
	InitChildProcess();
	xcSoarWnd = FindWindow(_T("XCSoarMain"),NULL);
}

void CYFDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, mButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_LOCK, mButtonLock);
	DDX_Control(pDX, IDC_BUTTON_BIKE_NAVI, mButtonBikeNavi);
	DDX_Control(pDX, IDC_BUTTON_BLUETOOTH, mButtonBluetooth);
	DDX_Control(pDX, IDC_BUTTON_OFF, mButtonOff);
}

BEGIN_MESSAGE_MAP(CYFDeviceDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_BUTTON_OFF, &CYFDeviceDlg::OnBnClickedButtonOff)
ON_BN_CLICKED(IDC_BUTTON_LOCK, &CYFDeviceDlg::OnBnClickedButtonLock)
ON_BN_CLICKED(IDC_BUTTON_BLUETOOTH, &CYFDeviceDlg::OnBnClickedButtonBluetooth)
ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CYFDeviceDlg::OnBnClickedCancel)
ON_WM_ACTIVATE()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON_BIKE_NAVI, &CYFDeviceDlg::OnBnClickedButtonBikeNavi)
END_MESSAGE_MAP()


// CYFDeviceDlg-Meldungshandler

BOOL CYFDeviceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

    DWORD dwAttrib = GetFileAttributes(LK8000_EXE_PATH);

	mButtonBluetooth.EnableWindow(dwAttrib != 
		INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	mButtonLock.EnableWindow(xcSoarWnd != NULL);
	mButtonLock.SetBitMap(IDB_BITMAP_UNLOCK);
	mButtonCancel.SetBitMap(IDB_BITMAP_CANCEL);
	mButtonBikeNavi.SetBitMap(IDB_BITMAP_NAVI);
	mButtonBluetooth.SetBitMap(IDB_BITMAP_BT);
	mButtonOff.SetBitMap(IDB_BITMAP_OFF);
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CYFDeviceDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_YFDEVICE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_YFDEVICE_DIALOG));
	}
}
#endif

void CYFDeviceDlg::OnBnClickedButtonOff()
{
	HANDLE hEvent2 = CreateEvent(NULL, TRUE, FALSE, _T("__PowerOffRepEvent__"));	
	SetEventData(hEvent2,0x01);
	EventModify(hEvent2,0x03);
	CloseHandle(hEvent2);
}

void CYFDeviceDlg::OnBnClickedButtonLock()
{
	bScreanLocked = !(bScreanLocked);
	xcSoarWnd->EnableWindow(bScreanLocked);
	xcSoarWnd->SetForegroundWindow();
	mButtonLock.SetWindowText(bScreanLocked? LOCK_STR : UNLOCK_STR);
	mButtonLock.SetBitMap(bScreanLocked? 
		IDB_BITMAP_UNLOCK : IDB_BITMAP_LOCK);	

	ShowWindow(SW_HIDE);
}


typedef void   (WINAPI *AnWBT_Init_t)();
typedef DWORD  (WINAPI *AnWBT_ScanAllService_t)();
typedef DWORD  (WINAPI *AnWBT_Discovery_t)(char *p);



void CYFDeviceDlg::OnBnClickedButtonBluetooth()
{
//  TODO BlueTooth function
	ShowWindow(SW_HIDE);
}

void CYFDeviceDlg::OnBnClickedCancel()
{
	if(xcSoarWnd != NULL) {
		xcSoarWnd->SetForegroundWindow();
	}
	ShowWindow(SW_HIDE);
}

void CYFDeviceDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(WA_ACTIVE & nState) {
		SetTimer(2, 100, NULL);
	}

	return CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CYFDeviceDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(WaitForSingleObject(hChildProcess,20)==WAIT_OBJECT_0) {
		CreateProcess(BIKE_NAV_EXE_PATH);
		CDialog::OnCancel();
	} else if(nIDEvent == 1) {
		if(WaitForSingleObject(hEvent,20)==WAIT_OBJECT_0) {
			DWORD rc=GetEventData(hEvent);
			if(rc == 0x800000c6)
			{
				ShowWindow(SW_SHOW);
				SetForegroundWindow();
				SetTimer(2, 10000, NULL);
			}
			else if(rc == 0x8000007d)
			{
				keybd_event(VK_ESCAPE, 0x81, 0, 0);
				Sleep(10);
				keybd_event(VK_ESCAPE, 0x81, KEYEVENTF_KEYUP, 0);
			}
		}
	}else if(nIDEvent == 2) {
		KillTimer(nIDEvent);
		SetTimer(1, 100, NULL);
		ShowWindow(SW_HIDE);
	}
	CDialog::OnTimer(nIDEvent);
}


HANDLE CYFDeviceDlg::CreateProcess(LPCWSTR pProcessName)
{
	PROCESS_INFORMATION pInfo;
	if(::CreateProcess( pProcessName, 
		NULL, NULL, NULL, FALSE, 0,  NULL, NULL,  NULL,  &pInfo))
	{
		return pInfo.hProcess;
	} else {
		return INVALID_HANDLE_VALUE;
	}
}

void CYFDeviceDlg::InitChildProcess()
{
	KillProcess(BIKE_NAV_EXE);
	if(WaitForSingleObject(hEvent, 3000) == WAIT_TIMEOUT) {
		hChildProcess = CreateProcess(XC_SOAR_EXE_PATH);
		Sleep(1000);
	} else {
		ResetEvent(hEvent);
		hChildProcess = CreateProcess(EXPLORER_PATH);
	}
}

void CYFDeviceDlg::KillProcess(LPCWSTR pProcessName)
{
	HANDLE proc;
	while((proc=GetProcessHandle(pProcessName)) != INVALID_HANDLE_VALUE)
		TerminateProcess(proc,0);
}

HANDLE  CYFDeviceDlg::GetProcessHandle(LPCWSTR szProcessName)
{
    //Get the snapshot of all processes in the system
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //Get the information of the first process
    if (!Process32First(hSnap, &pe32))
    {
        CloseHandle(hSnap);
        return INVALID_HANDLE_VALUE;
    }

    //Loop through all processes
    do
    {
        if (_tcscmp(szProcessName, pe32.szExeFile) == 0)
        {
            //Got the process ID
            CloseHandle(hSnap);
            printf("sz = %s; exe = %s; pid = %d\n", szProcessName, pe32.szExeFile, pe32.th32ProcessID);
                            //Error here, correct PID was found in both case
            return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        }
    } 
    while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return INVALID_HANDLE_VALUE;
}



void CYFDeviceDlg::OnBnClickedButtonBikeNavi()
{
	TerminateProcess(hChildProcess, 0);
}

