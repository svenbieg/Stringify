//==============
// ComPointer.h
//==============

#pragma once


//=============
// COM-Pointer
//=============

template <class _obj_t>
class ComPointer
{
public:
	// Con-/Destructors
	ComPointer(): m_Object(nullptr) {}
	ComPointer(_obj_t* Object): m_Object(Object) {}
	~ComPointer()
		{
		if(m_Object)
			m_Object->Release();
		m_Object=nullptr;
		}
	inline VOID Initialize(_obj_t* Object) { m_Object=Object; }

	// Access
	inline operator _obj_t*()const { return m_Object; }
	inline _obj_t* operator->()const { return m_Object; }

	// Modification
	ComPointer& operator=(_obj_t* Object)
		{
		if(m_Object==Object)
			return *this;
		if(m_Object)
			m_Object->Release();
		m_Object=Object;
		if(m_Object)
			m_Object->AddRef();
		return *this;
		}
	inline ComPointer& operator=(ComPointer const& Object) { return operator=(Object.m_Object); }

private:
	// Common
	_obj_t* m_Object;
};
