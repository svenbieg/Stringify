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
	// Using
	using IconMap=Clusters::map<UINT, HICON>;

	// Common
	IconMap cIcons;
	WORD uId;
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
	Handle(): pIcon(nullptr) {}
	Handle(std::nullptr_t): pIcon(nullptr) {}
	Handle(INT Id) { HandleCreate(&pIcon, new Icon(Id)); }
	Handle(ICON const* Resource) { HandleCreate(&pIcon, new Icon(Resource)); }
	Handle(Icon* Icon) { HandleCreate(&pIcon, Icon); }

	// Access
	inline operator bool()const { return pIcon!=nullptr; }
	inline Icon* operator->()const { return pIcon; }

private:
	// Common
	Icon* pIcon;
};

}}