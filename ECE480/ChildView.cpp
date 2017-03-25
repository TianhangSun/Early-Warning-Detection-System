
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "ECE480.h"
#include "ChildView.h"
#include "Sensor.h"
#include "SolarPanel.h"
#include <memory>
#include "DoubleBufferDC.h"
#include "afxwin.h"
#include "resource.h"
#include "SetSensorName.h"
#include "SensorVisitor.h"
#include "PanelVisitor.h"
#include "Scale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;
using namespace std;

/// Initial X location
const int InitialX = 200;

/// Initial Y location
const int InitialY = 200;

/// Frame duration in milliseconds
const int FrameDuration = 30;

// CChildView

CChildView::CChildView()
{
	srand((unsigned int)time(nullptr));
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_EDIT_ADDSENSOR, &CChildView::OnEditAddsensor)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_SAVEAS, &CChildView::OnFileSaveas)
	ON_COMMAND(ID_FILE_OPEN32773, &CChildView::OnFileOpen32773)
	ON_COMMAND(ID_MODE_EDITING, &CChildView::OnModeEditing)
	ON_COMMAND(ID_MODE_WARNING, &CChildView::OnModeWarning)
	ON_UPDATE_COMMAND_UI(ID_MODE_EDITING, &CChildView::OnUpdateModeEditing)
	ON_UPDATE_COMMAND_UI(ID_MODE_WARNING, &CChildView::OnUpdateModeWarning)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDSENSOR, &CChildView::OnUpdateEditAddsensor)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN32773, &CChildView::OnUpdateFileOpen32773)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, &CChildView::OnUpdateFileSaveas)
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_ADDSOLARPANEL, &CChildView::OnEditAddsolarpanel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDSOLARPANEL, &CChildView::OnUpdateEditAddsolarpanel)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SHOW_MAP, &CChildView::OnShowMap)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MAP, &CChildView::OnUpdateShowMap)
	ON_COMMAND(ID_SHOW_PLOT, &CChildView::OnShowPlot)
	ON_UPDATE_COMMAND_UI(ID_SHOW_PLOT, &CChildView::OnUpdateShowPlot)
	ON_COMMAND(ID_EDIT_SETPLOTSCALE, &CChildView::OnEditSetplotscale)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/** paint function */
void CChildView::OnPaint() 
{
	CPaintDC paintDC(this);     // device context for painting
	CDoubleBufferDC dc(&paintDC); // device context for painting
	Graphics graphics(dc.m_hDC);

	mGoogleMap.OnDraw(&graphics);

	if (mFirstDraw)
	{
		mFirstDraw = false;
		SetTimer(1, FrameDuration, nullptr);

		/* Initialize the elapsed time system */
		LARGE_INTEGER time, freq;
		QueryPerformanceCounter(&time);
		QueryPerformanceFrequency(&freq);

		mLastTime = time.QuadPart;
		mTimeFreq = double(freq.QuadPart);
	}

	/* Compute the elapsed time since the last draw */
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	long long diff = time.QuadPart - mLastTime;
	elapsed = double(diff) / mTimeFreq;
	mLastTime = time.QuadPart;

	mGoogleMap.Update(elapsed);
}

/** handle add sensor button*/
void CChildView::OnEditAddsensor()
{
	auto sensor = make_shared<CSensor>(&mGoogleMap);
	sensor->SetLocation(InitialX, InitialY);
	mGoogleMap.Add(sensor);
	Invalidate();
}

/** left click the mouse, do hit test*/
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mGrabbedItem = mGoogleMap.HitTest(point.x, point.y);
}

/** release mouse*/
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnMouseMove(nFlags, point);
}

/** move the item when move mouse*/
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// See if an item is currently being moved by the mouse
	if (mGrabbedItem != nullptr && mMode == true)
	{
		// If an item is being moved, we only continue to 
		// move it while the left button is down.
		if (nFlags & MK_LBUTTON){
			mGrabbedItem->SetLocation(point.x, point.y);
		}
		else{
			// When the left button is released, we release the
			// item.
			if (mGoogleMap.IsOverTrashcan(point.x, point.y)){
				mGoogleMap.DeleteItem(mGrabbedItem);
			}

			// When the left button is released, we release the
			// item.
			mGrabbedItem = nullptr;
		}

		// Force the screen to redraw
		Invalidate();
	}
}

/** erase everything*/
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

/** handle save file button*/
void CChildView::OnFileSaveas()
{
	CFileDialog dlg(false,  // false = Save dialog box
		L".sensorlocation",           // Default file extension
		nullptr,            // Default file name (none)
		OFN_OVERWRITEPROMPT,      // Flags (none)
		L"Sensor Location Files (*.sensorlocation)|*.sensorlocation|All Files (*.*)|*.*||"); // Filter

	if (dlg.DoModal() != IDOK)
		return;

	wstring filename = dlg.GetPathName();

	mGoogleMap.Save(filename);
}

/** handle open file button*/
void CChildView::OnFileOpen32773()
{
	CFileDialog dlg(true,  // true = Open dialog box
		L".sensorlocation",           // Default file extension
		nullptr,            // Default file name (none)
		0,    // Flags
		L"Sensor Location Files (*.sensorlocation)|*.sensorlocation|All Files (*.*)|*.*||");    // Filter
	if (dlg.DoModal() != IDOK)
		return;

	wstring filename = dlg.GetPathName();

	mGoogleMap.Load(filename);
	Invalidate();
}

/** handle editing mode button*/
void CChildView::OnModeEditing()
{
	mMode = true;
	mPlot = false;
	mGoogleMap.SetFactor(1, 0);
	mGoogleMap.ToggleTrashCanActive();
	mGoogleMap.SetWarningMode(false);

	map<string, vector<double>> m;
	CPanelVisitor PanelVisitor;
	PanelVisitor.SetMap(m);
	mGoogleMap.Accept(&PanelVisitor);

	Invalidate();
}

/** handle warning mode button*/
void CChildView::OnModeWarning()
{
	mMode = false;
	mGoogleMap.ToggleTrashCanActive();
	mGoogleMap.SetWarningMode(true);

	CSensorVisitor SensorVisitor;
	mGoogleMap.Accept(&SensorVisitor);
	map<string, vector<double>> m = SensorVisitor.GetMap();

	CPanelVisitor PanelVisitor;
	PanelVisitor.SetMap(m);
	mGoogleMap.Accept(&PanelVisitor);

	Invalidate();
}

/** check option support*/
void CChildView::OnUpdateModeEditing(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mMode == true);
}

/** check option support*/
void CChildView::OnUpdateModeWarning(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mMode == false);
}

/** disable add sensor button in waring mode*/
void CChildView::OnUpdateEditAddsensor(CCmdUI *pCmdUI)
{
	if (mMode == true)	pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

/** disable open file button in waring mode*/
void CChildView::OnUpdateFileOpen32773(CCmdUI *pCmdUI)
{
	if (mMode == true)	pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

/** disable save file button in waring mode*/
void CChildView::OnUpdateFileSaveas(CCmdUI *pCmdUI)
{
	if (mMode == true)	pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

/** timer event*/
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate();
	CWnd::OnTimer(nIDEvent);
}

/** handle add solar panel button*/
void CChildView::OnEditAddsolarpanel()
{
	auto panel = make_shared<CSolarPanel>(&mGoogleMap);
	panel->SetLocation(InitialX, InitialY);
	mGoogleMap.Add(panel);
	Invalidate();
}

/** disable add solar panel button in waring mode*/
void CChildView::OnUpdateEditAddsolarpanel(CCmdUI *pCmdUI)
{
	if (mMode == true)	pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

/** change sensor name when double click the sensor*/
void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	auto sensor = mGoogleMap.HitTest(point.x, point.y);
	if (sensor != nullptr)
	{
		// We double-clicked on a sensor
		// Bring up the name editing dialog box
		sensor->PropertiesDlg();
		Invalidate();
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CChildView::OnShowMap()
{
	mPlot = false;
	mGoogleMap.SetFactor(1, 0);
}


void CChildView::OnUpdateShowMap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mPlot == false);
	if (mMode == false)	pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}


void CChildView::OnShowPlot()
{
	mPlot = true;
	mGoogleMap.SetFactor(2.5, 25);
}


void CChildView::OnUpdateShowPlot(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mPlot == true);
	if (mMode == false)	pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}


void CChildView::OnEditSetplotscale()
{
	KillTimer(1);
	CScale dlg;
	dlg.SetScale(mGoogleMap.GetScale());
	if (dlg.DoModal() == IDOK)
	{
		mGoogleMap.SetScale(dlg.GetScale());
	}
	SetTimer(1, 30, nullptr);
}
