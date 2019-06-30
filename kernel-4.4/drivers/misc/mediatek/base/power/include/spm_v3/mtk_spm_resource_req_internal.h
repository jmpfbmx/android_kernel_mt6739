
#ifndef __MTK_SPM_RESOURCE_REQ_INTERNAL_H__
#define __MTK_SPM_RESOURCE_REQ_INTERNAL_H__

/* SPM resource request APIs: for internal use */

void spm_resource_req_dump(void);
void spm_resource_req_block_dump(void);
unsigned int spm_get_resource_usage(void);
bool spm_resource_req_init(void);
void spm_resource_req_debugfs_init(struct dentry *spm_dir);

#endif /* __MTK_SPM_RESOURCE_REQ_INTERNAL_H__ */
