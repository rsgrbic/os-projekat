//
// Created by os on 5/29/24.
//

#ifndef CONSOLE_H
#define CONSOLE_H
#include "BoundedBuffer.h"


class Console {
public:
    static void put_output(char c);
    static char get_output();
    static void put_input(char c);
    static char get_input();
    static bool OutputEmpty();
    static bool InputFull();
    static void fillInput();
    static void clearOutput();
    static Console& instance();
    Console(const Console&)=delete;
    Console& operator=(const Console&)=delete;
private:
    Console();
    BoundedBuffer *output,*input;//dva bafera od 512 karaktera


};



#endif //CONSOLE_H
