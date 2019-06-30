

#ifndef __HELIO_DVFSRC_OPP_H
#define __HELIO_DVFSRC_OPP_H

#if defined(CONFIG_MACH_MT6775)
#include <helio-dvfsrc-opp-mt6775.h>
#elif defined(CONFIG_MACH_MT6771)
#include <helio-dvfsrc-opp-mt6771.h>
#endif

#endif
