//===========
// OcrLine.h
//===========

#pragma once


//=======
// Using
//=======

#include "Collections/list.hpp"
#include "Graphics/Ocr/OcrWord.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ocr {


//==========
// OCR-Line
//==========

class OcrLine: public Object
{
public:
	// Using
	using WordList=::Collections::list<Handle<OcrWord>>;

	// Friends
	friend Object;
	friend OcrScanner;

	// Common
	Handle<String> Text;
	WordList Words;

private:
	// Con-/Destructors
	OcrLine()=default;
	static inline Handle<OcrLine> Create()
		{
		return Object::Create<OcrLine>();
		}
};

}}