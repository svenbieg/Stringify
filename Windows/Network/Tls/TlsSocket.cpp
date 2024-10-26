//===============
// TlsSocket.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Secur32.lib")

#include <schannel.h>
#include "TlsSocket.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//==================
// Con-/Destructors
//==================

TlsSocket::TlsSocket(Handle<TcpSocket> sock):
Socket(sock),
cStreamSizes({ 0 }),
hCredential({ 0, 0 }),
hSecurityContext({ 0, 0 }),
pBuffer(),
pContext(nullptr),
pInput(),
pOutput(),
uBufferSize(0),
uInputSize(0),
uMissingSize(0),
uOutputSize(0)
{}

TlsSocket::~TlsSocket()
{
CloseInternal();
}


//========
// Common
//========

BOOL TlsSocket::Accept(Handle<String> host_name)
{
HCERTSTORE store=CertOpenSystemStoreA(NULL, "My");
PCCERT_CONTEXT context=CertFindCertificateInStore(store, X509_ASN_ENCODING, 0, CERT_FIND_SUBJECT_STR_A, host_name->Begin(), nullptr);
CertCloseStore(store, 0);
if(!context)
	return false;
pContext=context;
SCHANNEL_CRED cred;
ZeroMemory(&cred, sizeof(SCHANNEL_CRED));
cred.dwVersion=SCHANNEL_CRED_VERSION;
cred.paCred=&pContext;
cred.cCreds=1;
HRESULT status=AcquireCredentialsHandle(nullptr, SCHANNEL_NAME, SECPKG_CRED_INBOUND, nullptr, &cred, nullptr, nullptr, &hCredential, nullptr);
if(FAILED(status))
	return false;
CtxtHandle* context_in=nullptr;
CtxtHandle* context_out=&hSecurityContext;
uMissingSize=TLS_BUF_SIZE;
while(1)
	{
	if(uMissingSize>0)
		{
		UINT read=(UINT)Socket->Read(&pBuffer[uBufferSize], uMissingSize);
		uBufferSize+=read;
		uMissingSize=0;
		}
	SecBuffer in_buf[2];
	in_buf[0].pvBuffer=pBuffer;
	in_buf[0].cbBuffer=uBufferSize;
	in_buf[0].BufferType=SECBUFFER_TOKEN;
	in_buf[1].pvBuffer=nullptr;
	in_buf[1].cbBuffer=0;
	in_buf[1].BufferType=SECBUFFER_EMPTY;
	SecBufferDesc in_buf_desc;
	in_buf_desc.pBuffers=in_buf;
	in_buf_desc.cBuffers=2;
	in_buf_desc.ulVersion=SECBUFFER_VERSION;
	SecBuffer out_buf[2];
	out_buf[0].pvBuffer=nullptr;
	out_buf[0].cbBuffer=0;
	out_buf[0].BufferType=SECBUFFER_EMPTY;
	out_buf[1].pvBuffer=nullptr;
	out_buf[1].cbBuffer=0;
	out_buf[1].BufferType=SECBUFFER_EMPTY;
	SecBufferDesc out_buf_desc;
	out_buf_desc.pBuffers=out_buf;
	out_buf_desc.cBuffers=2;
	out_buf_desc.ulVersion=SECBUFFER_VERSION;
	DWORD flags=ASC_REQ_ALLOCATE_MEMORY|ASC_REQ_REPLAY_DETECT|ASC_REQ_SEQUENCE_DETECT|ASC_REQ_STREAM;
	DWORD flags_out=0;
	status=AcceptSecurityContext(&hCredential, context_in, &in_buf_desc, flags, SECURITY_NATIVE_DREP, context_out, &out_buf_desc, &flags_out, nullptr);
	if(FAILED(status))
		return false;
	context_in=&hSecurityContext;
	context_out=nullptr;
	if(out_buf[0].cbBuffer&&out_buf[0].pvBuffer)
		{
		Socket->Write(out_buf[0].pvBuffer, out_buf[0].cbBuffer);
		FreeContextBuffer(out_buf[0].pvBuffer);
		}
	uBufferSize=0;
	uMissingSize=TLS_BUF_SIZE;
	if(status==SEC_E_OK)
		break;
	}
status=QueryContextAttributes(&hSecurityContext, SECPKG_ATTR_STREAM_SIZES, &cStreamSizes);
if(FAILED(status))
	return false;
return true;
}

VOID TlsSocket::Close()
{
DWORD type=SCHANNEL_SHUTDOWN;
SecBuffer buf;
buf.pvBuffer=&type;
buf.cbBuffer=sizeof(DWORD);
buf.BufferType=SECBUFFER_TOKEN;
SecBufferDesc buf_desc;
buf_desc.pBuffers=&buf;
buf_desc.cBuffers=1;
buf_desc.ulVersion=SECBUFFER_VERSION;
while(1)
	{
	HRESULT status=ApplyControlToken(&hSecurityContext, &buf_desc);
	if(FAILED(status))
		break;
	DWORD flags=ISC_REQ_SEQUENCE_DETECT|ISC_REQ_REPLAY_DETECT|ISC_REQ_CONFIDENTIALITY|ISC_RET_EXTENDED_ERROR|ISC_REQ_ALLOCATE_MEMORY|ISC_REQ_STREAM;
	DWORD flags_out=0;
	buf.pvBuffer=nullptr;
	buf.cbBuffer=0;
	buf.BufferType=SECBUFFER_TOKEN;
	buf_desc.pBuffers=&buf;
	buf_desc.cBuffers=1;
	buf_desc.ulVersion=SECBUFFER_VERSION;
	status=AcceptSecurityContext(&hCredential, &hSecurityContext, nullptr, flags, SECURITY_NATIVE_DREP, nullptr, &buf_desc, &flags_out, nullptr);
	if(FAILED(status))
		break;
	if(buf.cbBuffer!=0&&buf.pvBuffer)
		{
		Socket->Write(buf.pvBuffer, buf.cbBuffer);
		FreeContextBuffer(buf.pvBuffer);
		break;
		}
	}
CloseInternal();
}

BOOL TlsSocket::Handshake(Handle<String> host_name)
{
SCHANNEL_CRED cred;
ZeroMemory(&cred, sizeof(SCHANNEL_CRED));
cred.dwVersion=SCH_CREDENTIALS_VERSION;
HRESULT status=AcquireCredentialsHandle(nullptr, SCHANNEL_NAME, SECPKG_CRED_OUTBOUND, nullptr, &cred, nullptr, nullptr, &hCredential, nullptr);
if(FAILED(status))
	return false;
CtxtHandle* context_in=nullptr;
CtxtHandle* context_out=&hSecurityContext;
SecBuffer in_buf;
in_buf.pvBuffer=pBuffer;
in_buf.BufferType=SECBUFFER_TOKEN;
SecBufferDesc in_buf_desc;
in_buf_desc.pBuffers=&in_buf;
in_buf_desc.cBuffers=1;
in_buf_desc.ulVersion=SECBUFFER_VERSION;
SecBuffer out_buf;
out_buf.pvBuffer=nullptr;
out_buf.cbBuffer=0;
out_buf.BufferType=SECBUFFER_EMPTY;
SecBufferDesc out_buf_desc;
out_buf_desc.pBuffers=&out_buf;
out_buf_desc.cBuffers=1;
out_buf_desc.ulVersion=SECBUFFER_VERSION;
while(1)
	{
	if(uMissingSize>0)
		{
		Sleep(10);
		UINT read=(UINT)Socket->Read(&pBuffer[uBufferSize], uMissingSize);
		uBufferSize+=read;
		uMissingSize=0;
		}
	SecBufferDesc* in_bufs=nullptr;
	if(uBufferSize>0)
		{
		in_buf.cbBuffer=uBufferSize;
		in_bufs=&in_buf_desc;
		}
	out_buf.pvBuffer=nullptr;
	out_buf.cbBuffer=0;
	out_buf.BufferType=SECBUFFER_EMPTY;
	DWORD flags=ISC_REQ_ALLOCATE_MEMORY|ISC_REQ_REPLAY_DETECT|ISC_REQ_CONFIDENTIALITY|ISC_RET_EXTENDED_ERROR|ISC_REQ_MANUAL_CRED_VALIDATION;
	DWORD flags_out=0;
	status=InitializeSecurityContext(&hCredential, context_in, nullptr, flags, 0, SECURITY_NATIVE_DREP, in_bufs, 0, context_out, &out_buf_desc, &flags_out, nullptr);
	if(FAILED(status))
		return false;
	context_in=&hSecurityContext;
	context_out=nullptr;
	if(out_buf.cbBuffer&&out_buf.pvBuffer)
		{
		Socket->Write(out_buf.pvBuffer, out_buf.cbBuffer);
		FreeContextBuffer(out_buf.pvBuffer);
		}
	uBufferSize=0;
	uMissingSize=TLS_BUF_SIZE;
	if(status==SEC_E_OK)
		break;
	}
status=QueryContextAttributes(&hSecurityContext, SECPKG_ATTR_STREAM_SIZES, &cStreamSizes);
if(FAILED(status))
	return false;
return true;
}


//==============
// Input-Stream
//==============

SIZE_T TlsSocket::Available()
{
return uBufferSize;
}

SIZE_T TlsSocket::Read(VOID* dst, SIZE_T size)
{
SIZE_T read=0;
while(1)
	{
	if(uInputSize>0)
		{
		read=Min(size, (SIZE_T)uInputSize);
		CopyMemory(dst, pInput, read);
		if(read<uInputSize)
			MoveMemory(pInput, &pInput[read], uInputSize-read);
		uInputSize-=read;
		break;
		}
	while(1)
		{
		if(uMissingSize>0)
			{
			UINT read=(UINT)Socket->Read(&pBuffer[uBufferSize], uMissingSize);
			uBufferSize+=read;
			uMissingSize=0;
			}
		SecBuffer msg[4];
		msg[0].pvBuffer=pBuffer;
		msg[0].cbBuffer=uBufferSize;
		msg[0].BufferType=SECBUFFER_DATA;
		msg[1].BufferType=SECBUFFER_EMPTY;
		msg[2].BufferType=SECBUFFER_EMPTY;
		msg[3].BufferType=SECBUFFER_EMPTY;
		SecBufferDesc msg_desc;
		msg_desc.ulVersion=SECBUFFER_VERSION;
		msg_desc.cBuffers=4;
		msg_desc.pBuffers=msg;
		HRESULT status=DecryptMessage(&hSecurityContext, &msg_desc, 0, nullptr);
		SecBuffer* src_buf=nullptr;
		SecBuffer* extra_buf=nullptr;
		for(INT i=0; i<4; i++)
			{
			if(msg[i].BufferType==SECBUFFER_DATA)
				src_buf=&msg[i];
			if(msg[i].BufferType==SECBUFFER_EXTRA)
				extra_buf=&msg[i];
			}
		if(extra_buf!=nullptr)
			{
			UINT buf_pos=uBufferSize-extra_buf->cbBuffer;
			MoveMemory(pBuffer, &pBuffer[buf_pos], uBufferSize-buf_pos);
			uBufferSize-=buf_pos;
			}
		else
			{
			uBufferSize=0;
			uMissingSize=TLS_BUF_SIZE;
			}
		if(src_buf!=nullptr)
			{
			UINT copy=src_buf->cbBuffer;
			CopyMemory(&pInput[uInputSize], src_buf->pvBuffer, copy);
			uInputSize+=copy;
			break;
			}
		ThrowIfFailed(status);
		}
	}
return read;
}


//===============
// Output-Stream
//===============

VOID TlsSocket::Flush()
{
if(uOutputSize==0)
	return;
SecBuffer msg[4];
msg[0].pvBuffer=pOutput;
msg[0].cbBuffer=cStreamSizes.cbHeader;
msg[0].BufferType=SECBUFFER_STREAM_HEADER;
msg[1].pvBuffer=&pOutput[cStreamSizes.cbHeader];
msg[1].cbBuffer=uOutputSize;
msg[1].BufferType=SECBUFFER_DATA;
msg[2].pvBuffer=&pOutput[cStreamSizes.cbHeader+uOutputSize];
msg[2].cbBuffer=cStreamSizes.cbTrailer;
msg[2].BufferType=SECBUFFER_STREAM_TRAILER;
msg[3].pvBuffer=nullptr;
msg[3].cbBuffer=0;
msg[3].BufferType=SECBUFFER_EMPTY;
SecBufferDesc msg_desc;
msg_desc.ulVersion=SECBUFFER_VERSION;
msg_desc.cBuffers=4;
msg_desc.pBuffers=msg;
HRESULT status=EncryptMessage(&hSecurityContext, 0, &msg_desc, 0);
ThrowIfFailed(status);
SIZE_T copy=msg[0].cbBuffer+msg[1].cbBuffer+msg[2].cbBuffer;
SIZE_T written=Socket->Write(pOutput, copy);
if(written!=copy)
	throw E_FAIL;
uOutputSize=0;
}

SIZE_T TlsSocket::Write(VOID const* srcv, SIZE_T size)
{
auto src=(BYTE const*)srcv;
SIZE_T pos=0;
while(pos<size)
	{
	UINT msg_max=cStreamSizes.cbMaximumMessage;
	if(uOutputSize==msg_max)
		Flush();
	UINT copy=Min((UINT)(size-pos), msg_max-uBufferSize);
	CopyMemory(&pOutput[cStreamSizes.cbHeader+uOutputSize], &src[pos], copy);
	uOutputSize+=copy;
	pos+=copy;
	}
return pos;
}


//================
// Common Private
//================

VOID TlsSocket::CloseInternal()
{
if(hCredential.dwLower||hCredential.dwUpper)
	{
	FreeCredentialsHandle(&hCredential);
	ZeroMemory(&hCredential, sizeof(CredHandle));
	}
if(hSecurityContext.dwLower||hSecurityContext.dwUpper)
	{
	DeleteSecurityContext(&hSecurityContext);
	ZeroMemory(&hSecurityContext, sizeof(CtxtHandle));
	}
ZeroMemory(&cStreamSizes, sizeof(SecPkgContext_StreamSizes));
uBufferSize=0;
uInputSize=0;
uMissingSize=TLS_BUF_SIZE;
}

}}
