
#ifndef AUDIO_IPI_CLIENT_SPKPRORECT_H
#define AUDIO_IPI_CLIENT_SPKPRORECT_H

#include <linux/fs.h>           /* needed by file_operations* */
#include "audio_messenger_ipi.h"

void audio_ipi_client_spkprotect_init(void);
void audio_ipi_client_spkprotect_deinit(void);
void spkprotect_dump_message(struct ipi_msg_t *ipi_msg);
void spkprotect_open_dump_file(void);
void spkprotect_close_dump_file(void);



#endif /* end of AUDIO_IPI_CLIENT_PLAYBACK_H */

