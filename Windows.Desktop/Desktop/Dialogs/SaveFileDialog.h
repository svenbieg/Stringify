//==================
// SaveFileDialog.h
//==================

#pragma once


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//===========
// Save File
//===========

class SaveFileDialog
{
public:
	// Common
	static Handle<String> Show(Handle<String> Path, Handle<Sentence> Title=nullptr, Handle<Sentence> Filter=nullptr);
};

}}
