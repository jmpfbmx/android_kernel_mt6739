
#ifndef __DDP_PATH_MANAGER_H__
#define __DDP_PATH_MANAGER_H__

#include <linux/sched.h>
#include "ddp_info.h"
#include "disp_event.h"
#include "ddp_path.h"

#include "cmdq_record.h"

#define MAKE_DDP_IRQ_BIT(module, shift)	  ((module<<24)|(0x1<<shift))
#define IRQBIT_MODULE(irqbit)             (irqbit >> 24)
#define IRQBIT_BIT(irqbit)                (irqbit & 0xffffff)

/* IRQ and module are combined to consist DDP IRQ */
typedef enum {
	DDP_IRQ_OVL0_FRAME_COMPLETE = MAKE_DDP_IRQ_BIT(DISP_MODULE_OVL0, 1),
	DDP_IRQ_AAL_OUT_END_FRAME = MAKE_DDP_IRQ_BIT(DISP_MODULE_AAL, 1),

	DDP_IRQ_RDMA0_REG_UPDATE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA0, 0),
	DDP_IRQ_RDMA0_START = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA0, 1),
	DDP_IRQ_RDMA0_DONE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA0, 2),
	DDP_IRQ_RDMA0_UNDERFLOW = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA0, 3),
	DDP_IRQ_RDMA0_TARGET_LINE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA0, 4),

	DDP_IRQ_RDMA1_REG_UPDATE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA1, 0),
	DDP_IRQ_RDMA1_START = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA1, 1),
	DDP_IRQ_RDMA1_DONE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA1, 2),
	DDP_IRQ_RDMA1_UNDERFLOW = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA1, 3),
	DDP_IRQ_RDMA1_TARGET_LINE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA1, 4),

	DDP_IRQ_RDMA2_REG_UPDATE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA2, 0),
	DDP_IRQ_RDMA2_START = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA2, 1),
	DDP_IRQ_RDMA2_DONE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA2, 2),
	DDP_IRQ_RDMA2_UNDERFLOW = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA2, 3),
	DDP_IRQ_RDMA2_TARGET_LINE = MAKE_DDP_IRQ_BIT(DISP_MODULE_RDMA2, 4),

	DDP_IRQ_WDMA0_FRAME_COMPLETE = MAKE_DDP_IRQ_BIT(DISP_MODULE_WDMA0, 0),

	DDP_IRQ_WDMA1_FRAME_COMPLETE = MAKE_DDP_IRQ_BIT(DISP_MODULE_WDMA1, 0),

	DDP_IRQ_DSI0_EXT_TE = MAKE_DDP_IRQ_BIT(DISP_MODULE_DSI0, 2),
	DDP_IRQ_DSI0_CMD_DONE = MAKE_DDP_IRQ_BIT(DISP_MODULE_DSI0, 1),
	DDP_IRQ_DSI0_FRAME_DONE = MAKE_DDP_IRQ_BIT(DISP_MODULE_DSI0, 4),

	DDP_IRQ_MUTEX0_SOF = MAKE_DDP_IRQ_BIT(DISP_MODULE_MUTEX, 0),
	DDP_IRQ_MUTEX1_SOF = MAKE_DDP_IRQ_BIT(DISP_MODULE_MUTEX, 1),

	DDP_IRQ_DPI_VSYNC = MAKE_DDP_IRQ_BIT(DISP_MODULE_DPI, 0),
	DDP_IRQ_DPI_VDE   = MAKE_DDP_IRQ_BIT(DISP_MODULE_DPI, 1),

	DDP_IRQ_UNKNOWN = MAKE_DDP_IRQ_BIT(DISP_MODULE_UNKNOWN, 0),

} DDP_IRQ_BIT;

/* path handle */
typedef void *disp_path_handle;

int dpmgr_init(void);

disp_path_handle dpmgr_create_path(DDP_SCENARIO_ENUM scenario, cmdqRecHandle cmdq_handle);


int dpmgr_get_scenario(disp_path_handle dp_handle);

int dpmgr_modify_path_power_on_new_modules(disp_path_handle dp_handle,
					   DDP_SCENARIO_ENUM new_scenario, int sw_only);
int dpmgr_modify_path(disp_path_handle dp_handle, DDP_SCENARIO_ENUM new_scenario,
		      cmdqRecHandle cmdq_handle, DDP_MODE isvdomode, int sw_only);
int dpmgr_modify_path_power_off_old_modules(DDP_SCENARIO_ENUM old_scenario,
					    DDP_SCENARIO_ENUM new_scenario, int sw_only);

int dpmgr_destroy_path(disp_path_handle dp_handle, cmdqRecHandle cmdq_handle);

/* only destroy handle, don't disconnect path */
int dpmgr_destroy_path_handle(disp_path_handle dp_handle);



int dpmgr_path_memout_clock(disp_path_handle dp_handle, int clock_switch);

int dpmgr_path_add_memout(disp_path_handle dp_handle, DISP_MODULE_ENUM engine, void *cmdq_handle);

int dpmgr_path_remove_memout(disp_path_handle dp_handle, void *cmdq_handle);


int dpmgr_path_get_mutex(disp_path_handle dp_handle);


DISP_MODULE_ENUM dpmgr_path_get_dst_module(disp_path_handle dp_handle);
enum dst_module_type dpmgr_path_get_dst_module_type(disp_path_handle dp_handle);


int dpmgr_path_set_dst_module(disp_path_handle dp_handle, DISP_MODULE_ENUM dst_module);

int dpmgr_path_set_video_mode(disp_path_handle dp_handle, int is_vdo_mode);

int dpmgr_path_init(disp_path_handle dp_handle, int encmdq);


int dpmgr_path_connect(disp_path_handle dp_handle, int encmdq);

int dpmgr_path_disconnect(disp_path_handle dp_handle, int encmdq);


int dpmgr_path_deinit(disp_path_handle dp_handle, int encmdq);


int dpmgr_path_start(disp_path_handle dp_handle, int encmdq);


int dpmgr_path_stop(disp_path_handle dp_handle, int encmdq);


int dpmgr_path_reset(disp_path_handle dp_handle, int encmdq);


int dpmgr_path_config(disp_path_handle dp_handle, disp_ddp_path_config *config, void *cmdq_handle);


int dpmgr_path_flush(disp_path_handle dp_handle, int encmdq);


int dpmgr_check_status(disp_path_handle dp_handle);


void dpmgr_debug_path_status(int mutex_id);

void dpmgr_get_input_address(disp_path_handle dp_handle, unsigned long *addr);


int dpmgr_path_build_cmdq(disp_path_handle dp_handle, void *trigger_loop_handle, CMDQ_STATE state,
			  int reverse);


int dpmgr_path_trigger(disp_path_handle dp_handle, void *trigger_loop_handle, int encmdq);


int dpmgr_signal_event(disp_path_handle dp_handle, DISP_PATH_EVENT event);


int dpmgr_enable_event(disp_path_handle dp_handle, DISP_PATH_EVENT event);


int dpmgr_disable_event(disp_path_handle dp_handle, DISP_PATH_EVENT event);

int dpmgr_map_event_to_irq(disp_path_handle dp_handle, DISP_PATH_EVENT event, DDP_IRQ_BIT irq_bit);


int dpmgr_wait_event_timeout(disp_path_handle dp_handle, DISP_PATH_EVENT event, int timeout);


int dpmgr_wait_event(disp_path_handle dp_handle, DISP_PATH_EVENT event);
int dpmgr_wait_event_ts(disp_path_handle dp_handle, DISP_PATH_EVENT event, unsigned long long *event_ts);


int dpmgr_path_power_on(disp_path_handle dp_handle, CMDQ_SWITCH encmdq);
int dpmgr_path_power_on_bypass_pwm(disp_path_handle dp_handle, CMDQ_SWITCH encmdq);

int dpmgr_path_power_off(disp_path_handle dp_handle, CMDQ_SWITCH encmdq);
int dpmgr_path_power_off_bypass_pwm(disp_path_handle dp_handle, CMDQ_SWITCH encmdq);

int dpmgr_set_lcm_utils(disp_path_handle dp_handle, void *lcm_drv);


int dpmgr_path_is_busy(disp_path_handle dp_handle);


int dpmgr_path_is_idle(disp_path_handle dp_handle);

int dpmgr_path_user_cmd(disp_path_handle dp_handle, int msg, unsigned long arg, void *cmdqhandle);


int dpmgr_path_set_parameter(disp_path_handle dp_handle, int io_evnet, void *data);


int dpmgr_path_get_parameter(disp_path_handle dp_handle, int io_evnet, void *data);

int dpmgr_path_ioctl(disp_path_handle dp_handle, void *cmdq_handle, DDP_IOCTL_NAME ioctl_cmd,
		     void *params);

int dpmgr_path_enable_irq(disp_path_handle dp_handle, void *cmdq_handle, DDP_IRQ_LEVEL irq_level);
disp_ddp_path_config *dpmgr_path_get_last_config(disp_path_handle dp_handle);
disp_ddp_path_config *dpmgr_path_get_last_config_notclear(disp_path_handle dp_handle);

void dpmgr_get_input_buffer(disp_path_handle dp_handle, unsigned long *addr);
int dpmgr_module_notify(DISP_MODULE_ENUM module, DISP_PATH_EVENT event);

int dpmgr_insert_ovl1_sub(disp_path_handle dp_handle, void *cmdq_handle);
int dpmgr_remove_ovl1_sub(disp_path_handle dp_handle, void *cmdq_handle);

int dpmgr_factory_mode_test(int module_name, void *cmdqhandle, void *config);

int dpmgr_wait_ovl_available(int ovl_num);
#endif
