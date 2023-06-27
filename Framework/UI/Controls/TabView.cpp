//=============
// TabView.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "TabView.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TabView::TabView(Window* parent):
Control(parent)
{
hTabs=new TabMap();
}


//========
// Common
//========

Handle<TabView::TabInfo> TabView::AddTab(Window* tab)
{
tab->Parent=this;
Handle<TabInfo> tab_info=new TabInfo();
hTabs->Add(tab, tab_info);
return tab_info;
}

VOID TabView::RemoveTab(Window* tab)
{
tab->Parent=nullptr;
hTabs->Remove(tab);
}

}}