//========
// Icon.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/map.hpp"
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
	// Friends
	friend Handle<Icon>;

	// Con-/Destructors
	~Icon();
	static Handle<Icon> Create(WORD Id);
	static Handle<Icon> Create(ICON const* Resource);

	// Common
	HICON GetHandle(UINT Size=16);

private:
	// Con-/Destructors
	Icon(WORD Id);
	Icon(ICON const* Resource);

	// Common
	Collections::map<UINT, HICON> m_Handles;
	WORD s_Id;
};

}}


//========
// Handle
//========

template <>
class Handle<Graphics::Direct2D::Icon>: public HandleBase<Graphics::Direct2D::Icon>
{
public:
	// Using
	using _base_t=HandleBase<Graphics::Direct2D::Icon>;
	using _base_t::_base_t;
	using ICON=Resources::Icons::ICON;
	using Icon=Graphics::Direct2D::Icon;

	// Con-/Destructors
	Handle(INT Id) { Create(new Icon(Id)); }
	Handle(ICON const* Resource) { Create(new Icon(Resource)); }
};


//==================
// Con-/Destructors
//==================

namespace Graphics {
	namespace Direct2D {

inline Handle<Icon> Icon::Create(WORD Id)
{
return new Icon(Id);
}

inline Handle<Icon> Icon::Create(ICON const* Resource)
{
return new Icon(Resource);
}

}}