/************************************************* 
History: 1.0 Added by zhanyong.yin for XR 5581518 on 2017/11/20
*************************************************/
/*file discription : otp*/
struct hi1333_otp_struct {
int flag;
int module_integrator_id;
int lens_id;
int production_year;
int production_month;
int production_day;
int R_Gr_ratio;
int B_Gr_ratio;
int Gb_Gr_ratio;
int g_R_Gr_ratio;
int g_B_Gr_ratio;
int g_Gb_Gr_ratio;
int checksumAWB;
int vcm_id;
int vcm_ic_id;
int checksumAF;
int checksumLSC;
int checksumOTP;
int checksumTotal;
int VCM_macro;
int VCM_inf;
int VCM_dir;
int VCM_start;
};
struct hi1333_pdaf_struct {
int checksumpdaf;
};
int hi1333_read_otp(struct hi1333_otp_struct *otp_ptr);
int hi1333_read_pdaf(struct hi1333_pdaf_struct *pdaf_ptr);
void hi1333_otp_cali(void);
void hi1333_pdaf_cali(void);







