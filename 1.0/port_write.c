#include <stdio.h>  /* Standard input/output definitions */
#include <string.h> /* Sting function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h>  /* File control definitions */
#include <errno.h>  /* Error number definitions */
#include <termios.h>/* POSIX terminal control definitions */

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int main(void)
{
  int fd; /* File descriptor for the port */
  int n; 

  fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
    /*Could not open the port*/
    perror("open_port: Unable to oepn /dev/ttyS0 - ");
  } 
  else
    fcntl(fd, F_SETFL, 0);
  
  n = write(fd, "15", 2);
  if (n < 0)
    fputs("write() of 2 bytes failed!\n", stderr);

  return (fd);
}
