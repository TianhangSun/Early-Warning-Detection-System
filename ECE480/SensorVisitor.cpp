/**
 * \file SensorVisitor.cpp
 *
 * \author Tianhang Sun
 */

#include "stdafx.h"
#include "SensorVisitor.h"
#include "Sensor.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

CSensorVisitor::CSensorVisitor()
{
}

CSensorVisitor::~CSensorVisitor()
{
}

/** create a map from sensor name to location and warning flag*/
void CSensorVisitor::VisitSensor(CSensor *sensor)
{
	mTable[sensor->GetName()] = sensor->GetLocationFlag();
}