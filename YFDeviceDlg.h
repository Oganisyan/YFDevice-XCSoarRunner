// YFDeviceDlg.h : Headerdatei
//

#pragma once
#include "afxwin.h"
#include "MyButton.h"

#define BIKE_NAV_EXE_PATH	_T("\\ResidentFlash\\BikeNav\\program\\BikeNav.exe")
#define XC_SOAR_EXE_PATH	_T("\\SDMMC\\XCSoarData\\XCSoar.exe")
#define LK8000_EXE_PATH		_T("\\SDMMC\\LK8000\\LK8000-PNA.exe")

#define EXPLORER_PATH		_T("\\Windows\\explorer.exe")
#define BIKE_NAV_EXE		_T("BikeNav.exe")
//#define XC_SOAR_EXE			_T("XCSoar.exe")
//#define EXPLORER_EXE		_T("explorer.exe")

#define LOCK_STR			_T("Bildschirm sperren")
#define UNLOCK_STR			_T("Bildschirm entspe.")


// CYFDeviceDlg-Dialogfeld
class CYFDeviceDlg : public CDialog
{
// Konstruktion
public:
	CYFDeviceDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_YFDEVICE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

// Implementierung
protected:
	HICON m_hIcon;
	MyButton mButtonCancel;
	MyButton mButtonLock;
	MyButton mButtonBikeNavi;
	MyButton mButtonBluetooth;
	MyButton mButtonOff;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonOff();
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonBluetooth();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonBikeNavi();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:
	HANDLE hEvent;
	CWnd *xcSoarWnd;
	bool bScreanLocked;
	HANDLE hChildProcess;
	void InitChildProcess();
	HANDLE CreateProcess(LPCWSTR pProcessName);
	void KillProcess(LPCWSTR pProcessName);
	HANDLE  GetProcessHandle(LPCWSTR szProcessName);
};
