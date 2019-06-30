
#ifndef __CIRQ_H__
#define __CIRQ_H__

void mt_cirq_enable(void);
void mt_cirq_disable(void);
void mt_cirq_clone_gic(void);
void mt_cirq_flush(void);
void set_wakeup_sources(u32 *list, u32 num_events);

#endif  /*!__CIRQ_H__ */
