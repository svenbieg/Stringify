//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Core/Application.h"
#include "UI/Controls/Menus/Menu.h"
#include "UI/Controls/Input.h"


//===========
// Namespace
//===========

namespace UI {


//=============
// Application
//=============

class Application: public Core::Application
{
public:
	// Using
	using Input=UI::Controls::Input;
	using Interactive=UI::Controls::Interactive;
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using Menu=UI::Controls::Menus::Menu;
	using ShortcutMap=Collections::Map<UINT, Interactive*>;

	// Common
	static Application* Current;
	VOID ExitMenu();
	Input* GetCurrentInput()const { return m_CurrentInput; }
	Menu* GetCurrentMenu()const { return m_CurrentMenu; }
	Interactive* GetPointerFocus()const { return m_PointerFocus; }
	VOID SetCurrentInput(Input* Input);
	VOID SetCurrentMenu(Menu* Menu) { m_CurrentMenu=Menu; }
	VOID SetPointerFocus(Interactive* Focus);
	BOOL Shortcut(Handle<KeyEventArgs> Args);
	Handle<ShortcutMap> Shortcuts;

	// Edit
	VOID EditCopy();
	VOID EditCut();
	VOID EditDelete();
	VOID EditPaste();
	VOID EditSelectAll();

protected:
	// Con-/Destructors
	Application(LPCSTR Name);

private:
	// Common
	Input* m_CurrentInput;
	Menu* m_CurrentMenu;
	Interactive* m_PointerFocus;
};

}