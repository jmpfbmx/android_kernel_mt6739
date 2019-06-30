
#ifndef MTK_DRM_DEBUGFS_H
#define MTK_DRM_DEBUGFS_H

struct drm_device;
struct mtk_drm_private;

#ifdef CONFIG_DEBUG_FS
void mtk_drm_debugfs_init(struct drm_device *dev,
			  struct mtk_drm_private *priv);
void mtk_drm_debugfs_deinit(void);
#else
static inline void mtk_drm_debugfs_init(struct drm_device *dev,
					struct mtk_drm_private *priv) {}
static inline void mtk_drm_debugfs_deinit(void) {}
#endif
bool force_alpha(void);

#endif /* MTK_DRM_DEBUGFS_H */
