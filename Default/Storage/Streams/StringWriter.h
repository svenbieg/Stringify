//================
// StringWriter.h
//================

#pragma once


//=======
// Using
//=======

#include "CharHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Streams {


//===============
// String-Writer
//===============

class StringWriter
{
public:
	// Con-/Destructors
	StringWriter(OutputStream* Stream):
		pStream(nullptr),
		pWriteAnsi(nullptr),
		pWriteUnicode(nullptr)
		{
		SetStream(Stream);
		}

	// Common
	inline UINT Print(LPCSTR Value) { return DoPrint(pWriteAnsi, 0, Value); }
	inline UINT Print(LPCWSTR Value) { return DoPrint(pWriteUnicode, 0, Value); }
	inline UINT Print(UINT Length, LPCSTR Value) { return DoPrint(pWriteAnsi, Length, Value); }
	inline UINT Print(UINT Length, LPCWSTR Value) { return DoPrint(pWriteUnicode, Length, Value); }
	inline UINT Print(Handle<String> Value)
		{
		if(!Value)
			return 0;
		return Print(0, Value->Begin());
		}
	template <class... _args_t> UINT Print(LPCSTR Format, _args_t... Arguments)
		{
		Handle<String> str=new String(Format, Arguments...);
		return Print(0, str->Begin());
		}
	inline UINT PrintChar(CHAR Char, UINT Count=1) { return DoPrintChar(pWriteAnsi, Char, Count); }
	inline UINT PrintChar(WCHAR Char, UINT Count=1) { return DoPrintChar(pWriteUnicode, Char, Count); }
	VOID SetStream(OutputStream* Stream)
		{
		if(pStream==Stream)
			return;
		pStream=Stream;
		auto format=pStream->GetFormat();
		switch(format)
			{
			case StreamFormat::Ansi:
				{
				pWriteAnsi=AnsiWrite<CHAR>;
				pWriteUnicode=AnsiWrite<WCHAR>;
				break;
				}
			case StreamFormat::Unicode:
				{
				pWriteAnsi=UnicodeWrite<CHAR>;
				pWriteUnicode=UnicodeWrite<WCHAR>;
				break;
				}
			case StreamFormat::UTF8:
				{
				pWriteAnsi=Utf8Write<CHAR>;
				pWriteUnicode=Utf8Write<WCHAR>;
				break;
				}
			}
		}

private:
	// Using
	typedef UINT (*ANSI_PROC)(OutputStream* Stream, CHAR Char);
	typedef UINT (*UNICODE_PROC)(OutputStream* Stream, WCHAR Char);

	// Common
	template <class _proc_t, class _char_t> UINT DoPrint(_proc_t WriteChar, UINT Length, _char_t const* Value)
		{
		if(!Value)
			return 0;
		if(Length==0)
			Length=UINT_MAX;
		UINT size=0;
		for(UINT u=0; Value[u]; u++)
			{
			if(u==Length)
				break;
			size+=WriteChar(pStream, Value[u]);
			}
		return size;
		}
	template <class _proc_t, class _char_t> UINT DoPrintChar(_proc_t WriteChar, _char_t Char, UINT Count)
		{
		UINT size=0;
		for(UINT u=0; u<Count; u++)
			size+=WriteChar(pStream, Char);
		return size;
		}
	OutputStream* pStream;
	ANSI_PROC pWriteAnsi;
	UNICODE_PROC pWriteUnicode;
};

}}