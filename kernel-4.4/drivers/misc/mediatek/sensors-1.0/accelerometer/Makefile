# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/include

obj-y   +=  accel.o accel_factory.o

obj-$(CONFIG_MTK_ACCELHUB) += accelhub/
obj-$(CONFIG_MTK_MC3433)   += mc3433/
obj-$(CONFIG_MTK_LSM6DS3HG)+= lsm6ds3hg/
obj-$(CONFIG_MTK_LIS3DH)   += lis3dh/
#obj-$(CONFIG_MTK_BMI160_I2C)   += bmi160-i2c/
#obj-$(CONFIG_MTK_MC3410_I2C) += mc3410-i2c/
obj-$(CONFIG_MTK_MPU6050G)   += mpu6050g/
obj-$(CONFIG_MTK_LIS3DH_NEW) += lis3dh/
#Begin added by lanying.he for XR5424729 on 2017/10/20
obj-$(CONFIG_MTK_A3A_MC3413_P)   +=  A3A_mc3413-p/
#Begin modified by yan.gong for XR6019929 on 2018/2/27 sensor driver
obj-$(CONFIG_MTK_SIGNIFICANT_SENSOR)   +=  significant_motion/
#End modified by yan.gong for XR6019929 on 2018/2/27 sensor driver
#End added by lanying.he for XR5424729 on 2017/10/20  
#Begin added by lanying.he for XR5592217 on 2017/11/22  
#obj-$(CONFIG_MTK_A3A_BMA253_NEW)   +=  A3A_bma253/
#End added by lanying.he for XR5592217 on 2017/11/22  
#Begin added by zhi.chen.hz for u5a bma253 on 2017/12/16  
obj-$(CONFIG_MTK_U5A_BMA2X2)   +=  U5A_bma253/
obj-$(CONFIG_MTK_U5A_KXTJ2)   +=  U5A_kxtj2/
#End added by zhi.chen.hz for u5a bma253 on 2017/12/16 
