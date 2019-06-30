/*file discription : otp*/
#define OTP_DRV_LSC_SIZE 1868
#define CAM_CAL_DEV_MAJOR_NUMBER 226
#define PDAF_SIZE 1372
struct otp_struct {
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
//unsigned char lenc[OTP_DRV_LSC_SIZE];
int checksumAF;
int checksumLSC;
int checksumOTP;
int checksumTotal;
int VCM_macro;
int VCM_inf;
int VCM_dir;
int VCM_start;
//int checksumpdaf;
//unsigned char pdaf[PDAF_SIZE];
};
struct pdaf_struct {
int checksumpdaf;
unsigned char pdaf[PDAF_SIZE];
};
//#define RG_Ratio_Typical 0x125
//#define BG_Ratio_Typical 0x131

int ov13855_read_otp(struct otp_struct *otp_ptr);
int ov13855_read_pdaf(struct pdaf_struct *pdaf_ptr);
//int apply_otp(struct otp_struct *otp_ptr);
//int Decode_13853R2A(unsigned char*pInBuf, unsigned char* pOutBuf);
void ov13855_otp_cali(void);
void ov13855_pdaf_cali(void);
//void LumaDecoder(uint8_t *pData, uint8_t *pPara);
//void ColorDecoder(uint8_t *pData, uint8_t *pPara);
//extern int read_otp_info(int index, struct otp_struct *otp_ptr);
//extern int update_otp_wb(void);
//extern int update_otp_lenc(void);







