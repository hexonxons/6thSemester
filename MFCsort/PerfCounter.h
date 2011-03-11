/**
\file       PerfCounter.h
\brief      Определение классов и функций для измерения производительности
\author     Пашкин Матвей
\date       03.04.2005
\version    1.0
\std        1.0

Для включения замеров времени следует в настройках проекта определить константу CTDK_PERF_COUNTER
("Proect"->"Settings"->"C\C++"->"Preprocessor definitions".

Счетчики используются для замеров времени и количества вызовов функций и
участков кода. Например:

void TestCounter(int level = 0)
{
    START_PERF_COUNTER("TestCounter")
    if(level > 3)
        return;
    for(int i = 0; i < 1; i++)
        TestCounter(level + 1);
}

Номерные (именованные) счетчики используются для замеров времени работы
частей функции. Например:

int ParseFile(LPCTSTR pSrcFile, CStringArray *pWords)
{
    START_PERF_COUNTER("ParseFile") // Подсчет общего вермени работы функции ParseFile
    DECLARE_PERF_COUNTER_N(0, "ParseFile::Read") // Подсчет времени чтения из файла
    DECLARE_PERF_COUNTER_N(1, "ParseFile::ParseString") // Подстчет времени разбора строки

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

Результаты подстчета времени и числа вызовой получаются с помощью макроса PRINT_PERF_COUNTERS

\history
*/

#if !defined(CTDK_PERFCOUNTER_H__2005_04_03_INCLUDED_)
#define CTDK_PERFCOUNTER_H__2005_04_03_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef  CTDK_PERF_COUNTER

    // Выключаем предупреждение № 4786
    #pragma warning(disable : 4786)

    #include <map>
    #include <string>

    namespace CtDK
    {

    /** \class Хранит глобальное состояние счетчика
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

        /// Прибавляет значение к счетчику времени и увеличивает счетчик запусков
        inline void        Add(__int64 RunTime)
        {
            m_RunTime += RunTime;
            m_RunCount++;
        }


    protected:
        /// Имя, уникальный идетификатор функции
        std::string m_Function;
        /// Общее время выполнения функции (время между стартом и остановкой счетчика)
        __int64     m_RunTime;
        /// Общее число запусков функции (счетчика)
        __int64     m_RunCount;
    };

    /** \class Используется для подсчета времени работы функции

    Характеризуется использованием функции QueryPerformanceCounter,
    для определения времени работы с точностью до одного такта процессора.
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

        /// Засечь начальное время
        void Start();
        /// Засечь конечное время
        void Stop();
        /** Послать информацию со счетчика в CPerfStorage. Данная функция вызывается в деструкторе.
        Если на момент ее вызова счетчик не был остановлен, то счетчик останавливается.
        */
        void Send();
        /** Сбросить значения счетчика на 0. Если pszFunction пустая переменная,
        то у счетчика остается старое имя функции
        */
        void Reset(const char *pszFunction);

        inline __int64 GetBegin()   const {  return m_Begin;            }
        inline __int64 GetEnd()     const {  return m_End;              }
        inline __int64 GetRunTime() const {  return m_End - m_Begin;    }

        inline std::string GetFunction() const  {   return m_Function;  }
        /// Если pszFunction пустая переменная, то у счетчика остается старое имя функции
        void               SetFunction(const char *pszFunction);

    protected:
        /// Начало отсчета
        __int64             m_Begin;
        /// Конец отсчета
        __int64             m_End;
        /// Была ли выполнена посылка информации счетчика
        bool                m_IsSend;
        /// Имя, уникальный идетификатор функции
        std::string         m_Function;

    };

    /** \class Хранилище информации счетчиков
    */
    class CPerfStorage  
    {
    public:
        CPerfStorage();
        virtual ~CPerfStorage();

        /** Вызавается в начале работы счетчика. Используется для корректной обработки
        рекурсивных функций.
        */
        void BeginCounter(const CPerfCounter &counter);
        /** Вызывается счетчиком по завершении работы счетчика.
        Обновляет глобальную информацию о результатах работы счетчика.
        */
        void EndCounter(const CPerfCounter &counter);

        /// Получить время работы счетчика в миллисекундах
        __int64 GetCounterRunTimeMs(const char *pszFunction);
        /// Получить полное состояние счетчика
        CPerfState GetCounterState(const char *pszFunction);

        /// Перезапустить счетчик
        bool ResetCounter(const char *pszFunction);
        /// Перезапустить все счетчики
        bool ResetAllCounters();

        /// Получение текстовой таблички о состоянии счетчиков
        std::string PrintCounters(bool no_header = false);

    protected:
        /// Хранилище счетчиков
        CPerfState::Map   m_Counters;
        /// Хранилище начального состояния работающих счетчиков
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

    /// Определение формата функции вызываемой при начале и конце работы счетчика
    typedef void (*CounterFunc)(const CPerfCounter &counter);

    /// Получить время работы счетчика в миллисекундах
    __int64 glGetCounterRunTimeMs (const char *pszFunction);
    /// Получить полное состояние счетчика
    CPerfState glGetCounterState(const char *pszFunction);

    /// Перезапустить счетчик
    bool glResetCounter(const char *pszFunction);
    /// Перезапустить все счетчики
    bool glResetAllCounters();

    /// Получение текстовой таблички о состоянии счетчиков
    std::string glPrintCounters(bool no_header = false);
    /// Установить функцию вызываемую в начале работы счетчика
    bool        SetBeginCounterFunc(CounterFunc pFunc = NULL);
    /// Установить функцию вызываемую в конце работы счетчика
    bool        SetEndCounterFunc(CounterFunc pFunc = NULL);

    }; // namespace CtDK


    /** Начать замер времени работы функции с именем (глобальный ндентификатор) functionname.
    functionname - указывается обязательно в кавычках и должно быть уникальным.
    */
    #define START_PERF_COUNTER(functionname)                    \
        CtDK::CPerfCounter CtDK_Perf_Counter(functionname);     \
        CtDK_Perf_Counter.Start();

    /** Заверщить замер времени.
    Данный макрос не обязателен для использования, замер времени автоматически завершается
    при выходе из функции.
    */
    #define STOP_PERF_COUNTER                                   \
        CtDK_Perf_Counter.Send();


    /** Начать замер времени с возвожностью в последующем
    перезапустить счетчик с новым именем (NEXT_SECTION_PERF_COUNTER).
    Предназначен для разбиения какой-либо функции или блока на отдельные части
    с получением отчета по каждой из них.
    */
    #define SECTION_PERF_COUNTER(functionname)                  \
        CtDK::CPerfCounter CtDK_Perf_CounterS(functionname);    \
        CtDK_Perf_CounterS.Start();


    /// Перезапустить счетчик с другим именем функции
    #define NEXT_SECTION_PERF_COUNTER(functionname)             \
        CtDK_Perf_CounterS.Reset(functionname);                 \
        CtDK_Perf_CounterS.Start();



    /** Объявить номерной (именованный) счетчик.
    number - любой идентификатор по правилам C++
    functionname - указывается обязательно в кавычках и должно быть уникальным.
    */
    #define DECLARE_PERF_COUNTER_N(number, functionname)                \
        CtDK::CPerfCounter CtDK_Perf_Counter_##number(functionname);

    /// Начать замер времени с помощью номерного (именованного) счетчика
    #define START_PERF_COUNTER_N(number)                                \
        CtDK_Perf_Counter_##number.Reset(NULL);                         \
        CtDK_Perf_Counter_##number.Start();

    /** Завершить замер времени с помощью номерного (именоманного) счетчика.
    Данный макрос не обязателен для использования, замер времени автоматически завершается
    при выходе из функции.
    */
    #define STOP_PERF_COUNTER_N(number)                                 \
        CtDK_Perf_Counter_##number.Send();

    /** Перезапустить счетчик.
    functionname - имя счетчика, указывается обязательно в кавычках
    */
    #define RESET_PERF_COUNTER(functionname)                            \
        CtDK::glResetCounter(functionname);

    /** Перезапустить все счетчики  */
    #define RESET_ALL_PERF_COUNTERS                                     \
        CtDK::glResetAllCounters();

    /** Получтить табличку состояния счетчиков.
        Данный макрос должен использоваться как std::string()

        Например:
            printf("%s", PRINT_PERF_COUNTERS(false).c_str());
    */

    #define PRINT_PERF_COUNTERS(isTitle)                                \
        CtDK::glPrintCounters(isTitle)

#else // defined(CTDK_PERF_COUNTER)

    #include <string>
    // Определение макросов замера времени для условной компиляции без замеров времени 

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
