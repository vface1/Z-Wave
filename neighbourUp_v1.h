#ifndef NEIGHBOURUP_H
#define NEIGHBOURUP_H

#include "extern_files.h" //External files

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  

int node_neighbour_update()
{
	    char *buff;
	    char j;
	
		
	    struct termios options;
	    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY); //open the serial port
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

	     //char value[]={0x01,0x05,0x00,0x48,0x21,0x01,0x92}; //serial API for Node Neighbour Update
	     char value[]={0x01,0x05,0x00,0x48,0x57,0x01,0xE4};
	    //char value[]={0x01,0x05,0x00,0x48,0x4D,0x01,0xFE};

	    buff = "\0";
	    n = write(fd,value,sizeof(value));//write the value
	   	
		
	while(1)
	{
    	    ret_bytes=read(fd,  &buff, 255);//read the buffer
	    flag++;
	    printf("\nReturn Bytes: %d\n", ret_bytes);
            for(i=0;i<ret_bytes;i++)
	    {
                printf("H-%d \t",*((char *) &buff+i));
	    }
	
	    sleep (1);
		if(flag == 1)
		{
			if((ret_bytes == -1) || (ret_bytes == 0))
			{
				close(fd);
				//return (int*)-1;
				break;
			}
		}
		
		if(*((char *) &buff+5) == 34 || *((char *) &buff+5) == 35 )
		{
			
			break;
		}
			
		
		
	
	} //end of while()

	close(fd);
	return *((char *) &buff+5);
	
}

#endif