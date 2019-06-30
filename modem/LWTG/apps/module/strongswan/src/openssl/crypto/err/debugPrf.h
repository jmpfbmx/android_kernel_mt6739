
/* ssl/debugprf.h */

#ifndef HEADER_DBUBPRF_H 
#define HEADER_DBUBPRF_H 

#ifndef LOG_TAG 
#define LOG_TAG "OpenSSLLib"
#endif

#ifndef OPENSSL_HOST_LOGD
   
#define LOGD(...)  __android_log_print(3,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...) printf

#endif


#endif

