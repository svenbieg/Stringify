//===========
// TabView.h
//===========

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==========
// Tab-View
//==========

class TabView: public Control
{
public:
	// Tab-Info
	class TabInfo: public Object
		{
		public:
			// Common
			Handle<String> Title;
		};

	// Con-/Destructors
	TabView(Window* Parent=nullptr);

	// Common
	Handle<TabInfo> AddTab(Window* Tab);
	VOID RemoveTab(Window* Tab);

private:
	// Using
	using TabMap=Collections::Map<Window*, Handle<TabInfo>>;

	// Common
	Handle<TabMap> hTabs;
};

}}