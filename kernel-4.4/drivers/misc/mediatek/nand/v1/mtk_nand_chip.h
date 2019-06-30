#ifndef __MTK_NAND_CHIP_H__
#define __MTK_NAND_CHIP_H__

/* mtk_nand_chip_info option bit define*/
#define MTK_NAND_FDM_PARTIAL_SUPPORT		(1<<0)
#define MTK_NAND_PLANE_MODE_SUPPORT		(1<<1)
#define MTK_NAND_CACHE_PROGRAM_SUPPORT		(1<<2)
#define MTK_NAND_DISCONTIGOUS_BUFFER_SUPPORT	(1<<3)
#define MTK_NAND_MULTI_CHIP_SUPPORT		(1<<4)
#define MTK_NAND_MULTI_READ_DIFFERENT_OFFSET	(1<<5)
#define MTK_NAND_MULTI_READ_DIFFERENT_PAGE	(1<<6)

enum nand_flash_type {
	MTK_NAND_FLASH_SLC = 0,
	MTK_NAND_FLASH_MLC,
	MTK_NAND_FLASH_TLC,
};

#ifndef ENANDFLIPS
#define ENANDFLIPS	1024	/* Too many bitflips, corrected */
#define ENANDREAD	1025	/* Read fail, can't correct */
#define ENANDWRITE	1026	/* Write fail */
#define ENANDERASE	1027	/* Erase fail */
#define ENANDBAD	1028	/* Bad block */
#endif

#define BAD_BLOCK_MAX_NUM 1024
/**********  API For Wapper ***********/
struct mtk_nand_chip_bbt_info {
	unsigned int initial_bad_num;
	unsigned int bad_block_num;
	unsigned short bad_block_table[BAD_BLOCK_MAX_NUM];
};

#define MTK_NAND_CHIP_INFO_VERSION 1

struct mtk_nand_chip_info {
	int info_version;
	int data_block_num;		/* Number of data blocks */
	int data_page_num;		/* Number of page in a data block */
	int data_page_size;		/* Data page size */
	int data_oob_size;		/* Data OOB size in a page in bytes */
	int data_block_size;		/* Data block size */
	int data_pe;			/* Data block pe_cycle*/

	int log_block_num;		/* Number of log blocks */
	int log_page_num;		/* Number of page in a log block */
	int log_page_size;		/* Log page size */
	int log_oob_size;		/* Log OOB size in a page in bytes */
	int log_block_size;		/* Log block size */
	int log_pe;				/* Log block pe_cycle*/
	unsigned int slc_ratio;		/* FTL SLC ratio here */
	unsigned int start_block;	/* FTL partition start block number */
	unsigned int sector_size_shift;	/* Minimum Data size shift */
	/*      (1<<sector_size_shift) is sector size */
	/*      9: 512Bytes, 10: 1KB, 11: 2KB, others: Reserved */
	unsigned int max_keep_pages;	/* Max page number for write operation */
	/*      1 : MLC with single-plane */
	/*      2 : MLC with multi-plane */
	/*      9 : TLC with single plane */
	/*      18: TLC with multi-plane */
	enum nand_flash_type types;	/* Define nand flash types */
	unsigned int plane_mask;	/* Plane mask information */
	unsigned int plane_num;	/* Plane number */
	/*      1: Single plane, 2: Multi-plane, 4: Quad plane */
	unsigned int chip_num;       /* Chip number, 1/2/4 or more */
	unsigned int option;            /* Chip ability information */
	unsigned char block_type_bitmap[250];
	/*      bit[0]: FDM partial read */
	/*              0: Not support, 1: Support(Next Gen IP) */
	/*      bit[1]: Plane mode, 0: Not support, 1: Support */
	/*      bit[2]: Cache program, 0: Not support, 1: Support */
	/*      bit[3]: Buffer discontiguous address. */
	/*              this requirement for dma address */
	/*              0: Not support, 1: support, next gen IP  */
	/*      bit[4]: Multi-chip mode */
	/*              0: Not support, 1: Support */
	/*	bit[5]: Support different offset/size for multi-page read */
	/*	bit[6]: Support different page for multi-page read */
	/*      bit[7~31]: Reserved  */
};

struct mtk_nand_chip_info *mtk_nand_chip_init(void);

int mtk_nand_chip_read_page(struct mtk_nand_chip_info *info,
		unsigned char *data_buffer, unsigned char *oob_buffer,
		unsigned int block, unsigned int page,
		unsigned int offset, unsigned int size);

struct mtk_nand_chip_read_param {
	unsigned char *data_buffer;
	unsigned char *oob_buffer;
	unsigned int block;
	unsigned int page;
	unsigned int offset;
	unsigned int size;
};

int mtk_nand_chip_read_multi_pages(struct mtk_nand_chip_info *info,
		int page_num, struct mtk_nand_chip_read_param *param);

typedef int (*mtk_nand_callback_func)(struct mtk_nand_chip_info *info,
		unsigned char *data_buffer, unsigned char *oob_buffer,
		unsigned int block, unsigned int page, int status, void *userdata);

int mtk_nand_chip_write_page(struct mtk_nand_chip_info *info,
		unsigned char *data_buffer, unsigned char *oob_buffer,
		unsigned int block, unsigned int page, bool more_page,
		mtk_nand_callback_func callback, void *userdata);

int mtk_nand_chip_erase_block(struct mtk_nand_chip_info *info,
		unsigned int block, unsigned int more_block,
		mtk_nand_callback_func callback, void *userdata);


int mtk_nand_chip_sync(struct mtk_nand_chip_info *info);

const struct mtk_nand_chip_bbt_info *mtk_nand_chip_bmt(
		struct mtk_nand_chip_info *info);

void mtk_chip_mark_bad_block(struct mtk_nand_chip_info *info, unsigned int block);

int mtk_nand_update_block_type(int num, unsigned int *blk);

void dump_record(void);
#endif
