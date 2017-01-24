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

/*	control registers for enabling I2C interface, single shot measurement etc	*/
#define CTRL_REG1_ADDR	0x20
#define CTRL_REG2_ADDR	0x21
#define FIFO_CTRL_ADRR	0x2E

/*	registers containing pressure	and temperature measurements	*/
#define PRESS_OUT_XL_ADDR	0x28
#define PRESS_OUT_L_ADDR	0x29
#define PRESS_OUT_H_ADDR	0x2A
#define TEMP_OUT_L_ADDR		0x2B
#define TEMP_OUT_H_ADDR		0x2C

/*	defines for I2C 	*/
#define I2C_PATH	"/dev/i2c-1"

int main()
{
	/*	definition of some pressure/temperature values we need for calculations	*/
	uint8_t press_out_xl = 0;
	uint8_t press_out_l = 0;
	uint8_t press_out_h = 0;
	uint32_t press_out = 0;
	double pressure = 0.0;

	uint8_t temp_out_l = 0;
	uint8_t temp_out_h = 0;
	int16_t temp_out = 0;
	double temperature = 0.0;

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
	i2c_smbus_write_byte_data(i2c_fd, CTRL_REG1_ADDR, 0x00);

	/* power on device	*/
	i2c_smbus_write_byte_data(i2c_fd, CTRL_REG1_ADDR, 0x80);		

	/*	enable FIFO and FIFO decimation at 1Hz   	*/
	i2c_smbus_write_byte_data(i2c_fd, CTRL_REG2_ADDR, 0x50);	

	/*	enable FIFO Stream mode	*/	
	i2c_smbus_write_byte_data(i2c_fd, FIFO_CTRL_ADDR, 0x40);

	/*	now we should wait until the measurements are complete:	*/
	/*	every 10 msecs we should check for 0x00 in CTRL_REG2 address due to its self-clearing */	
	while(i2c_smbus_read_byte_data(i2c_fd, CTRL_REG2_ADDR) != 0)
	{ 
		usleep(25000);
	}

	/*	get measurements	*/
	press_out_xl = i2c_smbus_read_byte_data(i2c_fd, PRESS_OUT_XL_ADDR);	
	press_out_l = i2c_smbus_read_byte_data(i2c_fd, PRESS_OUT_L_ADDR);
	press_out_h = i2c_smbus_read_byte_data(i2c_fd, PRESS_OUT_H_ADDR);
	press_out = (press_out_h << 16) | (press_out_l << 8) | (press_out_xl);
	pressure = press_out / 4096.0;

	temp_out_l = i2c_smbus_read_byte_data(i2c_fd, TEMP_OUT_L_ADDR);
	temp_out_h = i2c_smbus_read_byte_data(i2c_fd, TEMP_OUT_H_ADDR);
	temp_out = (temp_out_h << 8) | (temp_out_l);
	temperature = 42.5 + (temp_out / 480.0);

	/*	print proccessed pressure in hectopascals (1hPa = 100Pa) and temperature in degree Celsius	*/
	printf("Temperature = %fC, pressure = %fhPa\n", temperature, pressure);

	/*	power off the device	*/
	i2c_smbus_write_byte_data(i2c_fd, CTRL_REG1_ADDR, 0x00);
	
	/*	close i2c communication	*/
	close(i2c_fd);


	return 0;
}
