subdir-ccflags-y += -Werror
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
# In case the platform does NOT support this type of sensors
obj-y   +=  mtk_tpd.o
obj-y   +=  tpd_button.o
obj-y   +=  tpd_calibrate.o
obj-y   +=  tpd_debug.o
obj-y   +=  tpd_default.o
obj-y   +=  tpd_init.o
obj-y   +=  tpd_misc.o
obj-y   +=  tpd_setting.o

# Modified by bin.song.hz for add mockup CTP driver at 2017.8.14 begin
obj-$(CONFIG_TOUCHSCREEN_MTK_A3A_FT8613)	+=  a3a_ft8613/
obj-$(CONFIG_TOUCHSCREEN_MTK_A3A_TD4310)	+=  a3a_td4310/
# Modified by bin.song.hz for add mockup CTP driver at 2017.8.14 end
obj-$(CONFIG_TOUCHSCREEN_MTK_GT1151)	+=  GT1151/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT5688)	+=  GT5688/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT911)	+=  GT911/
obj-$(CONFIG_TOUCHSCREEN_MTK_GSLX680)	+=  gslX680/
obj-$(CONFIG_TOUCHSCREEN_FT5X26)	+=  FT5X26/
#*** add for focaltech touch panel driver, 2017.12.06 , zlh ***
obj-$(CONFIG_TOUCHSCREEN_MTK_FOCALTECH_TS) += focaltech_touch/
#**************************************************************
#Begin added by yang.liu.hz for FR5597245 on 20171115,add CTP drivers.
obj-$(CONFIG_TOUCHSCREEN_MTK_U5APLUS_MSG22XX)	+=  u5aplus_msg22xx/
#End added by yang.liu.hz for FR5597245 on 20171115,add CTP drivers.

