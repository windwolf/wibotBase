#include "../inc/console.h"
#include "string.h"

static void _console_display(Console *console, char *msg, size_t msgSize);
static char *_console_next_token(const char *cmdText, char **remain);
static void _console_display_prompt(Console *console);
static void _console_receive_worker(Console *console);

static void _console_display_prompt(Console *console)
{
    char *path = tree_accessor_context_path_get(console->tree);
    stream_send(console->stream, path, strlen(path));
    stream_send(console->stream, ">", 1);
}

static void _console_display_line(Console *console, char *msg)
{
    stream_send(console->stream, msg, strlen(msg));
    stream_send(console->stream, "\r\n", 2);
}

static char *_console_next_token(const char *cmdText, char **remain)
{
    if (cmdText == NULL)
    {
        *remain = NULL;
        return NULL;
    }
    char *token = strtrim(cmdText, ' ');
    if (*token == 0x00)
    {
        *remain = token;
        return NULL;
    }
    char *tokenEnd = strchr(token, ' ');
    if (tokenEnd != NULL)
    {
        *remain = tokenEnd;
        return token;
    }
    else
    {
        *remain = token + strlen(token);
        return token;
    }
}

void console_create(Console *console, Stream *stream, TreeAccessor *tree, MessageParser *parser)
{
    console->stream = stream;
    console->tree = tree;
    console->parser = parser;
}
void console_command_execute(Console *console)
{
    TreeAccessor *tree = console->tree;
    OP_RESULT msgResult;
    OP_RESULT rst;
    while (rst == OP_RESULT_OK)
    {
        msgResult = message_parser_frame_get(console->parser, NULL, &console->_frame);
        if (msgResult != OP_RESULT_OK)
        {
            continue;
        }

        size_t len = console->_frame.contentLength;
        assert(len < CONSOLE_CMD_TEXT_BUFFER_MAX_SIZE);

        rst = message_parser_frame_content_extract(&console->_frame, console->_cmdTextBuffer);
        if (rst != OP_RESULT_OK)
        {
            _console_display_line(console, CONSOLE_ERROR_TEXT_COMMAND_PARSE);
        }

        char *cmdText = console->_cmdTextBuffer;
        cmdText[len] = 0x00;

        char *token = _console_next_token(cmdText, &cmdText);
        if (token == NULL)
        {
            _console_display_line(console, CONSOLE_ERROR_TEXT_COMMAND_PARSE);
            continue;
        }

        if (strncasecmp(token, "li", token))
        {
            char *path = _console_next_token(cmdText, &cmdText);
            if (path == NULL)
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_PATH_ARGUMENT_NEEDED);
                continue;
            }
            char **children = tree_accessor_item_list(tree, path);
            while (*children != NULL)
            {
                stream_send(console->stream, *children, strlen(*children));
                stream_send(console->stream, '\t', 1);
            }
            stream_send(console->stream, "\r\n", 2);
            _console_display_prompt(console);
        }
        else if (strncasecmp(token, "cd", token))
        {
            char *path = _console_next_token(cmdText, &cmdText);
            if (path == NULL)
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_PATH_ARGUMENT_NEEDED);
                continue;
            }
            if (!tree_accessor_context_change(tree, path))
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_CHANGE_CONTEXT_FAILED);
                continue;
            }
            _console_display_prompt(console);
        }
        else if (strncasecmp(token, "get", token))
        {
            char *path = _console_next_token(cmdText, &cmdText);
            if (path == NULL)
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_PATH_ARGUMENT_NEEDED);
                continue;
            }
            char *value;
            if (tree_accessor_value_get(console, path, &value))
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_VALUE_DISPLAY_ERROR);
                continue;
            }
            _console_display_line(console, value);
            _console_display_prompt(console);
        }
        else if (strncasecmp(token, "set", token))
        {
            char *path = _console_next_token(cmdText, &cmdText);
            if (path == NULL)
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_PATH_ARGUMENT_NEEDED);
                continue;
            }
            char *value = _console_next_token(cmdText, &cmdText);
            if (value == NULL)
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_VALUE_SET_FAILED);
                continue;
            }
            if (tree_accessor_value_set(console, path, value))
            {
                _console_display_line(console, CONSOLE_ERROR_TEXT_VALUE_DISPLAY_ERROR);
                continue;
            }
            _console_display_prompt(console);
        }
        else
        {
            _console_display_line(console, CONSOLE_ERROR_TEXT_COMMAND_NOT_RECOGNIZE);
            continue;
        }
    }
};

void console_start(Console *console)
{
    stream_server_start(console->stream);
    while (1)
    {
        stream_receive_ready_wait(console->stream, TX_WAIT_FOREVER);
        console_command_execute(console);
    }
}
