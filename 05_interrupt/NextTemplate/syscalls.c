/* Support files for GNU libc.  Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>

#include "rpi_lib/rpi.h"
// int uart0_putc (int c);

extern void *__bss_end;
unsigned int heap_end=(unsigned int)&__bss_end;
unsigned int prev_heap_end;

/* Forward prototypes.  */
int     _system      (const char *);
int     _rename      (const char *, const char *);
int     isatty       (int);
clock_t _times       (struct tms *);
int     _gettimeofday    (struct timeval *, struct timezone *);
void    _raise       (void);
int     _unlink      (void);
int     _link        (void);
int     _stat        (const char *, struct stat *);
int     _fstat       (int, struct stat *);
caddr_t _sbrk        (int);
int     _getpid      (int);
int     _kill        (int, int);
void    _exit        (int);
int     _close       (int);
int     _open        (const char *, int, ...);
int     _write       (int, char *, int);
int     _lseek       (int, int, int);
int     _read        (int, char *, int);
void    initialise_monitor_handles  (void);

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");

//static int
//remap_handle (int fh)
//{
    //return 0;
//}

void
initialise_monitor_handles (void)
{
}

// 注意: 以下2つを有効にするとprintfが動かなくなる。
// static int
// get_errno (void)
// {
//     return(0);
// }

// static int
// error (int result)
// {
//   errno = get_errno ();
//   return result;
// }


int
_read (int file,
       char * ptr,
       int len)
{
  return len;
}


int
_lseek (int file,
    int ptr,
    int dir)
{
    return 0;
}


int
_write (int    file,
    char * ptr,
    int    len)
{
    int r;
    for(r=0;r<len;r++){
      uart0_putc(ptr[r]);
    }
    return len;
}

int
_open (const char * path,
       int          flags,
       ...)
{
    return 0;
}


int
_close (int file)
{
    return 0;
}

void
_exit (int n)
{
    while(1);
}

int
_kill (int n, int m)
{
    return(0);
}

int
_getpid (int n)
{
  return 1;
  n = n;
}


caddr_t _sbrk (int incr) {
  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr)
    return (caddr_t) -1;
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

int
_fstat (int file, struct stat * st)
{
  return 0;
}

int _stat (const char *fname, struct stat *st)
{
  return 0;
}

int
_link (void)
{
  return -1;
}

int
_unlink (void)
{
  return -1;
}

void
_raise (void)
{
  return;
}

int
_gettimeofday (struct timeval * tp, struct timezone * tzp)
{
    if(tp)
    {
        tp->tv_sec = 10;
        tp->tv_usec = 0;
    }
    if (tzp)
    {
        tzp->tz_minuteswest = 0;
        tzp->tz_dsttime = 0;
    }
    return 0;
}

clock_t
_times (struct tms * tp)
{
    clock_t timeval;

    timeval = 10;
    if (tp)
    {
        tp->tms_utime  = timeval;   /* user time */
        tp->tms_stime  = 0; /* system time */
        tp->tms_cutime = 0; /* user time, children */
        tp->tms_cstime = 0; /* system time, children */
    }
    return timeval;
};


int
_isatty (int fd)
{
  return 1;
  fd = fd;
}

int
_system (const char *s)
{
  if (s == NULL)
    return 0;
  errno = ENOSYS;
  return -1;
}

int
_rename (const char * oldpath, const char * newpath)
{
  errno = ENOSYS;
  return -1;
}
