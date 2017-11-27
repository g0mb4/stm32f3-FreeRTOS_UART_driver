/**
  * @file	syscall.h
  * @brief	low-level UNIX function prototypes
  *
  * The GCC C library makes calls to the following functions to perform low-level I/O.
  * These functions are implemented witihin the GCC C library as stub routines with "weak" linkage.
  * If a declaration of any of the above functions appears in your own code, your substitute routine will
  * override the declaration in the library and be used instead of the default (non-functional) routine.
  *
  * @author	gmb
  * @date	2017
  */

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include <drivers/uart.h>

int _getpid(void);
int _kill(int pid, int sig);
void _exit(int status);

int _open(char *path, int flags);
ssize_t _read(int file, char *data, ssize_t len);
ssize_t _write(int file, char *data, ssize_t len);
int _close(int file);

int _lseek(int file, int ptr, int dir);
int _fstat(int file, struct stat *st);
int _isatty(int file);

int _wait(int *status);
int _times(struct tms *buf);

int _link(char *old, char *new);
int _unlink(char *name);

int _fork(void);
int _execve(char *name, char **argv, char **env);

#endif
