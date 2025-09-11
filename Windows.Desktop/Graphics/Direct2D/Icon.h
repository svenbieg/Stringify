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
class Handle<Graphics::Direct2D::Icon>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Using
	using ICON=Resources::Icons::ICON;
	using Icon=Graphics::Direct2D::Icon;

	// Con-/Destructors
	inline Handle(): m_Object(nullptr) {}
	inline Handle(nullptr_t): m_Object(nullptr) {}
	inline Handle(Icon* Copy) { Handle<Object>::Create(&m_Object, Copy); }
	inline Handle(Handle const& Copy) { Handle<Object>::Create(&m_Object, Copy.m_Object); }
	inline Handle(Handle&& Move)noexcept: m_Object(Move.m_Object) { Move.m_Object=nullptr; }
	inline Handle(INT Id): m_Object(nullptr) { operator=(Id); }
	inline Handle(ICON const* Resource): m_Object(nullptr) { operator=(Resource); }
	inline ~Handle() { Handle<Object>::Clear(&m_Object); }

	// Access
	inline operator BOOL()const { return m_Object!=nullptr; }
	inline operator Icon*()const { return m_Object; }
	inline Icon* operator->()const { return m_Object; }

	// Comparison
	inline BOOL operator==(nullptr_t)const { return m_Object==nullptr; }
	inline BOOL operator==(Icon* Value)const { return m_Object==Value; }
	inline BOOL operator!=(nullptr_t)const { return m_Object!=nullptr; }
	inline BOOL operator!=(Icon* Value)const { return m_Object!=Value; }

	// Assignment
	inline Handle& operator=(nullptr_t) { Handle<Object>::Clear(&m_Object); return *this; }
	inline Handle& operator=(Icon* Copy) { Handle<Object>::Set(&m_Object, Copy); return *this; }
	inline Handle& operator=(Handle const& Copy) { return operator=(Copy.m_Object); }
	inline Handle& operator=(UINT Id) { return operator=(Icon::Create(Id)); }
	inline Handle& operator=(ICON const* Resource) { return operator=(Icon::Create(Resource)); }

private:
	// Common
	Icon* m_Object;
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