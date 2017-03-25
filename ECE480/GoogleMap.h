/**
 * \file GoogleMap.h
 *
 * \author Tianhang Sun
 *
 *	this class is the main class, hold the map and general functions
 */

#include <memory>
#include "Item.h"
#include "ItemVisitor.h"
#include <vector>
#include <deque>
#include <map>
#include <string>

#pragma once
class CGoogleMap
{
public:
	CGoogleMap();
	virtual ~CGoogleMap();

	void CGoogleMap::OnDraw(Gdiplus::Graphics *graphics);
	void CGoogleMap::Add(std::shared_ptr<CItem> item);
	std::shared_ptr<CItem> CGoogleMap::HitTest(int x, int y);
	void CGoogleMap::Save(const std::wstring &filename);
	void CGoogleMap::Load(const std::wstring &filename);
	void CGoogleMap::Clear();
	void CGoogleMap::ToggleTrashCanActive();
	bool CGoogleMap::IsOverTrashcan(int x, int y);
	void CGoogleMap::DeleteItem(std::shared_ptr<CItem> item);
	void CGoogleMap::Update(double elapsed);

	

	/** Accept a visitor for the collection
	* \param visitor The visitor for the collection
	*/
	void CGoogleMap::Accept(CItemVisitor *visitor){
		for (auto i : mItems){
			i->Accept(visitor);
		}
	}

	void SetWarningMode(bool warning);
	void SetFactor(double factor, double edge){
		mFactor = factor;
		mEdge = edge;
		for (auto i : mItems)
			i->SetFactor(factor, edge);
	}

	void SetScale(double scale){ mScale = scale; for (auto i : mItems) i->SetScale(scale); }
	double GetScale(){ return mScale; }

private:
	std::unique_ptr<Gdiplus::Bitmap> mBackground; ///< Background image to use
	std::unique_ptr<Gdiplus::Bitmap> mEGRImage;	///< solar panel image
	std::unique_ptr<Gdiplus::Bitmap> mMSUImage;	///< solar panel image

	/// All of the items to populate our map
	std::vector<std::shared_ptr<CItem> > mItems;

	/// support for saving/loading function
	void CGoogleMap::XmlItem(const std::shared_ptr<xmlnode::CXmlNode> &node);

	std::unique_ptr<Gdiplus::Bitmap> mTrashcan; ///< Trashcan image to use
	bool mTrashcanActive = true;                ///< Is the trash can active?

	double mDuration = 0;	///< time event, update data per second

	double mFactor = 1;

	double mEdge = 0;

	std::map<std::string, std::deque<std::string>> mPlotData;

	double mScale = 1;
};


