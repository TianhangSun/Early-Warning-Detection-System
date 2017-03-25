// Scale.cpp : implementation file
//

#include "stdafx.h"
#include "ECE480.h"
#include "Scale.h"
#include "afxdialogex.h"


// CScale dialog

IMPLEMENT_DYNAMIC(CScale, CDialog)

CScale::CScale(CWnd* pParent /*=NULL*/)
	: CDialog(CScale::IDD, pParent)
	, mScale(0)
{

}

CScale::~CScale()
{
}

void CScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mScale);
	DDV_MinMaxDouble(pDX, mScale, 0.1, 10.0);
}


BEGIN_MESSAGE_MAP(CScale, CDialog)
END_MESSAGE_MAP()


// CScale message handlers
