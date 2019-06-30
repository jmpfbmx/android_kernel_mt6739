
#ifndef PPG_CONTROL_LIB_H
#define PPG_CONTROL_LIB_H



/* #include <stdint.h> */

#define INT32 int
#define INT16 short
#define UINT32 unsigned int
#define UINT16 unsigned short



/** @brief  This enum defines the return type of PPG control library.  */
enum ppg_control_status_t {
	PPG_CONTROL_STATUS_ERROR = -2,	   /**<  The function call failed. */
	PPG_CONTROL_STATUS_INVALID_PARAMETER = -1,/**<  Invalid parameter is given. */
	PPG_CONTROL_STATUS_OK = 0	   /**<  This function call is successful. */
};

/** @brief This enum defines the PPG control mode. */
enum {
	PPG_CONTROL_MODE_LED1 = 1,
			       /**< Single channel adjustment. */
	PPG_CONTROL_MODE_LEDS = 3
			       /**< Dual channel adjustment. */
};



struct ppg_control_t {
	INT32 *input;	    /**< A pointer to the input PPG signal. */
	INT32 *input_amb;   /**< A pointer to the input AMB signal. */
	INT32 input_fs;	    /**< The sampling frequency (fs) of the input data. (supports 125Hz only). */
	INT32 input_length; /**< The number of input samples. The maximum length = input_fs * 2= 250.*/
	/* INT32 input_bit_width; */ /**< The bit width of the input data. */
			/* If the data is already converted to signed representation, set bit width to 32. */
	INT32 input_config; /**< Configuration of the input signal (Please set to 1). */
	INT32 input_source; /**< 1: this is the data from MT2511 PPG channel 1 (PPG1). */
					/* 2: this is the data from MT2511 PPG channel 2 (PPG2). */
};


/* Function definition */
UINT32 ppg_control_get_version(void);

enum ppg_control_status_t ppg_control_init(void);

 /**
 * @brief Call this function to analyze the incoming PPG data and automatically adjust the setting of analog
 *        front-end on MediaTek MT2511.
 * @param[in] *ppg_control_input is the input data structure containing the actual data to be processed by this API,
 *            in order to adjust the setting of the the analog front-end on MediaTek MT2511.
 * @param[in] ppg_control_mode is to set the library according to the LED/PPG configuration. \n
 *            #PPG_CONTROL_MODE_LED1: Adjust only LED1, according to PPG1. \n
 *            #PPG_CONTROL_MODE_DUAL1: Adjust both LED1 and LED2 with equal current, according to PPG1.
 * @param[out] *ppg_control_output is the output signal data buffer
 * @return    Return the flag to indicate whether the setting of the MT2511 analog front-end is adjusted. \n
 *            #PPG_CONTROL_STATUS_ERROR, if the operation failed. \n
 *            #PPG_CONTROL_STATUS_INVALID_PARAMETER, if an invalid parameter was given. \n
 *            #PPG_CONTROL_STATUS_OK, if the operation completed successfully. \n
 *
*/
INT32 ppg_control_process(struct ppg_control_t *ppg_control_input);

INT32 ppg_control_get_status(INT32 ppg_control_internal_config);






#endif				/* PPG_CONTROL_LIB_H */
