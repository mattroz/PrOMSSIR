#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>

/*	main device defining registers */
#define DEV_ADDR		0x5C
#define DEV_ID			0xBD
#define WHO_AM_I_ADDR	0x0F

/*	control registers for enabling I2C interface, single shot measurment etc	*/
#define CTRL_REG1_ADDR	0x20
#define CTRL_REG2_ADDR	0x21

/*	registers containing pressure	and temperature measurments	*/
#define PRESS_OUT_XL_ADDR	0x28
#define PRESS_OUT_L_ADDR	0x29
#define PRESS_OUT_H_ADDR	0x2A
#define TEMP_OUT_L_ADDR		0x2B
#define TEMP_OUT_H_ADDR		0x2C

/*	defines for I2C 	*/
#define I2C_PATH	"/dev/i2c-1"


int main()
{
	/*	open i2c bus for communication	*/
	int i2c_fd = 0;
	
	if((i2c_fd = open(I2C_PATH, O_RDWR)) < 0)
	{
		perror("I2C device connection");
		exit(EXIT_FAILURE);
	}

	/*	initialize communication with the LPS25H slave	*/
	if(ioctl(i2c_fd, I2C_SLAVE, DEV_ADDR) < 0)
	{
		perror("I2C slave connection");
		close(i2c_fd);
		exit(EXIT_FAILURE);
	}

	/*	check if accessed device is LPS25H	*/
	if(i2c_smbus_read_byte_data(i2c_fd, WHO_AM_I_ADDR) != DEV_ID)
	{
		perror("Can't get access to LPS25H");
		close(i2c_fd);
		exit(EXIT_FAILURE);
	}

	/*	reset device	*/
		

	return 0;
}
