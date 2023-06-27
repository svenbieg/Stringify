//==============
// EditMenu.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Framework/Application.h"
#include "Resources/Icons/Edit.h"
#include "Resources/Strings/Edit.h"
#include "EditMenu.h"

using namespace Framework;
using namespace Graphics;
using namespace Resources::Icons;
using namespace Resources::Strings;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

EditMenu::EditMenu(Frame* frame):
PopupMenu(frame, nullptr)
{
auto app=Application::Current;
SelectAll=Add(STR_MM_EDIT_SELECT_ALL);
SelectAll->Clicked.Add(app, &Application::EditSelectAll);
SelectAll->Icon=new Icon(ICO_EDIT_SELECT_ALL);
Add(nullptr);
Copy=Add(STR_MM_EDIT_COPY);
Copy->Clicked.Add(app, &Application::EditCopy);
Copy->Icon=new Icon(ICO_EDIT_COPY);
Add(nullptr);
Cut=Add(STR_MM_EDIT_CUT);
Cut->Clicked.Add(app, &Application::EditCut);
Cut->Icon=new Icon(ICO_EDIT_CUT);
Delete=Add(STR_MM_EDIT_DELETE);
Delete->Clicked.Add(app, &Application::EditDelete);
Delete->Icon=new Icon(ICO_EDIT_DELETE);
Add(nullptr);
Paste=Add(STR_MM_EDIT_PASTE);
Paste->Clicked.Add(app, &Application::EditPaste);
Paste->Icon=new Icon(ICO_EDIT_PASTE);
}

}}}