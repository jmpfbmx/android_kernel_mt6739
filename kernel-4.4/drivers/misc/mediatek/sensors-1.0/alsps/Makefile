# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
obj-y   +=  alsps.o
obj-y   +=  alsps_factory.o
obj-y   +=  aal_control.o

obj-$(CONFIG_MTK_ALSPSHUB)      +=  alspshub/
obj-$(CONFIG_MTK_CM36558)	+=  cm36558/
#Begin added by lanying.he for XR5424729 on 2017/10/20 psensor driver
obj-$(CONFIG_MTK_A3A_STK3X1X_S34)	+=  A3A_stk3x1x-s34/
#End added by lanying.he for XR5424729 on 2017/10/20 psensor driver

#Begin added by zhi.chen for u5a-ltr578 on 2017/10/20 psensor driver
obj-$(CONFIG_MTK_U5A_LTR578)	+=  U5A_ltr578/
obj-$(CONFIG_MTK_U5A_STK3X1X)	+=  U5A_stk3x1x/
#End added by zhi.chen for u5a-ltr578 on 2017/10/20 psensor driver
