#ifndef __LAH_H__
#define __LAB_H__

#include <ctime>
#include <fstream>

#include "../includes/banned.h"
#include "../includes/memalloc.h"
#include "../MFCAVLHash/avltree.h"
#include "../MFCHash/hashTable.h"
#include "array.h"

using std::ios;

#ifdef _DEBUG
    #include <crtdbg.h>
    #define new new(__FILE__,__LINE__)
#endif

template <class T> class CTester
{
public:
    // функция сравнения
    typedef int (*Compare)(T *pElem1, T *pElem2);
    // функция вычисления хэша по строке
    typedef int (*CalcHash)(T *pElem);
    // функция генерации строк
    typedef T (*Gen)(int length);

    CTester(CalcHash calcHashFunction, Compare compareFunction, Gen genFunction)
        :   m_table(1000000, calcHashFunction, compareFunction)
        ,   m_tree(compareFunction)
        ,   m_array(compareFunction)
    {
        m_calcHashFunction = calcHashFunction;
        m_compareFunction = compareFunction;
        m_generator = genFunction;
        m_success = 0;
        m_numOfTests = 0;
        m_file.open("results", ios::out | ios::app);
        m_file << "\n\n||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\n";
    }

    virtual ~CTester()
    {
        m_file.close();
    }

    // функция заполнения хранилища случайными данними
    int fillStruct(unsigned int numOfTests)
    {
        testDestroy();
        unsigned int i;
        // таймеры
        clock_t start;
        clock_t finish;

        m_numOfTests = numOfTests;
        m_array.Init(numOfTests);

        start = clock();
        for(i = 0; i < m_numOfTests; ++i)
            m_storage.Add(m_generator(7 + rand() % 13));
        finish = clock();
        m_storageTime = (double)(finish - start) / CLOCKS_PER_SEC;
        // вывод в файл
        m_file << "Fill struct: \nNumber of tests: " << numOfTests << "\nTime :" << m_storageTime << "\n"; 
        return 0;
    }

    int testInsert()
    {
        // таймеры
        clock_t start;
        clock_t finish;

        start = clock();
        for(CBasicDataBase<CString>::Iterator it = m_storage.Begin(); it.isValid(); it.Next())
        {
            m_tree.AddElem(it.Current());
        }
        finish = clock();
        m_avlTime = (double)(finish - start) / CLOCKS_PER_SEC;

        start = clock();
        for(CBasicDataBase<CString>::Iterator it = m_storage.Begin(); it.isValid(); it.Next())
        {
            m_table.AddElem(it.Current());
        }
        finish = clock();
        m_hashTime = (double)(finish - start) / CLOCKS_PER_SEC;

        start = clock();
        for(CBasicDataBase<CString>::Iterator it = m_storage.Begin(); it.isValid(); it.Next())
        {
            m_array.AddElem(it.Current());
        }
        finish = clock();
        m_arrayTime = (double)(finish - start) / CLOCKS_PER_SEC;

        // вывод в файл
        m_file << "Test insert: \nAvl insert time: " << m_avlTime << "\nHash insert time: " << m_hashTime << "\nCollisions number: " << m_table.getCollisionCount() << "\nArray insert time: " << m_arrayTime << "\n"; 
        return 0;
    }

    // ищет в  таблице  данные
    int testFind()
    {
        // таймеры
        clock_t start;
        clock_t finish;
        // счетчик удачных попыток
        int success = 0;
        m_success = 0;

        start = clock();
        for(CBasicDataBase<CString>::Iterator it = m_storage.Begin(); it.isValid(); it.Next())
        {
            if(m_tree.FindElem(it.Current()) != NULL)
                ++success;
        }
        finish = clock();
        m_avlTime = (double)(finish - start) / CLOCKS_PER_SEC;
        m_success = success;

        success = 0;
        start = clock();
        for(CBasicDataBase<CString>::Iterator it = m_storage.Begin(); it.isValid(); it.Next())
        {
            if(m_table.FindElem(it.Current()) != NULL)
                ++success;
        }
        finish = clock();
        m_hashTime = (double)(finish - start) / CLOCKS_PER_SEC;

        // если все работает правильно, не должно срабатывать
        if(m_success != success)
        {
            OutputDebugString("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nError find rand obj\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            m_file << "Error find rand obj m_success: " << m_success << " success: " << success << "\n";
        }

        success = 0;
        start = clock();
        for(CBasicDataBase<CString>::Iterator it = m_storage.Begin(); it.isValid(); it.Next())
        {
            if(m_array.FindElem(it.Current()) != NULL)
                ++success;
        }
        finish = clock();
        m_arrayTime = (double)(finish - start) / CLOCKS_PER_SEC;

        // если все работает правильно, не должно срабатывать
        if(m_success != success)
        {
            OutputDebugString("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nError find rand obj\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            m_file << "Error find rand obj m_success: " << m_success << " success: " << success << "\n";
        }
        // вывод в файл
        m_file << "Test find: \nAvl find time: " << m_avlTime << "\nHash find time: " << m_hashTime << "\nArray find time: " << m_arrayTime << "\n"; 
        return 0;
    }

    int testRandomFind()
    {
        unsigned int i;
        // хранилище сгенерированных случайных объектов
        CBasicDataBase<CString> randomObjStorage;
        // таймеры
        clock_t start;
        clock_t finish;
        // счетчик удачных попыток
        int success = 0;

        m_success = 0;
        if (m_numOfTests == 0)
            return 0;

        // считаем время заполнения 2*N объектами
        start = clock();
        for(i = 0; i < m_numOfTests * 2; ++i)
            randomObjStorage.Add(m_generator(7 + rand() % 13));
        finish = clock();
        m_storageTime = (double)(finish - start) / CLOCKS_PER_SEC;
        
        // заполняем avl-дерево
        start = clock();
        for(CBasicDataBase<CString>::Iterator it = randomObjStorage.Begin(); it.isValid(); it.Next())
        {
            if(m_tree.FindElem(it.Current()) != NULL)
                ++success;
        }
        finish = clock();
        m_avlTime = (double)(finish - start) / CLOCKS_PER_SEC;
        m_success = success;

        success = 0;
        // заполняем хэш-таблицу
        start = clock();
        for(CBasicDataBase<CString>::Iterator it = randomObjStorage.Begin(); it.isValid(); it.Next())
        {
            if(m_table.FindElem(it.Current()) != NULL)
                ++success;
        }
        finish = clock();
        m_hashTime = (double)(finish - start) / CLOCKS_PER_SEC;

        // если все работает правильно, не должно срабатывать
        if(m_success != success)
        {
            OutputDebugString("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nError find rand obj\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            m_file << "Error find rand obj m_success: " << m_success << " success: " << success << "\n";
        }

        success = 0;
        start = clock();
        for(CBasicDataBase<CString>::Iterator it = randomObjStorage.Begin(); it.isValid(); it.Next())
        {
            if(m_array.FindElem(it.Current()) != NULL)
                ++success;
        }
        finish = clock();
        m_arrayTime = (double)(finish - start) / CLOCKS_PER_SEC;

        // если все работает правильно, не должно срабатывать
        if(m_success != success)
        {
            OutputDebugString("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nError find rand obj\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            m_file << "Error find rand obj m_success: " << m_success << " success: " << success << "\n";
        }

        // вывод в файл
        m_file << "Test random insert: \nAvl find random time: " << m_avlTime << "\nHash find random time :" << m_hashTime << "\nArray find random time" << m_arrayTime << "\nSucces find: " << m_success << "\n"; 
        return 0;
    }

    int testDestroy()
    {
        // таймеры
        clock_t start;
        clock_t finish;

        start = clock();
        m_tree.destroy();
        finish = clock();
        m_avlTime = (double)(finish - start) / CLOCKS_PER_SEC;
   
        start = clock();
        m_table.deleteTable();
        finish = clock();
        m_hashTime = (double)(finish - start) / CLOCKS_PER_SEC;

        start = clock();
        m_storage.destroy();
        finish = clock();
        m_storageTime = (double)(finish - start) / CLOCKS_PER_SEC;

        start = clock();
        m_array.destroy();
        finish = clock();
        m_arrayTime = (double)(finish - start) / CLOCKS_PER_SEC;

        // вывод в файл
        m_file << "Test destroy: \nAvl destroy time: " << m_avlTime << "\nHash destroy time: " << m_hashTime << "\nStorage destroy time: " << m_storageTime << "\nArray destroy time: " << m_arrayTime << "\n"; 
        m_numOfTests = 0;
        m_success = 0;
        return 0;
    }

    // функция возвращения работы таста для хэша
    double getAvlTime()
    {
        return m_avlTime;
    }

    // функция возвращения работы таста для avl-дерева
    double getHashTime()
    {
        return m_hashTime;
    }

    // функция возвращения времени заполнения хранилища
    double getStorageTime()
    {
        return m_storageTime;
    }

    // функция возвращения времени заполнения массива
    double getArrayTime()
    {
        return m_arrayTime;
    }
    
    // функция возвращения количества найденных значений
    int getSuccess()
    {
        return m_success;
    }
    
    // функция возвращения числа коллизий
    unsigned int getCollisionCount()
    {
        return m_table.getCollisionCount();
    }

private:
    // количество тестов
    unsigned int m_numOfTests;
    // класс avl-дерева
    CAvlTree<T> m_tree;
    // класс хэш-таблицы
    CHashTable<T> m_table;
    // класс массива
    CArr<T> m_array;
    // класс хранилища
    CBasicDataBase<T> m_storage;
    // функция вычислени хэша
    CalcHash m_calcHashFunction;
    // функция сравнения структур
    Compare m_compareFunction;
    // функция генератора произвольных объектов
    Gen m_generator;
    // время работы для avl-дерева
    double m_avlTime;
    // время работы для хэша
    double m_hashTime;
    // время работы функций хранилища
    double m_storageTime;
    // время работы массива
    double m_arrayTime;
    // количество найденних значений при поиске произвольных значений
    int m_success;
    // файл для записи
    std::ofstream m_file;
};

#ifdef _DEBUG
    #undef new
#endif

#endif  // __LAB_H__
