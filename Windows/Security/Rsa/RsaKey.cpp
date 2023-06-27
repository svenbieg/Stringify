//============
// RsaKey.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "bcrypt")

#include "RsaKey.h"


//===========
// Namespace
//===========

namespace Security {
	namespace Rsa {


//==================
// Con-/Destructors
//==================

RsaKey::RsaKey(Handle<Seekable> exp, Handle<Seekable> mod):
hKey(NULL)
{
BCRYPT_ALG_HANDLE provider=NULL;
auto status=BCryptOpenAlgorithmProvider(&provider, BCRYPT_RSA_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0);
ThrowIfFailed(status);
DWORD exp_len=(DWORD)exp->GetSize();
DWORD mod_len=(DWORD)mod->GetSize();
DWORD blob_len=sizeof(BCRYPT_RSAKEY_BLOB)+exp_len+mod_len;
Handle<Buffer> blob=new Buffer(blob_len);
auto blob_ptr=blob->Begin();
auto header=(BCRYPT_RSAKEY_BLOB*)blob_ptr;
ZeroMemory(header, sizeof(BCRYPT_RSAKEY_BLOB));
header->Magic=BCRYPT_RSAPUBLIC_MAGIC;
header->BitLength=mod_len*8;
header->cbPublicExp=exp_len;
header->cbModulus=mod_len;
auto exp_ptr=blob_ptr+sizeof(BCRYPT_RSAKEY_BLOB);
exp->Read(exp_ptr, exp_len);
auto mod_ptr=exp_ptr+exp_len;
mod->Read(mod_ptr, mod_len);
status=BCryptImportKeyPair(provider, NULL, BCRYPT_PUBLIC_KEY_BLOB, &hKey, blob_ptr, blob_len, 0);
BCryptCloseAlgorithmProvider(provider, 0);
ThrowIfFailed(status);
}

RsaKey::~RsaKey()
{
if(hKey!=NULL)
	BCryptDestroyKey(hKey);
}


//========
// Common
//========

BOOL RsaKey::Verify(Handle<StaticBuffer> data, Handle<Buffer> sig)
{
if(!data||!sig)
	return false;
BCRYPT_ALG_HANDLE provider=NULL;
auto status=BCryptOpenAlgorithmProvider(&provider, BCRYPT_SHA256_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0);
ThrowIfFailed(status);
DWORD hash_size=0;
ULONG result=0;
status=BCryptGetProperty(provider, BCRYPT_OBJECT_LENGTH, (BYTE*)&hash_size, sizeof(DWORD), &result, 0);
ThrowIfFailed(status);
Handle<Buffer> hash_obj=new Buffer(hash_size);
DWORD hash_len=0;
status=BCryptGetProperty(provider, BCRYPT_HASH_LENGTH, (BYTE*)&hash_len, sizeof(DWORD), &result, 0);
ThrowIfFailed(status);
Handle<Buffer> hash_buf=new Buffer(hash_len);
BCRYPT_HASH_HANDLE hash=NULL;
status=BCryptCreateHash(provider, &hash, hash_obj->Begin(), hash_size, nullptr, 0, 0);
BCryptCloseAlgorithmProvider(provider, 0);
ThrowIfFailed(status);
auto data_ptr=data->Begin();
auto data_len=(DWORD)data->GetSize();
status=BCryptHashData(hash, data_ptr, data_len, 0);
ThrowIfFailed(status);
status=BCryptFinishHash(hash, hash_buf->Begin(), hash_len, 0);
auto sig_ptr=sig->Begin();
auto sig_len=(DWORD)sig->GetSize();
BCRYPT_PKCS1_PADDING_INFO padding;
ZeroMemory(&padding, sizeof(BCRYPT_PKCS1_PADDING_INFO));
padding.pszAlgId=BCRYPT_SHA256_ALGORITHM;
status=BCryptVerifySignature(hKey, &padding, hash_buf->Begin(), hash_len, sig_ptr, sig_len, BCRYPT_PAD_PKCS1);
BCryptDestroyHash(hash);
return BCRYPT_SUCCESS(status);
}

}}