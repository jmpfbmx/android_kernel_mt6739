ccflags-y := -I$(srctree)/drivers/misc/mediatek/include
ccflags-$(CONFIG_MTK_PRINTK_UART_CONSOLE) += -I$(srctree)/drivers/misc/mediatek/mtprintk/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/uart

obj-y	= printk.o
# Begin add smart log by yinghe for XR5424644 on 2017-11-14
obj-$(CONFIG_JRD_PRINTD)  += printd.o
# End add smart log by yinghe for XR5424644 on 2017-11-14
# Begin added by yinghe for XR5424657 on 2017/12/26
obj-$(CONFIG_TCT_BIGLOG)  += smart_info.o
# End added by yinghe for XR5424657 on 2017/12/26
obj-$(CONFIG_A11Y_BRAILLE_CONSOLE)	+= braille.o
