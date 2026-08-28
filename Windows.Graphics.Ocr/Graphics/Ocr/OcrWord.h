//===========
// OcrWord.h
//===========

#pragma once


//=======
// Using
//=======

#include "Graphics/Rect.h"
#include "StringClass.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ocr {


//======================
// Forward-Declarations
//======================

class OcrScanner;


//==========
// OCR-Word
//==========

class OcrWord: public Object
{
public:
	// Friends
	friend Object;
	friend OcrScanner;

	// Common
	RECT Position;
	Handle<String> Text;

private:
	// Con-/Destructors
	inline OcrWord(Handle<String> Text, INT Left, INT Top, INT Right, INT Bottom):
		Position(Left, Top, Right, Bottom),
		Text(Text)
		{}
	static inline Handle<OcrWord> Create(Handle<String> Text, INT Left, INT Top, INT Right, INT Bottom)
		{
		return new OcrWord(Text, Left, Top, Right, Bottom);
		}
};

}}