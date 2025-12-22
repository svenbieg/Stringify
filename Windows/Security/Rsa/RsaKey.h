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
	using Seekable=Storage::Streams::Seekable;

	// Con-/Destructors
	RsaKey(Seekable* Exponent, Seekable* Module);
	~RsaKey();

	// Common
	BOOL Verify(Buffer* Data, Buffer* Signature);

private:
	// Common
	BCRYPT_KEY_HANDLE m_Key;
};

}}