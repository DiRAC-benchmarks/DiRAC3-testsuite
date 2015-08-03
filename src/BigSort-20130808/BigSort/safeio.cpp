
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
// mpi.h is only used for timing
#include "mpi.h"
#include <stdarg.h>

void ioerror(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    throw;
}

double read_time = 0.;
double write_time = 0.;
double open_time = 0.;

void iotime()
{
    printf("PosixFileReadTime(s) = %f\nPosixFileWriteTime(s) = %f\nPosixFileOpenTime(s) = %f\nPosixFileTotalTime(s) = %f\n", read_time, write_time, open_time, read_time+write_time+open_time);
}

ssize_t safeRead(int fd, void *buf, size_t count, const char* marker)
{
  double tm1, tm2;
  tm1 = MPI_Wtime();
  ssize_t ret = read(fd, buf, count);
  if (ret == -1) {
    if(errno == EAGAIN) 
      ioerror("%s The file descriptor fd (%d) refers to a file other than a socket and has been marked nonblocking (O_NONBLOCK), and the read would block.\n", marker, fd);
    else if(errno == EWOULDBLOCK)
      ioerror("%s The file descriptor fd (%d) refers to a socket and has been marked nonblocking (O_NONBLOCK), and the read would block. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.\n", marker, fd);
    else if(errno == EBADF)
      ioerror("%s fd (%d) is not a valid file descriptor or is not open for reading.\n", marker, fd);
    else if(errno == EFAULT)
      ioerror("%s fd (%d): buf is outside your accessible address space.\n", marker, fd);
    else if(errno == EINTR)
      ioerror("%s fd (%d): The call was interrupted by a signal before any data was read; see signal(7).\n", marker, fd);
    else if(errno == EINVAL)
      ioerror("%s fd (%d) is attached to an object which is unsuitable for reading; or the file was opened with the O_DIRECT flag, and either the address specified in buf, the value specified in count, or the current file offset is not suitably aligned.\n", marker, fd);
    else if(errno == EINVAL)
      ioerror("%s fd (%d) was created via a call to timerfd_create(2) and the wrong size buffer was given to read(); see timerfd_create(2) for further information.\n", marker, fd);
    else if(errno == EIO)
      ioerror("%s I/O error for fd (%d). This will happen for example when the process is in a background process group, tries to read from its controlling terminal, and either it is ignoring or blocking SIGTTIN or its process group is orphaned. It may also occur when there is a low-level I/O error while reading from a disk or tape.\n", marker, fd);
    else if(errno == EISDIR)
      ioerror("%s fd (%d) refers to a directory.\n", marker, fd);
    else 
      ioerror("%s fd (%d) unknown IO error.\n", marker, fd);
  }
  tm2 = MPI_Wtime();
  read_time += tm2-tm1;
  return ret;
}

ssize_t safeWrite(int fd, const void *buf, size_t count, const char* marker)
{
  double tm1, tm2;
  tm1 = MPI_Wtime();
  ssize_t ret = write(fd, buf, count);
  if (ret == -1) {
    if(errno == EAGAIN) 
      ioerror("%s The file descriptor fd (%d) refers to a file other than a socket and has been marked nonblocking (O_NONBLOCK), and the read would block.\n", marker, fd);
    else if(errno == EWOULDBLOCK)
      ioerror("%s The file descriptor fd (%d) refers to a socket and has been marked nonblocking (O_NONBLOCK), and the read would block. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.\n", marker, fd);
    else if(errno == EBADF)
      ioerror("%s fd (%d) is not a valid file descriptor or is not open for reading.\n", marker, fd);
    else if(errno == EFAULT)
      ioerror("%s fd (%d): buf is outside your accessible address space.\n", marker, fd);
    else if(errno == EINTR)
      ioerror("%s fd (%d): The call was interrupted by a signal before any data was read; see signal(7).\n", marker, fd);
    else if(errno == EINVAL)
      ioerror("%s fd (%d) is attached to an object which is unsuitable for reading; or the file was opened with the O_DIRECT flag, and either the address specified in buf, the value specified in count, or the current file offset is not suitably aligned.\n", marker, fd);
    else if(errno == EIO)
      ioerror("%s I/O error for fd (%d). This will happen for example when the process is in a background process group, tries to read from its controlling terminal, and either it is ignoring or blocking SIGTTIN or its process group is orphaned. It may also occur when there is a low-level I/O error while reading from a disk or tape.\n", marker, fd);
    else if(errno == EDESTADDRREQ)
      ioerror("%s fd (%d) refers to a datagram socket for which a peer address has not been set using connect(2).\n", marker, fd);
    else if(errno == EDQUOT)
      ioerror("%s The user's quota of disk blocks on the file system containing the file referred to by fd (%d) has been exhausted.\n", marker, fd);
    else if(errno == EFBIG)
      ioerror("%s fd (%d) An attempt was made to write a file that exceeds the implementation-defined maximum file size or the process's file size limit, or to write at a position past the maximum allowed offset.\n", marker, fd);
    else if(errno == ENOSPC)
      ioerror("%s The device containing the file referred to by fd (%d) has no room for the data.\n", marker, fd);
    else if(errno == EPIPE)
      ioerror("%s fd (%d) is connected to a pipe or socket whose reading end is closed. When this happens the writing process will also receive a SIGPIPE signal. (Thus, the write return value is seen only if the program catches, blocks or ignores this signal.)\n", marker, fd);
    else 
      ioerror("%s fd (%d) unknown IO error.\n", marker, fd);
  }
  tm2 = MPI_Wtime();
  write_time += tm2-tm1;
  return ret;
}


int safeOpen(const char *pathname,int flags)
{
   double tm1, tm2;
   tm1 = MPI_Wtime();
   int fd = open(pathname, flags);
   if (fd < 0){
     ioerror("Cannot open file %s\n", pathname);  
     throw;
   }
   tm2 = MPI_Wtime();
   open_time += tm2-tm1;
   return fd;
}
int safeOpen(const char *pathname,int flags, mode_t mode)
{
   double tm1, tm2;
   tm1 = MPI_Wtime();
   int fd = open(pathname, flags, mode);
   if (fd < 0){
     ioerror("Cannot open file %s\n", pathname);  
     throw;
   }
   tm2 = MPI_Wtime();
   open_time += tm2-tm1;
   return fd;
}
/*
EACCES
The requested access to the file is not allowed, or search permission is denied for one of the directories in the path prefix of pathname, or the file did not exist yet and write access to the parent directory is not allowed. (See also path_resolution(7).)
EDQUOT
Where O_CREAT is specified, the file does not exist, and the user's quota of disk blocks or inodes on the file system has been exhausted.
EEXIST
pathname already exists and O_CREAT and O_EXCL were used.
EFAULT
pathname points outside your accessible address space.
EFBIG
See EOVERFLOW.
EINTR
While blocked waiting to complete an open of a slow device (e.g., a FIFO; see fifo(7)), the call was interrupted by a signal handler; see signal(7).
EISDIR
pathname refers to a directory and the access requested involved writing (that is, O_WRONLY or O_RDWR is set).
ELOOP
Too many symbolic links were encountered in resolving pathname, or O_NOFOLLOW was specified but pathname was a symbolic link.
EMFILE
The process already has the maximum number of files open.
ENAMETOOLONG
pathname was too long.
ENFILE
The system limit on the total number of open files has been reached.
ENODEV
pathname refers to a device special file and no corresponding device exists. (This is a Linux kernel bug; in this situation ENXIO must be returned.)
ENOENT
O_CREAT is not set and the named file does not exist. Or, a directory component in pathname does not exist or is a dangling symbolic link.
ENOMEM
Insufficient kernel memory was available.
ENOSPC
pathname was to be created but the device containing pathname has no room for the new file.
ENOTDIR
A component used as a directory in pathname is not, in fact, a directory, or O_DIRECTORY was specified and pathname was not a directory.
ENXIO
O_NONBLOCK | O_WRONLY is set, the named file is a FIFO and no process has the file open for reading. Or, the file is a device special file and no corresponding device exists.
EOVERFLOW
pathname refers to a regular file that is too large to be opened. The usual scenario here is that an application compiled on a 32-bit platform without -D_FILE_OFFSET_BITS=64 tried to open a file whose size exceeds (2<<31)-1 bits; see also O_LARGEFILE above. This is the error specified by POSIX.1-2001; in kernels before 2.6.24, Linux gave the error EFBIG for this case.
EPERM
The O_NOATIME flag was specified, but the effective user ID of the caller did not match the owner of the file and the caller was not privileged (CAP_FOWNER).
EROFS
pathname refers to a file on a read-only file system and write access was requested.
ETXTBSY
pathname refers to an executable image which is currently being executed and write access was requested.
EWOULDBLOCK
The O_NONBLOCK flag was specified, and an incompatible lease was held on the file (see fcntl(2)).
*/
