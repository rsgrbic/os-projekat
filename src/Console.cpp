//
// Created by os on 5/29/24.
//

#include "../h/Console.h"

Console::Console()
{
    input=new BoundedBuffer;
    output=new BoundedBuffer;
}
Console& Console::instance()
{
    static Console konzola;
    return konzola;
}

char Console::get_input()
{
    return instance().input->getItem();
}

char Console::get_output()
{
    return instance().output->getItem();
}

void Console::put_input(char c)
{
    instance().input->putItem(c);
}

void Console::put_output(char c)
{
    instance().output->putItem(c);
}

bool Console::OutputEmpty()
{
    return instance().output->numAvailable==0;
}

bool Console::InputFull()
{
    return instance().output->numAvailable==BoundedBuffer::maxSize;
}

void Console::clearOutput()
{
    char volatile cstatus=*(char*)CONSOLE_STATUS;
    while(!OutputEmpty()&& (cstatus & CONSOLE_TX_STATUS_BIT))
    {
        char volatile char_output=get_output();
        *(uint64*)CONSOLE_TX_DATA=char_output;
        cstatus=*(char*)CONSOLE_STATUS;

    }

}


void Console::fillInput()
{
    char volatile cstatus=*(char*)CONSOLE_STATUS;
    while(!InputFull()&& (cstatus & CONSOLE_RX_STATUS_BIT))
    {
        char volatile char_input =*(char*)CONSOLE_RX_DATA;
        put_input(char_input);
        cstatus=*(char*)CONSOLE_STATUS;
    }
}







