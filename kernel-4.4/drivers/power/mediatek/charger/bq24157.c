#include <linux/types.h>
#include <linux/init.h>		/* For init/exit macros */
#include <linux/module.h>	/* For MODULE_ marcros  */
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif
#if defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
#include <mt-plat/charger_class.h>
#include "mtk_charger_intf.h"
#endif /* #if (CONFIG_MTK_GAUGE_VERSION == 30) */

#include "bq24157.h"

struct BQ24157_INFO {
	struct device *dev;
	struct i2c_client *i2c;
#if defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
	struct charger_device *chg_dev;
#endif /* #if (CONFIG_MTK_GAUGE_VERSION == 30) */
};

struct BQ24157_INFO *bq24157_info = NULL;

static const struct i2c_device_id bq24157_i2c_id[] = { {"bq24157", 0}, {} };

bool chargin_hw_init_done = false;
static int bq24157_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);

#ifdef CONFIG_OF
static const struct of_device_id bq24157_of_match[] = {
	{.compatible = "mediatek,sw_charger",},
	{},
};

MODULE_DEVICE_TABLE(of, bq24157_of_match);
#endif

static struct i2c_driver bq24157_driver = {
	.driver = {
		   .name = "bq24157",
#ifdef CONFIG_OF
		   .of_match_table = bq24157_of_match,
#endif
	},
	.probe = bq24157_driver_probe,
	.id_table = bq24157_i2c_id,
};

/**********************************************************
  *
  *   [Global Variable]
  *
  *********************************************************/
unsigned char bq24157_reg[bq24157_REG_NUM] = { 0 };

static DEFINE_MUTEX(bq24157_i2c_access);

int g_bq24157_hw_exist = 0;
int g_bq24157_hw_ver = 0;

/**********************************************************
  *
  *   [I2C Function For Read/Write bq24157]
  *
  *********************************************************/
int bq24157_read_byte(unsigned char cmd, unsigned char *returnData)
{
	char cmd_buf[1] = { 0x00 };
	char readData = 0;
	int ret = 0;

	mutex_lock(&bq24157_i2c_access);

	cmd_buf[0] = cmd;
	ret = i2c_master_send(bq24157_info->i2c, &cmd_buf[0], 1);
	if (ret < 0) {
		mutex_unlock(&bq24157_i2c_access);
		return 0;
	}

	ret = i2c_master_recv(bq24157_info->i2c, &cmd_buf[0], 1);
	if (ret < 0) {
		mutex_unlock(&bq24157_i2c_access);
		return 0;
	}

	readData = cmd_buf[0];
	*returnData = readData;

	mutex_unlock(&bq24157_i2c_access);
	return 1;
}

int bq24157_write_byte(unsigned char cmd, unsigned char writeData)
{
	char write_data[2] = { 0 };
	int ret = 0;

	mutex_lock(&bq24157_i2c_access);

	write_data[0] = cmd;
	write_data[1] = writeData;

	ret = i2c_master_send(bq24157_info->i2c, write_data, 2);
	if (ret < 0) {
		mutex_unlock(&bq24157_i2c_access);
		return 0;
	}

	mutex_unlock(&bq24157_i2c_access);


	return 1;
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
unsigned int bq24157_read_interface(unsigned char RegNum, unsigned char *val, unsigned char MASK,
				  unsigned char SHIFT)
{
	unsigned char bq24157_reg = 0;
	int ret = 0;

	ret = bq24157_read_byte(RegNum, &bq24157_reg);

	bq24157_reg &= (MASK << SHIFT);
	*val = (bq24157_reg >> SHIFT);

	chr_info("[bq24157_read_interface] reg[%x] = 0x%x\n",RegNum, *val);

	return ret;
}

unsigned int bq24157_config_interface(unsigned char RegNum, unsigned char val, unsigned char MASK,
				  unsigned char SHIFT)
{
	unsigned char bq24157_reg = 0;
	int ret = 0;

	ret = bq24157_read_byte(RegNum, &bq24157_reg);

	bq24157_reg &= ~(MASK << SHIFT);
	bq24157_reg |= (val << SHIFT);

	if (RegNum == bq24157_CON4 && val == 1 && MASK == CON4_RESET_MASK
	    && SHIFT == CON4_RESET_SHIFT) {
		/* RESET bit */
	} else if (RegNum == bq24157_CON4) {
		bq24157_reg &= ~0x80;	/* RESET bit read returs 1, so clear it */
	}

	ret = bq24157_write_byte(RegNum, bq24157_reg);
	chr_info("[bq24157_config_interface] write Reg[%x]=0x%x\n", RegNum, bq24157_reg);

	return ret;
}

/* write one register directly */
unsigned int bq24157_reg_config_interface(unsigned char RegNum, unsigned char val)
{
	int ret = 0;

	ret = bq24157_write_byte(RegNum, val);

	return ret;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
/* CON0 */

void bq24157_set_tmr_rst(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_TMR_RST_MASK),
				       (unsigned char) (CON0_TMR_RST_SHIFT)
	    );
}

unsigned int bq24157_get_otg_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON0),
				     (&val), (unsigned char) (CON0_OTG_MASK),
				     (unsigned char) (CON0_OTG_SHIFT)
	    );
	return val;
}

void bq24157_set_en_stat(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_EN_STAT_MASK),
				       (unsigned char) (CON0_EN_STAT_SHIFT)
	    );
}

unsigned int bq24157_get_chip_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON0),
				     (&val), (unsigned char) (CON0_STAT_MASK),
				     (unsigned char) (CON0_STAT_SHIFT)
	    );
	return val;
}

unsigned int bq24157_get_boost_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON0),
				     (&val), (unsigned char) (CON0_BOOST_MASK),
				     (unsigned char) (CON0_BOOST_SHIFT)
	    );
	return val;
}

unsigned int bq24157_get_fault_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON0),
				     (&val), (unsigned char) (CON0_FAULT_MASK),
				     (unsigned char) (CON0_FAULT_SHIFT)
	    );
	return val;
}

/* CON1 */

void bq24157_set_input_charging_current(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_LIN_LIMIT_MASK),
				       (unsigned char) (CON1_LIN_LIMIT_SHIFT)
	    );
}

unsigned int bq24157_get_input_charging_current(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON1),
				     (&val), (unsigned char) (CON1_LIN_LIMIT_MASK),
				     (unsigned char) (CON1_LIN_LIMIT_SHIFT)
	    );
	return val;
}

void bq24157_set_v_low(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_LOW_V_MASK),
				       (unsigned char) (CON1_LOW_V_SHIFT)
	    );
}

void bq24157_set_te(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_TE_MASK),
				       (unsigned char) (CON1_TE_SHIFT)
	    );
}

void bq24157_set_ce(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_CE_MASK),
				       (unsigned char) (CON1_CE_SHIFT)
	    );
}

unsigned int bq24157_get_ce(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON1),
				     (&val), (unsigned char) (CON1_CE_MASK),
				     (unsigned char) (CON1_CE_SHIFT)
	    );

	return val;
}

void bq24157_set_hz_mode(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_HZ_MODE_MASK),
				       (unsigned char) (CON1_HZ_MODE_SHIFT)
	    );
}

void bq24157_set_opa_mode(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_OPA_MODE_MASK),
				       (unsigned char) (CON1_OPA_MODE_SHIFT)
	    );
}

/* CON2 */

void bq24157_set_oreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_OREG_MASK),
				       (unsigned char) (CON2_OREG_SHIFT)
	    );
}

unsigned int bq24157_get_oreg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON2),
				     (&val), (unsigned char) (CON2_OREG_MASK),
				     (unsigned char) (CON2_OREG_SHIFT)
	    );

	return val;
}

void bq24157_set_otg_pl(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_OTG_PL_MASK),
				       (unsigned char) (CON2_OTG_PL_SHIFT)
	    );
}

void bq24157_set_otg_en(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_OTG_EN_MASK),
				       (unsigned char) (CON2_OTG_EN_SHIFT)
	    );
}

/* CON3 */

unsigned int bq24157_get_vender_code(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON3),
				     (&val), (unsigned char) (CON3_VENDER_CODE_MASK),
				     (unsigned char) (CON3_VENDER_CODE_SHIFT)
	    );
	return val;
}

unsigned int bq24157_get_pn(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON3),
				     (&val), (unsigned char) (CON3_PIN_MASK),
				     (unsigned char) (CON3_PIN_SHIFT)
	    );
	return val;
}

unsigned int bq24157_get_revision(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON3),
				     (&val), (unsigned char) (CON3_REVISION_MASK),
				     (unsigned char) (CON3_REVISION_SHIFT)
	    );
	return val;
}

/* CON4 */

void bq24157_set_reset(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_RESET_MASK),
				       (unsigned char) (CON4_RESET_SHIFT)
	    );
}

void bq24157_set_iocharge(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_I_CHR_MASK),
				       (unsigned char) (CON4_I_CHR_SHIFT)
	    );
}

unsigned int bq24157_get_iocharge(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON4),
				     (&val), (unsigned char) (CON4_I_CHR_MASK),
				     (unsigned char) (CON4_I_CHR_SHIFT)
	    );
	return val;
}

void bq24157_set_iterm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_I_TERM_MASK),
				       (unsigned char) (CON4_I_TERM_SHIFT)
	    );
}


unsigned int bq24157_get_iterm(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON4),
				     (&val), (unsigned char) (CON4_I_TERM_MASK),
				     (unsigned char) (CON4_I_TERM_SHIFT)
	    );
	return val;
}


/* CON5 */

void bq24157_set_dis_vreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_DIS_VREG_MASK),
				       (unsigned char) (CON5_DIS_VREG_SHIFT)
	    );
}

void bq24157_set_io_level(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_IO_LEVEL_MASK),
				       (unsigned char) (CON5_IO_LEVEL_SHIFT)
	    );
}

unsigned int bq24157_get_sp_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON5),
				     (&val), (unsigned char) (CON5_SP_STATUS_MASK),
				     (unsigned char) (CON5_SP_STATUS_SHIFT)
	    );
	return val;
}

unsigned int bq241575_get_en_level(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface((unsigned char) (bq24157_CON5),
				     (&val), (unsigned char) (CON5_EN_LEVEL_MASK),
				     (unsigned char) (CON5_EN_LEVEL_SHIFT)
	    );
	return val;
}

void bq24157_set_vsp(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_VSP_MASK),
				       (unsigned char) (CON5_VSP_SHIFT)
	    );
}

/* CON6 */

void bq24157_set_i_safe(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_ISAFE_MASK),
				       (unsigned char) (CON6_ISAFE_SHIFT)
	    );
}

void bq24157_set_v_safe(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24157_config_interface((unsigned char) (bq24157_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VSAFE_MASK),
				       (unsigned char) (CON6_VSAFE_SHIFT)
	    );
}

/**********************************************************
  *
  *   [Charge Function]
  *
  *********************************************************/
#define STATUS_OK	0
#define STATUS_FAIL	1
#define STATUS_UNSUPPORTED	-1
#define GETARRAYNUM(array) (sizeof(array)/sizeof(array[0]))

const u32 VBAT_CV_VTH[] = {
	BATTERY_VOLT_03_500000_V, BATTERY_VOLT_03_520000_V, BATTERY_VOLT_03_540000_V, BATTERY_VOLT_03_560000_V,
	BATTERY_VOLT_03_580000_V, BATTERY_VOLT_03_600000_V, BATTERY_VOLT_03_620000_V, BATTERY_VOLT_03_640000_V,
	BATTERY_VOLT_03_660000_V, BATTERY_VOLT_03_680000_V, BATTERY_VOLT_03_700000_V, BATTERY_VOLT_03_720000_V,
	BATTERY_VOLT_03_740000_V, BATTERY_VOLT_03_760000_V, BATTERY_VOLT_03_780000_V, BATTERY_VOLT_03_800000_V,
	BATTERY_VOLT_03_820000_V, BATTERY_VOLT_03_840000_V, BATTERY_VOLT_03_860000_V, BATTERY_VOLT_03_880000_V,
	BATTERY_VOLT_03_900000_V, BATTERY_VOLT_03_920000_V, BATTERY_VOLT_03_940000_V, BATTERY_VOLT_03_960000_V,
	BATTERY_VOLT_03_980000_V, BATTERY_VOLT_04_000000_V, BATTERY_VOLT_04_020000_V, BATTERY_VOLT_04_040000_V,
	BATTERY_VOLT_04_060000_V, BATTERY_VOLT_04_080000_V, BATTERY_VOLT_04_100000_V, BATTERY_VOLT_04_120000_V,
	BATTERY_VOLT_04_140000_V, BATTERY_VOLT_04_160000_V, BATTERY_VOLT_04_180000_V, BATTERY_VOLT_04_200000_V,
	BATTERY_VOLT_04_220000_V, BATTERY_VOLT_04_240000_V, BATTERY_VOLT_04_260000_V, BATTERY_VOLT_04_280000_V,
	BATTERY_VOLT_04_300000_V, BATTERY_VOLT_04_320000_V, BATTERY_VOLT_04_340000_V, BATTERY_VOLT_04_360000_V,
	BATTERY_VOLT_04_380000_V, BATTERY_VOLT_04_400000_V, BATTERY_VOLT_04_420000_V, BATTERY_VOLT_04_440000_V
};

const u32 CS_VTH[] = {
	500000, 650000, 750000, 850000, 950000, 1050000, 1150000, 1250000//modified by rongxiao.deng from 550->500mA for task 5397893 on 2017/10/12
};

const u32 INPUT_CS_VTH[] = {
	100000, 500000, 800000
};

const u32 VCDT_HV_VTH[] = {
	BATTERY_VOLT_04_200000_V, BATTERY_VOLT_04_250000_V, BATTERY_VOLT_04_300000_V, BATTERY_VOLT_04_350000_V,
	BATTERY_VOLT_04_400000_V, BATTERY_VOLT_04_450000_V, BATTERY_VOLT_04_500000_V, BATTERY_VOLT_04_550000_V,
	BATTERY_VOLT_04_600000_V, BATTERY_VOLT_06_000000_V, BATTERY_VOLT_06_500000_V, BATTERY_VOLT_07_000000_V,
	BATTERY_VOLT_07_500000_V, BATTERY_VOLT_08_500000_V, BATTERY_VOLT_09_500000_V, BATTERY_VOLT_10_500000_V
};

u32 charging_value_to_parameter(const u32 *parameter, const u32 array_size, const u32 val)
{
	if (val < array_size)
		return parameter[val];
	chr_err("Can't find the parameter \r\n");
	return parameter[0];
}

u32 charging_parameter_to_value(const u32 *parameter, const u32 array_size, const u32 val)
{
	u32 i;

	for (i = 0; i < array_size; i++)
		if (val == *(parameter + i))
			return i;

	chr_err("NO register value match \r\n");

	return 0;
}

static u32 bmt_find_closest_level(const u32 *pList, u32 number, u32 level)
{
	u32 i;
	u32 max_value_in_last_element;

	if (pList[0] < pList[1])
		max_value_in_last_element = true;
	else
		max_value_in_last_element = false;

	if (max_value_in_last_element == true) {
		for (i = (number - 1); i != 0; i--)	/* max value in the last element */
		{
			if (pList[i] <= level)
				return pList[i];
		}

		chr_err("Can't find closest level, small value first \r\n");
		return pList[0];
		/* return CHARGE_CURRENT_0_00_MA; */
	} else {
		for (i = 0; i < number; i++)	/* max value in the first element */
			if (pList[i] <= level)
				return pList[i];

		chr_err("Can't find closest level, large value first \r\n");
		return pList[number - 1];
		/* return CHARGE_CURRENT_0_00_MA; */
	}
}

static u32 bq24157_charging_hw_init(void)
{
	u32 status = STATUS_OK;

	chr_err("bq24157 charging_hw_init\r\n");
	
	// Modified init parameters by bin.song.hz for 5301378 at 2017.8.30 begin
    bq24157_reg_config_interface(0x06, 0x5A);       /* B[7:4].I_max=1050mA,  B[3:0].V_max=4.4V */
    bq24157_reg_config_interface(0x02, 0xB6);       /* B[7:2].Voreg=4.4V, B[1].OTG_PL=1, B[0].OTG_EN=0 */
    bq24157_reg_config_interface(0x00, 0xC0);       /* B[7].TMR_RST=1, B[6].EN_STAT=1 */
    bq24157_reg_config_interface(0x01, 0xC8);       /* B[7:6].Iin_Limit=unlimit, B[5:4].Vlow=3.4V, B[3].TE=1, B[2].CE=0, B[1].HZ_MODE=0, B[0].OPA_MODE=0 */
    bq24157_reg_config_interface(0x05, 0x06);       /* B[5].LOW_CHG=0, B[2:0].Vsp = 4.68V */
    bq24157_reg_config_interface(0x04, 0x52);       /* B[6:4].I_chrg=1050mA, B[2:0].I_term.150mA*/
	// Modified init parameters by bin.song.hz for 5301378 at 2017.8.30 end

	return status;
}

static int bq24157_charger_enable(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	chr_err("bq24157_charger_enable %d\r\n", en);

	if (true == enable) {
		bq24157_set_ce(0);
		bq24157_set_hz_mode(0);
		bq24157_set_opa_mode(0);
	} else {
		bq24157_set_ce(1);
		bq24157_set_hz_mode(1);
	}

	return status;
}

static int bq24157_charger_is_enabled(struct charger_device *chg_dev, bool *en)
{
	*en = bq24157_get_ce();
	chr_err("bq24157_charger_is_enabled %d\r\n", *en);
	return 0;
}

static int bq24157_charger_get_ichg(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;

	/* Get current level */
	array_size = GETARRAYNUM(CS_VTH);
	reg_value = bq24157_get_iocharge();
	*(u32 *) uA = charging_value_to_parameter(CS_VTH, array_size, reg_value);

	chr_err("bq24157_charger_get_ichg %dmA\r\n", *uA/1000);

	return status;
}

static int bq24157_charger_set_ichg(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u32 set_chr_current;
	u32 array_size;
	u32 register_value;

	if (uA <= 350000) {
		bq24157_set_io_level(1);
	} else {
		bq24157_set_io_level(0);
		array_size = GETARRAYNUM(CS_VTH);
		set_chr_current = bmt_find_closest_level(CS_VTH, array_size, uA);
		register_value = charging_parameter_to_value(CS_VTH, array_size, set_chr_current);
		bq24157_set_iocharge(register_value);
	}

	chr_err("bq24157_charger_set_ichg %dmA\r\n", set_chr_current/1000);

	return status;
}

static int bq24157_charger_get_min_ichg(struct charger_device *chg_dev, u32 *uA)
{
	*uA = CS_VTH[0];
	chr_err("bq24157_charger_get_min_ichg %dmA\r\n", *uA/1000);
	return 0;
}

static int bq24157_charger_set_cv(struct charger_device *chg_dev, u32 uV)
{
	u32 status = STATUS_OK;
	u16 register_value;
	u32 set_cv_voltage;	
	u32 array_size;

	array_size = GETARRAYNUM(VBAT_CV_VTH);
	set_cv_voltage = bmt_find_closest_level(VBAT_CV_VTH, array_size, uV);
	register_value = charging_parameter_to_value(VBAT_CV_VTH, array_size, set_cv_voltage);
	bq24157_set_oreg(register_value);

	chr_err("bq24157_charger_set_cv %dmV\r\n", set_cv_voltage/1000);
	return status;
}

static int bq24157_charger_get_cv(struct charger_device *chg_dev, u32 *uV)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;

	/* Get current level */
	array_size = GETARRAYNUM(VBAT_CV_VTH);
	reg_value = bq24157_get_oreg();
	*(u32 *) uV = charging_value_to_parameter(VBAT_CV_VTH, array_size, reg_value);

	chr_err("bq24157_charger_get_cv %dmV\r\n", *uV/1000);

	return status;
}

static int bq24157_charger_set_aicr(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u32 register_value;

	if (uA > 800000) {
		register_value = 0x3;
	} 
	else if(uA > 500000 )
	{
		register_value = 0x2;
	}
	else if(uA > 100000)
	{
		register_value = 0x1;
	}
	else
	{
		register_value = 0x0;
	}

	bq24157_set_input_charging_current(register_value);

	chr_err("bq24157_charger_set_aicr %dmA\r\n", uA/1000);

	return status;
}

static int bq24157_charger_get_aicr(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;

	/* Get current level */
	reg_value = bq24157_get_input_charging_current();
	switch(reg_value)
	{
	case 3:
		*(u32 *) uA = 1000000;
		break;
	case 2:
		*(u32 *) uA = 800000;
		break;	
	case 1:
		*(u32 *) uA = 500000;
		break;	
	case 0:
		*(u32 *) uA = 100000;
		break;	
	default:
		*(u32 *) uA = 0;
		break;								
	}

	chr_err("bq24157_charger_get_aicr %dmA\r\n", *uA/1000);

	return status;
}

static int bq24157_charger_get_min_aicr(struct charger_device *chg_dev, u32 *uA)
{
	*uA = INPUT_CS_VTH[0];
	chr_err("bq24157_charger_get_min_aicr %dmA\r\n", *uA/1000);
	return 0;
}

static int bq24157_charger_set_eoc_current(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;
	if(uA >= 400000)
		reg_value = 0x07;
	else if(uA <=50000)
		reg_value = 0;
	else
		reg_value = uA/50000 - 1;

	bq24157_set_iterm(reg_value);

	chr_err("bq24157_charger_set_eoc_current %dmA\r\n", (reg_value+1)*50);

	return status;
}

static int bq24157_charger_get_eoc_current(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;

	reg_value = bq24157_get_iterm();
	*uA = (reg_value+1) * 50000;

	return status;
}


static int bq24157_charger_enable_te(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	chr_err("bq24157_charger_enable_te %d\r\n", en);

	if (true == enable) {
		bq24157_set_te(1);
	} else {
		bq24157_set_te(0);
	}

	return status;
}

static int bq24157_charger_enable_otg(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	chr_err("bq24157_charger_enable_otg %d\r\n", en);

	if (true == enable) {
		bq24157_set_hz_mode(0);
		bq24157_set_opa_mode(1);
		bq24157_set_otg_pl(1);
		bq24157_set_otg_en(1);	
	} else {
		bq24157_set_otg_pl(0);
		bq24157_set_otg_en(0);
		bq24157_set_opa_mode(0);
		bq24157_set_hz_mode(1);
	}

	return status;
}

static int bq24157_charger_is_charging_done(struct charger_device *chg_dev, bool *done)
{
	u32 status = STATUS_OK;
	u32 ret_val;

	ret_val = bq24157_get_chip_status();

	if (ret_val == 0x2)
		* done = true;//Modified by rongxiao.deng for task 5642329 on 2017/11/24
	else
		* done = false;//Modified by rongxiao.deng for task 5642329 on 2017/11/24

	chr_err("bq24157_charger_is_charging_done %d\r\n", *done);

	return status;
}

static int bq24157_charger_dump_registers(struct charger_device *chg_dev)
{
	u32 status = STATUS_OK;

	bq24157_dump_register();

	return status;
}

static int bq24157_do_event(struct charger_device *chg_dev, u32 event, u32 args)
{
	if (chg_dev == NULL)
		return -EINVAL;

	chr_err("%s: event = %d\n", __func__, event);
	
	switch (event) {
	case EVENT_EOC:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_EOC);
		break;
	case EVENT_RECHARGE:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_RECHG);
		break;
	default:
		break;
	}
	return 0;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/

#define VENDOR_CODE_BQ24157   0x50
#define VENDOR_CODE_MASK      0xF8
#define IC_VER_MASK           0x07
  
void bq24157_hw_component_detect(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24157_read_interface(0x03, &val, 0xFF, 0x0);

	if((val&VENDOR_CODE_MASK) == VENDOR_CODE_BQ24157)
		g_bq24157_hw_exist = 1;
	else
		g_bq24157_hw_exist = 0;

    g_bq24157_hw_ver = val & IC_VER_MASK;

	chr_err("[bq24157_hw_component_detect] exist=%d, Reg[03]=0x%x\n", g_bq24157_hw_exist, val);
}

int is_bq24157_exist(void)
{
	chr_err("[is_bq24157_exist] g_bq24157_hw_exist=%d\n", g_bq24157_hw_exist);

	return g_bq24157_hw_exist;
}

void bq24157_dump_register(void)
{
	int i = 0;

	chr_err("[bq24157]\n");
	for (i = 0; i < bq24157_REG_NUM; i++) {
		bq24157_read_byte(i, &bq24157_reg[i]);
		chr_err("[0x%x]=0x%x\n", i, bq24157_reg[i]);
	}
}

static const struct charger_ops bq24157_chg_ops = {
	/* enable */
	.enable = bq24157_charger_enable,
	.is_enabled = bq24157_charger_is_enabled,
	/* charging current */
	.get_charging_current = bq24157_charger_get_ichg,
	.set_charging_current = bq24157_charger_set_ichg,
	.get_min_charging_current = bq24157_charger_get_min_ichg,
	/* charging voltage */
	.set_constant_voltage = bq24157_charger_set_cv,
	.get_constant_voltage = bq24157_charger_get_cv,
	/* charging input current */
	.get_input_current = bq24157_charger_get_aicr,
	.set_input_current = bq24157_charger_set_aicr,
	.get_min_input_current = bq24157_charger_get_min_aicr,
	/* set termination current */
	.get_eoc_current = bq24157_charger_get_eoc_current,
	.set_eoc_current = bq24157_charger_set_eoc_current,
	/* charing termination */
	.enable_termination = bq24157_charger_enable_te,
	/* OTG */
	.enable_otg = bq24157_charger_enable_otg,
	/* misc */
	.is_charging_done = bq24157_charger_is_charging_done,
	.dump_registers = bq24157_charger_dump_registers,
	/* Event */
	.event = bq24157_do_event,	
};

static const struct charger_properties bq24157_chg_props = {
	.alias_name = "bq24157",
};

extern char charger_module_name[256];//Modified by hailong.chen for task 5662647 cancel the annotation to adding charge ic deviceinfo on 2017/11/28

static int bq24157_driver_probe(struct i2c_client *i2c, const struct i2c_device_id *id)
{
	chr_err("******** bq24157_driver_probe!! ********\n");

	bq24157_info = devm_kzalloc(&i2c->dev, sizeof(*bq24157_info), GFP_KERNEL);
	if (!bq24157_info)
		return -ENOMEM;

	bq24157_info->dev = &i2c->dev;
	bq24157_info->i2c = i2c;
	i2c_set_clientdata(i2c, bq24157_info);
	
	mutex_init(&bq24157_i2c_access);

	bq24157_reg_config_interface(0x06, 0x7A);
	bq24157_hw_component_detect();
	
	if(g_bq24157_hw_exist == 0)
	    return -ENODEV;
	    
	bq24157_charging_hw_init();

#if defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
	/* charger class register */
	bq24157_info->chg_dev = charger_device_register("primary_chg", 
                                              bq24157_info->dev, 
                                              bq24157_info,
					      &bq24157_chg_ops,
					      &bq24157_chg_props);
	if (IS_ERR(bq24157_info->chg_dev)) {
		dev_info(bq24157_info->dev, "charger device register fail\n");
		return PTR_ERR(bq24157_info->chg_dev);
	}
#endif /* #if (CONFIG_MTK_GAUGE_VERSION == 30) */


	bq24157_dump_register();
	chargin_hw_init_done = true;
    //Begin Modified by hailong.chen for task 5662647 cancel the annotation to adding charge ic deviceinfo on 2017/11/28
    sprintf(charger_module_name,"BQ24157:TI:0x%x", g_bq24157_hw_ver);
    //End Modified by hailong.chen for task 5662647 cancel the annotation to adding charge ic deviceinfo on 2017/11/28
	return 0;
}
/**********************************************************
  *
  *   [platform_driver API]
  *
  *********************************************************/
unsigned char g_reg_value_bq24157 = 0;
static ssize_t show_bq24157_access(struct device *dev, struct device_attribute *attr, char *buf)
{
	chr_err("[show_bq24157_access] 0x%x\n", g_reg_value_bq24157);
	return sprintf(buf, "%u\n", g_reg_value_bq24157);
}

static ssize_t store_bq24157_access(struct device *dev, struct device_attribute *attr,
				    const char *buf, size_t size)
{
	int ret = 0;
	char *pvalue = NULL, *addr, *val;
	unsigned int reg_value = 0;
	unsigned int reg_address = 0;

	chr_err("[store_bq24157_access]\n");

	if (buf != NULL && size != 0) {

		pvalue = (char *)buf;
		if (size > 3) {
			addr = strsep(&pvalue, " ");
			ret = kstrtou32(addr, 16, (unsigned int *)&reg_address);
		} else
			ret = kstrtou32(pvalue, 16, (unsigned int *)&reg_address);

		if (size > 3) {
			val = strsep(&pvalue, " ");
			ret = kstrtou32(val, 16, (unsigned int *)&reg_value);

			chr_err("[store_bq24157_access] write bq24157 reg 0x%x with value 0x%x !\n",
			     reg_address, reg_value);
			ret = bq24157_config_interface(reg_address, reg_value, 0xFF, 0x0);
		} else {
			ret = bq24157_read_interface(reg_address, &g_reg_value_bq24157, 0xFF, 0x0);
			chr_err("[store_bq24157_access] read bq24157 reg 0x%x with value 0x%x !\n",
			     reg_address, g_reg_value_bq24157);
			chr_err("[store_bq24157_access] Please use \"cat bq24157_access\" to get value\r\n");
		}
	}
	return size;
}

static DEVICE_ATTR(bq24157_access, 0664, show_bq24157_access, store_bq24157_access);	/* 664 */

static int bq24157_user_space_probe(struct platform_device *dev)
{
	int ret_device_file = 0;

	chr_err("******** bq24157_user_space_probe!! ********\n");

	ret_device_file = device_create_file(&(dev->dev), &dev_attr_bq24157_access);

	return 0;
}

struct platform_device bq24157_user_space_device = {
	.name = "bq24157-user",
	.id = -1,
};

static struct platform_driver bq24157_user_space_driver = {
	.probe = bq24157_user_space_probe,
	.driver = {
		   .name = "bq24157-user",
	},
};

static int __init bq24157_init(void)
{
	int ret = 0;

	if (i2c_add_driver(&bq24157_driver) != 0) {
		chr_err("[bq24157_init] failed to register bq24157 i2c driver.\n");
	} else {
		chr_err("[bq24157_init] Success to register bq24157 i2c driver.\n");
	}

    //Begin Modified by hailong.chen for task 5662647 cancel the annotation to make the charging ic compatible on 2017/11/28
    if(g_bq24157_hw_exist == 0)
    {
        chr_err("[bq24157_init] bq24157 is not exist, delete bq24157 i2c driver.\n");
        i2c_del_driver(&bq24157_driver);
        return 0;
    }
    //End Modified by hailong.chen for task 5662647 cancel the annotation to make the charging ic compatible on 2017/11/28

	/* bq24157 user space access interface */
	ret = platform_device_register(&bq24157_user_space_device);
	if (ret) {
		chr_err("****[bq24157_init] Unable to device register(%d)\n", ret);
		return ret;
	}
	ret = platform_driver_register(&bq24157_user_space_driver);
	if (ret) {
		chr_err("****[bq24157_init] Unable to register driver (%d)\n", ret);
		return ret;
	}

	return 0;
}

static void __exit bq24157_exit(void)
{
	i2c_del_driver(&bq24157_driver);
}

module_init(bq24157_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C bq24157 Driver");
MODULE_AUTHOR("James Lo<james.lo@mediatek.com>");
