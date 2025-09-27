//========
// Icon.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/list.hpp"
#include "Storage/Buffer.h"


//===========
// Namespace
//===========

namespace Storage {


//========
// Struct
//========

typedef struct
{
Handle<Buffer> Buffer;
UINT Position;
UINT Size;
WORD Width;
}ICON;


//======
// Icon
//======

class Icon: public Object
{
public:
	// Using
	using icon_list=Collections::list<ICON>;
	using const_iterator=typename icon_list::const_iterator;

	// Con-/Destructors
	static inline Handle<Icon> Create(Handle<String> Path) { return new Icon(Path); }

	// Common
	const_iterator cbegin()const { return m_Icons.cbegin(); }

private:

	// Con-/Destructors
	Icon(Handle<String> Path);

	// Common
	icon_list m_Icons;
};

}