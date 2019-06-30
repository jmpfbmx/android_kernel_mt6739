#ifndef BMA2XX_H
#define BMA2XX_H
	 
#include <linux/ioctl.h>


	#define BMA2XX_I2C_SLAVE_WRITE_ADDR		0x10
	 
	 /* BMA2XX Register Map  (Please refer to BMA2XX Specifications) */
	#define BMA2XX_REG_DEVID				0x00
	#define BMA2XX_FIXED_DEVID			0xFA
	#define BMA2XX_REG_OFSX				0x16
	#define BMA2XX_REG_OFSX_HIGH			0x1A
	#define BMA2XX_REG_BW_RATE			0x10
	#define BMA2XX_BW_MASK				0x1f
	#define BMA2XX_BW_200HZ				0x0d
	#define BMA2XX_BW_100HZ				0x0c
	#define BMA2XX_BW_50HZ				0x0b
	#define BMA2XX_BW_25HZ				0x0a
	#define BMA2XX_REG_POWER_CTL		0x11		
	#define BMA2XX_REG_DATA_FORMAT		0x0f
	#define BMA2XX_RANGE_MASK			0x0f
	#define BMA2XX_RANGE_2G				0x03
	#define BMA2XX_RANGE_4G				0x05
	#define BMA2XX_RANGE_8G				0x08
	#define BMA2XX_REG_DATAXLOW			0x02
	#define BMA2XX_REG_DATA_RESOLUTION	0x14
	#define BMA2XX_MEASURE_MODE			0x80	
	#define BMA2XX_SELF_TEST           			0x32
	#define BMA2XX_SELF_TEST_AXIS_X		0x01
	#define BMA2XX_SELF_TEST_AXIS_Y		0x02
	#define BMA2XX_SELF_TEST_AXIS_Z		0x03
	#define BMA2XX_SELF_TEST_POSITIVE	0x00
	#define BMA2XX_SELF_TEST_NEGATIVE	0x04
	#define BMA2XX_INT_REG_1           			0x16
	#define BMA2XX_INT_REG_2          		 	0x17

	//Added by yuduan.xie for significant on 2017.2.27 begin
	#define KXTJ2_1009_REG_POWER_CTL		0x30

	#define KXTJ2_1009_MOTION_DETECT       0x0A

	#define N2DM_WAKEUP_THRESHOLD 0x32
	#define N2DM_WAKEUP_TIMER         0x33

	//#define KXTJ2_1009_INT_SOURCE2_XYZ    0x17
	#define KXTJ2_1009_INT_SOURCE1            0x31
	#define KXTJ2_1009_INT_SOURCE2            0x35
	#define KXTJ2_1009_INT_REG                     0x1a
	//Added by yuduan.xie for significant on 2017.2.27 end

	
#define BMA2XX_SUCCESS						0
#define BMA2XX_ERR_I2C						-1
#define BMA2XX_ERR_STATUS					-3
#define BMA2XX_ERR_SETUP_FAILURE			-4
#define BMA2XX_ERR_GETGSENSORDATA			-5
#define BMA2XX_ERR_IDENTIFICATION			-6
	 
	 
	 
#define BMA2XX_BUFSIZE				256

#define BMA2XX_AXES_NUM        3

/*----------------------------------------------------------------------------*/
typedef enum{
    BMA2XX_CUST_ACTION_SET_CUST = 1,
    BMA2XX_CUST_ACTION_SET_CALI,
    BMA2XX_CUST_ACTION_RESET_CALI
}CUST_ACTION;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t    action;
}BMA2XX_CUST;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t    action;
    uint16_t    part;
    int32_t     data[0];
}BMA2XX_SET_CUST;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t    action;
    int32_t     data[BMA2XX_AXES_NUM];
}BMA2XX_SET_CALI;
/*----------------------------------------------------------------------------*/
typedef BMA2XX_CUST BMA2XX_RESET_CALI;
/*----------------------------------------------------------------------------*/
typedef union
{
    uint32_t                data[10];
    BMA2XX_CUST         cust;
    BMA2XX_SET_CUST     setCust;
    BMA2XX_SET_CALI     setCali;
    BMA2XX_RESET_CALI   resetCali;
}BMA2XX_CUST_DATA;
/*----------------------------------------------------------------------------*/

#define BMA255_CHIP_ID		0XFA
#define BMA250E_CHIP_ID		0XF9
#define BMA222E_CHIP_ID		0XF8
#define BMA280_CHIP_ID		0XFB

#define BMA222E_TYPE		0
#define BMA250E_TYPE		1
#define BMA255_TYPE		2
#define BMA280_TYPE		3
#define UNSUPPORT_TYPE		-1

struct bma2x2_type_map_t
{
	/*! bma2x2 sensor chip id */
	uint16_t chip_id;

	/*! bma2x2 sensor type */
	int16_t sensor_type;

	/*! bma2x2 sensor name */
	const char *sensor_name;
};
#endif

