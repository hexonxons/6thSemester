/**
\file       PerfCounter.h
\brief      ����������� ������� � ������� ��� ��������� ������������������
\author     ������ ������
\date       03.04.2005
\version    1.0
\std        1.0

��� ��������� ������� ������� ������� � ���������� ������� ���������� ��������� CTDK_PERF_COUNTER
("Proect"->"Settings"->"C\C++"->"Preprocessor definitions".

�������� ������������ ��� ������� ������� � ���������� ������� ������� �
�������� ����. ��������:

void TestCounter(int level = 0)
{
    START_PERF_COUNTER("TestCounter")
    if(level > 3)
        return;
    for(int i = 0; i < 1; i++)
        TestCounter(level + 1);
}

�������� (�����������) �������� ������������ ��� ������� ������� ������
������ �������. ��������:

int ParseFile(LPCTSTR pSrcFile, CStringArray *pWords)
{
    START_PERF_COUNTER("ParseFile") // ������� ������ ������� ������ ������� ParseFile
    DECLARE_PERF_COUNTER_N(0, "ParseFile::Read") // ������� ������� ������ �� �����
    DECLARE_PERF_COUNTER_N(1, "ParseFile::ParseString") // �������� ������� ������� ������

    const int BUF_SIZE = 100000;
    char  Buf[BUF_SIZE + 1] = {0};
    FILE *pfSrc = NULL;
    pfSrc = fopen(pSrcFile, "rb");
    if(!pfSrc)
        return 0;

    CString cs;
    START_PERF_COUNTER_N(0)
    long nRead = fread(Buf, 1, BUF_SIZE, pfSrc);
    STOP_PERF_COUNTER_N(0)
    cs = Buf;
    while (nRead > 0)
    {
        cs.MakeLower();
        
        START_PERF_COUNTER_N(1)
        cs = CMorfeFunc::ParseString(cs, pWords);
        STOP_PERF_COUNTER_N(1)

        memset(Buf, 0, BUF_SIZE + 1);
        
        START_PERF_COUNTER_N(0)
        nRead = fread(Buf, 1, BUF_SIZE, pfSrc);
        STOP_PERF_COUNTER_N(0)
        
        cs+= CString(Buf, nRead);
    }
    START_PERF_COUNTER_N(1)
    cs = CMorfeFunc::ParseString(cs, pWords);
    STOP_PERF_COUNTER_N(1)
        
    fclose(pfSrc);
    return 1;
}

���������� ��������� ������� � ����� ������� ���������� � ������� ������� PRINT_PERF_COUNTERS

\history
*/

#if !defined(CTDK_PERFCOUNTER_H__2005_04_03_INCLUDED_)
#define CTDK_PERFCOUNTER_H__2005_04_03_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef  CTDK_PERF_COUNTER

    // ��������� �������������� � 4786
    #pragma warning(disable : 4786)

    #include <map>
    #include <string>

    namespace CtDK
    {

    /** \class ������ ���������� ��������� ��������
    */
    class CPerfState  
    {
    public:
        typedef std::map<std::string, CPerfState> Map;
    public:
        CPerfState();
        CPerfState(const char *pszFunction);
        virtual ~CPerfState();
        CPerfState(const CPerfState &src);
        CPerfState & operator = (const CPerfState &src);


        inline std::string GetFunction() const  {   return m_Function;  }
        void               SetFunction(const char *pszFunction);
        inline __int64     GetRunTime()  const  {  return m_RunTime;    }
        inline __int64     GetRunCount()  const {  return m_RunCount;    }

        /// ���������� �������� � �������� ������� � ����������� ������� ��������
        inline void        Add(__int64 RunTime)
        {
            m_RunTime += RunTime;
            m_RunCount++;
        }


    protected:
        /// ���, ���������� ������������ �������
        std::string m_Function;
        /// ����� ����� ���������� ������� (����� ����� ������� � ���������� ��������)
        __int64     m_RunTime;
        /// ����� ����� �������� ������� (��������)
        __int64     m_RunCount;
    };

    /** \class ������������ ��� �������� ������� ������ �������

    ��������������� �������������� ������� QueryPerformanceCounter,
    ��� ����������� ������� ������ � ��������� �� ������ ����� ����������.
    */
    class CPerfCounter  
    {
    public:
        typedef std::map<std::string, CPerfCounter> Map;
    public:
        CPerfCounter();
        CPerfCounter(const char *pszFunction);
        virtual ~CPerfCounter();

        CPerfCounter(const CPerfCounter &);
        CPerfCounter & operator = (const CPerfCounter &);

    public:

        /// ������ ��������� �����
        void Start();
        /// ������ �������� �����
        void Stop();
        /** ������� ���������� �� �������� � CPerfStorage. ������ ������� ���������� � �����������.
        ���� �� ������ �� ������ ������� �� ��� ����������, �� ������� ���������������.
        */
        void Send();
        /** �������� �������� �������� �� 0. ���� pszFunction ������ ����������,
        �� � �������� �������� ������ ��� �������
        */
        void Reset(const char *pszFunction);

        inline __int64 GetBegin()   const {  return m_Begin;            }
        inline __int64 GetEnd()     const {  return m_End;              }
        inline __int64 GetRunTime() const {  return m_End - m_Begin;    }

        inline std::string GetFunction() const  {   return m_Function;  }
        /// ���� pszFunction ������ ����������, �� � �������� �������� ������ ��� �������
        void               SetFunction(const char *pszFunction);

    protected:
        /// ������ �������
        __int64             m_Begin;
        /// ����� �������
        __int64             m_End;
        /// ���� �� ��������� ������� ���������� ��������
        bool                m_IsSend;
        /// ���, ���������� ������������ �������
        std::string         m_Function;

    };

    /** \class ��������� ���������� ���������
    */
    class CPerfStorage  
    {
    public:
        CPerfStorage();
        virtual ~CPerfStorage();

        /** ���������� � ������ ������ ��������. ������������ ��� ���������� ���������
        ����������� �������.
        */
        void BeginCounter(const CPerfCounter &counter);
        /** ���������� ��������� �� ���������� ������ ��������.
        ��������� ���������� ���������� � ����������� ������ ��������.
        */
        void EndCounter(const CPerfCounter &counter);

        /// �������� ����� ������ �������� � �������������
        __int64 GetCounterRunTimeMs(const char *pszFunction);
        /// �������� ������ ��������� ��������
        CPerfState GetCounterState(const char *pszFunction);

        /// ������������� �������
        bool ResetCounter(const char *pszFunction);
        /// ������������� ��� ��������
        bool ResetAllCounters();

        /// ��������� ��������� �������� � ��������� ���������
        std::string PrintCounters(bool no_header = false);

    protected:
        /// ��������� ���������
        CPerfState::Map   m_Counters;
        /// ��������� ���������� ��������� ���������� ���������
        CPerfCounter::Map m_Stack;      

    private:
        CPerfStorage(const CPerfStorage &)
        {
            //ASSERT(0);
        }
        CPerfStorage & operator = (const CPerfStorage &)
        {
            //ASSERT(0);
            return *this;
        }
    };

    /// ����������� ������� ������� ���������� ��� ������ � ����� ������ ��������
    typedef void (*CounterFunc)(const CPerfCounter &counter);

    /// �������� ����� ������ �������� � �������������
    __int64 glGetCounterRunTimeMs (const char *pszFunction);
    /// �������� ������ ��������� ��������
    CPerfState glGetCounterState(const char *pszFunction);

    /// ������������� �������
    bool glResetCounter(const char *pszFunction);
    /// ������������� ��� ��������
    bool glResetAllCounters();

    /// ��������� ��������� �������� � ��������� ���������
    std::string glPrintCounters(bool no_header = false);
    /// ���������� ������� ���������� � ������ ������ ��������
    bool        SetBeginCounterFunc(CounterFunc pFunc = NULL);
    /// ���������� ������� ���������� � ����� ������ ��������
    bool        SetEndCounterFunc(CounterFunc pFunc = NULL);

    }; // namespace CtDK


    /** ������ ����� ������� ������ ������� � ������ (���������� �������������) functionname.
    functionname - ����������� ����������� � �������� � ������ ���� ����������.
    */
    #define START_PERF_COUNTER(functionname)                    \
        CtDK::CPerfCounter CtDK_Perf_Counter(functionname);     \
        CtDK_Perf_Counter.Start();

    /** ��������� ����� �������.
    ������ ������ �� ���������� ��� �������������, ����� ������� ������������� �����������
    ��� ������ �� �������.
    */
    #define STOP_PERF_COUNTER                                   \
        CtDK_Perf_Counter.Send();


    /** ������ ����� ������� � ������������ � �����������
    ������������� ������� � ����� ������ (NEXT_SECTION_PERF_COUNTER).
    ������������ ��� ��������� �����-���� ������� ��� ����� �� ��������� �����
    � ���������� ������ �� ������ �� ���.
    */
    #define SECTION_PERF_COUNTER(functionname)                  \
        CtDK::CPerfCounter CtDK_Perf_CounterS(functionname);    \
        CtDK_Perf_CounterS.Start();


    /// ������������� ������� � ������ ������ �������
    #define NEXT_SECTION_PERF_COUNTER(functionname)             \
        CtDK_Perf_CounterS.Reset(functionname);                 \
        CtDK_Perf_CounterS.Start();



    /** �������� �������� (�����������) �������.
    number - ����� ������������� �� �������� C++
    functionname - ����������� ����������� � �������� � ������ ���� ����������.
    */
    #define DECLARE_PERF_COUNTER_N(number, functionname)                \
        CtDK::CPerfCounter CtDK_Perf_Counter_##number(functionname);

    /// ������ ����� ������� � ������� ��������� (������������) ��������
    #define START_PERF_COUNTER_N(number)                                \
        CtDK_Perf_Counter_##number.Reset(NULL);                         \
        CtDK_Perf_Counter_##number.Start();

    /** ��������� ����� ������� � ������� ��������� (������������) ��������.
    ������ ������ �� ���������� ��� �������������, ����� ������� ������������� �����������
    ��� ������ �� �������.
    */
    #define STOP_PERF_COUNTER_N(number)                                 \
        CtDK_Perf_Counter_##number.Send();

    /** ������������� �������.
    functionname - ��� ��������, ����������� ����������� � ��������
    */
    #define RESET_PERF_COUNTER(functionname)                            \
        CtDK::glResetCounter(functionname);

    /** ������������� ��� ��������  */
    #define RESET_ALL_PERF_COUNTERS                                     \
        CtDK::glResetAllCounters();

    /** ��������� �������� ��������� ���������.
        ������ ������ ������ �������������� ��� std::string()

        ��������:
            printf("%s", PRINT_PERF_COUNTERS(false).c_str());
    */

    #define PRINT_PERF_COUNTERS(isTitle)                                \
        CtDK::glPrintCounters(isTitle)

#else // defined(CTDK_PERF_COUNTER)

    #include <string>
    // ����������� �������� ������ ������� ��� �������� ���������� ��� ������� ������� 

    #define START_PERF_COUNTER(functionname)
    #define STOP_PERF_COUNTER
    #define SECTION_PERF_COUNTER(functionname)
    #define NEXT_SECTION_PERF_COUNTER(functionname)
    #define DECLARE_PERF_COUNTER_N(number, functionname)
    #define START_PERF_COUNTER_N(number)
    #define STOP_PERF_COUNTER_N(number)
    #define RESET_PERF_COUNTER(functionname)
    #define RESET_ALL_PERF_COUNTERS
    #define PRINT_PERF_COUNTERS(isTitle)                   std::string()
#endif

#endif // !defined(CTDK_PERFCOUNTER_H__2005_04_03_INCLUDED_)
