
extern int usb3_switch_en(struct usbtypc *typec, int on);
extern int usb3_switch_sel(struct usbtypc *typec, int sel);
extern int usb_redriver_config(struct usbtypc *typec, int ctrl_pin, int stat);
extern int usbc_pinctrl_init(void);
extern struct usbtypc *get_usbtypec(void);

