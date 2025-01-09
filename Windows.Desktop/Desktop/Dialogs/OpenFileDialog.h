//==================
// OpenFileDialog.h
//==================

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//===========
// Open File
//===========

class OpenFileDialog
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Common
	static Handle<String> Show(Handle<String> Path, Handle<Sentence> Title=nullptr, Handle<Sentence> Filter=nullptr);
};

}}
