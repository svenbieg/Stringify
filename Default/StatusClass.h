//===============
// StatusClass.h
//===============

#pragma once


//========
// Status
//========

enum class Status: UINT
{
AccessDenied,
Error,
FileExists,
FileNotFound,
InvalidArgument,
Success
};


//========
// Common
//========

inline bool Succeeded(Status Status)
{
if(Status==Status::Success)
	return true;
return false;
}

inline bool Failed(Status Status)
{
if(Status==Status::Success)
	return false;
return true;
}
