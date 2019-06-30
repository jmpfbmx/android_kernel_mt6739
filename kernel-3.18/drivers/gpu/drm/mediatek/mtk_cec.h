#ifndef _MTK_CEC_H
#define _MTK_CEC_H

struct device;

void mtk_cec_set_hpd_event(struct device *dev,
			   void (*hotplug_event)(bool hpd, void *data),
			   void *hotplug_event_data);
bool mtk_cec_hpd_high(struct device *dev);
int mtk_cec_irq(struct device *dev);

#endif /* _MTK_CEC_H */
