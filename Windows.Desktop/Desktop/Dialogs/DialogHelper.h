//================
// DialogHelper.h
//================

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


//========
// Common
//========

class DialogHelper
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Common
	static LPCTSTR ExtensionFromFilter(Handle<String> Filter, UINT Id);
	static Handle<String> FilterFromSentence(Handle<Sentence> Sentence);
};

}}