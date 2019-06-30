

#ifndef __MSTAR_DRV_COMMON_H__
#define __MSTAR_DRV_COMMON_H__

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
//#include <linux/earlysuspend.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/miscdevice.h>
#include <linux/time.h>
#include <linux/input/mt.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

/*--------------------------------------------------------------------------*/
/* TOUCH DEVICE DRIVER RELEASE VERSION                                      */
/*--------------------------------------------------------------------------*/

#define DEVICE_DRIVER_RELEASE_VERSION   ("3.6.0.0")


/*--------------------------------------------------------------------------*/
/* COMPILE OPTION DEFINITION                                                */
/*--------------------------------------------------------------------------*/

//#define CONFIG_TOUCH_DRIVER_RUN_ON_SPRD_PLATFORM
//#define CONFIG_TOUCH_DRIVER_RUN_ON_QCOM_PLATFORM
#define CONFIG_TOUCH_DRIVER_RUN_ON_MTK_PLATFORM

//#define CONFIG_ENABLE_TOUCH_DRIVER_FOR_MUTUAL_IC
#define CONFIG_ENABLE_TOUCH_DRIVER_FOR_SELF_IC


#define CONFIG_ENABLE_REGULATOR_POWER_ON

//#define CONFIG_ENABLE_TOUCH_PIN_CONTROL


//#define CONFIG_USE_IRQ_INTERRUPT_FOR_MTK_PLATFORM

#define CONFIG_ENABLE_DMA_IIC


#define CONFIG_TP_HAVE_KEY

#define CONFIG_ENABLE_REPORT_KEY_WITH_COORDINATE

#define CONFIG_ENABLE_FIRMWARE_DATA_LOG (1)   // 1 : Enable, 0 : Disable

#define CONFIG_ENABLE_SEGMENT_READ_FINGER_TOUCH_DATA

#define CONFIG_ENABLE_GESTURE_WAKEUP

// ------------------- #ifdef CONFIG_ENABLE_GESTURE_WAKEUP ------------------- //
#ifdef CONFIG_ENABLE_GESTURE_WAKEUP

//#define CONFIG_SUPPORT_64_TYPES_GESTURE_WAKEUP_MODE

//#define CONFIG_ENABLE_GESTURE_DEBUG_MODE

//#define CONFIG_ENABLE_GESTURE_INFORMATION_MODE

#endif //CONFIG_ENABLE_GESTURE_WAKEUP
// ------------------- #endif CONFIG_ENABLE_GESTURE_WAKEUP ------------------- //


#define CONFIG_ENABLE_ITO_MP_TEST

// ------------------- #ifdef CONFIG_ENABLE_ITO_MP_TEST ------------------- //
#ifdef CONFIG_ENABLE_ITO_MP_TEST

#define CONFIG_ENABLE_MP_TEST_ITEM_FOR_2R_TRIANGLE

#endif //CONFIG_ENABLE_ITO_MP_TEST
// ------------------- #endif CONFIG_ENABLE_ITO_MP_TEST ------------------- //


#define CONFIG_UPDATE_FIRMWARE_BY_SW_ID

// ------------------- #ifdef CONFIG_UPDATE_FIRMWARE_BY_SW_ID ------------------- //
#ifdef CONFIG_UPDATE_FIRMWARE_BY_SW_ID

#define CONFIG_UPDATE_FIRMWARE_BY_TWO_DIMENSIONAL_ARRAY 

#endif //CONFIG_UPDATE_FIRMWARE_BY_SW_ID
// ------------------- #endif CONFIG_UPDATE_FIRMWARE_BY_SW_ID ------------------- //

//#define CONFIG_ENABLE_HOTKNOT


//#define CONFIG_ENABLE_PROXIMITY_DETECTION

//#define CONFIG_ENABLE_NOTIFIER_FB


#define CONFIG_ENABLE_COUNT_REPORT_RATE


//#define CONFIG_ENABLE_GLOVE_MODE


#define CONFIG_ENABLE_JNI_INTERFACE


//#define CONFIG_ENABLE_CHARGER_DETECTION


#define CONFIG_ENABLE_CODE_FOR_DEBUG


//#define CONFIG_ENABLE_TYPE_B_PROTOCOL


//#define CONFIG_ENABLE_FORCE_TOUCH


//#define CONFIG_ENABLE_UPDATE_FIRMWARE_WITH_8_BYTE_EACH_TIME 

//#define CONFIG_ENABLE_UPDATE_FIRMWARE_WITH_SUPPORT_I2C_SPEED_400K 


//#define CONFIG_ENABLE_ESD_CHECK 
#ifdef CONFIG_ENABLE_ESD_CHECK
extern void esd_check_enable(void);
extern void esd_check_disable(void);
extern void esd_check_init(void);
#endif


/*--------------------------------------------------------------------------*/
/* PREPROCESSOR CONSTANT DEFINITION                                         */
/*--------------------------------------------------------------------------*/

#define u8   unsigned char
#define u16  unsigned short
#define u32  unsigned int
#define s8   signed char
#define s16  signed short
#define s32  signed int
#define s64  int64_t
#define u64  uint64_t


#define CHIP_TYPE_MSG21XX   (0x01) // EX. MSG2133
#define CHIP_TYPE_MSG21XXA  (0x02) // EX. MSG2133A/MSG2138A(Besides, use version to distinguish MSG2133A/MSG2138A, you may refer to _DrvFwCtrlUpdateFirmwareCash()) 
#define CHIP_TYPE_MSG26XXM  (0x03) // EX. MSG2633M
#define CHIP_TYPE_MSG22XX   (0x7A) // EX. MSG2238/MSG2256
#define CHIP_TYPE_MSG28XX   (0x85) // EX. MSG2856

#define PACKET_TYPE_TOOTH_PATTERN   (0x20)
#define PACKET_TYPE_GESTURE_WAKEUP  (0x50)
#define PACKET_TYPE_GESTURE_DEBUG  		(0x51)
#define PACKET_TYPE_GESTURE_INFORMATION	(0x52)

#define TOUCH_SCREEN_X_MIN   (0)
#define TOUCH_SCREEN_Y_MIN   (0)
#define TOUCH_SCREEN_X_MAX   (480)  //LCD_WIDTH
#define TOUCH_SCREEN_Y_MAX   (854) //LCD_HEIGHT
#define TPD_WIDTH   (2048)
#define TPD_HEIGHT  (2048)


#define BIT0  (1<<0)  // 0x0001
#define BIT1  (1<<1)  // 0x0002
#define BIT2  (1<<2)  // 0x0004
#define BIT3  (1<<3)  // 0x0008
#define BIT4  (1<<4)  // 0x0010
#define BIT5  (1<<5)  // 0x0020
#define BIT6  (1<<6)  // 0x0040
#define BIT7  (1<<7)  // 0x0080
#define BIT8  (1<<8)  // 0x0100
#define BIT9  (1<<9)  // 0x0200
#define BIT10 (1<<10) // 0x0400
#define BIT11 (1<<11) // 0x0800
#define BIT12 (1<<12) // 0x1000
#define BIT13 (1<<13) // 0x2000
#define BIT14 (1<<14) // 0x4000
#define BIT15 (1<<15) // 0x8000


#define MAX_DEBUG_REGISTER_NUM     (10)
#define MAX_DEBUG_COMMAND_ARGUMENT_NUM      (4)

#define MAX_UPDATE_FIRMWARE_BUFFER_SIZE    (130) // 130KB. The size shall be large enough for stored any kind firmware size of MSG21XXA(33KB)/MSG22XX(48.5KB)/MSG26XXM(40KB)/MSG28XX(130KB).

#define MAX_I2C_TRANSACTION_LENGTH_LIMIT      (250) // Please change the value depends on the I2C transaction limitation for the platform that you are using.
#define MAX_TOUCH_IC_REGISTER_BANK_SIZE       (256) // It is a fixed value and shall not be modified.


#define PROCFS_AUTHORITY (0666)


#ifdef CONFIG_ENABLE_GESTURE_WAKEUP
#define GESTURE_WAKEUP_MODE_DOUBLE_CLICK_FLAG     0x00000001    //0000 0000 0000 0000   0000 0000 0000 0001
#define GESTURE_WAKEUP_MODE_UP_DIRECT_FLAG        0x00000002    //0000 0000 0000 0000   0000 0000 0000 0010
#define GESTURE_WAKEUP_MODE_DOWN_DIRECT_FLAG      0x00000004    //0000 0000 0000 0000   0000 0000 0000 0100
#define GESTURE_WAKEUP_MODE_LEFT_DIRECT_FLAG      0x00000008    //0000 0000 0000 0000   0000 0000 0000 1000
#define GESTURE_WAKEUP_MODE_RIGHT_DIRECT_FLAG     0x00000010    //0000 0000 0000 0000   0000 0000 0001 0000
#define GESTURE_WAKEUP_MODE_m_CHARACTER_FLAG      0x00000020    //0000 0000 0000 0000   0000 0000 0010 0000
#define GESTURE_WAKEUP_MODE_W_CHARACTER_FLAG      0x00000040    //0000 0000 0000 0000   0000 0000 0100 0000
#define GESTURE_WAKEUP_MODE_C_CHARACTER_FLAG      0x00000080    //0000 0000 0000 0000   0000 0000 1000 0000
#define GESTURE_WAKEUP_MODE_e_CHARACTER_FLAG      0x00000100    //0000 0000 0000 0000   0000 0001 0000 0000
#define GESTURE_WAKEUP_MODE_V_CHARACTER_FLAG      0x00000200    //0000 0000 0000 0000   0000 0010 0000 0000
#define GESTURE_WAKEUP_MODE_O_CHARACTER_FLAG      0x00000400    //0000 0000 0000 0000   0000 0100 0000 0000
#define GESTURE_WAKEUP_MODE_S_CHARACTER_FLAG      0x00000800    //0000 0000 0000 0000   0000 1000 0000 0000
#define GESTURE_WAKEUP_MODE_Z_CHARACTER_FLAG      0x00001000    //0000 0000 0000 0000   0001 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE1_FLAG         0x00002000    //0000 0000 0000 0000   0010 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE2_FLAG         0x00004000    //0000 0000 0000 0000   0100 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE3_FLAG         0x00008000    //0000 0000 0000 0000   1000 0000 0000 0000

#ifdef CONFIG_SUPPORT_64_TYPES_GESTURE_WAKEUP_MODE
#define GESTURE_WAKEUP_MODE_RESERVE4_FLAG         0x00010000    //0000 0000 0000 0001   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE5_FLAG         0x00020000    //0000 0000 0000 0010   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE6_FLAG         0x00040000    //0000 0000 0000 0100   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE7_FLAG         0x00080000    //0000 0000 0000 1000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE8_FLAG         0x00100000    //0000 0000 0001 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE9_FLAG         0x00200000    //0000 0000 0010 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE10_FLAG        0x00400000    //0000 0000 0100 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE11_FLAG        0x00800000    //0000 0000 1000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE12_FLAG        0x01000000    //0000 0001 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE13_FLAG        0x02000000    //0000 0010 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE14_FLAG        0x04000000    //0000 0100 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE15_FLAG        0x08000000    //0000 1000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE16_FLAG        0x10000000    //0001 0000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE17_FLAG        0x20000000    //0010 0000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE18_FLAG        0x40000000    //0100 0000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE19_FLAG        0x80000000    //1000 0000 0000 0000   0000 0000 0000 0000

#define GESTURE_WAKEUP_MODE_RESERVE20_FLAG        0x00000001    //0000 0000 0000 0000   0000 0000 0000 0001
#define GESTURE_WAKEUP_MODE_RESERVE21_FLAG        0x00000002    //0000 0000 0000 0000   0000 0000 0000 0010
#define GESTURE_WAKEUP_MODE_RESERVE22_FLAG        0x00000004    //0000 0000 0000 0000   0000 0000 0000 0100
#define GESTURE_WAKEUP_MODE_RESERVE23_FLAG        0x00000008    //0000 0000 0000 0000   0000 0000 0000 1000
#define GESTURE_WAKEUP_MODE_RESERVE24_FLAG        0x00000010    //0000 0000 0000 0000   0000 0000 0001 0000
#define GESTURE_WAKEUP_MODE_RESERVE25_FLAG        0x00000020    //0000 0000 0000 0000   0000 0000 0010 0000
#define GESTURE_WAKEUP_MODE_RESERVE26_FLAG        0x00000040    //0000 0000 0000 0000   0000 0000 0100 0000
#define GESTURE_WAKEUP_MODE_RESERVE27_FLAG        0x00000080    //0000 0000 0000 0000   0000 0000 1000 0000
#define GESTURE_WAKEUP_MODE_RESERVE28_FLAG        0x00000100    //0000 0000 0000 0000   0000 0001 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE29_FLAG        0x00000200    //0000 0000 0000 0000   0000 0010 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE30_FLAG        0x00000400    //0000 0000 0000 0000   0000 0100 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE31_FLAG        0x00000800    //0000 0000 0000 0000   0000 1000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE32_FLAG        0x00001000    //0000 0000 0000 0000   0001 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE33_FLAG        0x00002000    //0000 0000 0000 0000   0010 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE34_FLAG        0x00004000    //0000 0000 0000 0000   0100 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE35_FLAG        0x00008000    //0000 0000 0000 0000   1000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE36_FLAG        0x00010000    //0000 0000 0000 0001   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE37_FLAG        0x00020000    //0000 0000 0000 0010   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE38_FLAG        0x00040000    //0000 0000 0000 0100   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE39_FLAG        0x00080000    //0000 0000 0000 1000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE40_FLAG        0x00100000    //0000 0000 0001 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE41_FLAG        0x00200000    //0000 0000 0010 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE42_FLAG        0x00400000    //0000 0000 0100 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE43_FLAG        0x00800000    //0000 0000 1000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE44_FLAG        0x01000000    //0000 0001 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE45_FLAG        0x02000000    //0000 0010 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE46_FLAG        0x04000000    //0000 0100 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE47_FLAG        0x08000000    //0000 1000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE48_FLAG        0x10000000    //0001 0000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE49_FLAG        0x20000000    //0010 0000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE50_FLAG        0x40000000    //0100 0000 0000 0000   0000 0000 0000 0000
#define GESTURE_WAKEUP_MODE_RESERVE51_FLAG        0x80000000    //1000 0000 0000 0000   0000 0000 0000 0000
#endif //CONFIG_SUPPORT_64_TYPES_GESTURE_WAKEUP_MODE

#define GESTURE_WAKEUP_PACKET_LENGTH    (6)

#ifdef CONFIG_ENABLE_GESTURE_DEBUG_MODE
#define GESTURE_DEBUG_MODE_PACKET_LENGTH	(128)
#endif //CONFIG_ENABLE_GESTURE_DEBUG_MODE

#ifdef CONFIG_ENABLE_GESTURE_INFORMATION_MODE
#define GESTURE_WAKEUP_INFORMATION_PACKET_LENGTH	(128)
#endif //CONFIG_ENABLE_GESTURE_INFORMATION_MODE

#endif //CONFIG_ENABLE_GESTURE_WAKEUP

#define FEATURE_GESTURE_WAKEUP_MODE         0x0001   
#define FEATURE_GESTURE_DEBUG_MODE          0x0002
#define FEATURE_GESTURE_INFORMATION_MODE    0x0003

#define FEATURE_TOUCH_DRIVER_DEBUG_LOG      0x0010
#define FEATURE_FIRMWARE_DATA_LOG           0x0011
#define FEATURE_FORCE_TO_UPDATE_FIRMWARE    0x0012


#define I2C_WRITE_COMMAND_DELAY_FOR_FIRMWARE   (20) // delay 20ms

#define FIRMWARE_FILE_PATH_ON_SD_CARD      "/mnt/sdcard/msctp_update.bin"


#define POWER_SUPPLY_BATTERY_STATUS_PATCH  "/sys/class/power_supply/battery/status"


#define CONFIG_TOUCH_DRIVER_DEBUG_LOG_LEVEL (1)   // 1 : Default, 0 : No log. The bigger value, the more detailed log is output.

/*=============================================================*/
// EXTERN VARIABLE DECLARATION
/*=============================================================*/

extern u8 TOUCH_DRIVER_DEBUG_LOG_LEVEL;

/*--------------------------------------------------------------------------*/
/* PREPROCESSOR MACRO DEFINITION                                            */
/*--------------------------------------------------------------------------*/


#define DEBUG_LEVEL(level, fmt, arg...) do {\
	                                           if (level <= TOUCH_DRIVER_DEBUG_LOG_LEVEL)\
	                                               printk(fmt, ##arg);\
                                        } while (0)

#define DBG(fmt, arg...) DEBUG_LEVEL(7, fmt, ##arg) 


/*--------------------------------------------------------------------------*/
/* DATA TYPE DEFINITION                                                     */
/*--------------------------------------------------------------------------*/

typedef enum
{
    EMEM_ALL = 0,
    EMEM_MAIN,
    EMEM_INFO
} EmemType_e;

typedef enum
{
    ITO_TEST_MODE_OPEN_TEST = 1,
    ITO_TEST_MODE_SHORT_TEST = 2,
    ITO_TEST_MODE_WATERPROOF_TEST = 3
} ItoTestMode_e;

typedef enum
{
    ITO_TEST_OK = 0,
    ITO_TEST_FAIL,
    ITO_TEST_GET_TP_TYPE_ERROR,
    ITO_TEST_UNDEFINED_ERROR,
    ITO_TEST_UNDER_TESTING

} ItoTestResult_e;

typedef enum
{
    ADDRESS_MODE_8BIT = 0,
    ADDRESS_MODE_16BIT = 1
} AddressMode_e;

typedef enum
{
    MUTUAL = 0,
    SELF,
    WATERPROOF,
    MUTUAL_SINGLE_DRIVE
} ItoTestFwMode_e;

/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLE DEFINITION                                               */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION DECLARATION                                              */
/*--------------------------------------------------------------------------*/

extern u8 DrvCommonCalculateCheckSum(u8 *pMsg, u32 nLength);
extern u32 DrvCommonConvertCharToHexDigit(char *pCh, u32 nLength);
extern u32 DrvCommonCrcDoReflect(u32 nRef, s8 nCh);
extern u32 DrvCommonCrcGetValue(u32 nText, u32 nPrevCRC);
extern void DrvCommonCrcInitTable(void);
extern void DrvCommonReadFile(char *pFilePath, u8 *pBuf, u16 nLength);

#endif  /* __MSTAR_DRV_COMMON_H__ */
