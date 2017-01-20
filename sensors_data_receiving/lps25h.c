#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/i2c-dev.h>

/*	main device defining registers */
#define DEV_ADDR			0x5C
#define DEV_ID				0xBD
#define WHO_AM_I_ADDR	0x0F

/*	control registers for enabling I2C interface, single shot measurment etc	*/
#define CTRL_REG1	0x20
#define CTRL_REG2	0x21

/*	registers containing pressure	and temperature measurments	*/
#define PRESS_OUT_XL	0x28
#define PRESS_OUT_L 	0x29
#define PRESS_OUT_H 	0x2A
#define TEMP_OUT_L 	0x2B
#define TEMP_OUT_H	0x2C


int main()
{
	return 0;
}
