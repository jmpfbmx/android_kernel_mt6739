
#ifndef MTK_DRM_FB_H
#define MTK_DRM_FB_H

#define FBDEV_BPP	16

struct drm_gem_object *mtk_fb_get_gem_obj(struct drm_framebuffer *fb);
int mtk_fb_wait(struct drm_framebuffer *fb);
struct drm_framebuffer *mtk_drm_mode_fb_create(struct drm_device *dev,
					       struct drm_file *file,
					       struct drm_mode_fb_cmd2 *cmd);

void mtk_drm_mode_output_poll_changed(struct drm_device *dev);
int mtk_fbdev_create(struct drm_device *dev);
void mtk_fbdev_destroy(struct drm_device *dev);

#endif /* MTK_DRM_FB_H */
