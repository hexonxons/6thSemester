#pragma once
const char alphabet[] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZàáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß -_1234567890"};


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