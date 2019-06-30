
#ifndef _KD_SENINF_DEFINE_H_
#define _KD_SENINF_DEFINE_H_

/* In KERNEL mode,SHOULD be sync with mediatype.h */
/* CHECK before remove or modify */
/* #undef BOOL */
/* #define BOOL signed int */
#ifndef _MEDIA_TYPES_H
typedef unsigned char MUINT8;
typedef unsigned short MUINT16;
typedef unsigned int MUINT32;
typedef signed char MINT8;
typedef signed short MINT16;
typedef signed int MINT32;
#endif

struct KD_SENINF_MMAP {
	MUINT32 map_addr;
	MUINT32 map_length;
};

struct KD_SENINF_REG {
	struct KD_SENINF_MMAP seninf;
	struct KD_SENINF_MMAP ana;
	struct KD_SENINF_MMAP gpio;
};

#endif
