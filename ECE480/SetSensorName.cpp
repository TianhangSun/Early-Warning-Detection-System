// SetSensorName.cpp : implementation file
//

#include "stdafx.h"
#include "ECE480.h"
#include "SetSensorName.h"
#include "afxdialogex.h"
#include <string>


// CSetSensorName dialog

IMPLEMENT_DYNAMIC(CSetSensorName, CDialogEx)

CSetSensorName::CSetSensorName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetSensorName::IDD, pParent)
{

}

CSetSensorName::~CSetSensorName()
{
}

void CSetSensorName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ZONING, m_sensorCtrl);

	/// 10 sensor available at this moment
	if (!pDX->m_bSaveAndValidate)
	{
		m_sensorCtrl.AddString(L"Sensor0");
		m_sensorCtrl.AddString(L"Sensor1");
		m_sensorCtrl.AddString(L"Sensor2");
		m_sensorCtrl.AddString(L"Sensor3");
		m_sensorCtrl.AddString(L"Sensor4");
		m_sensorCtrl.AddString(L"Sensor5");
		m_sensorCtrl.AddString(L"Sensor6");
		m_sensorCtrl.AddString(L"Sensor7");
		m_sensorCtrl.AddString(L"Sensor8");
		m_sensorCtrl.AddString(L"Sensor9");
		m_sensorCtrl.AddString(L"Sensor10");
		m_sensorCtrl.AddString(L"Sensor11");
		m_sensorCtrl.AddString(L"Sensor12");
		m_sensorCtrl.AddString(L"Sensor13");
		m_sensorCtrl.AddString(L"Sensor14");
		m_sensorCtrl.AddString(L"Sensor15");

		m_sensorCtrl.SetCurSel((int)mSensorName);
	}
	else
	{
		mSensorName = (CSensor::Name)m_sensorCtrl.GetCurSel();
	}
}


BEGIN_MESSAGE_MAP(CSetSensorName, CDialogEx)
END_MESSAGE_MAP()


// CSetSensorName message handlers
