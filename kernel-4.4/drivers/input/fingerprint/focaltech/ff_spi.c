/**
 * The platform spi device for FocalTech's fingerprint sensor.
 *
 * Copyright (C) 2016-2017 FocalTech Systems Co., Ltd. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
**/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/spi/spi.h>

#if defined CONFIG_ARCH_MT6735M || \
    defined CONFIG_ARCH_MT6755 || \
    defined CONFIG_ARCH_MT6739 || \
    defined CONFIG_ARCH_MT6753
# include <mt_spi.h>
#elif defined CONFIG_ARCH_MT6795
# include <mach/mt_spi.h>
#else
//# error "unknown arch platform."
#endif

#include "ff_log.h"

# undef LOG_TAG
#define LOG_TAG "ff_spi"

#if defined (CONFIG_MTK_PLATFORM)
/*static struct mt_chip_conf mt_spi_chip_conf = {
    .setuptime    = 20,
    .holdtime     = 20,
    .high_time    = 25,
    .low_time     = 25,
    .cs_idletime  = 10,
    .ulthgh_thrsh =  0,
    .sample_sel   = POSEDGE,
    .cs_pol       = ACTIVE_LOW,
    .cpol         = SPI_CPOL_0,
    .cpha         = SPI_CPHA_0,
    .tx_mlsb      = SPI_MSB,
    .rx_mlsb      = SPI_MSB,
    .tx_endian    = SPI_LENDIAN,
    .rx_endian    = SPI_LENDIAN,
    .com_mod      = DMA_TRANSFER, // FIFO_TRANSFER/DMA_TRANSFER
    .pause        = PAUSE_MODE_ENABLE,
    .finish_intr  = FINISH_INTR_EN,
    .deassert     = DEASSERT_DISABLE,
    .ulthigh      = ULTRA_HIGH_DISABLE,
    .tckdly       = TICK_DLY0,
};*/
#endif

/*
 * Note: In order to enable spidev driver for REE-Emulation,
 * the CONFIG_SPIDEV=y must be set in '.config' file.
 */
static struct spi_board_info spi_desc = {
    .modalias        = "spidev",
    .bus_num         = 32766,
    .chip_select     = 0,
    .mode            = SPI_MODE_0,
#if defined (CONFIG_MTK_PLATFORM)
    .controller_data = 0,
#endif
};
static struct spi_master *spi_ctl = NULL;
struct spi_device *g_ff_spi_dev = NULL;

////////////////////////////////////////////////////////////////////////////////
int ff_spi_register_device(int bus, int cs)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);

    /* Assign the 'bus' number and 'cs' number passed from ff_config.dat */
   // spi_desc.bus_num = bus;
    spi_desc.bus_num = 32766;
    spi_desc.chip_select = cs;

    /* Retrieve the master controller handle. */
    spi_ctl = spi_busnum_to_master(spi_desc.bus_num);
    if (!spi_ctl) {
        FF_LOGE("there is no spi master controller on bus #%d.", spi_desc.bus_num);
        return (-ENODEV);
    }

    /* Register the spi device. */
    g_ff_spi_dev = spi_new_device(spi_ctl, &spi_desc);
    if (!g_ff_spi_dev) {
        FF_LOGE("failed to register spidev device.");
        spi_master_put(spi_ctl);
        spi_ctl = NULL;
        return (-EBUSY);
    }
    FF_LOGI("spidev device has been registered.");

    FF_LOGV("'%s' leave.", __func__);
    return err;
}

void ff_spi_unregister_device(void)
{
    FF_LOGV("'%s' enter.", __func__);

    /* Unregister the spi device. */
    if (g_ff_spi_dev) {
        spi_master_put(g_ff_spi_dev->master);
        spi_unregister_device(g_ff_spi_dev);

        /* CHECKME: will spi_unregister_device(..) free the memory of 'spi_dev'? */
        /* kfree(spi_dev); */
        g_ff_spi_dev = NULL;
        FF_LOGI("spidev device has been unregistered.");
    }

    /* Release the master controller handle. */
    if (spi_ctl) {
        spi_master_put(spi_ctl);
        spi_ctl = NULL;
    }

    FF_LOGV("'%s' leave.", __func__);
}

