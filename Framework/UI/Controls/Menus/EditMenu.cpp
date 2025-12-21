//==============
// EditMenu.cpp
//==============

#include "EditMenu.h"


//=======
// Using
//=======

#include "Resources/Icons/Edit.h"
#include "Resources/Strings/Edit.h"
#include "UI/Application.h"

using namespace Graphics;
using namespace Resources::Icons;
using namespace Resources::Strings;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==========================
// Con-/Destructors Private
//==========================

EditMenu::EditMenu():
PopupMenu(nullptr)
{
auto app=Application::GetCurrent();
SelectAll=Add(STR_MM_EDIT_SELECT_ALL);
SelectAll->Clicked.Add(app, &Application::EditSelectAll);
SelectAll->Icon=Icon::Create(ICO_EDIT_SELECT_ALL);
Add(nullptr);
Copy=Add(STR_MM_EDIT_COPY);
Copy->Clicked.Add(app, &Application::EditCopy);
Copy->Icon=Icon::Create(ICO_EDIT_COPY);
Add(nullptr);
Cut=Add(STR_MM_EDIT_CUT);
Cut->Clicked.Add(app, &Application::EditCut);
Cut->Icon=Icon::Create(ICO_EDIT_CUT);
Delete=Add(STR_MM_EDIT_DELETE);
Delete->Clicked.Add(app, &Application::EditDelete);
Delete->Icon=Icon::Create(ICO_EDIT_DELETE);
Add(nullptr);
Paste=Add(STR_MM_EDIT_PASTE);
Paste->Clicked.Add(app, &Application::EditPaste);
Paste->Icon=Icon::Create(ICO_EDIT_PASTE);
}

}}}