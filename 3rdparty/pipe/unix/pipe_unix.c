#include <unistd.h>
#include <stdlib.h>
#include "pipe.h"


struct pipe_s {
    int fds[2];
};


pipe_handle_t pipe_create()
{
    return malloc(sizeof(pipe_t));
}


void pipe_destroy(pipe_handle_t pipe_handle)
{
    if (pipe_handle)
        free(pipe_handle);
}


int pipe_open(pipe_handle_t pipe_handle)
{
    return pipe_handle ? pipe(pipe_handle->fds) : -1;
}


void pipe_close(pipe_handle_t pipe_handle)
{
    if (pipe_handle) {
        close(pipe_handle->fds[1]);
        close(pipe_handle->fds[0]);
    }
}


ssize_t pipe_read(pipe_handle_t pipe_handle, void *buffer, size_t buf_size)
{
    fd_set fds;
    struct timeval timeout = { 3, 0 };
    ssize_t rc = -1;

    if (pipe_handle) {
        FD_ZERO(&fds);
        FD_SET(pipe_handle->fds[0], &fds);
        if (select(pipe_handle->fds[0] + 1, &fds, NULL, NULL, &timeout) > 0) {
            if(FD_ISSET(pipe_handle->fds[0], &fds)) {
                rc = read(pipe_handle->fds[0], buffer, buf_size);
            }
        }
    }

    return rc;
}


ssize_t pipe_write(pipe_handle_t pipe_handle, void *data, size_t data_size)
{
    return pipe_handle ? write(pipe_handle->fds[1], data, data_size) : -1;
}
