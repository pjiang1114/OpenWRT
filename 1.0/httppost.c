#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "port_read_write.h"

/*
int read_port(void)
{
  int fd;  // File descriptor
  fd = open("/dev/ttyS0", O_RDONLY | O_NOCTTY);
  if (fd == -1)
  {
     // Cout not open the port
     perror("open_port: Unable to open /dev/ttyS0 - ");
  }
  printf("serial port ttyS0 opened!\n");
  return (fd);
}
*/
int main(void)
{  
  char *portname = "/dev/ttys000";
  // open port with a file descriptor
  int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC); 
  if (fd < 0)
  {
     fprintf(stderr,"error %d opening %s: %s \n", errno, portname, strerror(errno));
     return -1;
  } 
  
  set_interface_attribs (fd, B115200, 0); // set speed to 115,200 bps,
                                          // 8in1(no parity)
  set_blocking (fd, 0);                   // set no blocking

  write(fd,"10",2);                        // send 2 character
  
  usleep ((10+25)*100);         // sleep enough to transmit the 2 char plus
                               // receive 25: approx 100 uS per char transmit
  char buffer[2];
  int n = read(fd, buffer,sizeof buffer);
  if (n<0)
  {  
      fputs("read failed!\n", stderr);
      return(2); 
  }
  printf("read from port ... done!\n");
  printf("In buffer: %c%c \n", buffer[0], buffer[1]);
  //create update data
  char *key = "key=TYAY6R6VPZWQBQNE";
  char *field = "field1="; 
 // int To_int_data = buffer;
  char *To_string_data = malloc(sizeof buffer); 
        To_string_data = buffer;
  char *postfield; 
  if((postfield = malloc(strlen(key)+strlen(field)+3)) != NULL)
  {
    postfield[0]='\0'; // ensures the memory is an empty string
    strcat(postfield, key);
    strcat(postfield, "&");
    strcat(postfield, field);
    strcat(postfield, To_string_data);
    //To_string_data = &buffer[1];
    //strcat(postfield, To_string_data);
  }
  printf("postfiled= %s\n", postfield);


  CURL *curl;
  CURLcode res;
  char *url;
  url = "http://api.thingspeak.com/update";
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* Now specify the POST data */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  } 
  curl_global_cleanup();
  return 0;
}

