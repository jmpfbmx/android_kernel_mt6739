
#ifndef _AUDDRV_BTCVSD_IOCTL_MSG_H
#define _AUDDRV_BTCVSD_IOCTL_MSG_H





/*below is control message*/
#define AUD_DRV_BTCVSD_IOC_MAGIC 'C'

#define ALLOCATE_FREE_BTCVSD_BUF _IOWR(AUD_DRV_BTCVSD_IOC_MAGIC, 0xE0, unsigned int)
#define SET_BTCVSD_STATE         _IOWR(AUD_DRV_BTCVSD_IOC_MAGIC, 0xE1, unsigned int)
#define GET_BTCVSD_STATE         _IOWR(AUD_DRV_BTCVSD_IOC_MAGIC, 0xE2, unsigned int)
#define GET_BTCVSD_RX_TIME_BUFFER_INFO _IOWR(AUD_DRV_BTCVSD_IOC_MAGIC, 0xE3, unsigned long long)
#define GET_BTCVSD_TX_TIME_BUFFER_INFO _IOWR(AUD_DRV_BTCVSD_IOC_MAGIC, 0xE4, unsigned long long)






#endif

