
#ifndef __CCU_I2C_HW_H__
#define __CCU_I2C_HW_H__

/*---------------------------------------------------------------------------*/
/*        i2c interface from ccu_i2c_hw.c */
/*---------------------------------------------------------------------------*/
extern int ccu_i2c_set_n3d_base(unsigned long n3d_base);
extern int ccu_trigger_i2c_hw(enum CCU_I2C_CHANNEL channel, int transac_len, MBOOL do_dma_en);

#endif
