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
	ComPointer(): pObject(nullptr) {}
	ComPointer(_obj_t* Object): pObject(Object)
		{
		if(pObject)
			pObject->AddRef();
		}
	ComPointer(ComPointer const& Pointer): pObject(Pointer.pObject)
		{
		if(pObject)
			pObject->AddRef();
		}
	ComPointer(ComPointer&& Pointer)noexcept: pObject(Pointer.pObject)
		{
		Pointer.pObject=nullptr;
		}
	~ComPointer()
		{
		if(pObject)
			pObject->Release();
		pObject=nullptr;
		}

	// Access
	inline operator _obj_t*()const { return pObject; }
	inline _obj_t* operator->()const { return pObject; }
	inline _obj_t* Get()const { return pObject; }

	// Modification
	ComPointer& operator=(_obj_t* Object)
		{
		if(pObject==Object)
			return *this;
		if(pObject)
			pObject->Release();
		pObject=Object;
		if(pObject)
			pObject->AddRef();
		return *this;
		}
	ComPointer& operator=(ComPointer const& Pointer) { return operator=(Pointer.pObject); }
	inline void** GetAddressOf() { return (void**)&pObject; }
	inline _obj_t** GetObjectPointer() { return &pObject; }
	inline IUnknown** GetUnknownPointer() { return (IUnknown**)&pObject; }

private:
	// Common
	_obj_t* pObject;
};
