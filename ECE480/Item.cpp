/**
 * \file Item.cpp
 *
 * \author Tianhang Sun
 */

#include "stdafx.h"
#include "Item.h"
#include "GoogleMap.h"
#include <vector>
using namespace std;

/** Constructor
* \param  this item is a member of */
CItem::CItem(CGoogleMap *googlemap) : mGoogleMap(googlemap)
{
}

/** destructor */
CItem::~CItem()
{
}

/** Save this item to an XML node
* \param node The node we are going to be a child of */
std::shared_ptr<xmlnode::CXmlNode>
CItem::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
	auto itemNode = node->AddChild(L"item");

	itemNode->SetAttribute(L"x", mX);
	itemNode->SetAttribute(L"y", mY);

	return itemNode;
}

/**
* Load the attributes for an item node.
*
* This is the  base class version that loads the attributes
* common to all items. Override this to load custom attributes
* for specific items.
*
* \param node The Xml node we are loading the item from
*/
void CItem::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
	mX = node->GetAttributeDoubleValue(L"x", 0);
	mY = node->GetAttributeDoubleValue(L"y", 0);
}

vector<double> CItem::Location(){
	vector<double> v;
	v.push_back(mX);
	v.push_back(mY);
	return v;
}