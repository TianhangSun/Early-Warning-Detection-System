/**
 * \file Sensor.h
 *
 * \author Tianhang Sun
 *
 * this class handles events unique to sensors
 */

#pragma once
#include "Item.h"
#include "ItemVisitor.h"
#include <memory>
#include <string>
#include <vector>

class CSensor : public CItem
{
public:
	CSensor(CGoogleMap *googlemap);
	virtual ~CSensor();

	/// possible sensor names
	enum Name {Sensor0 = 0, Sensor1 = 1, Sensor2 = 2, Sensor3 = 3, Sensor4 = 4,
			   Sensor5 = 5, Sensor6 = 6, Sensor7 = 7, Sensor8 = 8, Sensor9 = 9,
			   Sensor10 = 10, Sensor11 = 11, Sensor12 = 12, Sensor13 = 13, Sensor14 = 14, Sensor15 = 15};

	/// Default constructor (disabled)
	CSensor() = delete;

	/// Copy constructor (disabled)
	CSensor(const CSensor &) = delete;

	void CSensor::Draw(Gdiplus::Graphics *graphics);
	bool CSensor::HitTest(int x, int y);

	virtual std::shared_ptr<xmlnode::CXmlNode>
		XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node) override;
	void CSensor::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node);

	void CSensor::Update(double elapsed);
	void PropertiesDlg();
	void SetElement(std::vector<std::string> ele);
	std::string GetName();
	std::vector<double> GetLocationFlag();

	/** Accept a visitor
	* \param visitor The visitor we accept */
	virtual void Accept(CItemVisitor *visitor) override { visitor->VisitSensor(this); }

private:
	std::unique_ptr<Gdiplus::Bitmap> mSensorImage;	///< sensor image
	std::unique_ptr<Gdiplus::Bitmap> mWarningImage;	///< warning sign

	double mCurrentData;		///< current value of sensor
	double mDuration = 0;		///< timer, update per second
	bool mFlag = false;				///< true when voltage lower than threshold

	Name mSensorName = Sensor0;				///< default sensor name
	std::vector<std::string> mElement;		///< vector of data passed from CGoogleMap
};

