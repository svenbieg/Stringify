//========
// Icon.h
//========

#pragma once


//=======
// Using
//=======

#include "Clusters/map.hpp"
#include "Graphics/Icon.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//======
// Icon
//======

class Icon: public Graphics::Icon
{
public:
	// Con-/Destructors
	Icon(WORD Id);
	Icon(ICON const* Icon);
	~Icon();

	// Common
	HICON GetHandle(UINT Size=16);

private:
	// Common
	Clusters::map<UINT, HICON> m_Handles;
	WORD m_Id;
};


//========
// Handle
//========

template <>
class Handle<Icon>
{
public:
	// Using
	using ICON=Resources::Icons::ICON;

	// Con-/Destructors
	Handle(): m_Icon(nullptr) {}
	Handle(INT Id) { HandleCreate(&m_Icon, new Icon(Id)); }
	Handle(ICON const* Resource) { HandleCreate(&m_Icon, new Icon(Resource)); }
	Handle(Icon* Icon) { HandleCreate(&m_Icon, Icon); }

	// Access
	inline operator bool()const { return m_Icon!=nullptr; }
	inline Icon* operator->()const { return m_Icon; }

private:
	// Common
	Icon* m_Icon;
};

}}