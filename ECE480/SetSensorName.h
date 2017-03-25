#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "Sensor.h"
#include <string>

// CSetSensorName dialog

class CSetSensorName : public CDialogEx
{
	DECLARE_DYNAMIC(CSetSensorName)

public:
	CSetSensorName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetSensorName();

// Dialog Data
	enum { IDD = IDD_TILEEDITDLG };

	CSensor::Name GetName(){ return mSensorName; };
	void SetName(CSensor::Name name){ mSensorName = name; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_sensorCtrl;     ///< The combo box control for sensors
	CSensor::Name mSensorName;	///< sensor name
};

