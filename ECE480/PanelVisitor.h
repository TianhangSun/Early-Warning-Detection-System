#pragma once
#include "ItemVisitor.h"
#include <map>
#include <string>
#include <vector>

class CPanelVisitor : public CItemVisitor
{
public:
	CPanelVisitor();
	virtual ~CPanelVisitor();

	void VisitSolarPanel(CSolarPanel *panel);

	void SetMap(std::map<std::string, std::vector<double>> m ){ mTable = m; }

private:
	/// map from sensor name to location and warning flag
	std::map<std::string, std::vector<double>> mTable;
};

