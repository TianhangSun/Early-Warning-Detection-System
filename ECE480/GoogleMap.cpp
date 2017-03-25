/**
 * \file GoogleMap.cpp
 *
 * \author Tianhang Sun
 */

#include "stdafx.h"
#include "GoogleMap.h"
#include "Sensor.h"
#include "XmlNode.h"
#include "Sensor.h"
#include "SolarPanel.h"
#include <memory>
#include <afx.h>
#include <afxinet.h>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using namespace Gdiplus;
using namespace std;
using namespace xmlnode;

/// Margin to right and below trash can in pixels
const int TrashCanMargin = 50;
const int v = 4;
const int h = 4;
const int t = 30;

/** constructor */
CGoogleMap::CGoogleMap()
{
	/// load campus map
	mBackground = unique_ptr<Gdiplus::Bitmap>(
		Bitmap::FromFile(L"images/CampusMap.png"));
	if (mBackground->GetLastStatus() != Ok){
		AfxMessageBox(L"Failed to open images/CampusMap.png");
	}

	/// load trash can
	mTrashcan = unique_ptr<Bitmap>(Bitmap::FromFile(L"images/trashcan.png"));
	if (mTrashcan->GetLastStatus() != Ok){
		AfxMessageBox(L"Failed to open images/trashcan.png");
	}

	mEGRImage = unique_ptr<Bitmap>(Bitmap::FromFile(L"images/EGR.gif"));
	if (mEGRImage->GetLastStatus() != Ok){
		wstring msg(L"Failed to open ");
		msg += L"EGR.gif";
		AfxMessageBox(msg.c_str());
	}
	mMSUImage = unique_ptr<Bitmap>(Bitmap::FromFile(L"images/MSU.jpg"));
	if (mMSUImage->GetLastStatus() != Ok){
		wstring msg(L"Failed to open ");
		msg += L"MSU.jpg";
		AfxMessageBox(msg.c_str());
	}
}

/** destructor */
CGoogleMap::~CGoogleMap()
{
}

/** Draw the map
* \param graphics The GDI+ graphics context to draw on */
void CGoogleMap::OnDraw(Gdiplus::Graphics *graphics)
{
	FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 16);
	Gdiplus::Font fontSmall(&fontFamily, 12);
	Pen pen(Color(0, 0, 0), 2);

	/// draw campus map
	SolidBrush green(Color(0, 64, 0));
	graphics->DrawImage(mBackground.get(), int(0 + mEdge), int(0 + mEdge),
		mBackground->GetWidth() / mFactor, mBackground->GetHeight() / mFactor);

	if (mFactor == 1)
	{
		double wid1 = mEGRImage->GetWidth();
		double hit1 = mEGRImage->GetHeight();
		double wid2 = mMSUImage->GetWidth();
		double hit3 = mMSUImage->GetHeight();
		graphics->DrawImage(mEGRImage.get(), (int)880, (int)50,
			(int)mEGRImage->GetWidth() / 2, (int)mEGRImage->GetHeight() / 2);
		graphics->DrawImage(mMSUImage.get(), (int)880, (int)580,
			(int)mMSUImage->GetWidth() / 2, (int)mMSUImage->GetHeight() / 2);
	}

	/// pass draw functions to items in the map
	for (auto item : mItems){
		item->Draw(graphics);
	}

	/// draw trash can, disable in warning mode
	if (mTrashcanActive){
		int trashcanX = mBackground->GetWidth() - mTrashcan->GetWidth() - TrashCanMargin;
		int trashcanY = mBackground->GetHeight() - mTrashcan->GetHeight() - TrashCanMargin;

		graphics->DrawImage(mTrashcan.get(),
			trashcanX, trashcanY,
			mTrashcan->GetWidth(), mTrashcan->GetHeight());
	}

	if (!mPlotData.empty())
	{
		SolidBrush black(Color(0, 0, 0));
		wstringstream out;
		out << "Time range: " << t << " s";
		//graphics->DrawString(out.str().c_str(),  // String to draw
		//	-1,         // String length, -1 means it figures it out on its own
		//	&font,      // The font to use
		//	PointF(float(100), float(300)),   // Where to draw (top left corner)
		//	&black);    // The brush to draw the text with

		int y = 125;
		for (int i = 0; i < v; i++)
		{
			int x = 480;
			for (int j = 0; j < h; j++)
			{
				graphics->DrawLine(&pen, x, y, x + 120, y);
				graphics->DrawLine(&pen, x, y, x, y - 100);
				graphics->DrawLine(&pen, x + 120, y, x + 115, y - 5);
				graphics->DrawLine(&pen, x + 120, y, x + 115, y + 5);
				graphics->DrawLine(&pen, x, y - 100, x + 5, y - 95);
				graphics->DrawLine(&pen, x, y - 100, x - 5, y - 95);
				graphics->DrawLine(&pen, x, y - 75, x - 5, y - 75);
				graphics->DrawLine(&pen, x + 100, y, x + 100, y + 5);
				graphics->DrawLine(&pen, x + 13, y, x + 13, y + 5);
				graphics->DrawString(L"t",  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&fontSmall,      // The font to use
					PointF(float(x + 115), float(y + 8)),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
				graphics->DrawString(L"v",  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&fontSmall,      // The font to use
					PointF(float(x - 25), float(y - 110)),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
				graphics->DrawString(L"2.5",  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&fontSmall,      // The font to use
					PointF(float(x - 35), float(y - 80)),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with
				Pen color(Color(200 - 60 * i, 60 * j, 200 - 20 * i * j), 2);
				stringstream name;
				name << "sensor" << i * v + j << ":";
				wstringstream nameDisplay;
				nameDisplay << "sensor " << i * v + j;
				graphics->DrawString(nameDisplay.str().c_str(),  // String to draw
					-1,         // String length, -1 means it figures it out on its own
					&fontSmall,      // The font to use
					PointF(float(x + 20), float(y + 8)),   // Where to draw (top left corner)
					&black);    // The brush to draw the text with

				for (int k = 0; k < mPlotData[name.str()].size(); k++)
				{
					graphics->DrawLine(&color, x + 3*(k+1) + 10, y,
						x +3*(k+1) + 10, y - (stol(mPlotData[name.str()][k]) * mScale / 500) * 0.75);
				}
				x = x + 150;
			}
			y = y + 150;
		}

		int y0 = 550;
		int x0 = 120;
		graphics->DrawLine(&pen, x0, y0, x0 + 240, y0);
		graphics->DrawLine(&pen, x0, y0, x0, y0 - 100);
		graphics->DrawLine(&pen, x0 + 240, y0, x0 + 235, y0 - 5);
		graphics->DrawLine(&pen, x0 + 240, y0, x0 + 235, y0 + 5);
		graphics->DrawLine(&pen, x0, y0 - 100, x0 + 5, y0 - 95);
		graphics->DrawLine(&pen, x0, y0 - 100, x0 - 5, y0 - 95);
		graphics->DrawLine(&pen, x0, y0 - 75, x0 - 5, y0 - 75);
		graphics->DrawLine(&pen, x0 + 100, y0, x0 + 100, y0 + 5);
		graphics->DrawLine(&pen, x0 + 15, y0, x0 + 15, y0 + 5);
		graphics->DrawString(L"t",  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&fontSmall,      // The font to use
			PointF(float(x0 + 235), float(y0 + 8)),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with
		graphics->DrawString(L"v",  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&fontSmall,      // The font to use
			PointF(float(x0 - 25), float(y0 - 110)),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with
		graphics->DrawString(L"2.5",  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&fontSmall,      // The font to use
			PointF(float(x0 - 35), float(y0 - 80)),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with
		Pen color(Color(255, 0, 0), 1);

		wstringstream nameDisplay;
		nameDisplay << "Solar Panel Location";
		graphics->DrawString(nameDisplay.str().c_str(),  // String to draw
			-1,         // String length, -1 means it figures it out on its own
			&fontSmall,      // The font to use
			PointF(float(x0 + 20), float(y0 + 8)),   // Where to draw (top left corner)
			&black);    // The brush to draw the text with

		for (int k = 0; k < mPlotData["sensor1:"].size(); k++)
		{
			graphics->DrawLine(&color, x0 + 3 * (k + 1) + 10, y0,
				x0 + 3 * (k + 1) + 10, y0 - (stol(mPlotData["sensor1:"][k])*mScale / 500) * 0.75);
		}

	}
}

/** Add an item to the map
* \param item New item to add */
void CGoogleMap::Add(std::shared_ptr<CItem> item)
{
	mItems.push_back(item);
}

/** Test an x,y click location to see if it clicked
* on some item in the aquarium.
* \param x X location
* \param y Y location
* \returns Pointer to item we clicked on or nullptr if none. */
std::shared_ptr<CItem> CGoogleMap::HitTest(int x, int y)
{
	for (auto i = mItems.rbegin(); i != mItems.rend(); i++){
		if ((*i)->HitTest(x, y)){
			return *i;
		}
	}
	return  nullptr;
}

/** save function
* \param filename is the file name we want to save to*/
void CGoogleMap::Save(const std::wstring &filename)
{
	// Create an XML document
	auto root = CXmlNode::CreateDocument(L"aqua");

	// Iterate over all items and save them
	for (auto item : mItems){
		item->XmlSave(root);
	}

	try{
		root->Save(filename);
	}
	catch (CXmlNode::Exception ex){
		AfxMessageBox(ex.Message().c_str());
	}
}

/** load function
* \param filename is the file name we want to load from*/
void CGoogleMap::Load(const std::wstring &filename)
{
	// We surround with a try/catch to handle errors
	try{
		// Open the document to read
		shared_ptr<CXmlNode> root = CXmlNode::OpenDocument(filename);

		// Once we know it is open, clear the existing data
		Clear();

		// Traverse the children of the root
		// node of the XML document in memory!!!!
		for (auto node : root->GetChildren()){
			if (node->GetType() == NODE_ELEMENT && node->GetName() == L"item"){
				XmlItem(node);
			}
		}
	}
	catch (CXmlNode::Exception ex){
		AfxMessageBox(ex.Message().c_str());
	}
}

/** delete everything in map*/
void CGoogleMap::Clear()
{
	mItems.clear();
}

/** Handle an item node.
* \param node Pointer to XML node we are handling */
void CGoogleMap::XmlItem(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
	// A pointer for the item we are loading
	shared_ptr<CItem> item;

	// We have an item. What type?
	wstring type = node->GetAttributeValue(L"type", L"");
	if (type == L"sensor"){
		item = make_shared<CSensor>(this);
	}

	if (type == L"solarpanel"){
		item = make_shared<CSolarPanel>(this);
	}

	if (item != nullptr){
		item->XmlLoad(node);
		Add(item);
	}
}

/** Toggle the state of the flag mTrashCanActive */
void CGoogleMap::ToggleTrashCanActive()
{
	mTrashcanActive = !mTrashcanActive;
}

/** Is this location over the trash can?
* \param x X location
* \param y Y location
* \returns true if x,y is over the trash can */
bool CGoogleMap::IsOverTrashcan(int x, int y)
{
	if (!mTrashcanActive){
		return false;
	}

	/// location of trash can
	int trashcanX = mBackground->GetWidth() - mTrashcan->GetWidth() - TrashCanMargin;
	int trashcanY = mBackground->GetHeight() - mTrashcan->GetHeight() - TrashCanMargin;

	return x >= trashcanX &&
		x < trashcanX + (int)mTrashcan->GetWidth() &&
		y >= trashcanY &&
		y < trashcanY + (int)mTrashcan->GetHeight();
}

/** Delete an item from the map
* \param item The item to delete. */
void CGoogleMap::DeleteItem(std::shared_ptr<CItem> item)
{
	auto loc = find(begin(mItems), end(mItems), item);
	if (loc != end(mItems)){
		mItems.erase(loc);
	}
}

/** Get file from internet
* \param szUrl is the website URL
* \param szFileName is the filename we store the website to*/
void GetFile(WCHAR *szUrl, WCHAR *szFileName)
{
	/// to be honest, I don't know what's happening in this function
	TCHAR sz[1024];
	CInternetSession session(_T("MyTest agent"), 1,
		INTERNET_OPEN_TYPE_DIRECT);
	CStdioFile* pFile = NULL;
	WCHAR   szHead[] = L"Accept: */*\r\n\r\n";
	DWORD nRead;
	CFile myFile;
	CFileException fileException;

	if (!myFile.Open(szFileName, CFile::modeCreate | CFile::modeReadWrite,
		&fileException)) {
		return;
	}

	try{
		pFile = session.OpenURL(szUrl, 1, INTERNET_FLAG_RELOAD
			| INTERNET_FLAG_TRANSFER_BINARY, szHead, -1L);
	}
	catch (CInternetException *pEx) { return; }

	do{
		nRead = pFile->Read(sz, 1023);
		if (nRead != 0){
			myFile.Write(sz, nRead);
		}
	} while (nRead != 0);

	myFile.Close();
	pFile->Close();
	if (pFile != NULL)
		delete pFile;
	session.Close();
	return;
}

/** split one line into vector of elements
* \param str is input line
* \param ele is the output vector
* \param delim is whilte space which we use to sperate elements*/
void split(const string &str, vector<string> &ele, char delim = ' '){
	stringstream ss(str);
	string item;
	while (getline(ss, item, delim)){
		if (!item.empty())
			ele.push_back(item);
	}
}

/** Handle updates for animation
* \param elapsed The time since the last update */
void CGoogleMap::Update(double elapsed)
{
	/// pass time event to other items
	for (auto item : mItems){
		item->Update(elapsed);
	}

	/// update data from web each second
	mDuration += elapsed;
	if (mDuration > 1){
		string line1, line2;
		vector<string> element1, element2, element;

		GetFile(L"https://agent.electricimp.com/6hmIZEP5UR5U", L"output1.txt");
		GetFile(L"https://agent.electricimp.com/48H2Qgwd8We5", L"output2.txt");

		fstream file1("output1.txt");
		fstream file2("output2.txt");

		/// split line into vector
		while (getline(file1, line1))
			split(line1, element1);
		while (getline(file2, line2))
			split(line2, element2);

		for (auto i : element1){ element.push_back(i); }
		for (auto i : element2){ element.push_back(i); }

		/// pass element vector to items
		for (auto item : mItems){
			item->SetElement(element);
		}

		if (mFactor != 1)
		{
			for (int i = 0; i < element.size(); i++)
			{
				if (element[i].substr(0, 6) == "sensor")
				{
					mPlotData[element[i]].push_back(element[i + 1]);
					while (mPlotData[element[i]].size() < t){
						mPlotData[element[i]].push_front("0");
					}
					if (mPlotData[element[i]].size() > t)
						mPlotData[element[i]].pop_front();
				}
			}
		}
		else mPlotData.clear();

		file1.close();
		file2.close();

		for (auto item : mItems){
			if (item->GetLocationFlag().size() == 3){
				for (auto i : mItems){
					i->SetFlag(item->GetName(), item->GetLocationFlag()[2]);
				}
			}
		}
		mDuration = 0;
	}
}

/** set warning mode for items*/
void CGoogleMap::SetWarningMode(bool warning)
{ 
	for (auto item : mItems){
		item->SetMode(warning);
	}
}