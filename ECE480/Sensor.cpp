/*
*\file Sensor.cpp
*
* \author Tianhang Sun
*/

#include "stdafx.h"
#include "Sensor.h"
#include "SetSensorName.h"
#include <string>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
using namespace Gdiplus;

/// sensor,warning filename 
const wstring SensorName(L"images/Sensor.jpg");
const wstring Warning(L"images/warning.png");

/// some const to use
const double SensorWarningDistance = 50;
const double TextDistance = 45;
const double threshold = 10000;
// const double MaxVoltage = 5;	///< max voltage of light sensor

/**Constructor */
CSensor::CSensor(CGoogleMap *googlemap) : CItem(googlemap)
{
	/// load sensor image
	mSensorImage = unique_ptr<Bitmap>(Bitmap::FromFile(SensorName.c_str()));
	if (mSensorImage->GetLastStatus() != Ok){
		wstring msg(L"Failed to open ");
		msg += SensorName;
		AfxMessageBox(msg.c_str());
	}

	/// load warning image
	mWarningImage = unique_ptr<Bitmap>(Bitmap::FromFile(Warning.c_str()));
	if (mWarningImage->GetLastStatus() != Ok){
		wstring msg(L"Failed to open ");
		msg += SensorName;
		AfxMessageBox(msg.c_str());
	}

}

/** destructor */
CSensor::~CSensor()
{
}

/** Draw this item
* \param graphics Graphics device to draw on */
void CSensor::Draw(Gdiplus::Graphics *graphics)
{
	double wid = mSensorImage->GetWidth();
	double hit = mSensorImage->GetHeight();
	graphics->DrawImage(mSensorImage.get(),
		float(GetX() - wid / 2) / mFactor + mEdge, float(GetY() - hit / 2) / mFactor + mEdge,
		(float)mSensorImage->GetWidth() / (float)(1.5*mFactor), (float)mSensorImage->GetHeight() / (1.5*(float)mFactor));
	

	FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 10);

	wstringstream data;
	data << L"S" << (int)mSensorName << L": " << mCurrentData;

	SolidBrush white(Color(255, 255, 255));
	graphics->DrawString(data.str().c_str(),  // String to draw
		-1,         // String length, -1 means it figures it out on its own
		&font,      // The font to use
		PointF(float(GetX() - wid / 2) / mFactor + mEdge, float(GetY() - hit / 2) / mFactor + 0.75 * TextDistance / mFactor + mEdge),   // Where to draw (top left corner)
		&white);    // The brush to draw the text with

	// draw warning sign in warning mode
	if (mFlag == true){
		graphics->DrawImage(mWarningImage.get(),
			float(GetX() / mFactor - wid / (1.5 * mFactor)) + SensorWarningDistance / (1.5 * mFactor) + mEdge,
			float(GetY() / mFactor - hit / mFactor + mEdge),
			(float)mWarningImage->GetWidth()*0.8, (float)mWarningImage->GetHeight()*0.8);
	}

}

/** Test to see if we hit this object with a mouse.
* \param x X position to test
* \param y Y position to test
* \return true if hit. */
bool CSensor::HitTest(int x, int y)
{
	double wid = mSensorImage->GetWidth();
	double hit = mSensorImage->GetHeight();

	// Make x and y relative to the top-left corner of the bitmap image
	// Subtracting the center makes x, y relative to the center of the image.
	// Adding half the size makes x, y relative to the top corner of the image
	double testX = (x - GetX() + wid / 2) / mFactor + mEdge;
	double testY = (y - GetY() + hit / 2) / mFactor + mEdge;

	// Test to see if x, y are in the image
	if (testX < 0 || testY < 0 || testX >= wid || testY >= hit){
		// We are outside the image
		return false;
	}

	// Test to see if x, y are in the drawn part of the image
	auto format = mSensorImage->GetPixelFormat();
	if (format == PixelFormat32bppARGB || format == PixelFormat32bppPARGB)
	{
		// This image has an alpha map, which implements the 
		// transparency. If so, we should check to see if we
		// clicked on a pixel where alpha is not zero, meaning
		// the pixel shows on the screen.
		Color color;
		mSensorImage->GetPixel((int)testX, (int)testY, &color);
		return color.GetAlpha() != 0;
	}
	else {
		return true;
	}
}

/** Save this item to an XML node
* \param node The node we are going to be a child of */
std::shared_ptr<xmlnode::CXmlNode>
CSensor::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
	auto itemNode = CItem::XmlSave(node);
	itemNode->SetAttribute(L"type", L"sensor");
	wstringstream str;
	str << (int)mSensorName;
	itemNode->SetAttribute(L"name", str.str().c_str());
	return itemNode;
}

void CSensor::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
	CItem::XmlLoad(node);
	int i = node->GetAttributeIntValue(L"name", 0);
	switch (i)
	{
	case 0: mSensorName = Sensor0; break;
	case 1: mSensorName = Sensor1; break;
	case 2: mSensorName = Sensor2; break;
	case 3: mSensorName = Sensor3; break;
	case 4: mSensorName = Sensor4; break;
	case 5: mSensorName = Sensor5; break;
	case 6: mSensorName = Sensor6; break;
	case 7: mSensorName = Sensor7; break;
	case 8: mSensorName = Sensor8; break;
	case 9: mSensorName = Sensor9; break;
	case 10: mSensorName = Sensor10; break;
	case 11: mSensorName = Sensor11; break;
	case 12: mSensorName = Sensor12; break;
	case 13: mSensorName = Sensor13; break;
	case 14: mSensorName = Sensor14; break;
	case 15: mSensorName = Sensor15; break;
	default: mSensorName = Sensor0; break;
	}
}

/** change enum sensor name to string*/
string ToString(CSensor::Name name){
	switch (name)
	{
	case 0:	return "sensor0";
	case 1:	return "sensor1";
	case 2:	return "sensor2";
	case 3:	return "sensor3";
	case 4:	return "sensor4";
	case 5:	return "sensor5";
	case 6:	return "sensor6";
	case 7:	return "sensor7";
	case 8:	return "sensor8";
	case 9:	return "sensor9";
	case 10: return "sensor10";
	case 11: return "sensor11";
	case 12: return "sensor12";
	case 13: return "sensor13";
	case 14: return "sensor14";
	case 15: return "sensor15";
	default: return "Bad Name";
	}
}

/** update function*/
void CSensor::Update(double elapsed)
{
	CItem::Update(elapsed);
	mDuration += elapsed;

	/*if (mDuration > 1){
	mCurrentData = ((double)rand() / RAND_MAX) * MaxVoltage;
	if (mCurrentData < threshold){
	mWarning = true;
	}
	else
	mWarning = false;
	mDuration = 0;
	}*/

	/// update data for this sensor
	if (mDuration > 1){
		string name;
		name = ToString(mSensorName) + ':';

		for (int i = 0; i < mElement.size(); i++)
		{
			if (mElement[i] == name)
			{
				mCurrentData = stol(mElement[i + 1]);
				if (mCurrentData < threshold)
					mFlag = true;
				else
					mFlag = false;
			}
		}
		mDuration = 0;
	}
}

/** PropertiesDlg to choose sensor name */
void CSensor::PropertiesDlg()
{
	CSetSensorName dlg;
	dlg.SetName(mSensorName);

	if (dlg.DoModal() == IDOK)
	{
		mSensorName = dlg.GetName();
	}
}

/** store vector of data into the sensor*/
void CSensor::SetElement(vector<string> ele)
{
	mElement.clear();
	for (auto i : ele){
		mElement.push_back(i);
	}
}

/** get sensor name */
string CSensor::GetName()
{
	return ToString(mSensorName);
}

/** get sensor location and flag */
vector<double> CSensor::GetLocationFlag()
{
	vector<double> loc;
	loc.push_back(mX);
	loc.push_back(mY);
	if (mFlag == true)
		loc.push_back(1);
	else
		loc.push_back(0);
	return loc;
}