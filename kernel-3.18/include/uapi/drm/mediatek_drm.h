

#ifndef _UAPI_MEDIATEK_DRM_H
#define _UAPI_MEDIATEK_DRM_H

#include <drm/drm.h>

struct drm_mtk_gem_create {
	uint64_t size;
	uint32_t flags;
	uint32_t handle;
};

struct drm_mtk_gem_map_off {
	uint32_t handle;
	uint32_t pad;
	uint64_t offset;
};

#define DRM_MTK_GEM_CREATE		0x00
#define DRM_MTK_GEM_MAP_OFFSET		0x01

#define DRM_IOCTL_MTK_GEM_CREATE	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_MTK_GEM_CREATE, struct drm_mtk_gem_create)

#define DRM_IOCTL_MTK_GEM_MAP_OFFSET	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_MTK_GEM_MAP_OFFSET, struct drm_mtk_gem_map_off)


#endif /* _UAPI_MEDIATEK_DRM_H */
