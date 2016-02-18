#ifndef REMOVENODE_H
#define REMOVENODE_H

#include "extern_files.h" //External files
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
//#include "serial_init.h"

struct Add* remove_node_from_network()
{
		char *buff;
		char j;
		
	    
	    char value[]={0x01,0x05,0x00,0x4B,0X01,0X01,0XB1};//serial API for Remove function


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
	
	    n = write(fd,value,sizeof(value));//write the value
	
	    
     		

	while(1)
	{
    	    ret_bytes=read(fd,  &buff, 255);//read the buffer
	    if (ret_bytes <=0) break ;
	   
            printf("\nReturn Bytes: %d\n", ret_bytes);
            for(i=0;i<ret_bytes;i++)
	    {
                printf("H-%x \t",*((char *) &buff+i));
		p->swap[i] = *((char*)&buff+i);
	    }
            sleep (1); 
       	    


	} //end of while()
	close(fd);

	return p;
	
} //end of main()

#endif