#ifndef _PIPE_H
#define _PIPE_H
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
typedef int ssize_t;
#else
#include <sys/types.h>
#endif


//========================================================
//  type declarations
//========================================================
typedef struct pipe_s pipe_t;
typedef pipe_t *pipe_handle_t;



//========================================================
//  API declarations
//========================================================
pipe_handle_t pipe_create();


void pipe_destroy(pipe_handle_t pipe);


int pipe_open(pipe_handle_t pipe);


void pipe_close(pipe_handle_t pipe);


ssize_t pipe_read(pipe_handle_t pipe, void *buffer, size_t buf_size);


ssize_t pipe_write(pipe_handle_t pipe, void *data, size_t data_size);



#ifdef __cplusplus
}
#endif
#endif
