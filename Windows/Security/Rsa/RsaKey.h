//==========
// RsaKey.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Buffer.h"


//===========
// Namespace
//===========

namespace Security {
	namespace Rsa {


//=========
// RSA-Key
//=========

class RsaKey: public Object
{
public:
	// Using
	using Buffer=Storage::Buffer;
	using ISeekable=Storage::Streams::ISeekable;

	// Con-/Destructors
	RsaKey(ISeekable* Exponent, ISeekable* Module);
	~RsaKey();

	// Common
	BOOL Verify(Buffer* Data, Buffer* Signature);

private:
	// Common
	BCRYPT_KEY_HANDLE m_Key;
};

}}