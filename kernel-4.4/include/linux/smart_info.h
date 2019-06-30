#ifndef _SMART_INFO_H
#define _SMART_INFO_H
#ifdef CONFIG_TCT_BIGLOG

/* includes */
#include <linux/kernel.h>

/* macros */
/*
 * structure of the smart partition:
 *
 * /------flag region:       1K--------------/
 * /------tool data:         4M-1K-----------/
 * /------big data:          4M--------------/
 * /------pl log region:     1M--------------/
 * /------lk log region:     1M--------------/
 * /------kernel log region: 5M--------------/
 * /------fec data:          1M--------------/
 */
#define AFTER_SALE_FLAG_OFFSET_ADDR     (0)
#define VERSION_OFFSET_ADDR             (4)

#define TOOL_DATA_OFFSET_ADDR           (1024)
#define LIFE_TIME_INFO_ADDR             (1066)

#define BIG_DATA_OFFSET_ADDR            (4*1024*1024)
#define SYSTEM_ABNORMAL_WRITE_ADDR      (BIG_DATA_OFFSET_ADDR)
#define PART_DAMAGE_RECORD_OFFSET_ADDR  (BIG_DATA_OFFSET_ADDR+128)
#define FEC_TIME_RECORD_ADDR            (BIG_DATA_OFFSET_ADDR+128+24)
#define LK_LAUCHER_TIME_RECORD_ADDR     (BIG_DATA_OFFSET_ADDR+128+24+60)
#define E2FSCK_INFO_ADDR                (BIG_DATA_OFFSET_ADDR+128+24+60+800)
#define DATA_RO_INFO_ADDR               (BIG_DATA_OFFSET_ADDR+128+24+60+800+22)
#define BOOT_MODE_AND_REASON_ADDR       (BIG_DATA_OFFSET_ADDR+128+24+60+800+22+22)

#define PRELOADER_LOG_OFFSET_ADDR       (8*1024*1024)
#define LK_LOG_OFFSET_ADDR              (9*1024*1024)
#define KERNEL_LOG_OFFSET_ADDR          (10*1024*1024)

#define FEC_DATA_OFFSET_ADDR            (15*1024*1024)

#define SMART_PART_MAJOR_VERSION   2
#define SMART_PART_MINOR_VERSION   0
#define SMART_PART_REVISION        0

/* types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


#pragma pack(push)
#pragma pack(1)

struct smart_partion_version {
	// version: [major_version_number].[minor_version_number].[revision_number], like 1.2.0
	u16 major_version_number;
	u16 minor_version_number;
	u16 revision_number;
};

struct system_abnormal_write_info_struct {
	u8 warn_info[128];
};

struct part_damaged_count_record {
	u16 lk1_bad_count;
	u16 lk1_correct_count;
	u16 lk2_bad_count;
	u16 lk2_correct_count;
	u16 boot1_bad_count;
	u16 boot1_correct_count;
	u16 boot2_bad_count;
	u16 boot2_correct_count;
	u16 recovery1_bad_count;
	u16 recovery1_correct_count;
	u16 recovery2_bad_count;
	u16 recovery2_correct_count;
};

struct part_fec_record {
	// record the latest time of fec decode, fec fail and fec repair
	// format of time: a string like "2017-05-23 11:20:00"
	u8 fec_decode_time[20];
	u8 fec_failed_time[20];
	u8 fec_repair_time[20];
};

struct lk_laucher_start_time_record {
	// format of time: a string like "2017-05-23 11:20:00"
	u8 lk_start_time[20];
	u8 laucher_start_time[20];
};

struct e2fsck_info_struct {
	u16 e2fsck_count;
	u8 e2fsck_time[20];
};

struct data_part_ro_info_struct {
	u16 data_ro_cnt;
	u8 data_ro_time[20];
};

struct life_time_struct {
	u32 emmc_life_time;
};

struct boot_mode_and_reason_struct {
	u16 boot_mode;
	u16 boot_reason;
};

struct smart_part_flag_region {
	u32 smart_log_after_sale_flag;
	struct smart_partion_version version;

	u8 reserved[0];
};

struct tool_data_struct {
	u8 history_reserved[42];
	struct life_time_struct life_time_info;

	u8 reserved[0];
};

struct big_data_struct {
	struct system_abnormal_write_info_struct system_abnormal_write_info;
	struct part_damaged_count_record damage_record;
	struct part_fec_record fec_record;
	struct lk_laucher_start_time_record lk_laucher_time[20];
	struct e2fsck_info_struct e2fsck_info;
	struct data_part_ro_info_struct data_ro_info;
	struct boot_mode_and_reason_struct boot_mode_and_reason[20];

	u8 reserved[0];
};

/*
 * structure of the smart partition:
 *
 * /------flag region:       1K--------------/
 * /------tool data:         4M-1K-----------/
 * /------big data:          4M--------------/
 * /------pl log region:     1M--------------/
 * /------lk log region:     1M--------------/
 * /------kernel log region: 5M--------------/
 * /------fec data:          1M--------------/
 *
 * And the total size of the smart partition is 16M.
 * WARNING: DON'T USE THIS STRUCT TO DEFINE, IT'S NOT CORRECT!!!!
 */
struct smart_part {
	// flag region
	struct smart_part_flag_region flag_region;	// flag region, 1K
	// tool data
	struct tool_data_struct tool_data;		// tool data, 4M-1K
	// big data
	struct big_data_struct big_data;		// big data, 4M
	// log region
	u8 smartlog_pl_log[1024*1024];			// preoader log, 1M
	u8 smartlog_lk_log[1024*1024];			// lk log, 1M
	u8 smartlog_kernel_log[5*1024*1024];		// kernel log, 5M
	// fec data
	u8 fec_data[1024*1024];				// fec data, 1M
};

#pragma pack(pop)

/* functions */
void system_abnormal_write(u8 *source, size_t size);
void fec_time_write(u8* decode_time, u8* failed_time, u8* repair_time);
void write_laucher_time(void);
void write_life_time(int life_time);

#endif /* CONFIG_TCT_BIGLOG */
#endif /* _SMART_INFO_H */

