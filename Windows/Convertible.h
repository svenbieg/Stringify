//===============
// Convertible.h
//===============

#pragma once


//========
// Common
//========

template <class _convert_t> inline _convert_t* Convert(Object* Object)
{
return dynamic_cast<_convert_t*>(Object);
}
