
#ifndef MTK_DRM_DEBUGFS_H
#define MTK_DRM_DEBUGFS_H

#if defined(CONFIG_DEBUG_FS)
int mtk_drm_dsi_debugfs_init(struct mtk_dsi *dsi);
void mtk_drm_dsi_debugfs_exit(struct mtk_dsi *dsi);
#else
int mtk_drm_dsi_debugfs_init(struct mtk_dsi *dsi)
{
	return 0;
}

void mtk_drm_dsi_debugfs_exit(struct mtk_dsi *dsi)
{
}
#endif /* CONFIG_DEBUG_FS */

#endif /* MTK_DRM_DEBUGFS_H */
