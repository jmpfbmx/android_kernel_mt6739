#ifndef __SINGLE_CMA_H__
#define __SINGLE_CMA_H__


phys_addr_t zmc_base(void);
struct page *zmc_cma_alloc(struct cma *cma, int count, unsigned int align, struct single_cma_registration *p);
bool zmc_cma_release(struct cma *cma, struct page *pages, int count);
#endif
