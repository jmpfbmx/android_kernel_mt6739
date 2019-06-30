OBJS = bbm.o fc8180.o fc8180_bb.o fc8180_i2c.o fc8180_isr.o fc8180_ppi.o fc8180_spi.o fc8180_spib.o fc8180_tun.o fci_bypass.o fci_hal.o fci_hpi.o fci_i2c.o fci_oal.o fci_ringbuffer.o fci_tun.o

#EXTRA_CFLAGS += -I$(PWD)/../inc
#KDIR = /home/mv4412/src/mv4412-kernel-3.0.15
#ODIR = $(PWD)/../

#obj-m = isdbt.o
obj-y = isdbt.o
isdbt-objs = $(OBJS)
ccflags-y += -Idrivers/misc/mediatek/fc8180/inc -Idrivers/misc/mediatek/include/mt-plat -I$(srctree)/drivers/misc/mediatek/base/power/include
#-Idrivers/spi/mediatek/mt6580
#

#all:
#	$(MAKE) -C $(KDIR) M=$(PWD) modules
#	mv *.ko			$(ODIR)
#	mv *.o $(ODIR)
#	mv isdbt.mod.c $(ODIR)
#	mv Module.symvers $(ODIR)
#	mv modules.order $(ODIR)
#	mv .*.o.cmd $(ODIR)
#	mv .*.ko.cmd $(ODIR)
#
#clean:
#	rm -rf ../*.ko
#	rm -rf *.o
#	rm -rf isdbt.mod.c
#	rm -rf Module.symvers
#	rm -rf modules.order
#	rm -rf .*.o.cmd
#
#+OBJS = bbm.o fc8180.o fc8180_bb.o fc8180_i2c.o fc8180_isr.o fc8180_ppi.o fc8180_spi.o fc8180_spib.o fc8180_tun.o fci_bypass.o fci_hal.o fci_hpi.o fci_i2c.o fci_oal.o fci_ringbuffer.o fci_tun.o
#+obj-y = isdbt.o
#+isdbt-objs = $(OBJS)
#+ccflags-y += -Idrivers/media/fc8180/inc -Idrivers/misc/mediatek/include/mt-plat -Idrivers/spi/mediatek/mt6580
#
