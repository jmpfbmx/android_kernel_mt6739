
#ifndef __MODEM_REG_BASE_H__
#define __MODEM_REG_BASE_H__

/* ============================================================ */
/* Modem 1 part */
/* ============================================================ */
/* MD peripheral register: MD bank8; AP bank2 */
#define MD_BOOT_VECTOR_EN 0x20000024

#define MD_GLOBAL_CON0 0x20000450
#define MD_GLOBAL_CON0_CLDMA_BIT 12
/* #define MD_PEER_WAKEUP 0x20030B00 */
#define MD_TOPSM_STATUS_BASE 0x200D0000
#define MD_TOPSM_STATUS_LENGTH  0xC2C
#define MD_PC_MONITOR_BASE 0x0D0E3000
#define MD_PC_MONITOR_LENGTH 0x500
#define MD_PC_MONITORL1_BASE 0x0D0DF000
#define MD_PC_MONITORL1_LENGTH 0x500
#define MD_BUS_STATUS_BASE 0x203B0000
#define MD_BUS_STATUS_LENGTH 0x12C
#define MD_BUS_DUMP_ADDR1   0x20060000
#define MD_BUS_DUMP_LEN1    (0xF4)
#define MD_BUS_DUMP_ADDR2   0x20200000
#define MD_BUS_DUMP_LEN2    (0xE0)

/*BUS REC*/
#define MD_BUSREC_DUMP_ADDR		(0x0D0E5000)
#define MD_BUSREC_DUMP_LEN		(0x418)
#define MD_BUSREC_L1_DUMP_ADDR	(0x0D0DE000)
#define MD_BUSREC_L1_DUMP_LEN	(0xC18)
/*OST STATUS*/
#define MD_OST_STATUS_BASE  0x200E0000
#define MD_OST_STATUS_LENGTH  0x224

/*open dbg sys*/
#define MD_DBGSYS_CLK_ADDR   0x200D0A00
#define MD_DBGSYS_CLK_LEN    (0x30)

/*ECT*/
#define MD_ECT_DUMP_ADDR1   0x0D0E8130
#define MD_ECT_DUMP_LEN1    (8)
#define MD_ECT_DUMP_ADDR2   0x0D0DB130
#define MD_ECT_DUMP_LEN2    (8)
#define MD_ECT_DUMP_ADDR3   0x0D0DC130
#define MD_ECT_DUMP_LEN3    (8)

#define CCIF_SRAM_SIZE 512

/*-------------Modem WDT ---------------*/
/* BASE_ADDR_MDRSTCTL */
#define BASE_ADDR_MDRSTCTL   0x200f0000  /* From md, no use by AP directly */
#define MD_RGU_BASE          (BASE_ADDR_MDRSTCTL + 0x100)  /* AP use */
#define REG_MDRSTCTL_WDTCR            (0x0000) /*WDT_MODE*/
#define REG_MDRSTCTL_WDTRR            (0x0010) /*WDT_RESTART*/
#define REG_MDRSTCTL_WDTIR            (0x023C) /*LENGTH*/
#define REG_MDRSTCTL_WDTSR            (0x0034) /*WDT_STATUS*/
#define WDT_MD_MODE		REG_MDRSTCTL_WDTCR
#define WDT_MD_LENGTH           REG_MDRSTCTL_WDTIR
#define WDT_MD_RESTART          REG_MDRSTCTL_WDTRR
#define WDT_MD_STA              REG_MDRSTCTL_WDTSR
#define WDT_MD_MODE_KEY	        (0x55000008)

/* L1_BASE_ADDR_L1RGU */
#define L1_BASE_ADDR_L1RGU   0x26010000  /* From md, no use by AP directly  */
#define L1_RGU_BASE          L1_BASE_ADDR_L1RGU    /* AP use */
#define REG_L1RSTCTL_WDT_MODE         (0x0000)
#define REG_L1RSTCTL_WDT_LENGTH       (0x0004)
#define REG_L1RSTCTL_WDT_RESTART      (0x0008)
#define REG_L1RSTCTL_WDT_STA          (0x000C)
#define REG_L1RSTCTL_WDT_SWRST        (0x001C)
#define L1_WDT_MD_MODE_KEY	(0x00002200)

/*-------------------MD1 PLL----------------------*/
/* MD_CLKSW_BASE */
#define MD_CLKSW_BASE	(0x20150000)
#define MD_CLKSW_LENGTH  0x118
#define R_CLKSEL_CTL			(0x0024)
#define R_FLEXCKGEN_SEL0		(0x0028)
#define R_FLEXCKGEN_SEL1		(0x002C)
#define R_FLEXCKGEN_SEL2		(0x0044)
/* Bit  0: PSMCUPLL_RDY, Bit  1: L1MCUPLL_RDY */
#define R_PLL_STS			(0x0040)
#define R_FLEXCKGEN_STS0		(0x0030)
#define R_FLEXCKGEN_STS1		(0x0034)
#define R_FLEXCKGEN_STS2		(0x0048)
	/* Bit  15: INTF_CK_RDY, Bit  23: MD2G_CK_RDY */


/* MD_GLOBAL_CON_DCM_BASE*/
#define MD_GLOBAL_CON_DCM_BASE	(0x20130000)
#define MD_GLOBAL_CON_DCM_LEN  0x1004
/* Bit 26-20: DBC_CNT, Bit 16-12: IDLE_FSEL, Bit 2: DBC_EN, Bit 1: DCM_EN */
#define R_PSMCU_DCM_CTL0		(0x0010)
/* Bit 5: APB_LOAD_TOG, Bit 4-0: APB_SEL */
#define R_PSMCU_DCM_CTL1		(0x0014)
/* Bit 26-20: DBC_CNT, Bit 16-12: IDLE_FSEL, Bit 2: DBC_EN, Bit 1: DCM_EN */
#define R_ARM7_DCM_CTL0			(0x0020)
#define R_ARM7_DCM_CTL1			(0x0024)/* Bit	5: APB_LOAD_TOG, Bit 4-0: APB_SEL */
#define MD_GLOBAL_CON_DUMMY		(0x1000)
#define MD_PLL_MAGIC_NUM		(0x67550000)

/* MDSYS_CLKCTL_BASE*/
#define MDSYS_CLKCTL_BASE	(0x20120000)
#define MDSYS_CLKCTL_LEN   0xD0
#define R_DCM_SHR_SET_CTL		(0x0004)
#define R_LTEL2_BUS_DCM_CTL		(0x0010)
#define R_MDDMA_BUS_DCM_CTL		(0x0014)
#define R_MDREG_BUS_DCM_CTL		(0x0018)
#define R_MODULE_BUS2X_DCM_CTL		(0x001C)
#define R_MODULE_BUS1X_DCM_CTL		(0x0020)
#define R_MDINFRA_CKEN			(0x0044)
#define R_MDPERI_CKEN			(0x0048)
#define R_MDPERI_DCM_MASK		(0x0064)
#define R_PSMCU_AO_CLK_CTL		(0x00C0)

/* PMDL1A0_BASE+ */
#define REG_DCM_PLLCK_SEL		(0x0188) /* Bit 7: 0: clock do not from PLL, 1: clock from PLL */
#define R_L1MCU_PWR_AWARE		(0x0190)
#define R_L1AO_PWR_AWARE		(0x0194)
#define R_BUSL2DCM_CON3			(0x0198)
#define R_L1MCU_DCM_CON2		(0x0184)
#define R_L1MCU_DCM_CON			(0x0180)

#define MD_Clkctrl_DUMP_ADDR01    0x20150100
#define MD_Clkctrl_DUMP_LEN01  0x18
#define MD_Clkctrl_DUMP_ADDR02    0x20151000
#define MD_Clkctrl_DUMP_LEN02  0x4
#define MD_Clkctrl_DUMP_ADDR03    0x20140000
#define MD_Clkctrl_DUMP_LEN03  0x9C
#define MD_Clkctrl_DUMP_ADDR04    0x20140200
#define MD_Clkctrl_DUMP_LEN04  0x8
#define MD_Clkctrl_DUMP_ADDR05    0x20140300
#define MD_Clkctrl_DUMP_LEN05  0x10
#define MD_Clkctrl_DUMP_ADDR06    0x20140400
#define MD_Clkctrl_DUMP_LEN06  0x138
#define MD_Clkctrl_DUMP_ADDR07    0x20140600
#define MD_Clkctrl_DUMP_LEN07  0x34
#define MD_Clkctrl_DUMP_ADDR08    0x20140800
#define MD_Clkctrl_DUMP_LEN08  0x8
#define MD_Clkctrl_DUMP_ADDR09    0x20130000
#define MD_Clkctrl_DUMP_LEN09  0x50
#define MD_Clkctrl_DUMP_ADDR10    0x20130080
#define MD_Clkctrl_DUMP_LEN10  0x90
#define MD_Clkctrl_DUMP_ADDR11    0x20130400
#define MD_Clkctrl_DUMP_LEN11  0x4
#define MD_Clkctrl_DUMP_ADDR12    0x20130800
#define MD_Clkctrl_DUMP_LEN12  0x8
#define MD_Clkctrl_DUMP_ADDR13    0x20130C00
#define MD_Clkctrl_DUMP_LEN13  0x10
#define MD_Clkctrl_DUMP_ADDR14    0x20131000
#define MD_Clkctrl_DUMP_LEN14  0x4
#define MD_Clkctrl_DUMP_ADDR15    0x20120000
#define MD_Clkctrl_DUMP_LEN15  0xC4

/* MD1 PLL */
#define MD_PERI_MISC_BASE	(0x20060000)
#define MD_PERI_MISC_LEN	0xD0
/* MD_PERI_MISC_BASE+ */
#define R_L1_PMS			(0x00C4)

#define MDL1A0_BASE			(0x260F0000)
#define MDL1A0_LEN			0x200
#define MDTOP_PLLMIXED_BASE	(0x20140000)

/*MD_PERI_MISC_BASE+*/
#define R_DBGSYS_CLK_PMS_1	(0x0094)
#define R_DBGSYS_CLK_PMS_2	(0x009C)


/*MD bootup register*/
#define MD1_CFG_BASE (0x1020E000) /*=Infra cfg base*/
#define MD1_CFG_BOOT_STATS0 (0x300)/*MD1_CFG_BASE +0x300*/
#define MD1_CFG_BOOT_STATS1 (0x304)/*MD1_CFG_BASE +0x304*/

/*MD PCore SRAM register*/
#define MD_SRAM_PMS_BASE	(0x20060000)
#define MD_SRAM_PMS_LEN		(0xD0)
#define MD_SRAM_MDSYS_MD_PMS		(0x80)
#define MD_SRAM_MDPERISYS1_MD_PMS	(0x84)
#define MD_SRAM_MDPERISYS2_MD_PMS	(0x88)
#define MD_SRAM_PSMCUAPB_MD_PMS		(0x8C)
#define MD_SRAM_MDSYS_AP_PMS		(0x90)
#define MD_SRAM_MDPERISYS1_AP_PMS	(0x94)
#define MD_SRAM_MDPERISYS2_AP_PMS	(0x98)
#define MD_SRAM_PSMCUAPB_AP_PMS		(0x9C)
#define MD_SRAM_MDSYS_TZ_PMS		(0xA0)
#define MD_SRAM_MDPERISYS1_TZ_PMS	(0xA4)
#define MD_SRAM_MDPERISYS2_TZ_PMS	(0xA8)
#define MD_SRAM_PSMCUAPB_TZ_PMS		(0xAC)
#define MD_SRAM_MDSYS_L1_PMS		(0xB0)
#define MD_SRAM_MDPERISYS1_L1_PMS	(0xB4)
#define MD_SRAM_MDPERISYS2_L1_PMS	(0xB8)
#define MD_SRAM_PSMCUAPB_L1_PMS		(0xBC)
#define MD_SRAM_L1SYS_PMS			(0xC4)

#define MD_SRAM_PD_PSMCUSYS_SRAM_BASE	(0x200D0100)
#define MD_SRAM_PD_PSMCUSYS_SRAM_LEN	(0x30)
#define MD_SRAM_PD_PSMCUSYS_SRAM_1		(0x14)
#define MD_SRAM_PD_PSMCUSYS_SRAM_2		(0x18)
#define MD_SRAM_PD_PSMCUSYS_SRAM_3		(0x1C)
#define MD_SRAM_PD_PSMCUSYS_SRAM_4		(0x20)

/* ============================================================ */
/* Modem 2 part */
/* ============================================================ */
#define MD2_BOOT_VECTOR 0x30190000
#define MD2_BOOT_VECTOR_KEY 0x3019379C
#define MD2_BOOT_VECTOR_EN 0x30195488

#define MD2_BOOT_VECTOR_VALUE 0x00000000
#define MD2_BOOT_VECTOR_KEY_VALUE 0x3567C766
#define MD2_BOOT_VECTOR_EN_VALUE 0xA3B66175
#define MD2_RGU_BASE 0x30050000
#define APCCIF1_SRAM_SIZE 512

#define MD3_BOOT_VECTOR 0x30190000
#define MD3_BOOT_VECTOR_KEY 0x3019379C
#define MD3_BOOT_VECTOR_EN 0x30195488

#define MD3_BOOT_VECTOR_VALUE		0x00000000
#define MD3_BOOT_VECTOR_KEY_VALUE	0x3567C766
#define MD3_BOOT_VECTOR_EN_VALUE	0xA3B66175

#define MD3_RGU_BASE			0x3A001080
#define APCCIF1_SRAM_SIZE		512

#endif				/* __MODEM_REG_BASE_H__ */
