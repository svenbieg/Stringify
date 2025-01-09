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
	Handle(): m_Object(nullptr) {}
	Handle(nullptr_t): m_Object(nullptr) {}
	Handle(Icon* Object): m_Object(Object)
		{
		if(m_Object)
			m_Object->m_RefCount++;
		}
	Handle(Handle const& Copy): Handle(Copy.m_Object) {}
	Handle(Handle&& Move)noexcept: m_Object(Move.m_Object)
		{
		Move.m_Object=nullptr;
		}
	Handle(INT Id): m_Object(nullptr) { operator=(Id); }
	Handle(ICON const* Resource): m_Object(nullptr) { operator=(Resource); }
	~Handle()
		{
		if(m_Object)
			{
			m_Object->Release();
			m_Object=nullptr;
			}
		}

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
	inline Handle& operator=(nullptr_t)
		{
		this->~Handle();
		return *this;
		}
	Handle& operator=(Icon* Object)
		{
		if(m_Object==Object)
			return *this;
		if(m_Object)
			m_Object->Release();
		m_Object=Object;
		if(m_Object)
			m_Object->m_RefCount++;
		return *this;
		}
	inline Handle& operator=(Handle const& Copy) { return operator=(Copy.m_Object); }
	Handle& operator=(UINT Id)
		{
		auto value=Icon::Create(Id);
		return operator=(value);
		}
	Handle& operator=(ICON const* Resource)
		{
		auto value=Icon::Create(Resource);
		return operator=(value);
		}

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