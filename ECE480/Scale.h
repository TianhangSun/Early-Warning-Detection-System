#pragma once


// CScale dialog

class CScale : public CDialog
{
	DECLARE_DYNAMIC(CScale)

public:
	CScale(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScale();
	double GetScale(){ return mScale; }
	void SetScale(double scale){ mScale = scale; }

// Dialog Data
	enum { IDD = IDD_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	double mScale = 1;
};
