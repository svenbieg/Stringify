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
#include "UI/Controls/Interactive.h"
#include "UI/Input/KeyEvent.h"


//===========
// Namespace
//===========

namespace UI {


//=============
// Application
//=============

class Application: public Object
{
public:
	// Using
	using Interactive=UI::Controls::Interactive;
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using Sentence=Culture::Sentence;
	using ShortcutMap=Collections::Map<UINT, Interactive*>;
	using Theme=Graphics::Theme;

	// Con-/Destructors
	~Application();

	// Common
	static inline Application* GetCurrent() { return s_Current; }
	inline Handle<Sentence> GetName()const { return m_Name; }
	virtual VOID Quit()=0;
	BOOL Shortcut(Handle<KeyEventArgs> Args);
	Handle<ShortcutMap> Shortcuts;

	// Edit
	virtual VOID EditCopy();
	virtual VOID EditCut();
	virtual VOID EditDelete();
	virtual VOID EditPaste();
	virtual VOID EditSelectAll();

protected:
	// Con-/Destructors
	Application(Handle<Sentence> Name);

private:
	// Common
	Handle<Sentence> m_Name;
	static Application* s_Current;
};

}