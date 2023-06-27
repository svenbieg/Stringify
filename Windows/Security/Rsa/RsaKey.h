//==========
// RsaKey.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Buffer.h"
#include "Storage/StaticBuffer.h"


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
	using Seekable=Storage::Seekable;
	using StaticBuffer=Storage::StaticBuffer;

	// Con-/Destructors
	RsaKey(Handle<Seekable> Exponent, Handle<Seekable> Module);
	~RsaKey();

	// Common
	BOOL Verify(Handle<StaticBuffer> Data, Handle<Buffer> Signature);

private:
	// Common
	BCRYPT_KEY_HANDLE hKey;
};

}}