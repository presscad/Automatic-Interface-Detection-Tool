#include <Windows.h>
#include "pipe.h"


struct pipe_s {
    HANDLE read_handle;
    HANDLE write_handle;
};


pipe_handle_t pipe_create()
{
    return malloc(sizeof(pipe_t));
}


void pipe_destroy(pipe_handle_t pipe)
{
    free(pipe);
}


int pipe_open(pipe_handle_t pipe)
{
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (CreatePipe(&pipe->read_handle, &pipe->write_handle, &sa, 0) == FALSE)
        return -1;

    return 0;
}


void pipe_close(pipe_handle_t pipe)
{
    CloseHandle(pipe->write_handle);
    pipe->write_handle = NULL;
    CloseHandle(pipe->read_handle);
    pipe->read_handle = NULL;
}


ssize_t pipe_read(pipe_handle_t pipe, void *buffer, size_t buf_size)
{
    ssize_t read_size;

    if (ReadFile(pipe->read_handle, buffer, buf_size, &read_size, NULL) == FALSE)
        return -1;

    return read_size;
}


ssize_t pipe_write(pipe_handle_t pipe, void *data, size_t data_size)
{
    ssize_t write_size;

    if (WriteFile(pipe->write_handle, data, data_size, &write_size, NULL) == FALSE)
        return -1;

    return write_size;
}
