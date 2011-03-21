/** 
\file       PerfCounter.cpp
\brief      реализация классов и методов используемых для подсчета производительности 
\author     Матвей Пашкин
\date       03.04.2005
\version    1.0
\std        1.0
*/

#include "stdafx.h"
#include "PerfCounter.h"
#include <windows.h>

#ifndef ASSERT
    #include <crtdbg.h>
    #define ASSERT(x)  _ASSERT(x)
#endif

#if defined(CTDK_PERF_COUNTER)

using namespace CtDK;

const int MS_IN_SECOND = 1000;
const int BUF_SIZE     = 160;

/// Хранилище результатов работы счетчиков
static CPerfStorage s_glPerfStorage;

/** Стандартная функция запускаемая в начале работы счетчика.
Используется для отсечки рекурсивных вызовов функций.
*/
static void stBeginCounter(const CPerfCounter &counter);
/** Стандартная функция запускаемая в конце работы счетчика.
Используется для суммирования времен вызовов функции
*/
static void stEndCounter(const CPerfCounter &counter);

///Вызывается в начале работы счетчика
static CounterFunc s_ptrBeginCounter = stBeginCounter;
///Вызывается при завершении работы счетчика
static CounterFunc s_ptrEndCounter   = stEndCounter;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPerfState::CPerfState():
    m_RunTime(0), m_RunCount(0)
{
    
}

CPerfState::CPerfState(const char *pszFunction):
    m_Function(pszFunction), m_RunTime(0), m_RunCount(0)
{
    
}

CPerfState::~CPerfState()
{
}

CPerfState::CPerfState(const CPerfState &src):
    m_Function(src.m_Function), m_RunTime(src.m_RunTime),
    m_RunCount(src.m_RunCount)
{

}

CPerfState & CPerfState::operator = (const CPerfState &src)
{
    m_Function = src.m_Function;
    m_RunTime  = src.m_RunTime;
    m_RunCount = src.m_RunCount;

    return *this;
}

void CPerfState::SetFunction(const char *pszFunction)
{
    m_Function = pszFunction;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPerfCounter::CPerfCounter():
    m_Begin(0), m_End(0),
    m_IsSend(false)
{

}

CPerfCounter::CPerfCounter(const char *pszFunction):
    m_Begin(0), m_End(0),
    m_IsSend(false), m_Function(pszFunction)
{

}

CPerfCounter::~CPerfCounter()
{
    Send();
}

CPerfCounter::CPerfCounter(const CPerfCounter &src):
    m_Begin(src.m_Begin), m_End(src.m_End),
    m_IsSend(true), m_Function(src.m_Function)
{
    // копия не посылает сообщения о подсчете
}

CPerfCounter & CPerfCounter::operator = (const CPerfCounter &src)
{
    m_Begin     = src.m_Begin;
    m_End       = src.m_End;
    m_IsSend    = true;     // копия не посылает сообщения о подсчете
    m_Function  = src.m_Function;

    return *this;
}

void CPerfCounter::SetFunction(const char *pszFunction)
{
    if(pszFunction && (strlen(pszFunction) > 0))
        m_Function = pszFunction;
}

void CPerfCounter::Start()
{
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_Begin));
    m_End = 0;
    if(s_ptrBeginCounter)
        s_ptrBeginCounter(*this);
}

void CPerfCounter::Stop()
{
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_End));
}

void CPerfCounter::Send()
{
    if(m_IsSend || !m_Begin)
        return;
    if(!m_End)
        Stop();
    m_IsSend = true;
    if(s_ptrEndCounter)
        s_ptrEndCounter(*this);
}

void CPerfCounter::Reset(const char *pszFunction)
{
    if(m_Begin != 0)
        Send();
    m_Begin     = 0;
    m_End       = 0;
    m_IsSend    = false;
    SetFunction(pszFunction);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPerfStorage::CPerfStorage()
{

}

CPerfStorage::~CPerfStorage()
{

}

void CPerfStorage::BeginCounter(const CPerfCounter &counter)
{
    CPerfCounter::Map::iterator pcounter = m_Stack.find(counter.GetFunction());
    if(pcounter != m_Stack.end())
    {
        if(!pcounter->second.GetBegin()) // функция уже запускалась, но завершила свою работу
        {
            // Обновляем значение счетчика в стеке
            pcounter->second = counter;
            return;
        }
        
        _ASSERT(pcounter->second.GetBegin() < counter.GetBegin());
        // Рекурсивный запуск функции, поэтому просто выходим, без обновления счетчика
        return;
    }
    m_Stack[counter.GetFunction()] = counter;
}

void CPerfStorage::EndCounter(const CPerfCounter &counter)
{
    CPerfCounter::Map::iterator pcounter = m_Stack.find(counter.GetFunction());
    _ASSERT(pcounter != m_Stack.end());
    if(pcounter != m_Stack.end())
    {
        _ASSERT(pcounter->second.GetBegin() <= counter.GetBegin());
        if(pcounter->second.GetBegin() < counter.GetBegin())
        {
            // функция была запущена рекурсивно
            return;
        }
        else
        {
            // Для ускорения работы, просто очищаем счетчик в стеке
            pcounter->second.Reset(NULL);
        }
    }

    CPerfState::Map::iterator pperfstate = m_Counters.find(counter.GetFunction());
    if(pperfstate != m_Counters.end())
    {
        pperfstate->second.Add(counter.GetRunTime());
        return;
    }
    CPerfState state_add(counter.GetFunction().c_str());
    state_add.Add(counter.GetRunTime());
    m_Counters[state_add.GetFunction()] = state_add;
}

__int64 CPerfStorage::GetCounterRunTimeMs(const char *pszFunction)
{
    __int64 freq = 0;
    ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq));

    if(!freq)
        return -1;

    std::string Function = pszFunction;
    CPerfState::Map::iterator pperfstate = m_Counters.find(Function);
    if(pperfstate != m_Counters.end())
        return MS_IN_SECOND * pperfstate->second.GetRunTime() / freq;
    return -1;
}

CPerfState CPerfStorage::GetCounterState(const char *pszFunction)
{
    std::string Function = pszFunction;
    CPerfState::Map::iterator pperfstate = m_Counters.find(Function);
    if(pperfstate != m_Counters.end())
        return pperfstate->second;
    return CPerfState();
}

bool CPerfStorage::ResetCounter(const char *pszFunction)
{
    std::string Function = pszFunction;
    CPerfState::Map::iterator pperfstate = m_Counters.find(Function);
    if(pperfstate == m_Counters.end())
        return false;
    m_Counters.erase(pperfstate);
    return true;
}

bool CPerfStorage::ResetAllCounters()
{
    m_Counters.erase(m_Counters.begin(), m_Counters.end());
    return true;
}

std::string CPerfStorage::PrintCounters(bool no_header)
{
    const char HEADER[] = "\n----------- Performance -----------\
\nFuctionName\ttime(ms)\tCount\n";
    const char FOOTER[] = "----------- /Performance ----------\n";
    const char ROW[] = "%s\t%I64d\t%I64d\n";

    std::string sRet;

    __int64 freq = 0;
    ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq));

    if(!freq)
        return std::string();

    if(!no_header)
    {
        sRet = HEADER;
        OutputDebugString(HEADER);
    }
    for(CPerfState::Map::iterator pi = m_Counters.begin();
                                                    pi != m_Counters.end(); pi ++)
    {
        __int64 ms = MS_IN_SECOND * pi->second.GetRunTime() / freq;

        char buf[BUF_SIZE] = {0};
        sprintf(buf, ROW, pi->second.GetFunction().c_str(), ms, pi->second.GetRunCount());
        sRet += buf;
        OutputDebugString(buf);
    }
    if(!no_header)
    {
        sRet += FOOTER;
        OutputDebugString(FOOTER);
    }
    return sRet;
}

__int64 CtDK::glGetCounterRunTimeMs(const char *pszFunction)
{
    return s_glPerfStorage.GetCounterRunTimeMs(pszFunction);
}

CPerfState CtDK::glGetCounterState(const char *pszFunction)
{
    return s_glPerfStorage.GetCounterState(pszFunction);
}

bool CtDK::glResetCounter(const char *pszFunction)
{
    return s_glPerfStorage.ResetCounter(pszFunction);
}

bool CtDK::glResetAllCounters()
{
    return s_glPerfStorage.ResetAllCounters();
}

std::string CtDK::glPrintCounters(bool no_header)
{
    return s_glPerfStorage.PrintCounters(no_header);
}

void stBeginCounter(const CPerfCounter &counter)
{
    s_glPerfStorage.BeginCounter(counter);
}

void stEndCounter(const CPerfCounter &counter)
{
    s_glPerfStorage.EndCounter(counter);
}

bool CtDK::SetBeginCounterFunc(CounterFunc pFunc)
{
    if(!pFunc)
    {
        s_ptrBeginCounter = stBeginCounter;
        return true;
    }
    s_ptrBeginCounter = pFunc;
    return true;
}

bool CtDK::SetEndCounterFunc(CounterFunc pFunc)
{
    if(!pFunc)
    {
        s_ptrEndCounter = stEndCounter;
        return true;
    }
    s_ptrEndCounter = pFunc;
    return true;
}



#endif //CTDK_PERF_COUNTER