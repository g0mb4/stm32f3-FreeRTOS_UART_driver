/**
  * @file	syscall.c
  * @brief	low-level UNIX function definitions
  *
  * The GCC C library makes calls to the following functions to perform low-level I/O.
  * These functions are implemented witihin the GCC C library as stub routines with "weak" linkage.
  * If a declaration of any of the above functions appears in your own code, your substitute routine will
  * override the declaration in the library and be used instead of the default (non-functional) routine.
  *
  * @author	gmb
  * @date	2017
  */

#include <syscalls.h>

/**
 * get process identification
 * @return  the process ID (PID) of the calling process
 */
int _getpid(void){
	return 1;
}

/**
 * send a signal to a process or a group of processes
 * @param  pid the process ID
 * @param  sig signal
 * @return     0 on succes, otherwise -1
 */
int _kill(int pid, int sig){
	errno = EINVAL;
	return -1;
}

/**
 * cause normal process termination
 * @param status * EXIT_SUCCESS
 * 				 * EXIT_FAILURE
 */
void _exit(int status){
	_kill(status, -1);
	for(;;);
}

/**
 * open and possibly create a file
 * @param  path    he file specified by pathname
 * @param  flags   flags must include one of the following access modes: O_RDONLY, O_WRONLY, or O_RDWR.
 * @return         the new file descriptor, or -1 if an error occurred
 */
int _open(char *path, int flags){
	return -1;
}

/**
 * read from a file descriptor
 * @param  file file descriptor
 * @param  data data
 * @param  len  data length
 * @return      number of read bytes
 */
ssize_t _read(int file, char *data, ssize_t len){
	if ((file == STDIN_FILENO)){
		return UART_read(file, data, len);
	} else {
		errno = EBADF;
		return -1;
	}
}

/**
 * write to a file descriptor
 * @param  file file descriptor
 * @param  data data
 * @param  len  data length
 * @return      number of written bytes
 */
ssize_t _write(int file, char *data, ssize_t len){
	if ((file == STDOUT_FILENO) || (file == STDERR_FILENO)){
		return UART_write(file, data, len);
	} else {
		errno = EBADF;
		return -1;
	}
}

/**
 * closes the file descriptor
 * \n not used
 * @param  file file descriptor
 * @return      -1
 */
int _close(int file){
    return -1;
}

/**
 * reposition read/write file offset
 * \n not used
 * @param  file file descriptor
 * @param  ptr  offset
 * @param  dir  direction (SEEK_SET, SEEK_CUR, SEEK_END)
 * @return      0
 */
int _lseek(int file, int ptr, int dir){
    return 0;
}

/**
 * get file status
 * \n not used
 * @param  file file descriptor
 * @param  st   stat structure
 * @return      0
 */
int _fstat(int file, struct stat *st){
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 * test whether a file descriptor refers to a terminal
 * \n not used
 * @param  file file descriptor
 * @return      1 if fd is an open file descriptor referring to a
 *      	terminal, otherwise 0
 */
int _isatty(int file){
    if( (file == STDOUT_FILENO) || (file == STDIN_FILENO) || (file == STDERR_FILENO) ){
        return 1;
    }

    errno = EBADF;
    return 0;
}

/**
 * wait for process to change state
 * @param  status status
 * @return        on success, returns the process ID of the terminated child; on error, -1 is returned
 */
int _wait(int *status){
	errno = ECHILD;
	return -1;
}

/**
 * get process times
 * @param  buf struct tms is as defined in <sys/times.h>
 * @return     the number of clock ticks that have elapsed since an arbitrary point in the past
 */
int _times(struct tms *buf){
	return -1;
}

/**
 * make a new name for a file
 * @param  old old file path
 * @param  new new file path
 * @return     on success, zero is returned; on error, -1 is returned
 */
int _link(char *old, char *new){
	errno = EMLINK;
	return -1;
}

/**
 * delete a name and possibly the file it refers to
 * @param  name file path
 * @return      on success, zero is returned; on error, -1
 */
int _unlink(char *name){
	errno = ENOENT;
	return -1;
}

/**
 * create a child process
 * @return  on success, the PID of the child process is returned in the parent, and 0 is returned in the child.  On failure, -1 is returned in the parent, no child process is created
 */
int _fork(void){
	errno = EAGAIN;
	return -1;
}

/**
 * execute program
 * @param  name program path to execute
 * @param  argv an array of argument strings passed to the new program
 * @param  env  array of strings, conventionally of the form key=value, which are passed as environment to the new program
 * @return      on success, execve() does not return, on error -1 is returned
 */
int _execve(char *name, char **argv, char **env){
	errno = ENOMEM;
	return -1;
}
