// YFDevice.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "YFDevice.h"
#include "YFDeviceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYFDeviceApp

BEGIN_MESSAGE_MAP(CYFDeviceApp, CWinApp)
END_MESSAGE_MAP()


// CYFDeviceApp-Erstellung
CYFDeviceApp::CYFDeviceApp()
	: CWinApp()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CYFDeviceApp-Objekt
CYFDeviceApp theApp;

// CYFDeviceApp-Initialisierung

BOOL CYFDeviceApp::InitInstance()
{

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));


	INT_PTR rc; 
	CYFDeviceDlg dlg;
	m_pMainWnd = &dlg;
	rc = dlg.DoModal();

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
