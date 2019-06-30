#include <linux/types.h>
#include <linux/genhd.h> 
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

//#include "efi.h"

#define DIS_WP
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/core.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/bug.h>
#include <linux/scatterlist.h>
#include <linux/module.h>
#include "mtk_sd.h"

#define TAG     "[PART_KL]"

#define part_err(fmt, args...)       \
    printk(KERN_ERR TAG);           \
    printk(KERN_CONT fmt, ##args) 
#define part_info(fmt, args...)      \
    printk(KERN_NOTICE TAG);        \
    printk(KERN_CONT fmt, ##args)

struct part_t {
    u64 start;
    u64 size;
    u32 part_id;
    u8 name[64];
};

#define WP_MAX 1024
#define EMMC_HOST_NUM            0
#define EXT_CSD_BOOT_WP_STATUS	 174	

struct wp_region {
    u64 start_addr;
    u64 end_addr;
    struct wp_region *next;
    char type;
};

u32 sg_wp_size = 0;
char wp_bitmap[WP_MAX] = {0};
u8  boot_wp_status;
u32 bitmap_len;

struct wp_region *head,*pre,*tmp;
extern struct msdc_host *mtk_msdc_host[];

#define UNSTUFF_BITS(resp, start, size) \
    ({                              \
    const int __size = size;                \
    const u32 __mask = (__size < 32 ? 1 << __size : 0) - 1; \
    const int __off = 3 - ((start) / 32);           \
    const int __shft = (start) & 31;            \
    u32 __res;                      \
    __res = resp[__off] >> __shft;              \
    if (__size + __shft > 32)               \
        __res |= resp[__off-1] << ((32 - __shft) % 32); \
    __res & __mask;                     \
    })

void switch_8bytes(char *buf)
{
    int i = 0;
    char tmp;

    for(;i<4;i++){
        tmp = *(buf + i);
        *(buf + i) = *(buf + 7 - i);
        *(buf + 7 - i) = tmp;
    }
}

struct msdc_host *emmc_get_host(void)
{
	return mtk_msdc_host[EMMC_HOST_NUM];
}

u8 get_2bits(u8 a ,u8 bit)
{
    u8 mask ;
    bit &= 0x3;
    mask = 0x3 << (2*bit);
    return ((a & mask) >> (2*bit));
}

u64 sector_addr(int byteaddr,int bitaddr)
{
    return (byteaddr * 4 +  bitaddr) * sg_wp_size * 512;
}

char *type2str(char type)
{
    switch(type) {
        case 0x0:
            return "None";
        case 0x1:
            return "Temporary";
        case 0x2:
            return "Power_On";
        case 0x3:
            return "Permanent";
        default:
            return "Error";
    }

}

char *boottype2str(char type)
{
    switch(type) {
        case 0x0:
            return "None";
        case 0x1:
            return "Power_On";
        case 0x2:
            return "Permanent";
        default:
            return "Error";
    }

}

static void handle_wp_bitmap(void)
{
    int i,j,b_index;
    u8 next_region;

    head=pre=tmp=kmalloc(sizeof(struct wp_region),GFP_KERNEL);
    tmp->next = NULL;
    next_region = true; //head not contain region ,we will abandon it

    for(i=0;i<bitmap_len;i++) {
        //printk("****** %s wp_bitmap %d ,0x%x\n",__func__,i,wp_bitmap[i]);
        if(wp_bitmap[i] == 0) {
            continue;
        } else {
            for(j = 0;j < 4;) {
                if(get_2bits(wp_bitmap[i],j) == 0) {
                    j++;
                    continue;
                }
                if(next_region) {
                    tmp = kmalloc(sizeof(struct wp_region),GFP_KERNEL);
                    pre->next = tmp;
                    pre = tmp;
                    tmp->next = NULL;
                    tmp->start_addr = sector_addr(i,j);
                 }

                for(b_index = j;b_index < 4; b_index++) {
                    if( (b_index + 1) < 4 ) {
                        if(get_2bits(wp_bitmap[i],b_index) ==
                            get_2bits(wp_bitmap[i],b_index + 1)) { //the same wp type ,so put them in same wp_region
                            next_region = false;
                            continue;
                        } else {
                            tmp->end_addr = sector_addr(i,b_index) + sg_wp_size * 512 - 1;
                            tmp->type = get_2bits(wp_bitmap[i],b_index);
                            next_region = true;
                            j = b_index + 1;
                            break; //handle next 2bits
                        } //end of get_2bits......
                    } else { //pre handle the next byte in wp_bitmap
                        if( (i + 1) < bitmap_len ) {
                            if(get_2bits(wp_bitmap[i],b_index) == get_2bits(wp_bitmap[i+1],0)) {
                                next_region = false;
                                j = b_index + 1;
                                break; //handle the same type in next byte
                            } else {
                                tmp->end_addr = sector_addr(i,b_index) + sg_wp_size * 512 - 1;
                                tmp->type = get_2bits(wp_bitmap[i],b_index);
                                next_region = true;
                                j = b_index + 1;
                                break; //handle next bytes 
                            }
                        } else { //we have handled the end byte of wp_bitmap
                            tmp->end_addr = sector_addr(i,b_index) + sg_wp_size * 512 - 1;
                            tmp->type = get_2bits(wp_bitmap[i],b_index);
                            next_region = true; // have handled all bytes in wp_bitmap
                            j = b_index + 1;
                            break;
                        }
                    }
                }//end of for(b_index = j;b_index > 0; b_index --)
            } //end of for(j=3;j>=0;)
        } //end of if(wp_bitmap[i] == 0)
    } //end of for(i=0;i<bitmap_len;i++)

    pre = head->next;
    kfree(head);
    head=pre;
}

unsigned int emmc_wp_size(void)
{
    u8 *l_ext_csd, wp_group_size;
	struct msdc_host *host_ctl;
    u32 *resp;

	if (0 == sg_wp_size) {
		host_ctl = emmc_get_host();

		BUG_ON(!host_ctl);
		BUG_ON(!host_ctl->mmc);
		BUG_ON(!host_ctl->mmc->card);

		part_err("[%s:%d]mmc_host is : 0x%p\n", __func__, __LINE__, host_ctl->mmc);

        resp = host_ctl->mmc->card->raw_csd;

		mmc_claim_host(host_ctl->mmc);
		//mmc_send_ext_csd(host_ctl->mmc->card, l_ext_csd);
		mmc_get_ext_csd(host_ctl->mmc->card, &l_ext_csd);
		mmc_release_host(host_ctl->mmc);


		if (l_ext_csd[EXT_CSD_ERASE_GROUP_DEF] & 0x1) {
			sg_wp_size = (1024 * l_ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] *
				 l_ext_csd[EXT_CSD_HC_WP_GRP_SIZE]);
		} else {
            wp_group_size = UNSTUFF_BITS(resp, 32, 5) + 1;
			sg_wp_size = ( wp_group_size * host_ctl->mmc->card->erase_size);
		}

        boot_wp_status = l_ext_csd[EXT_CSD_BOOT_WP_STATUS] & 0x0f;
        kfree(l_ext_csd);
	}

    part_err("[%s] get wp_size %d MB\n",__func__,sg_wp_size/2048);
	return sg_wp_size;
}

static void emmc_wp_info(unsigned int end_sector)
{
    int i,count_wp = (end_sector  + sg_wp_size - 1)/sg_wp_size;
    int count_wp_group = (count_wp + 31)/32;
    u8 *l_ext_csd;

	struct scatterlist msdc_sg;
	struct mmc_data msdc_data;
	struct mmc_command msdc_cmd;
	struct mmc_request msdc_mrq;
	struct msdc_host *host_ctl = emmc_get_host();

	BUG_ON(!host_ctl);
	BUG_ON(!host_ctl->mmc);
	BUG_ON(!host_ctl->mmc->card);

	memset(&msdc_data, 0, sizeof(struct mmc_data));
	memset(&msdc_mrq, 0, sizeof(struct mmc_request));
	memset(&msdc_cmd, 0, sizeof(struct mmc_command));

	msdc_mrq.cmd = &msdc_cmd;
	msdc_mrq.data = &msdc_data;

    msdc_cmd.opcode = 31;    //MMC_SEND_WRITE_PROT_TYPE;
    msdc_cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;

	msdc_data.flags = MMC_DATA_READ;
	msdc_data.blocks = 1;	/* one block Request */
	msdc_data.stop = NULL;
	msdc_data.blksz = 8;
	msdc_data.sg = &msdc_sg;
	msdc_data.sg_len = 1;

    memset(wp_bitmap,0x0,WP_MAX);

    mmc_claim_host(host_ctl->mmc);

    /* make sure access user data area */
    //mmc_send_ext_csd(host_ctl->mmc->card, l_ext_csd);
	mmc_get_ext_csd(host_ctl->mmc->card, &l_ext_csd);
    if (l_ext_csd[179] & 0x7) {
        l_ext_csd[179] &= ~0x7;
        l_ext_csd[179] |= 0x0;
        mmc_switch(host_ctl->mmc->card, 0, 179, l_ext_csd[179], 1000);
    }
    kfree(l_ext_csd);

    for(i=0;i<count_wp_group;i++) {
        msdc_cmd.arg = 32 * i * sg_wp_size;
        sg_init_one(&msdc_sg,(void *)(wp_bitmap + i*8), 8);
	    mmc_set_data_timeout(&msdc_data, host_ctl->mmc->card);
	    mmc_wait_for_req(host_ctl->mmc, &msdc_mrq);

        if((msdc_cmd.error) || (msdc_data.error)) {
            part_err("[%s] read WP Group %d WP type error ????????????????????\n",__func__,msdc_cmd.arg);
            //break;
        }

        switch_8bytes(wp_bitmap + i*8);
    }
	mmc_release_host(host_ctl->mmc);

    bitmap_len = (count_wp + 3)/4;
    handle_wp_bitmap();
}

static u8 region_in_bitmap(u32 start_sect,u32 nr_sects)
{
    u32 byte_index,bit_index,start_byte,start_bit,i,wp_count,start_wp_group;
    u8 wp_type,pre_wp_type;

    if((nr_sects == 0) || (sg_wp_size == 0))
        return 0;

    wp_count = (nr_sects + sg_wp_size - 1 ) / sg_wp_size;

    start_wp_group = start_sect / sg_wp_size ;
    byte_index = start_byte = start_wp_group / 4;
    bit_index  = start_bit = start_wp_group % 4;

    pre_wp_type = wp_type = get_2bits(wp_bitmap[start_byte],start_bit);

    if((pre_wp_type & 0x3) == 0)
        return 0;

    for(i = 0; i < wp_count; i++) {
        wp_type = get_2bits(wp_bitmap[byte_index],bit_index++);
        if(wp_type != pre_wp_type)  {
            return -1;
        }

        if(bit_index == 4) {
            byte_index += 1;
            bit_index = 0;
        }
    }

    return wp_type;
}

static int wp_info_show_proc(struct seq_file *m, void *v)
{
    dev_t devt;
    int partno,region_wp_type,wp_endsector = 0;
    struct disk_part_iter piter;
    struct gendisk *disk;
    struct hd_struct *part;

    u32 last_sect=0;
    u64 last = 0;

    emmc_wp_size();

    devt = blk_lookup_devt("mmcblk0", 0);
    disk = get_gendisk(devt, &partno);

    seq_printf(m, "%-16s %-16s\t%-16s\t%-16s\n", "Name", "Start", "Size","WP_Type");

    seq_printf(m, "%-16s 0x%016x\t%-16s\t%-16s\n", "boot0", 0U, "boot0size",boottype2str(boot_wp_status & 0x3));
    seq_printf(m, "%-16s 0x%016x\t%-16s\t%-16s\n", "boot1", 0U, "boot1size",boottype2str((boot_wp_status>>2) & 0x3));

    if (!disk || get_capacity(disk) == 0) {
        return 0;
    }

	disk_part_iter_init(&piter, disk, 0);
    while ((part = disk_part_iter_next(&piter))) {
        last_sect = part->start_sect + part->nr_sects;
        if((part->info) && (strncmp(part->info->volname,"userdata",8) == 0)) {
            wp_endsector = part->start_sect;
        }
	}

    if(wp_endsector == 0)
        wp_endsector = last_sect;   //if no userdata partition,then let wp_endsector to total emmc size

    emmc_wp_info(wp_endsector);

	disk_part_iter_init(&piter, disk, 0);
    region_wp_type = 0; //for pgpt
    seq_printf(m, "%-16s 0x%016x\t0x%016x\t%-16s\n", "pgpt", 0U, 32 * 1024,type2str(region_wp_type));

    while ((part = disk_part_iter_next(&piter))) {
        region_wp_type = region_in_bitmap(part->start_sect,part->nr_sects);
        seq_printf(m, "%-16s 0x%016llx\t0x%016llx\t%-16s\n", 
            part->info ? (char *)(part->info->volname) : "unknown",
            (u64)part->start_sect * 512,
            (u64)part->nr_sects * 512,
            type2str(region_wp_type));
        last = (part->start_sect + part->nr_sects) * 512;
	}

    region_wp_type = 0; //for sgpt
    seq_printf(m, "%-16s 0x%016llx\t0x%016x\t%-16s\n", "sgpt", last, 16896,type2str(region_wp_type));

    seq_printf(m, "\n%-16s    %-16s\t %-16s\n", "start", "End", "WP_Type");
    while(head != NULL) {
        seq_printf(m,"0x%016llx->0x%016llx : %s\n",head->start_addr,head->end_addr,type2str(head->type));
        tmp = head->next;
        kfree(head);
        head = tmp;
    }

	disk_part_iter_exit(&piter);

	return 0;
}

static int wp_info_open(struct inode *inode, struct file *file)
{
	return single_open(file, wp_info_show_proc, inode);
}

static const struct file_operations wp_info_proc_fops = { 
    .open       = wp_info_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static int __init wp_init(void)
{
    struct proc_dir_entry *wp_info_proc;

    wp_info_proc = proc_create("wp_info", 0444, NULL, &wp_info_proc_fops);
    if (!wp_info_proc) {
        part_err("[%s]fail to register /proc/wp_info\n", __func__);
    }


    return 0;
}

static void __exit wp_exit(void)
{
    remove_proc_entry("wp_info", NULL);
}

module_init(wp_init);
module_exit(wp_exit);
