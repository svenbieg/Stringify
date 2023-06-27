//================
// StringReader.h
//================

#pragma once


//=======
// Using
//=======

#include "Clusters/list.hpp"
#include "CharHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Streams {


//===============
// String-Reader
//===============

class StringReader
{
public:
	// Con-/Destructors
	StringReader(InputStream* Stream):
		LastChar(0),
		pReadAnsi(nullptr),
		pReadUnicode(nullptr),
		pStream(nullptr)
		{
		SetStream(Stream);
		}

	// Common
	inline UINT FindChar(LPCSTR Chars) { return DoFindChar(pReadAnsi, Chars); }
	inline UINT FindChar(LPCWSTR Chars) { return DoFindChar(pReadUnicode, Chars); }
	TCHAR LastChar;
	inline UINT ReadString(LPSTR Buffer, UINT Size) { return DoReadString(pReadAnsi, Buffer, Size); }
	inline UINT ReadString(LPWSTR Buffer, UINT Size) { return DoReadString(pReadUnicode, Buffer, Size); }
	inline UINT ReadString(LPSTR Buffer, UINT Size, CHAR Escape) { return DoReadString(pReadAnsi, Buffer, Size, Escape); }
	inline UINT ReadString(LPWSTR Buffer, UINT Size, CHAR Escape) { return DoReadString(pReadUnicode, Buffer, Size, Escape); }
	inline UINT ReadString(LPSTR Buffer, UINT Size, LPCSTR Escape, LPCSTR Truncate=nullptr) { return DoReadString(pReadAnsi, Buffer, Size, Escape, Truncate); }
	inline UINT ReadString(LPWSTR Buffer, UINT Size, LPCSTR Escape, LPCSTR Truncate=nullptr) { return DoReadString(pReadUnicode, Buffer, Size, Escape, Truncate); }
	inline Handle<String> ReadString(SIZE_T* Size=nullptr, LPCSTR Escape=nullptr, LPCSTR Truncate=nullptr)
		{
		#ifdef UNICODE
		return DoReadString(pReadUnicode, Size, Escape, Truncate);
		#else
		return DoReadString(pReadAnsi, Size, Escape, Truncate);
		#endif
		}
	VOID SetStream(InputStream* Stream)
		{
		if(pStream==Stream)
			return;
		pStream=Stream;
		auto format=pStream->GetFormat();
		switch(format)
			{
			case StreamFormat::Ansi:
				{
				pReadAnsi=AnsiRead<CHAR>;
				pReadUnicode=AnsiRead<WCHAR>;
				break;
				}
			case StreamFormat::Unicode:
				{
				pReadAnsi=UnicodeRead<CHAR>;
				pReadUnicode=UnicodeRead<WCHAR>;
				break;
				}
			case StreamFormat::UTF8:
				{
				pReadAnsi=Utf8Read<CHAR>;
				pReadUnicode=Utf8Read<WCHAR>;
				break;
				}
			}
		}
	UINT Skip(UINT Count=1)
		{
		UINT size=0;
		for(UINT u=0; u<Count; u++)
			{
			CHAR c=0;
			size+=pReadAnsi(pStream, &c);
			}
		return size;
		}

private:
	// Using
	typedef UINT (*ANSI_PROC)(InputStream* Stream, CHAR* Char);
	typedef UINT (*UNICODE_PROC)(InputStream* Stream, WCHAR* Char);

	// Common
	template <class _proc_t, class _char_t> UINT DoFindChar(_proc_t ReadChar, _char_t const* Chars)
		{
		UINT size=0;
		while(1)
			{
			_char_t c=0;
			size+=ReadChar(pStream, &c);
			LastChar=CharToChar<TCHAR, _char_t>(c);
			if(c==0)
				break;
			for(UINT u=0; Chars[u]; u++)
				{
				if(c==Chars[u])
					return size;
				}
			}
		return size;
		}

	template <class _proc_t, class _char_t> UINT DoReadString(_proc_t ReadChar, _char_t* Buffer, UINT Size)
		{
		UINT size=0;
		UINT pos=0;
		while(1)
			{
			_char_t c=0;
			size+=ReadChar(pStream, &c);
			LastChar=CharToChar<TCHAR, _char_t>(c);
			if(pos==Size)
				{
				Buffer[pos-1]=0;
				break;
				}
			if(!c)
				break;
			}
		return size;
		}
	template <class _proc_t, class _char_t> UINT DoReadString(_proc_t ReadChar, _char_t* Buffer, UINT Size, CHAR Escape)
		{
		UINT size=0;
		UINT pos=0;
		while(1)
			{
			_char_t c=0;
			size+=ReadChar(pStream, &c);
			LastChar=CharToChar<TCHAR, _char_t>(c);
			if(CharEqual(c, Escape))
				c=0;
			Buffer[pos++]=c;
			if(pos==Size)
				{
				Buffer[pos-1]=0;
				break;
				}
			if(!c)
				break;
			}
		return size;
		}
	template <class _proc_t, class _char_t> UINT DoReadString(_proc_t ReadChar, _char_t* Buffer, UINT Size, LPCSTR Escape, LPCSTR Truncate)
		{
		UINT size=0;
		UINT pos=0;
		while(1)
			{
			_char_t c=0;
			size+=ReadChar(pStream, &c);
			LastChar=CharToChar<TCHAR, _char_t>(c);
			BOOL skip=false;
			if(Truncate)
				{
				for(UINT u=0; Truncate[u]; u++)
					{
					if(CharEqual(c, Truncate[u]))
						{
						skip=true;
						break;
						}
					}
				}
			if(skip)
				continue;
			Truncate=nullptr;
			if(Escape)
				{
				for(UINT u=0; Escape[u]; u++)
					{
					if(CharEqual(c, Escape[u]))
						{
						c=0;
						break;
						}
					}
				}
			Buffer[pos++]=c;
			if(pos==Size)
				{
				Buffer[pos-1]=0;
				break;
				}
			if(!c)
				break;
			}
		return size;
		}
	template <class _proc_t> Handle<String> DoReadString(_proc_t ReadChar, SIZE_T* Size, LPCSTR Escape, LPCSTR Truncate)
		{
		SIZE_T size=0;
		Clusters::list<TCHAR> buf;
		while(1)
			{
			TCHAR c=0;
			size+=ReadChar(pStream, &c);
			LastChar=c;
			BOOL skip=false;
			if(Truncate)
				{
				for(UINT u=0; Truncate[u]; u++)
					{
					if(CharEqual(c, Truncate[u]))
						{
						skip=true;
						break;
						}
					}
				}
			if(skip)
				continue;
			Truncate=nullptr;
			if(Escape)
				{
				for(UINT u=0; Escape[u]; u++)
					{
					if(CharEqual(c, Escape[u]))
						{
						c=0;
						break;
						}
					}
				}
			if(c==0)
				break;
			buf.append(c);
			}
		if(Size)
			*Size+=size;
		UINT len=buf.get_count();
		if(!len)
			return nullptr;
		Handle<String> str=new String(len, nullptr);
		LPTSTR str_ptr=const_cast<LPTSTR>(str->Begin());
		buf.get_many(0, str_ptr, len);
		str_ptr[len]=0;
		return str;
		}
	ANSI_PROC pReadAnsi;
	UNICODE_PROC pReadUnicode;
	InputStream* pStream;
};

}}