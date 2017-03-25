/**
* \file SolarPanel.cpp
*
* \author Tianhang Sun
*/

#include "stdafx.h"
#include "SolarPanel.h"
#include "Item.h"
#include <math.h>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace Gdiplus;
using namespace std;

const wstring PanelName(L"images/SolarPanel.png");	///< image name
const double TextDistance = 50;
const double factor = 0.5;

/** Constructor */
CSolarPanel::CSolarPanel(CGoogleMap *googlemap) : CItem(googlemap)
{
	/// load image
	mPanelImage = unique_ptr<Bitmap>(Bitmap::FromFile(PanelName.c_str()));
	if (mPanelImage->GetLastStatus() != Ok){
		wstring msg(L"Failed to open ");
		msg += PanelName;
		AfxMessageBox(msg.c_str());
	}
	//mEGRImage = unique_ptr<Bitmap>(Bitmap::FromFile(L"images/EGR.gif"));
	//if (mPanelImage->GetLastStatus() != Ok){
	//	wstring msg(L"Failed to open ");
	//	msg += PanelName;
	//	AfxMessageBox(msg.c_str());
	//}
	//mMSUImage = unique_ptr<Bitmap>(Bitmap::FromFile(L"images/MSU.jpg"));
	//if (mPanelImage->GetLastStatus() != Ok){
	//	wstring msg(L"Failed to open ");
	//	msg += PanelName;
	//	AfxMessageBox(msg.c_str());
}

/** destructor*/
CSolarPanel::~CSolarPanel()
{
}

/** Draw this item
* \param graphics Graphics device to draw on */
void CSolarPanel::Draw(Gdiplus::Graphics *graphics)
{
	/// draw the solar panel
	double wid = mPanelImage->GetWidth();
	double hit = mPanelImage->GetHeight();
	graphics->DrawImage(mPanelImage.get(),
		float(GetX() - wid / 2) / mFactor + mEdge, float(GetY() - hit / 2) / mFactor + mEdge,
		(float)mPanelImage->GetWidth() / (float)mFactor, (float)mPanelImage->GetHeight() / mFactor);
	
	//if (mFactor == 1)
	//{
	//	double wid1 = mEGRImage->GetWidth();
	//	double hit1 = mEGRImage->GetHeight();
	//	double wid2 = mMSUImage->GetWidth();
	//	double hit3 = mMSUImage->GetHeight();
	//	graphics->DrawImage(mEGRImage.get(),(int)880,(int)50,
	//		(int)mEGRImage->GetWidth()/2, (int)mEGRImage->GetHeight()/2);
	//	graphics->DrawImage(mMSUImage.get(), (int)880, (int)600,
	//		(int)mMSUImage->GetWidth()/2, (int)mMSUImage->GetHeight()/2);
	//}

	/// display the cloud arrival time
	FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 12);
	Gdiplus::Font font2(&fontFamily, 16);

	wstringstream data;
	data << L"T: " << mArrival;

	wstringstream drop;
	drop << L"D: " << mDrop << L" %";

	wstringstream time;
	time << L"Approching in: " << (int)mArrival << " s";
	wstringstream time0;
	time0 << L"Approching in: \n" << (int)mArrival << " s";

	wstringstream pdrop;
	pdrop << L"Power Drop: " << mDrop << " %\n" << L"Est output: " << fixed << setprecision(2) << mOutput << " MW";
	wstringstream pdrop0;
	pdrop0 << L"Power Drop: \n" << mDrop << " %\n\n" << L"Est output: \n" << fixed << setprecision(2) << mOutput << " MW";

	SolidBrush white(Color(255, 255, 255));
	SolidBrush black(Color(0, 0, 0));
	Pen pen(Color(255, 255, 255), 3);

	if (draw == true && mFactor != 1){
		graphics->DrawString(data.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font,      // The font to use
			PointF(float(GetX() - wid / 2), float(GetY() - hit / 2) + TextDistance),   // Where to draw (top left corner)
			&white);    // The brush to draw the text with

		graphics->DrawString(drop.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font,      // The font to use
			PointF(float(GetX() - wid / 2), float(GetY() - hit / 2) + 1.3 * TextDistance),   // Where to draw (top left corner)
			&white);    // The brush to draw the text with

		if (mFactor != 1){
			graphics->DrawString(time.str().c_str(),  // String to draw
				-1,         // String length, -1 means it figures it out on its own
				&font2,      // The font to use
				PointF(80, 325),   // Where to draw (top left corner)
				&black);    // The brush to draw the text with

			graphics->DrawString(pdrop.str().c_str(),  // String to draw
				-1,         // String length, -1 means it figures it out on its own
				&font2,      // The font to use
				PointF(80, 350),   // Where to draw (top left corner)
				&black);    // The brush to draw the text with
		}

		if (mFactor != 1 && !mDirection.empty()){
			if (mDirection[0] > 0 && mDirection[1] > 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " degree South of East";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(80, 400),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
			if (mDirection[0] > 0 && mDirection[1] < 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " degree North of East";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(80, 400),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
			if (mDirection[0] < 0 && mDirection[1] < 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " degree North of West";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(80, 400),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
			if (mDirection[0] < 0 && mDirection[1] > 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " degree South of West";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(80, 400),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
		}

		/*if (mFactor != 1 && !mPredictionQueue.empty()){
			int y0 = 550;
			int x0 = 121;
			Pen color(Color(0, 0, 255), 1);
			for (int k = 0; k < mPredictionQueue.size(); k++)
			{
				graphics->DrawLine(&color, x0 + 3 * (k + 1) + 10, y0,
					x0 + 3 * (k + 1) + 10, y0 - ((mPredictionQueue[k]) / 500) * 0.75);
			}
		}*/
	}
	else if (draw == true){
		graphics->DrawString(time0.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font2,      // The font to use
			PointF(880, 200),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with

		graphics->DrawString(pdrop0.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font2,      // The font to use
			PointF(880, 300),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with
		if (mFactor == 1 && !mDirection.empty()){
			if (mDirection[0] > 0 && mDirection[1] > 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " °\nSouth of East";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(880, 480),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
			if (mDirection[0] > 0 && mDirection[1] < 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " °\nNorth of East";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(880, 480),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
			if (mDirection[0] < 0 && mDirection[1] < 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " °\nNorth of West";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(880, 480),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
			if (mDirection[0] < 0 && mDirection[1] > 0){
				wstringstream out;
				out << "Direction: " << int(atan(abs(mDirection[1] / mDirection[0])) * 180 / 3.1415926) << " °\nSouth of West";
				graphics->DrawString(out.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&font2,      // The font to use
					PointF(880, 480),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
			}
		}

		//graphics->DrawString(L"Hey! Try cover\nthe sensor!",  // String to draw
		//	-1,         // String length, -1 means it figures it out on its own
		//	&font2,      // The font to use
		//	PointF(750, 50),   // Where to draw (top left corner)
		//	&white);    // The brush to draw the text with
	}

	if (mFactor != 1 && !mPredictionQueue.empty()){
		int y0 = 550;
		int x0 = 121;
		Pen color(Color(0, 0, 255), 1);
		for (int k = 0; k < mPredictionQueue.size(); k++)
		{
			graphics->DrawLine(&color, x0 + 3 * (k + 1) + 10, y0,
				x0 + 3 * (k + 1) + 10, y0 - ((mPredictionQueue[k]) / 500)*mScale * 0.75);
		}
	}

	if (mFactor != 1)
	{
		graphics->DrawString(L"blue: prediction\nred: current",  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&font,      // The font to use
			PointF(300, 430),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with
	}
}

/** Test to see if we hit this object with a mouse.
* \param x X position to test
* \param y Y position to test
* \return true if hit. */
bool CSolarPanel::HitTest(int x, int y)
{
	double wid = mPanelImage->GetWidth();
	double hit = mPanelImage->GetHeight();

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
	auto format = mPanelImage->GetPixelFormat();
	if (format == PixelFormat32bppARGB || format == PixelFormat32bppPARGB)
	{
		// This image has an alpha map, which implements the 
		// transparency. If so, we should check to see if we
		// clicked on a pixel where alpha is not zero, meaning
		// the pixel shows on the screen.
		Color color;
		mPanelImage->GetPixel((int)testX, (int)testY, &color);
		return color.GetAlpha() != 0;
	}
	else {
		return true;
	}
}

/** Save this item to an XML node
* \param node The node we are going to be a child of */
std::shared_ptr<xmlnode::CXmlNode>
CSolarPanel::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
	auto itemNode = CItem::XmlSave(node);
	itemNode->SetAttribute(L"type", L"solarpanel");
	return itemNode;
}

/** this function set the flag of sensor
* \param name is the sensor name
* \param flag is the flag */
void CSolarPanel::SetFlag(std::string name, double flag)
{
	/// when it's in warning mode, activated
	if (!mTable.empty()){
		CItem::SetFlag(name, flag);

		/// store three data points after a sensor is triggered
		/*if (flag == 1){
		for (int i = 0; i < mElement.size(); i++){
		if (mElement[i] == (name + ':') && mValue[name].size() < 3)
		mValue[name].push_back(stol(mElement[i + 1]));
		}
		}*/

		/// change the flag if needed, create a map from triggered time to sensor name
		if (mTable[name][2] != flag){
			mTable[name][2] = flag;
			if (flag == 1 && mTimer.size() == 0){
				mDuration = 0;
				mTimer.clear();
				double buff = mDuration;
				mTimer[buff] = name;
				mTimeTriggered.push_back(buff);
				for (int i = 0; i < mElement.size(); i++){
					if (mElement[i] == (name + ':'))
						mCloudyValue.push_back(stol(mElement[i + 1]));
					else if (mElement[i].back() == ':')
						mSunnyValue.push_back(stol(mElement[i + 1]));
				}
			}

			/// only check the first three triggered sensor
			else if (flag == 1 && mTimer.size() < 3){
				double buff = mDuration;

				if (mDuration - mTimeTriggered.back() > 0.1 && mTimer.rbegin()->second != name)
				{
					mTimer[buff] = name;
					mTimeTriggered.push_back(buff);
					for (int i = 0; i < mElement.size(); i++){
						if (mElement[i] == (name + ':'))
							mCloudyValue.push_back(stol(mElement[i + 1]));
					}
				}
			}
		}
	}
}

/** store vector of data into the sensor*/
void CSolarPanel::SetElement(vector<string> ele)
{
	mElement.clear();
	for (auto i : ele){
		mElement.push_back(i);
	}
}

/** calculate the distance from two given points*/
double Distance(vector<double> &p1, vector<double> &p2){
	double x, y;
	x = p1[0] - p2[0];
	y = p1[1] - p2[1];
	return sqrt(x*x + y*y);
}

vector<double> Vector(vector<double> &panel, vector<double> &s1, vector<double> &s2, vector<double> &s3)
{
	double x = 3 * panel[0] - s1[0] - s2[0] - s3[0];
	double y = 3 * panel[1] - s1[1] - s2[1] - s3[1];
	double size = sqrt(x*x + y*y) / 80;
	vector<double> direction;
	direction.push_back(x / size);
	direction.push_back(y / size);
	return direction;
}

/** update function*/
void CSolarPanel::Update(double elapsed)
{
	CItem::Update(elapsed);		///< up call to upadte function in Item (from Childview)
	mDuration += elapsed;		///< duration from first triggered sensor (calculate next two trigger time)
	mArrival -= elapsed;		///< update the arrival time
	mPredictionTimer += elapsed;

	/// when there's two triggered sensor, esitimate the arrival time
	/// mTimeTriggereed[num] is the triggered time 
	/// mTimer[mTimeTriggered[num]] is the triggered sensor name
	/// mTable[mTimer[mTimeTriggered[num]]] is the associated vlotage value
	/// CItem::Location() is the panel location
	if (mTimer.size() == 2 && mArrival < 0 && buff == 0){
		//mPredictionQueue.clear();
		buff = 1;
		double d1, d2, d12, d2cos;
		d1 = Distance(CItem::Location(), mTable[mTimer[mTimeTriggered[0]]]);
		d2 = Distance(CItem::Location(), mTable[mTimer[mTimeTriggered[1]]]);
		d12 = Distance(mTable[mTimer[mTimeTriggered[0]]], mTable[mTimer[mTimeTriggered[1]]]);
		d2cos = (d1*d1 + d2*d2 - d12*d12) / (2 * d1);

		double speed;
		if (mTimeTriggered[1] == mTimeTriggered[0]) speed = (d1 - d2cos) / factor;
		else speed = (d1 - d2cos) / (mTimeTriggered[1] - mTimeTriggered[0]);
		mArrival = d1 / speed;

		/// calculate voltage drop
		double before = 0, after = 0;
		for (auto i : mCloudyValue){ after += i; }
		after = after / mCloudyValue.size();

		for (auto i : mSunnyValue){ before += i; }
		before = before / mSunnyValue.size();
		mDrop = (int)(100 * after / before);

		mOutput = 2 * after / 10000;

		draw = true;
	}

	if (mTimer.size() == 3 && buff == 1){
		buff = -1;
		double d1, d2, d3, d12, d2cos, d13, d3cos;
		d1 = Distance(CItem::Location(), mTable[mTimer[mTimeTriggered[0]]]);
		d2 = Distance(CItem::Location(), mTable[mTimer[mTimeTriggered[1]]]);
		d12 = Distance(mTable[mTimer[mTimeTriggered[0]]], mTable[mTimer[mTimeTriggered[1]]]);
		d2cos = (d1*d1 + d2*d2 - d12*d12) / (2 * d1);
		d3 = Distance(CItem::Location(), mTable[mTimer[mTimeTriggered[2]]]);
		d13 = Distance(mTable[mTimer[mTimeTriggered[0]]], mTable[mTimer[mTimeTriggered[2]]]);
		d3cos = (d1*d1 + d3*d3 - d13*d13) / (2 * d1);

		mDirection = Vector(CItem::Location(), mTable[mTimer[mTimeTriggered[0]]],
			mTable[mTimer[mTimeTriggered[1]]], mTable[mTimer[mTimeTriggered[2]]]);

		double speed1, speed2, speed3;
		if (mTimeTriggered[1] == mTimeTriggered[0])	speed1 = (d1 - d2cos) / factor;
		else speed1 = (d1 - d2cos) / (mTimeTriggered[1] - mTimeTriggered[0]);

		if (mTimeTriggered[2] == mTimeTriggered[1]) speed2 = (d2cos - d3cos) / factor;
		else speed2 = (d2cos - d3cos) / (mTimeTriggered[2] - mTimeTriggered[1]);

		if (mTimeTriggered[2] == mTimeTriggered[0])	speed3 = (d1 - d3cos) / factor;
		else speed3 = (d1 - d3cos) / (mTimeTriggered[2] - mTimeTriggered[0]);
		double speed = (speed1 + speed2 + speed3) / 3;
		mArrival = d3cos / speed;

		double before = 0, after = 0;
		for (auto i : mCloudyValue){ after += i; }
		after = after / mCloudyValue.size();

		for (auto i : mSunnyValue){ before += i; }
		before = before / mSunnyValue.size();
		mDrop = (int)(100 - 100 * after / before);
		mOutput = 2 * after / 10000;
		draw = true;
		drawDirection = true;

		mBefore = before;
		mAfter = after;
		if (mArrival <= 30){
			for (int i = 0; i < (int)mArrival + 30; i++)
				mPredictionQueue.push_back(before);
			for (int j = (int)mArrival + 30; j < 60; j++)
				mPredictionQueue.push_back(after);
			mPredictionTimer = 0;
		}

		else if (mArrival > 30)
		{
			for (int i = 0; i < 60; i++)
				mPredictionQueue.push_back(before);
			mPredictionFlag = true;
		}

		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);
		wstringstream message;
		message << L"Arriving in: " << mArrival << L" s\nPower drop: " << mDrop << L" %\nEstimate Power Output: "
			<< fixed << setprecision(2) << mOutput << L"MW\nEstimate Arrival Time: " <<
			now->tm_hour << L":" << now->tm_min << L":" << now->tm_sec + (int)mArrival;
		//AfxMessageBox(message.str().c_str());
	}

	/// local reset
	if (mArrival < -10){ draw = false; drawDirection = false; }
	if (buff == -1 && mArrival < -10){
		mPredictionQueue.clear();
		mValue.clear();
		mTimer.clear();
		mTimeTriggered.clear();
		buff = 0;
	}

	if (!mPredictionQueue.empty() && mPredictionTimer >1 && mPredictionFlag == false)
	{
		mPredictionQueue.pop_front();
		mPredictionQueue.push_back(mAfter);
		mPredictionTimer = 0;
	}

	if (mPredictionFlag == true)
	{
		if (mArrival < 30){
			mPredictionFlag = false;
			mPredictionTimer = 0;
		}
	}
}