/*
 * Copyright (C) 2010 Trusted Logic S.A.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * history-1: Added by meng.zhang for task 5424939 on 2017/10/21 
 *			this driver is copied from longfang.liu, original check-in is: http://10.92.32.10:8081/#/c/168380/ 
 *
 */

#define PN553_MAGIC	0xE9

/*
 * PN553 power control via ioctl
 * PN553_SET_PWR(0): power off
 * PN553_SET_PWR(1): power on
 * PN553_SET_PWR(2): reset and power on with firmware download enabled
 */
#define PN553_SET_PWR	_IOW(PN553_MAGIC, 0x01, unsigned int)

struct PN553_i2c_platform_data{
	unsigned int irq_gpio;
	unsigned int ven_gpio;
	unsigned int firm_gpio;
};
