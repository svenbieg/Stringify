//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Culture/Sentence.h"
#include "Graphics/Theme.h"
#include "UI/Controls/Menus/Menu.h"
#include "UI/Controls/Input.h"


//===========
// Namespace
//===========

namespace UI {


//=============
// Application
//=============

class Application: public virtual Object
{
public:
	// Using
	using Input=UI::Controls::Input;
	using Interactive=UI::Controls::Interactive;
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using Menu=UI::Controls::Menus::Menu;
	using Sentence=Culture::Sentence;
	using ShortcutMap=Collections::Map<UINT, Interactive*>;
	using Theme=Graphics::Theme;

	// Con-/Destructors
	~Application();
	static inline Application* Get() { return s_Current; }

	// Common
	VOID ExitMenu();
	inline Input* GetCurrentInput()const { return m_CurrentInput; }
	inline Menu* GetCurrentMenu()const { return m_CurrentMenu; }
	inline Handle<Sentence> GetName()const { return m_Name; }
	inline Interactive* GetPointerFocus()const { return m_PointerFocus; }
	virtual Handle<Theme> GetTheme()=0;
	VOID Quit();
	VOID SetCurrentInput(Input* Input);
	inline VOID SetCurrentMenu(Menu* Menu) { m_CurrentMenu=Menu; }
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
	Application(Handle<Sentence> Name);

private:
	// Common
	Input* m_CurrentInput;
	Menu* m_CurrentMenu;
	Handle<Sentence> m_Name;
	Interactive* m_PointerFocus;
	static Application* s_Current;
};

}