#ifndef ISFAILED_H
#define ISFAILED_H

#include "extern_files.h" //External files

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  

int is_failed_node()
{
	    char *buff;
	    char j;

 	    char value[]={0x01,0x04,0x00,0x62,0x57,0xCE}; //serial API for isFailedNode()
	    
	    struct termios options;
	    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);//open the serial port
	    fcntl(fd, F_SETFL, 0);
	    tcgetattr(fd, &options);
	    
	    cfsetispeed(&options, B115200);//setting Baud rate
	    cfsetospeed(&options, B115200);
	    
	    options.c_cflag |= (CLOCAL | CREAD);
	    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
	    options.c_oflag     &= ~OPOST;
	    options.c_cc[VMIN]  = 0;
	    options.c_cc[VTIME] = 10;
	    tcsetattr(fd, TCSANOW, &options);
	
	   
	    buff = "\0";
	    n = write(fd,value,sizeof(value));//write the value
	    
	while(1)
	{
    	    ret_bytes=read(fd,  &buff, 255);//read the buffer
	    if (ret_bytes <= 0) break;
	    printf("\nReturned value from read: %d", ret_bytes);
            
            for(i=0;i<ret_bytes;i++)
	    {
                printf("i=%d,%x \t",i,*((char *) &buff+i));
	    }
	    sleep (1); 
       	   
	} //end of while()
	
	
	close(fd);
	return *((char *) &buff+4);
	
}
#endif