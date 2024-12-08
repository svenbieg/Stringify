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
	// Con-/Destructors
	Icon(WORD Id);
	Icon(ICON const* Icon);
	~Icon();

	// Common
	HICON GetHandle(UINT Size=16);

private:
	// Common
	Collections::map<UINT, HICON> m_Handles;
	WORD s_Id;
};


//========
// Handle
//========

template <>
class Handle<Icon>: public ::Details::HandleBase<Icon>
{
public:
	// Using
	using _base_t=::Details::HandleBase<Icon>;
	using _base_t::_base_t;
	using ICON=Resources::Icons::ICON;

	// Con-/Destructors
	Handle(INT Id) { Create(new Icon(Id)); }
	Handle(ICON const* Resource) { Create(new Icon(Resource)); }
};

}}