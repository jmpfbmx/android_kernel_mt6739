

#ifndef _fan53526_SW_H_
#define _fan53526_SW_H_

#define fan53526_REG_NUM 6

extern int is_fan53526_exist(void);
extern int is_fan53526_sw_ready(void);
extern void fan53526_dump_register(void);
extern unsigned int fan53526_read_interface(unsigned char RegNum, unsigned char *val,
					    unsigned char MASK, unsigned char SHIFT);
extern unsigned int fan53526_config_interface(unsigned char RegNum, unsigned char val,
					      unsigned char MASK, unsigned char SHIFT);
/*extern int fan53526_vosel(unsigned long val);*/
extern int fan53526_enable(unsigned char en);
extern int fan53526_set_voltage(unsigned long val);
extern int fan53526_set_mode(unsigned char mode);
extern unsigned int fan53526_read_byte(unsigned char cmd, unsigned char *returnData);
extern int is_fan53526_sw_ready(void);
extern int get_fan53526_i2c_ch_num(void);

#endif				/* _fan53526_SW_H_ */
