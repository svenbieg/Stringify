//=============
// OcrResult.h
//=============

#pragma once


//=======
// Using
//=======

#include "Collections/index.hpp"
#include "Graphics/Ocr/OcrLine.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ocr {


//============
// OCR-Result
//============

class OcrResult: public Object
{
public:
	// Using
	using LineList=::Collections::list<Handle<OcrLine>>;
	using WordIndex=::Collections::index<Handle<String>>;

	// Friends
	friend Object;
	friend OcrScanner;

	// Common
	WordIndex Index;
	LineList Lines;

private:
	// Con-/Destructors
	OcrResult()=default;
	static inline Handle<OcrResult> Create()
		{
		return Object::Create<OcrResult>();
		}
};

}}