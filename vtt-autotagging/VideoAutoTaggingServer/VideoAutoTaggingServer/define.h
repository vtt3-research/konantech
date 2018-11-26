#pragma once
#include <string>
#include <mutex>

using namespace std;

extern std::mutex g_mutexLog;
extern std::mutex g_mutexProgress;

extern FILE* g_fpDecLog;

#define LOG0(fmt)						{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt)), fflush(g_fpDecLog); } }
#define LOG1(fmt,a1)					{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt),a1), fflush(g_fpDecLog); } }
#define LOG2(fmt,a1,a2)				{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt),a1,a2), fflush(g_fpDecLog); } }
#define LOG3(fmt,a1,a2,a3)				{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt),a1,a2,a3), fflush(g_fpDecLog); } }
#define LOG4(fmt,a1,a2,a3,a4)			{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt),a1,a2,a3,a4), fflush(g_fpDecLog); } }
#define LOG5(fmt,a1,a2,a3,a4,a5)		{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt),a1,a2,a3,a4,a5), fflush(g_fpDecLog); } }
#define LOG6(fmt,a1,a2,a3,a4,a5,a6)		{ if (g_fpDecLog) { _ftprintf(g_fpDecLog,_T(fmt),a1,a2,a3,a4,a5,a6), fflush(g_fpDecLog); } }

