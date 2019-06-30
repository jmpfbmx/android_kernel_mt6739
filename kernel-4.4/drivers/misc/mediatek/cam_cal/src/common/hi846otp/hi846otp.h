/*****************************************************************************
 *
 * Filename:
 * ---------
 *   S-24CS64A.h
 *
 * Project:
 * --------
 *   ALPS
 *
 * Description:
 * ------------
 *   Header file of CAM_CAL driver
 *
 *
 * Author:
 * -------
 *   Ronnie Lai (MTK01420)
 *
 *============================================================================*/
#ifndef __HI846OTP_H
#define __HI846OTP_H

#define CAM_CAL_DEV_MAJOR_NUMBER 226

/* CAM_CAL READ/WRITE ID */
#define HI846OTP_DEVICE_ID							0xA0//0x5a
#define I2C_UNIT_SIZE                                  1 //in byte
#define OTP_START_ADDR                            0x0A04
#define OTP_SIZE                                      24
unsigned int hi846otp_selective_read_region(struct i2c_client *client, unsigned int addr,	unsigned char *data, unsigned int size);
unsigned int hi847_otp_selective_read_region(struct i2c_client *client, unsigned int addr,unsigned char *data, unsigned int size);
#endif /* __CAM_CAL_H */

