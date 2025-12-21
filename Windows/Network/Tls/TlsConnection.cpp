//===================
// TlsConnection.cpp
//===================

#include "Network/Tls/TlsConnection.h"


//=======
// Using
//=======

#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Secur32.lib")

#include <schannel.h>
#include "ErrorHelper.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//========
// Common
//========

VOID TlsConnection::Close()
{
ShutdownTls();
if(m_Socket==INVALID_SOCKET)
	return;
shutdown(m_Socket, SD_BOTH);
closesocket(m_Socket);
m_RemoteAddress=0;
m_Socket=INVALID_SOCKET;
}

VOID TlsConnection::Handshake(Handle<String> host_name)
{
HCERTSTORE store=CertOpenSystemStoreA(NULL, "My");
PCCERT_CONTEXT context=CertFindCertificateInStore(store, X509_ASN_ENCODING, 0, CERT_FIND_SUBJECT_STR_A, host_name->Begin(), nullptr);
CertCloseStore(store, 0);
if(!context)
	throw InvalidArgumentException();
m_Context=context;
TCHAR package[MAX_PATH];
StringHelper::Copy(package, MAX_PATH, SCHANNEL_NAME);
SCHANNEL_CRED cred={ 0 };
cred.dwVersion=SCHANNEL_CRED_VERSION;
cred.paCred=&m_Context;
cred.cCreds=1;
HRESULT status=AcquireCredentialsHandle(nullptr, package, SECPKG_CRED_INBOUND, nullptr, &cred, nullptr, nullptr, &m_Credential, nullptr);
ErrorHelper::ThrowIfFailed(status);
CtxtHandle* context_in=nullptr;
CtxtHandle* context_out=&m_SecurityContext;
m_MissingSize=TLS_BUF_SIZE;
while(1)
	{
	if(m_MissingSize>0)
		{
		INT read=recv(m_Socket, &m_Buffer[m_BufferSize], m_MissingSize, 0);
		if(read<=0)
			throw DeviceNotReadyException();
		m_BufferSize+=read;
		m_MissingSize=0;
		}
	SecBuffer in_buf[2];
	in_buf[0].pvBuffer=m_Buffer;
	in_buf[0].cbBuffer=m_BufferSize;
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
	status=AcceptSecurityContext(&m_Credential, context_in, &in_buf_desc, flags, SECURITY_NATIVE_DREP, context_out, &out_buf_desc, &flags_out, nullptr);
	ErrorHelper::ThrowIfFailed(status);
	context_in=&m_SecurityContext;
	context_out=nullptr;
	if(out_buf[0].cbBuffer&&out_buf[0].pvBuffer)
		{
		INT written=send(m_Socket, (LPSTR)out_buf[0].pvBuffer, out_buf[0].cbBuffer, 0);
		if(written<=0)
			throw DeviceNotReadyException();
		FreeContextBuffer(out_buf[0].pvBuffer);
		}
	m_BufferSize=0;
	m_MissingSize=TLS_BUF_SIZE;
	if(status==SEC_E_OK)
		break;
	}
status=QueryContextAttributes(&m_SecurityContext, SECPKG_ATTR_STREAM_SIZES, &m_StreamSizes);
ErrorHelper::ThrowIfFailed(status);
/*SCHANNEL_CRED cred={ 0 };
cred.dwVersion=SCH_CREDENTIALS_VERSION;
HRESULT status=AcquireCredentialsHandle(nullptr, SCHANNEL_NAME, SECPKG_CRED_OUTBOUND, nullptr, &cred, nullptr, nullptr, &m_Credential, nullptr);
ThrowIfFailed(status);
CtxtHandle* context_in=nullptr;
CtxtHandle* context_out=&m_SecurityContext;
SecBuffer in_buf;
in_buf.pvBuffer=m_Buffer;
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
	if(m_MissingSize>0)
		{
		Sleep(10);
		INT read=recv(m_Socket, &m_Buffer[m_BufferSize], m_MissingSize, 0);
		if(read<=0)
			throw DeviceNotReadyException();
		m_BufferSize+=read;
		m_MissingSize=0;
		}
	SecBufferDesc* in_bufs=nullptr;
	if(m_BufferSize>0)
		{
		in_buf.cbBuffer=m_BufferSize;
		in_bufs=&in_buf_desc;
		}
	out_buf.pvBuffer=nullptr;
	out_buf.cbBuffer=0;
	out_buf.BufferType=SECBUFFER_EMPTY;
	DWORD flags=ISC_REQ_ALLOCATE_MEMORY|ISC_REQ_REPLAY_DETECT|ISC_REQ_CONFIDENTIALITY|ISC_RET_EXTENDED_ERROR|ISC_REQ_MANUAL_CRED_VALIDATION;
	DWORD flags_out=0;
	status=InitializeSecurityContext(&m_Credential, context_in, nullptr, flags, 0, SECURITY_NATIVE_DREP, in_bufs, 0, context_out, &out_buf_desc, &flags_out, nullptr);
	ThrowIfFailed(status);
	context_in=&m_SecurityContext;
	context_out=nullptr;
	if(out_buf.cbBuffer&&out_buf.pvBuffer)
		{
		INT written=send(m_Socket, (LPSTR)out_buf.pvBuffer, out_buf.cbBuffer, 0);
		if(written<=0)
			throw DeviceNotReadyException();
		FreeContextBuffer(out_buf.pvBuffer);
		}
	m_BufferSize=0;
	m_MissingSize=TLS_BUF_SIZE;
	if(status==SEC_E_OK)
		break;
	}
status=QueryContextAttributes(&m_SecurityContext, SECPKG_ATTR_STREAM_SIZES, &m_StreamSizes);
ThrowIfFailed(status);*/
}


//==============
// Input-Stream
//==============

SIZE_T TlsConnection::Available()
{
if(!m_Context)
	{
	u_long available=0;
	if(ioctlsocket(m_Socket, FIONREAD, &available)<0)
		available=0;
	return available;
	}
return m_BufferSize;
}

SIZE_T TlsConnection::Read(VOID* dst, SIZE_T size)
{
if(!m_Context)
	{
	INT read=recv(m_Socket, (LPSTR)dst, (INT)size, 0);
	if(read<=0)
		throw DeviceNotReadyException();
	return read;
	}
SIZE_T read=0;
while(1)
	{
	if(m_InputSize>0)
		{
		read=TypeHelper::Min(size, m_InputSize);
		MemoryHelper::Copy(dst, m_Input, read);
		if(read<m_InputSize)
			MemoryHelper::Move(m_Input, &m_Input[read], m_InputSize-read);
		m_InputSize-=read;
		break;
		}
	while(1)
		{
		if(m_MissingSize>0)
			{
			INT read=recv(m_Socket, &m_Buffer[m_BufferSize], m_MissingSize, 0);
			m_BufferSize+=read;
			m_MissingSize=0;
			}
		SecBuffer msg[4];
		msg[0].pvBuffer=m_Buffer;
		msg[0].cbBuffer=m_BufferSize;
		msg[0].BufferType=SECBUFFER_DATA;
		msg[1].BufferType=SECBUFFER_EMPTY;
		msg[2].BufferType=SECBUFFER_EMPTY;
		msg[3].BufferType=SECBUFFER_EMPTY;
		SecBufferDesc msg_desc;
		msg_desc.ulVersion=SECBUFFER_VERSION;
		msg_desc.cBuffers=4;
		msg_desc.pBuffers=msg;
		HRESULT status=DecryptMessage(&m_SecurityContext, &msg_desc, 0, nullptr);
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
			UINT buf_pos=m_BufferSize-extra_buf->cbBuffer;
			MemoryHelper::Move(m_Buffer, &m_Buffer[buf_pos], m_BufferSize-buf_pos);
			m_BufferSize-=buf_pos;
			}
		else
			{
			m_BufferSize=0;
			m_MissingSize=TLS_BUF_SIZE;
			}
		if(src_buf!=nullptr)
			{
			UINT copy=src_buf->cbBuffer;
			MemoryHelper::Copy(&m_Input[m_InputSize], src_buf->pvBuffer, copy);
			m_InputSize+=copy;
			break;
			}
		ErrorHelper::ThrowIfFailed(status);
		}
	}
return read;
}


//===============
// Output-Stream
//===============

VOID TlsConnection::Flush()
{
if(m_OutputSize==0)
	return;
SecBuffer msg[4];
msg[0].pvBuffer=m_Output;
msg[0].cbBuffer=m_StreamSizes.cbHeader;
msg[0].BufferType=SECBUFFER_STREAM_HEADER;
msg[1].pvBuffer=&m_Output[m_StreamSizes.cbHeader];
msg[1].cbBuffer=m_OutputSize;
msg[1].BufferType=SECBUFFER_DATA;
msg[2].pvBuffer=&m_Output[m_StreamSizes.cbHeader+m_OutputSize];
msg[2].cbBuffer=m_StreamSizes.cbTrailer;
msg[2].BufferType=SECBUFFER_STREAM_TRAILER;
msg[3].pvBuffer=nullptr;
msg[3].cbBuffer=0;
msg[3].BufferType=SECBUFFER_EMPTY;
SecBufferDesc msg_desc;
msg_desc.ulVersion=SECBUFFER_VERSION;
msg_desc.cBuffers=4;
msg_desc.pBuffers=msg;
HRESULT status=EncryptMessage(&m_SecurityContext, 0, &msg_desc, 0);
ErrorHelper::ThrowIfFailed(status);
INT copy=msg[0].cbBuffer+msg[1].cbBuffer+msg[2].cbBuffer;
INT written=send(m_Socket, m_Output, copy, 0);
if(written!=copy)
	throw DeviceNotReadyException();
m_OutputSize=0;
}

SIZE_T TlsConnection::Write(VOID const* srcv, SIZE_T size)
{
auto src=(BYTE const*)srcv;
if(!m_Context)
	{
	INT written=send(m_Socket, (LPCSTR)src, (INT)size, 0);
	if(written<=0)
		throw DeviceNotReadyException();
	return written;
	}
SIZE_T pos=0;
while(pos<size)
	{
	UINT msg_max=m_StreamSizes.cbMaximumMessage;
	if(m_OutputSize==msg_max)
		Flush();
	UINT copy=TypeHelper::Min((UINT)(size-pos), msg_max-m_BufferSize);
	MemoryHelper::Copy(&m_Output[m_StreamSizes.cbHeader+m_OutputSize], &src[pos], copy);
	m_OutputSize+=copy;
	pos+=copy;
	}
return pos;
}


//==========================
// Con-/Destructors Private
//==========================

TlsConnection::TlsConnection(SOCKET sock, IP_ADDR remote_ip):
m_Buffer(),
m_BufferSize(0),
m_Context(nullptr),
m_Credential({ 0, 0 }),
m_Input(),
m_InputSize(0),
m_MissingSize(0),
m_Output(),
m_OutputSize(0),
m_RemoteAddress(remote_ip),
m_SecurityContext({ 0, 0 }),
m_Socket(sock),
m_StreamSizes({ 0 })
{}


//================
// Common Private
//================

VOID TlsConnection::ShutdownTls()
{
if(!m_Context)
	return;
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
	HRESULT status=ApplyControlToken(&m_SecurityContext, &buf_desc);
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
	status=AcceptSecurityContext(&m_Credential, &m_SecurityContext, nullptr, flags, SECURITY_NATIVE_DREP, nullptr, &buf_desc, &flags_out, nullptr);
	if(FAILED(status))
		break;
	if(buf.cbBuffer!=0&&buf.pvBuffer)
		{
		send(m_Socket, (LPCSTR)buf.pvBuffer, buf.cbBuffer, 0);
		FreeContextBuffer(buf.pvBuffer);
		break;
		}
	}
if(m_Credential.dwLower||m_Credential.dwUpper)
	{
	FreeCredentialsHandle(&m_Credential);
	m_Credential={ 0 };
	}
if(m_SecurityContext.dwLower||m_SecurityContext.dwUpper)
	{
	DeleteSecurityContext(&m_SecurityContext);
	m_SecurityContext={ 0 };
	}
m_StreamSizes={ 0 };
m_BufferSize=0;
m_InputSize=0;
m_MissingSize=TLS_BUF_SIZE;
}

}}