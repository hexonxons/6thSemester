#pragma once
const char alphabet[] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ�������������������������������������Ũ�������������������������� -_1234567890"};


CString generate(int length)
{
    int i;
    CString arr;
    for (i = 0; i < length; ++i)
    {
        int index = rand() % 131;
        arr += alphabet[index];
    }
    return arr;
}