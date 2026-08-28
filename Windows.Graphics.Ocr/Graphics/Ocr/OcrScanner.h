//==============
// OcrScanner.h
//==============

#pragma once


//=======
// Using
//=======

#include "Graphics/Ocr/OcrResult.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ocr {


//=============
// OCR-Scanner
//=============

class OcrScanner
{
public:
	// Common
	static Handle<OcrResult> ScanImage(Handle<String> Path);
};

}}
