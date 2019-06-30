

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include "lens_info.h"


#define AF_DRVNAME "FP5510AF_DRV"
#define AF_I2C_SLAVE_ADDR        0x18

#define AF_DEBUG
#ifdef AF_DEBUG
#define LOG_INF(format, args...) pr_info(AF_DRVNAME " [%s] " format, __func__, ##args)
#else
#define LOG_INF(format, args...)
#endif


static struct i2c_client *g_pstAF_I2Cclient;
static int *g_pAF_Opened;
static spinlock_t *g_pAF_SpinLock;
static DEFINE_MUTEX(af_write_lock);

static unsigned long g_u4AF_INF;
static unsigned long g_u4AF_MACRO = 1023;
static unsigned long g_u4TargetPosition;
static unsigned long g_u4CurrPosition;

static int g_sr = 3;
static int g_init_setup = 0;

static int s4AF_ReadReg(unsigned short *a_pu2Result)
{
	int i4RetValue = 0;
	char pBuff[2];

	g_pstAF_I2Cclient->addr = AF_I2C_SLAVE_ADDR >> 1;

	i4RetValue = i2c_master_recv(g_pstAF_I2Cclient, pBuff, 2);

	if (i4RetValue < 0) {
		LOG_INF("I2C read failed!!\n");
		return -1;
	}

	*a_pu2Result = (((u16) pBuff[0]) << 4) + (pBuff[1] >> 4);

	return 0;
}

static int s4AF_WriteReg(u16 a_u2Data)
{
	int i4RetValue = 0;

	char puSendCmd[2] = { (char)(a_u2Data >> 4), (char)((a_u2Data & 0xF) << 4) };

	LOG_INF("g_sr %d, write %d\n", g_sr, a_u2Data);

	g_pstAF_I2Cclient->addr = AF_I2C_SLAVE_ADDR >> 1;

	i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puSendCmd, 2);

	if (i4RetValue < 0) {
		LOG_INF("I2C send failed!!\n");
		return -1;
	}

	return 0;
}

static inline int getAFInfo(__user struct stAF_MotorInfo *pstMotorInfo)
{
	struct stAF_MotorInfo stMotorInfo;

	stMotorInfo.u4MacroPosition = g_u4AF_MACRO;
	stMotorInfo.u4InfPosition = g_u4AF_INF;
	stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
	stMotorInfo.bIsSupportSR = 1;

	stMotorInfo.bIsMotorMoving = 1;

	if (*g_pAF_Opened >= 1)
		stMotorInfo.bIsMotorOpen = 1;
	else
		stMotorInfo.bIsMotorOpen = 0;

	if (copy_to_user(pstMotorInfo, &stMotorInfo, sizeof(struct stAF_MotorInfo)))
		LOG_INF("copy to user failed when getting motor information\n");

	return 0;
}


//----------------------------------------------------
// smooth af 
//----------------------------------------------------
static unsigned long g_destPosition;
static unsigned long g_startPosition;
static unsigned long g_StepWidth;
static struct hrtimer g_timeOutTimer;
static struct work_struct workTimeOut;
static int g_timeOutTimeMs = 16;

static void smooth_movAF(void)
{
	if(g_startPosition >= g_destPosition) { //down
		
	  if((g_startPosition - g_destPosition) > g_StepWidth)
		  g_startPosition -= g_StepWidth;
	  else
		  g_startPosition = g_destPosition;
		
	} else { //up
		
	  if((g_destPosition - g_startPosition) > g_StepWidth)
		  g_startPosition += g_StepWidth;
	  else
		  g_startPosition = g_destPosition;
		
	}
	
	mutex_lock(&af_write_lock);
	if(g_startPosition != g_u4CurrPosition) {
	  if (s4AF_WriteReg((unsigned short)g_startPosition) == 0) {
	    //spin_lock(g_pAF_SpinLock);
	    g_u4CurrPosition = g_startPosition;
	    //spin_unlock(g_pAF_SpinLock);
	  }
	}
	mutex_unlock(&af_write_lock);
	
}

static void smoothAF_timer_start(unsigned long rDestPos, unsigned long rStartPos, unsigned long rStepWidth, int mode)
{
	ktime_t ktime;
	
	g_destPosition = rDestPos;
	g_startPosition = rStartPos;
	g_StepWidth = rStepWidth;
	
	LOG_INF("destPosition: %d, g_startPosition: %d\n", (int)g_destPosition,(int)g_startPosition);
	
	if(mode == 0)
	  smooth_movAF();
	
	g_timeOutTimeMs = 16;
	ktime = ktime_set( 0, g_timeOutTimeMs*1000000 );
	hrtimer_start( &g_timeOutTimer, ktime, HRTIMER_MODE_REL );
}

static void smoothAF_timer_cancel(void)
{
	hrtimer_cancel( &g_timeOutTimer );
}

static void work_timeOutFunc(struct work_struct *data)
{
	unsigned long rTemp;
	
	smooth_movAF();
	
	if(g_startPosition != g_destPosition) {
		
		spin_lock(g_pAF_SpinLock);
		rTemp = g_u4TargetPosition;
		spin_unlock(g_pAF_SpinLock);
		
		if(rTemp == g_destPosition) {
		  smoothAF_timer_start(g_destPosition,g_startPosition,g_StepWidth,1);
		}
		
	}
}

static enum hrtimer_restart AFTimeOutCallback(struct hrtimer *timer)
{
	schedule_work(&workTimeOut);
	return HRTIMER_NORESTART;
}

static void smoothAF_timerInit(void)
{
	static int init_flag = 0;

	if (init_flag == 0) {
		init_flag = 1;
		
		LOG_INF("Start\n");
		
		INIT_WORK(&workTimeOut, work_timeOutFunc);
		g_timeOutTimeMs = 16;
		hrtimer_init(&g_timeOutTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		g_timeOutTimer.function = AFTimeOutCallback;
	}
}

int FP5510AF_getExtraMode(void)
{
  int i;
  
  //--af power off--
  if (g_u4CurrPosition > 0) {
		 smoothAF_timer_start(g_u4TargetPosition,g_u4CurrPosition,20,0);
	} else {
		 g_destPosition = g_startPosition = 0;
	}
	//----------------
	
  //400ms delay
  for (i = 0; i <= 25; i++) {
    
    if(g_startPosition == g_destPosition) {
    	LOG_INF("wait destPosition OK!!!\n");
    	hrtimer_cancel( &g_timeOutTimer );
    	g_init_setup = 0;
      return 0;
    }
    
    msleep(16);
  }
  
  LOG_INF("wait destPosition Fail!!!\n");
  hrtimer_cancel( &g_timeOutTimer );
  g_init_setup = 0;
  return 0;
}
//----------------------------------------------------


//init setup
static int FP5510AF_mode_setup(void)
{
	int i4RetValue = 0;
	char puSendCmd[2] = { 0xEC, 0xA3 };	//Selection mode on

	LOG_INF("FP5510AF_mode_setup enter!!\n");
	
	g_pstAF_I2Cclient->addr = AF_I2C_SLAVE_ADDR >> 1;
	
	i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puSendCmd, 2);
	if (i4RetValue < 0) {
		LOG_INF("I2C send failed!!\n");
		return -1;
	}

	//DVO[3:1] DLC and MCLK[1:0] setting
	puSendCmd[0] = 0xA1;
	//puSendCmd[1] = 0x4D;	//DVO[3:1] : 010(Delta Value=64), MCLK[1:0] : 01, DLC mode
	puSendCmd[1] = 0x15;	//DVO[3:1] : 000/(Delta Value=0), MCLK[1:0] : 01, DLC mode
	i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puSendCmd, 2);
	if (i4RetValue < 0) {
		LOG_INF("I2C send failed!!\n");
		return -1;
	}

	//T_SRC[4:0] setting
	puSendCmd[0] = 0xF2;
	//puSendCmd[1] = 0x60;	//T_SRC[4:0] : 0x60, Tvib/2 = 5.12
	puSendCmd[1] = 0x18;	//T_SRC[4:0] : 0x00, Tvib/2 = 6.33
	i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puSendCmd, 2);
	if (i4RetValue < 0) {
		LOG_INF("I2C send failed!!\n");
		return -1;
	}

	//Selection mode off
	puSendCmd[0] = 0xDC;
	puSendCmd[1] = 0x51;
	i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puSendCmd, 2);
	if (i4RetValue < 0) {
		LOG_INF("I2C send failed!!\n");
		return -1;
	}

	LOG_INF("FP5510AF_mode_setup exit!!\n");
	g_init_setup = 1;
	return 0;
}

static inline int moveAF(unsigned long a_u4Position)
{
	int ret = 0;

	if ((a_u4Position > g_u4AF_MACRO) || (a_u4Position < g_u4AF_INF)) {
		LOG_INF("out of range\n");
		return -EINVAL;
	}

	if (*g_pAF_Opened == 1) {
		unsigned short InitPos;

		ret = s4AF_ReadReg(&InitPos);

		if (ret == 0) {
			LOG_INF("Init Pos %6d, Target pos %6d\n", InitPos,(unsigned short)a_u4Position);

			spin_lock(g_pAF_SpinLock);
			g_u4CurrPosition = (unsigned long)InitPos;
			spin_unlock(g_pAF_SpinLock);
			
		} else {
			spin_lock(g_pAF_SpinLock);
			g_u4CurrPosition = 0;
			spin_unlock(g_pAF_SpinLock);
		}

		if(!g_init_setup) {
			if(FP5510AF_mode_setup())
				LOG_INF("FP5510AF mode setup failed!!\n");
		}

		spin_lock(g_pAF_SpinLock);
		*g_pAF_Opened = 2;
		spin_unlock(g_pAF_SpinLock);
	}
	
	if (g_u4CurrPosition == a_u4Position) {
		return 0;
	}
  
	spin_lock(g_pAF_SpinLock);
	g_u4TargetPosition = a_u4Position;
	spin_unlock(g_pAF_SpinLock);

	//------smooth af-------
	smoothAF_timer_cancel();
	//----------------------

	/* LOG_INF("move [curr] %d [target] %d\n", (int)g_u4CurrPosition, (int)a_u4Position); */

	spin_lock(g_pAF_SpinLock);
	g_sr = 3;
	spin_unlock(g_pAF_SpinLock);

	mutex_lock(&af_write_lock);
	if(g_u4TargetPosition != g_u4CurrPosition) {
	  if (s4AF_WriteReg((unsigned short)g_u4TargetPosition) == 0) {
	    //spin_lock(g_pAF_SpinLock);
	    g_u4CurrPosition = g_u4TargetPosition;
	    //spin_unlock(g_pAF_SpinLock);
	  }
	}
	mutex_unlock(&af_write_lock);

	return 0;
}

static inline int setAFInf(unsigned long a_u4Position)
{
	spin_lock(g_pAF_SpinLock);
	g_u4AF_INF = a_u4Position;
	spin_unlock(g_pAF_SpinLock);
	return 0;
}

static inline int setAFMacro(unsigned long a_u4Position)
{
	spin_lock(g_pAF_SpinLock);
	g_u4AF_MACRO = a_u4Position;
	spin_unlock(g_pAF_SpinLock);
	return 0;
}

/* ////////////////////////////////////////////////////////////// */
long FP5510AF_Ioctl(struct file *a_pstFile, unsigned int a_u4Command, unsigned long a_u4Param)
{
	long i4RetValue = 0;

	switch (a_u4Command) {
	case AFIOC_G_MOTORINFO:
		i4RetValue = getAFInfo((__user struct stAF_MotorInfo *) (a_u4Param));
		break;

	case AFIOC_T_MOVETO:
		i4RetValue = moveAF(a_u4Param);
		break;

	case AFIOC_T_SETINFPOS:
		i4RetValue = setAFInf(a_u4Param);
		break;

	case AFIOC_T_SETMACROPOS:
		i4RetValue = setAFMacro(a_u4Param);
		break;

	default:
		LOG_INF("No CMD\n");
		i4RetValue = -EPERM;
		break;
	}

	return i4RetValue;
}

/* Main jobs: */
/* 1.Deallocate anything that "open" allocated in private_data. */
/* 2.Shut down the device on last close. */
/* 3.Only called once on last time. */
/* Q1 : Try release multiple times. */
int FP5510AF_Release(struct inode *a_pstInode, struct file *a_pstFile)
{
	LOG_INF("Start\n");

	if (*g_pAF_Opened == 2) {
		g_sr = 5;
		//begin, add for bug 5780657 by zhijun.fu on 20180106
		 if(s4AF_WriteReg(0)<0)   //reset motor 
		 	pr_err("FP5510AF reset motor failed\n");
		  else 
		  	msleep(10);
		//  if (s4AF_WriteReg(0x80)<0)   //power down  
		  //	pr_err("FP5510AF power down failed\n");
		//  else 
		 // 	msleep(10);
		 //end, add for bug 5780657 by zhijun.fu on 20180106
		//------smooth af-------
		spin_lock(g_pAF_SpinLock);
		g_u4TargetPosition = 0;
		spin_unlock(g_pAF_SpinLock);
		//----------------------
	}

	if (*g_pAF_Opened) {
		LOG_INF("Free\n");

		spin_lock(g_pAF_SpinLock);
		*g_pAF_Opened = 0;
		spin_unlock(g_pAF_SpinLock);
	}

	LOG_INF("End\n");

	return 0;
}

int FP5510AF_SetI2Cclient(struct i2c_client *pstAF_I2Cclient, spinlock_t *pAF_SpinLock, int *pAF_Opened)
{
	g_pstAF_I2Cclient = pstAF_I2Cclient;
	g_pAF_SpinLock = pAF_SpinLock;
	g_pAF_Opened = pAF_Opened;
	
	//------smooth af-------
	if(*g_pAF_Opened == 1) {
	  smoothAF_timerInit();
	}
	//----------------------
	return 1;
}
