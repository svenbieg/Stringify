//=======
// Log.h
//=======

#pragma once


//=======
// Using
//=======

#include "Collections/List.h"
#include "Storage/Streams/InputStream.h"
#include "Storage/Streams/OutputStream.h"
#include "LogEntry.h"


//===========
// Namespace
//===========

namespace Collections {


//=====
// Log
//=====

class Log: public Object
{
public:
	// Using
	using EntryList=List<Handle<LogEntry>>;
	using InputStream=Storage::Streams::InputStream;
	using OutputStream=Storage::Streams::OutputStream;
	using TimePoint=Physics::TimePoint;

	// Con-/Destructors
	Log();

	// Common
	VOID Clear();
	Handle<EntryList> Entries;
	SIZE_T ReadFromStream(InputStream* Stream);
	VOID Write(Handle<Sentence> Message) { Write(nullptr, Message); }
	VOID Write(Handle<TimePoint> TimePoint, Handle<Sentence> Message);
	SIZE_T WriteToStream(OutputStream* Stream);
	Event<Log> Changed;

private:
	// Common
	VOID OnTimePointChanged();
};

}