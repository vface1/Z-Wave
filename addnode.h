/////////////////////addnode.h/////////////////////////////

#ifndef ADDNODE_H
#define ADDNODE_H

#include "extern_files.h" //External files

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  
//#include "serial_init.h"

struct Add
{
	int code;
	char *desc;
	char swap[25];
}*p;

struct Add* add_node_to_network()
{
		char *buff;
		char j;
		static int swap[22];
		char value[]={0x01,0x05,0x00,0x4A,0X01,0X01,0XB0};//serial API for Add function
	        p = malloc(sizeof(struct Add));
	 

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
		perror("n");
	
	   /* if (n == -1)
       	    {
         	close(fd);
         	return (int*)-1;
       	    }*/
     

	while(1)
	{
    	    ret_bytes=read(fd,  &buff, 255);//read the buffer
	    
	    printf("\nReturn Bytes: %d", ret_bytes);
           
	    if(*((char *) &buff+0) == 06)
	    {
		p->code = *((char *) &buff+0);
		p->desc= "Ack";
	    }

	    for(i=0;i<ret_bytes;i++)
	    	{
                	printf("H-%x \t",*((char *) &buff+i));
                	 p->swap[i]= *((char*)&buff+i);
	    	}
	  
       	    sleep(1);
	    if (ret_bytes <=0) break ;

	} //end of while()
	close(fd);
	
	return p;
	//free(p);
	
} //end of main()

#endif