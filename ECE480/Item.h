/**
 * \file Item.h
 *
 * \author Tianhang Sun
 *
 * this is base class for sensor and panel, handle function common to them
 */

#include "Xmlnode.h"
#include "ItemVisitor.h"
#include <string>
#include <vector>

#pragma once

class CGoogleMap;

class CItem
{
public:
	virtual ~CItem();

	/// Default constructor (disabled)
	CItem() = delete;

	/// Copy constructor (disabled)
	CItem(const CItem &) = delete;

	/** The X location of the item
	* \returns X location in pixels */
	double GetX() const { return mX; }

	/** The Y location of the item
	* \returns Y location in pixels */
	double GetY() const { return mY; }

	/// Set the item location
	/// \param x X location
	/// \param y Y location
	void SetLocation(double x, double y) { mX = x; mY = y; }

	/// Draw this item
	/// \param graphics Graphics device to draw on
	virtual void Draw(Gdiplus::Graphics *graphics) = 0;

	/** Test this item to see if it has been clicked on
	* \param x X location on the aquarium to test
	* \param y Y location on the aquarium to test
	* \return true if clicked on */
	virtual bool HitTest(int x, int y) = 0;

	virtual std::shared_ptr<xmlnode::CXmlNode>
		CItem::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node) = 0;

	virtual void CItem::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node);

	/// Handle updates for animation
	/// \param elapsed The time since the last update
	virtual void Update(double elapsed) {}

	/// useless functions, pass panel or sensor when iterate our items
	virtual void PropertiesDlg() = 0;
	virtual void SetElement(std::vector<std::string> ele) = 0;
	virtual std::vector<double> GetLocationFlag(){ return{}; };
	virtual void SetFlag(std::string name, double flag){};
	virtual std::string GetName(){ return ""; };

	/** Accept a visitor
	* \param visitor The visitor we accept */
	virtual void Accept(CItemVisitor *visitor) = 0;

	/** set warning mode for all items*/
	virtual void SetMode(bool warning){ mWarning = warning; }
	void SetFactor(double factor, double edge){ mFactor = factor; mEdge = edge; }
	void SetScale(double scale){ mScale = scale; }

protected:
	CItem(CGoogleMap *googlemap);

	bool mWarning = false;		///< in warning mode?

	std::vector<double> Location();

	// Item location in the map
	double  mX = 0;     ///< X location for the center of the item
	double  mY = 0;     ///< Y location for the center of the item

	double mFactor = 1;
	double mEdge = 0;
	double mScale = 1;

private:
	/// this item is contained in
	CGoogleMap   *mGoogleMap;

};

