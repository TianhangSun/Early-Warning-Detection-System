/**
* \file SolarPanel.h
*
* \author Tianhang Sun
*
* this class handle event of solar panel, including algorithm
*/

#pragma once
#include "Item.h"
#include "ItemVisitor.h"
#include <string>
#include <vector>
#include <map>
#include <deque>

class CSolarPanel : public CItem
{
public:
	CSolarPanel(CGoogleMap *googlemap);
	virtual ~CSolarPanel();

	/// Default constructor (disabled)
	CSolarPanel() = delete;

	/// Copy constructor (disabled)
	CSolarPanel(const CSolarPanel &) = delete;

	void CSolarPanel::Draw(Gdiplus::Graphics *graphics);
	bool CSolarPanel::HitTest(int x, int y);

	virtual std::shared_ptr<xmlnode::CXmlNode>
		XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node) override;

	void PropertiesDlg(){};
	void Update(double elapsed);
	void SetElement(std::vector<std::string> ele);

	/** Accept a visitor
	* \param visitor The visitor we accept */
	virtual void Accept(CItemVisitor *visitor) override { visitor->VisitSolarPanel(this); }

	void SetMap(std::map<std::string, std::vector<double>> m){ mTable = m; }
	void SetFlag(std::string name, double flag);

private:
	std::unique_ptr<Gdiplus::Bitmap> mPanelImage;	///< solar panel image
	//std::unique_ptr<Gdiplus::Bitmap> mEGRImage;	///< solar panel image
	//std::unique_ptr<Gdiplus::Bitmap> mMSUImage;	///< solar panel image

	/// map from sensor name to location and warning flag
	std::map<std::string, std::vector<double>> mTable;

	/// map from sensor name to first three data value
	std::map<std::string, std::vector<double>> mValue;

	std::vector<std::string> mElement;		///< vector of data passed from CGoogleMap
	std::map<double, std::string> mTimer;	///< map from time triggered to sensor name
	std::vector<double> mTimeTriggered;		///< vector of time triggered

	double mDuration = 0;			///< timer
	double mArrival = 0;			///< time when the cloud will arrive
	bool draw = false;				///< display the remaining time?
	int mDrop = 100;
	double mOutput;
	std::vector<double> mSunnyValue;
	std::vector<double> mCloudyValue;
	std::vector<double> mDirection;
	int buff = 0;
	bool drawDirection = false;

	int mBefore = 0;
	int mAfter = 0;
	std::deque<int> mPredictionQueue;

	double mPredictionTimer = 0;
	bool mPredictionFlag = false;
};

