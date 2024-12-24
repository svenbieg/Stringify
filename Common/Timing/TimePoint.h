//=============
// TimePoint.h
//=============

#pragma once


//=======
// Using
//=======

#include "Culture/Language.h"
#include "TimeSpan.h"


//===========
// Namespace
//===========

namespace Timing {


//======================
// Forward-Declarations
//======================

class Clock;


//========
// Struct
//========

struct TIMEPOINT
{
BYTE Second;
BYTE Minute;
BYTE Hour;
BYTE DayOfWeek;
BYTE DayOfMonth;
BYTE Month;
WORD Year;
};

inline bool operator==(TIMEPOINT const& TimePoint1, TIMEPOINT const& TimePoint2)
{
return MemoryHelper::Compare(&TimePoint1, &TimePoint2, sizeof(TIMEPOINT))==0;
}


//=============
// Time-Format
//=============

enum class TimeFormat
{
Full,
DateTime,
Time
};


//============
// Time-Point
//============

class TimePoint: public TypedVariable<TIMEPOINT>
{
private:
	// Using
	using Language=Culture::Language;
	using LanguageCode=Culture::LanguageCode;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	TimePoint();
	TimePoint(TIMEPOINT const& TimePoint);
	TimePoint(Handle<String> Name);
	TimePoint(Handle<String> Name, TIMEPOINT const& TimePoint);

	// Access
	static UINT GetDayOfWeek(LPCSTR String);
	static UINT GetDayOfYear(TIMEPOINT const& TimePoint);
	static UINT GetMonth(LPCSTR String);
	BOOL IsAbsolute();
	UINT64 ToSeconds();
	static UINT64 ToSeconds(TIMEPOINT const& TimePoint);
	Handle<String> ToString(LanguageCode Language=Language::Current)override;
	Handle<String> ToString(TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static Handle<String> ToString(TIMEPOINT const& TimePoint, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static UINT ToString(TIMEPOINT const& TimePoint, LPSTR Buffer, UINT Size, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Comparison
	BOOL operator==(TIMEPOINT const& TimePoint);

	// Modification
	VOID Clear(BOOL Notify=true);
	static VOID FromSeconds(TIMEPOINT* TimePoint, UINT64 Seconds);
	static BOOL FromTimeStamp(TIMEPOINT* TimePoint, LPCSTR TimeStamp);
	VOID Set(TIMEPOINT const& TimePoint, BOOL Notify=true)override;

private:
	// Common
	UINT64 GetTickCount(TIMEPOINT const& TimePoint);
	VOID OnClockSecond(Clock* Clock);
	static UINT ToStringDateTime(TIMEPOINT const& TimePoint, LPSTR Buffer, UINT Size, LanguageCode Language);
	static UINT ToStringFull(TIMEPOINT const& TimePoint, LPSTR Buffer, UINT Size, LanguageCode Language);
	static UINT ToStringRelative(UINT64 TickCount, LPSTR Buffer, UINT Size, TimeFormat Format, LanguageCode Language);
	static UINT ToStringTime(TIMEPOINT const& TimePoint, LPSTR Buffer, UINT Size, LanguageCode Language);
	VOID UpdateTimer();
};

}


//===================
// Handle Time-Point
//===================

template <>
class Handle<Timing::TimePoint>: public Details::VariableHandle<Timing::TimePoint, Timing::TIMEPOINT>
{
public:
	// Using
	using _base_t=Details::VariableHandle<Timing::TimePoint, Timing::TIMEPOINT>;
	using _base_t::_base_t;
	using TIMEPOINT=Timing::TIMEPOINT;

	// Modification
	Handle& operator=(TIMEPOINT const& Value) { Set(Value); return *this; }
};
