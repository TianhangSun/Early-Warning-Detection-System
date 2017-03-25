/**
 * \file ItemVisitor.h
 *
 * \author Tianhang Sun
 *
 * this class is visitor base class
 */

#pragma once

class CSensor;
class CSolarPanel;

class CItemVisitor
{
public:
	CItemVisitor();
	virtual ~CItemVisitor();

	/** Visit a CTileBuilding object
	* \param building Building we are visiting */
	virtual void VisitSensor(CSensor *sensor) {}

	/** Visit a CTileCoalmine object
	* \param coalmine Coal mine we are visiting */
	virtual void VisitSolarPanel(CSolarPanel *panel) {}
};

