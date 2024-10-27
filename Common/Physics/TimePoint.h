//=============
// TimePoint.h
//=============

#pragma once


//=======
// Using
//=======

#include "Culture/Language.h"
#include "TimeSpan.h"


//======================
// Forward-Declarations
//======================

namespace Devices
{
class Clock;
}


//===========
// Namespace
//===========

namespace Physics {


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

inline BOOL operator==(TIMEPOINT const& Value1, TIMEPOINT const& Value2)
{
return CompareMemory(&Value1, &Value2, sizeof(TIMEPOINT))==0;
}

inline BOOL operator!=(TIMEPOINT const& Value1, TIMEPOINT const& Value2)
{
return CompareMemory(&Value1, &Value2, sizeof(TIMEPOINT))!=0;
}

VOID TimePointFromTickCount(TIMEPOINT* TimePoint, UINT64 TickCount);
UINT64 TimePointToTickCount(TIMEPOINT const& TimePoint);


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
	using Clock=Devices::Clock;
	using LanguageCode=Culture::LanguageCode;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Friends
	friend class Devices::Clock;

	// Con-/Destructors
	TimePoint();
	TimePoint(TIMEPOINT const& TimePoint);
	TimePoint(Handle<String> Name);
	TimePoint(Handle<String> Name, TIMEPOINT const& TimePoint);

	// Access
	static UINT GetDayOfWeek(LPCSTR String);
	static UINT GetMonth(LPCSTR String);
	BOOL IsAbsolute();
	UINT64 ToSeconds();
	static UINT64 ToSeconds(TIMEPOINT const& TimePoint);
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override;
	Handle<String> ToString(TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static Handle<String> ToString(TIMEPOINT const& TimePoint, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static UINT ToString(TIMEPOINT const& TimePoint, LPSTR Buffer, UINT Size, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Comparison
	BOOL operator==(TIMEPOINT const& TimePoint);

	// Modification
	VOID Clear(BOOL Notify=true);
	static BOOL FromTimeStamp(TIMEPOINT* TimePoint, LPCSTR TimeStamp);
	VOID Set(TIMEPOINT const& TimePoint, BOOL Notify=true)override;

private:
	// Common
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
class Handle<Physics::TimePoint>
{
private:
	// Using
	using TIMEPOINT=Physics::TIMEPOINT;
	using TimePoint=Physics::TimePoint;

public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): m_Object(nullptr) {}
	Handle(TimePoint* TimePoint) { HandleCreate(&m_Object, TimePoint); }
	Handle(Handle<TimePoint> const& Handle) { HandleCreate(&m_Object, Handle.m_Object); }
	Handle(Handle<TimePoint>&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator TIMEPOINT()const { return VariableGet<TimePoint, TIMEPOINT>(m_Object, { 0, 0, 0, 0, 0, 0, 0 }); }
	operator TimePoint*()const { return m_Object; }
	TimePoint* operator->()const { return m_Object; }

	// Comparison
	bool operator==(TIMEPOINT const& Value) { return VariableEqual(m_Object, Value); }
	bool operator!=(TIMEPOINT const& Value) { return !VariableEqual(m_Object, Value); }

	// Assignment
	Handle<TimePoint>& operator=(decltype(nullptr)) { HandleClear(&m_Object); return *this; }
	Handle<TimePoint>& operator=(TIMEPOINT const& Value) { VariableAssign(&m_Object, Value); return *this; }
	Handle& operator=(TimePoint* Value) { HandleAssign(&m_Object, Value); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&m_Object, Handle.m_Object); return *this; }

private:
	// Common
	TimePoint* m_Object;
};
