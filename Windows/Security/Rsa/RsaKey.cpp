//============
// RsaKey.cpp
//============

#include "RsaKey.h"


//=======
// Using
//=======

#pragma comment(lib, "bcrypt")

#include "ErrorHelper.h"


//===========
// Namespace
//===========

namespace Security {
	namespace Rsa {


//==================
// Con-/Destructors
//==================

RsaKey::RsaKey(Seekable* exp, Seekable* mod):
m_Key(NULL)
{
BCRYPT_ALG_HANDLE provider=NULL;
auto status=BCryptOpenAlgorithmProvider(&provider, BCRYPT_RSA_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0);
ErrorHelper::ThrowIfFailed(status);
DWORD exp_len=(DWORD)exp->GetSize();
DWORD mod_len=(DWORD)mod->GetSize();
DWORD blob_len=sizeof(BCRYPT_RSAKEY_BLOB)+exp_len+mod_len;
auto blob=Buffer::Create(blob_len);
auto blob_ptr=blob->Begin();
auto header=(BCRYPT_RSAKEY_BLOB*)blob_ptr;
MemoryHelper::Fill(header, sizeof(BCRYPT_RSAKEY_BLOB), 0);
header->Magic=BCRYPT_RSAPUBLIC_MAGIC;
header->BitLength=mod_len*8;
header->cbPublicExp=exp_len;
header->cbModulus=mod_len;
auto exp_ptr=blob_ptr+sizeof(BCRYPT_RSAKEY_BLOB);
exp->Read(exp_ptr, exp_len);
auto mod_ptr=exp_ptr+exp_len;
mod->Read(mod_ptr, mod_len);
status=BCryptImportKeyPair(provider, NULL, BCRYPT_PUBLIC_KEY_BLOB, &m_Key, blob_ptr, blob_len, 0);
BCryptCloseAlgorithmProvider(provider, 0);
ErrorHelper::ThrowIfFailed(status);
}

RsaKey::~RsaKey()
{
if(m_Key!=NULL)
	BCryptDestroyKey(m_Key);
}


//========
// Common
//========

BOOL RsaKey::Verify(Buffer* data, Buffer* sig)
{
if(!data||!sig)
	return false;
BCRYPT_ALG_HANDLE provider=NULL;
auto status=BCryptOpenAlgorithmProvider(&provider, BCRYPT_SHA256_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0);
ErrorHelper::ThrowIfFailed(status);
DWORD hash_size=0;
ULONG result=0;
status=BCryptGetProperty(provider, BCRYPT_OBJECT_LENGTH, (BYTE*)&hash_size, sizeof(DWORD), &result, 0);
ErrorHelper::ThrowIfFailed(status);
auto hash_obj=Buffer::Create(hash_size);
DWORD hash_len=0;
status=BCryptGetProperty(provider, BCRYPT_HASH_LENGTH, (BYTE*)&hash_len, sizeof(DWORD), &result, 0);
ErrorHelper::ThrowIfFailed(status);
auto hash_buf=Buffer::Create(hash_len);
BCRYPT_HASH_HANDLE hash=NULL;
status=BCryptCreateHash(provider, &hash, hash_obj->Begin(), hash_size, nullptr, 0, 0);
BCryptCloseAlgorithmProvider(provider, 0);
ErrorHelper::ThrowIfFailed(status);
auto data_ptr=data->Begin();
auto data_len=(DWORD)data->GetSize();
status=BCryptHashData(hash, data_ptr, data_len, 0);
ErrorHelper::ThrowIfFailed(status);
status=BCryptFinishHash(hash, hash_buf->Begin(), hash_len, 0);
ErrorHelper::ThrowIfFailed(status);
auto sig_ptr=sig->Begin();
auto sig_len=(DWORD)sig->GetSize();
BCRYPT_PKCS1_PADDING_INFO padding={ 0 };
padding.pszAlgId=BCRYPT_SHA256_ALGORITHM;
status=BCryptVerifySignature(m_Key, &padding, hash_buf->Begin(), hash_len, sig_ptr, sig_len, BCRYPT_PAD_PKCS1);
BCryptDestroyHash(hash);
return BCRYPT_SUCCESS(status);
}

}}