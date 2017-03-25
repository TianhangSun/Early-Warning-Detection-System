
// ChildView.h : interface of the CChildView class
//

#include "GoogleMap.h"

#pragma once

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CGoogleMap mGoogleMap;
	// Any item we are currently dragging
	std::shared_ptr<CItem> mGrabbedItem;

	bool mMode = true;	///< true in editing mode, false in warning mode
	bool mPlot = false;

	/// True until the first time we draw
	bool mFirstDraw = true;

	long long mLastTime;    ///< Last time we read the timer
	double mTimeFreq;       ///< Rate the timer updates
	double mDuration = 0;	///< update per second
	double elapsed = 0;		///< timer

public:
	afx_msg void OnEditAddsensor();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFileSaveas();
	afx_msg void OnFileOpen32773();
	afx_msg void OnModeEditing();
	afx_msg void OnModeWarning();
	afx_msg void OnUpdateModeEditing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeWarning(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAddsensor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen32773(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveas(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditAddsolarpanel();
	afx_msg void OnUpdateEditAddsolarpanel(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnShowMap();
	afx_msg void OnUpdateShowMap(CCmdUI *pCmdUI);
	afx_msg void OnShowPlot();
	afx_msg void OnUpdateShowPlot(CCmdUI *pCmdUI);
	afx_msg void OnEditSetplotscale();
};

