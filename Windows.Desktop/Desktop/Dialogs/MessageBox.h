//==============
// MessageBox.h
//==============

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


//=============
// Message-Box
//=============

class MessageBox
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Common
	static VOID Error(Handle<Sentence> Title, Handle<Sentence> Message);
	static VOID Info(Handle<Sentence> Title, Handle<Sentence> Message);
	static BOOL Question(Handle<Sentence> Title, Handle<Sentence> Message);
	static BOOL Warning(Handle<Sentence> Title, Handle<Sentence> Message);

private:
	// Common
	static INT Show(LPCTSTR Title, LPCTSTR Message, UINT Type);
};

}}