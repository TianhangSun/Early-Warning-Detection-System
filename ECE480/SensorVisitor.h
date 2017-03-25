/**
 * \file SensorVisitor.h
 *
 * \author Tianhang Sun
 *
 * this class visit each sensor
 */

#pragma once
#include "ItemVisitor.h"
#include <map>
#include <string>
#include <vector>

class CSensorVisitor : public CItemVisitor
{
public:
	CSensorVisitor();
	virtual ~CSensorVisitor();

	void VisitSensor(CSensor *sensor);

	std::map<std::string, std::vector<double>> GetMap(){ return mTable; }

private:
	/// map from sensor name to location and warning flag
	std::map<std::string, std::vector<double>> mTable;
};

