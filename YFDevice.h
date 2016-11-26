// YFDevice.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif


// CYFDeviceApp:
// Siehe YFDevice.cpp für die Implementierung dieser Klasse
//
class CYFDeviceApp : public CWinApp
{
public:
	CYFDeviceApp();
	
// Überschreibungen
public:
	virtual BOOL InitInstance();
// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CYFDeviceApp theApp;
