/**
 * @file main.c
 * @author Kamyar Hatamnezhad (TheOddZer0@protonmail.com)
 * @brief The (portable) driver code for algo.c
 * @copyright Copyright (c) 2022 Kamyar Hatamnezhad
 */
/*
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
#include "algo.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintUsage()
{
    puts("Usage: lsb-stego encode HOLDER_FILE MESSAGE_FILE");
    puts("   Or: lsb-stego decode INPUT_FILE SIZE");
    puts("Result will be written to standard output");
}

long GetFileSize(char *filename)
{
    FILE *f;
    long file_size;
    f = fopen(filename, "rb");
    if (f == NULL)
    {
        return -1;
    }
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fclose(f);
    return file_size;
}

char *ReadAll(char *filename, long file_size)
{
    char *result = malloc(file_size);
    if (result == NULL)
    {
        return NULL;
    }
    FILE *f;
    f = fopen(filename, "rb");
    if (f == NULL)
    {
        free(result);
        return NULL;
    }
    fread(result, 1, file_size, f);
    return result;
}

int main(int argc, char **argv)
{
    /*
    Boring command line options
    WARNING: You are about to see A LOT of if-statements. Why? error handling is
    a thing
    */
    if (argc < 2)
    {
        PrintUsage();
        return 1;
    }
    if (strcmp(argv[1], "encode") == 0)
    {
        if (argc != 4)
        {
            PrintUsage();
            return 1;
        }
        /* argv[2] is the holder file, argv[3] is the message file */
        long holder_size = GetFileSize(argv[2]);
        if (holder_size < 0)
        {
            printf("Failed to get size of '%s'\n", argv[2]);
            return 1;
        }
        long message_size = GetFileSize(argv[3]);
        if (message_size < 0)
        {
            printf("Failed to get size of '%s'\n", argv[3]);
            return 1;
        }
        if (message_size * 8 > holder_size)
        {
            printf("'%s' Cannot hold content of '%s'. %li bytes is needed\n",
                   argv[2], argv[3], message_size * 8);
            return 1;
        }
        if (holder_size == 0 || message_size == 0)
        {
            return 1;
        }
        char *holder_data = ReadAll(argv[2], holder_size);
        if (holder_data == NULL)
        {
            printf("Failed to read the content of '%s'\n", argv[2]);
            return 1;
        }
        char *message_data = ReadAll(argv[3], message_size);
        if (holder_data == NULL)
        {
            printf("Failed to read the content of '%s'\n", argv[3]);
            return 1;
        }
        /* Now the real encoding */
        LsbEncode(holder_data, message_data, message_size);
        fwrite(holder_data, 1, holder_size, stdout);
        /* Let's not forget cleaning up! */
        free(holder_data);
        free(message_data);
        return 0;
    }
    else if (strcmp(argv[1], "decode") == 0)
    {
        if (argc != 4)
        {
            PrintUsage();
            return 1;
        }
        /* argv[2] is the input file, argv[3] is the size of the encoded message
         */
        long input_size = GetFileSize(argv[2]);
        if (input_size < 0)
        {
            printf("Failed to get size of '%s'\n", argv[2]);
            return 1;
        }
        long output_size = strtol(argv[3], NULL, 10);
        if (output_size <= 0)
        {
            puts("Message size should be a positive non-zero number");
            return 1;
        }
        /* Does that make any sense? */
        if (output_size * 8 > input_size)
        {
            /* No, it doesn't */
            puts("Input file cannot possibly hold the required amount of "
                 "bytes, wrong size passed?");
            return 1;
        }
        char *output_data = malloc(output_size);
        if (output_data == NULL)
        {
            puts("Failed to allocate enough memory for holding the output");
            return 1;
        }
        /* Yes, it does */
        char *input_data = ReadAll(argv[2], input_size);
        if (input_data == NULL)
        {
            printf("Failed to read the content of '%s'\n", argv[2]);
            return 1;
        }
        LsbDecode(input_data, output_data, output_size);
        fwrite(output_data, 1, output_size, stdout);
        return 0;
    }
    else
    {
        PrintUsage();
        return 1;
    }
}
