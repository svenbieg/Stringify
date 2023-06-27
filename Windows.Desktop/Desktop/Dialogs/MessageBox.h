//==============
// MessageBox.h
//==============

#pragma once


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
	// Common
	static VOID Error(Handle<Sentence> Title, Handle<Sentence> Message);
	static VOID Info(Handle<Sentence> Title, Handle<Sentence> Message);
	static BOOL Question(Handle<Sentence> Title, Handle<Sentence> Message);
	static BOOL Warning(Handle<Sentence> Title, Handle<Sentence> Message);

private:
	// Common
	static INT Show(LPCSTR Title, LPCSTR Message, UINT Type);
};

}}