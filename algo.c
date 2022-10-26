/**
 * @file algo.c
 * @author Kamyar Hatamnezhad (TheOddZer0@protonmail.com)
 * @brief The LSB steganography algorithm in c (it's freestanding)
 * @copyright Copyright (c) 2022 Kamyar Hatamnezhad
 */
/*
Copyright 2021 TheOddZer0

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef NULL
#define NULL ((void *)0)
#endif

long lsb_encode(void *input, void *payload, long size)
{
    unsigned char *cinput = (unsigned char *)input;
    unsigned char *cpayload = (unsigned char *)payload;
    /* Index in the input bytes */
    long i;
    if (input == NULL || payload == NULL || size == 0)
    {
        return 0;
    }
    for (i = 0; i < size * 8; i++)
    {
        /* If that specific bit in payload is set */
        if (cpayload[i / 8] & ((unsigned char)1 << (i % 8)))
        {
            /*
            We check the payload bit by bit, if it is set,
            then we set the last bit of the current byte in the input to 1
            if not, then we clear it (set it to 0)
            */
            cinput[i] |= (unsigned char)1 << 7;
        }
        else
        {
            cinput[i] |= ~((unsigned char)1 << 7);
        }
    }
    return i + 1;
}

long LsbDecode(void *input, void *output, long size)
{
    unsigned char *cinput = (unsigned char *)input;
    unsigned char *coutput = (unsigned char *)output;
    /* Index in the input bytes */
    long i;
    /* Sanity checks */
    if (size == 0 || input == NULL || output == NULL || (size % 8) != 0)
    {
        return 0;
    }
    for (i = 0; i < size; i++)
    {
        /* If the last bit is set */
        if (cinput[i / 8] & ((unsigned char)1 << 7))
        {
            /*
            Here might need some explanation
            First of all, both bit access and array access are zero-based.
            So we first get the index in the input and
            divide it by 8, to get which byte of the output should we access.
            Then we use modulo to get which bit should we access.
            */
            coutput[i / 8] |= (unsigned char)1 << (i % 8);
        }
        else
        {
            coutput[i / 8] |= ~((unsigned char)1 << (i % 8));
        }
    }
    return i + 1;
}
