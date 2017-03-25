#include "stdafx.h"
#include "PanelVisitor.h"
#include "SolarPanel.h"


CPanelVisitor::CPanelVisitor()
{
}


CPanelVisitor::~CPanelVisitor()
{
}

void CPanelVisitor::VisitSolarPanel(CSolarPanel *panel)
{
	panel->SetMap(mTable);
}