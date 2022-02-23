#ifndef ___CONSOLE_H__
#define ___CONSOLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "shared.h"
#include "communication_protocol/inc/tree_accessor.h"
#include "communication_protocol/inc/message_parser.h"
#include "common/stream.h"

#define CONSOLE_CMD_TEXT_BUFFER_MAX_SIZE 1024

#define CONSOLE_ERROR_TEXT_COMMAND_PARSE "command parse error."
#define CONSOLE_ERROR_TEXT_COMMAND_NOT_RECOGNIZE "command can not be recognized."
#define CONSOLE_ERROR_TEXT_PATH_ARGUMENT_NEEDED "path argument needed."
#define CONSOLE_ERROR_TEXT_VALUE_ARGUMENT_NEEDED "value argument needed."
#define CONSOLE_ERROR_TEXT_CHANGE_CONTEXT_FAILED "change context failed."
#define CONSOLE_ERROR_TEXT_VALUE_DISPLAY_ERROR "value display error."
#define CONSOLE_ERROR_TEXT_VALUE_GET_FAILED "value get failed."
#define CONSOLE_ERROR_TEXT_VALUE_SET_FAILED "value set failed."
    typedef struct Console
    {
        Stream *stream;
        TreeAccessor *tree;
        MessageParser *parser;
        MessageFrame _frame;
        char _cmdTextBuffer[CONSOLE_CMD_TEXT_BUFFER_MAX_SIZE];
    } Console;

    void console_create(Console *console, Stream *stream, TreeAccessor *tree, MessageParser *parser);

    void console_start(Console *console);

    void console_command_execute(Console *console);

#ifdef __cplusplus
}
#endif

#endif // ___CONSOLE_H__