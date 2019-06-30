
enum {
	Default = 0,
	Low_Power_Mode = 1,
	Sport_Mode = 2,
	Performance_Mode = 3,
};


int disable_cg_fliper(void);
void enable_cg_fliper(int);
void enable_total_fliper(int);
int cg_set_threshold(int, int, int);
int cg_set_bw(int);
int total_set_threshold(int, int, int);
int cg_restore_threshold(void);
int total_restore_threshold(void);
extern int dram_steps_freq(unsigned int step);
extern int get_ddr_type(void);
