/*
  Strawberry-linux's i2c lcd control sample
  LCD : http://strawberry-linux.com/catalog/items?code=27030

  Coded by Yasuhiro ISHII,13 Mar,2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

#define I2C_SLAVEADDR_LCD 0x3e

int sendLcdInit(int fd);
int sendLcdString(int fd,char* str);
int sendLcdLocateXY(int fd,int row,int col);

int main(void)
{
    int fd;

    fd = open("/dev/i2c-1",O_RDWR);
    if(fd < 0){
	printf("i2c open error\n");
	exit(1);
    }
    
    sendLcdInit(fd);
    sendLcdString(fd,"Hello");
    
    close(fd);
}

int sendLcdInit(int fd)
{
    unsigned char init_1[] = { 0x3e,0x39,0x14,0x75,0x5e,0x6c };
    unsigned char init_2[] = { 0x0c,0x01,0x06 };
    unsigned char buff[2];
    int i;
    int result;

    ioctl(fd,I2C_SLAVE,I2C_SLAVEADDR_LCD);
  
    buff[0] = 0;
    for(i=0;i<sizeof(init_1);i++){
	buff[1] = init_1[i];
	result = write(fd,buff,2);
	usleep(30);
    }
  
    usleep(200000);

    for(i=0;i<sizeof(init_2);i++){
	buff[1] = init_2[i];
	result = write(fd,buff,2);
	usleep(30);
    }

    return(0);
}

int sendLcdString(int fd,char* str)
{
    int i;
    unsigned char buff[2];
    int result;

    ioctl(fd,I2C_SLAVE,I2C_SLAVEADDR_LCD);

    buff[0] = 0x40;
    while(*str){
	buff[1] = *str;
	result = write(fd,buff,2);
	str++;
    }

    return(0);
}

int sendLcdLocateXY(int fd,int row,int col)
{
    unsigned char buff[2];
    int result;

    buff[0] = 0x00;
    buff[1] = 0x40;

    if(col != 0){
	buff[1] += 0x40;
    }
    buff[1] += row;

    result = write(fd,buff,2);

    return(0);
}
