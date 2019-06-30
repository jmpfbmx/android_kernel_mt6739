/* C code produced by gperf version 3.0.4 */
/* Command-line: /usr/bin/gperf -m 10 -C -G -D -t  */
/* Computed positions: -k'1-2,6,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


/*
 * Copyright (C) 2005 Andreas Steffen
 * Hochschule fuer Technik Rapperswil, Switzerland
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <string.h>

#include "keywords.h"

struct kw_entry {
    char *name;
    kw_token_t token;
};

#define TOTAL_KEYWORDS 189
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 17
#define MIN_HASH_VALUE 14
#define MAX_HASH_VALUE 404
/* maximum key range = 391, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned short asso_values[] =
    {
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405,  19,
      193, 405,  55, 405,   3, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405,   7, 405,  75,   7,  82,
      109,   2, 121, 146, 103,   1, 405,  95,   2, 113,
       43,  17,  55, 405,   6,  27,  13, 184,   2,   4,
        4,  25,   3, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405, 405, 405, 405, 405,
      405, 405, 405, 405, 405, 405
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
      case 4:
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

static const struct kw_entry wordlist[] =
  {
    {"lifetime",          KW_KEYLIFE},
    {"rightimei",         KW_RIGHTIMEI},
    {"leftfirewall",      KW_LEFTFIREWALL},
    {"left",              KW_LEFT},
    {"right",             KW_RIGHT},
    {"leftcert",          KW_LEFTCERT},
    {"leftsendcert",      KW_LEFTSENDCERT},
    {"rightikeport",      KW_RIGHTIKEPORT},
    {"rightintsubnet",    KW_RIGHTINTSUBNET},
    {"leftprotoport",     KW_LEFTPROTOPORT},
    {"rightikeportnatt",  KW_RIGHTIKEPORTNATT},
    {"rekey",             KW_REKEY},
    {"leftforcetsifull",  KW_LEFTFORCETSIFULL},
    {"leftsigkey",        KW_LEFTSIGKEY},
    {"leftallowany",      KW_LEFTALLOWANY},
    {"type",              KW_TYPE},
    {"leftcertpolicy",    KW_LEFTCERTPOLICY},
    {"leftgroups",        KW_LEFTGROUPS},
    {"rightrsasigkey",    KW_RIGHTSIGKEY},
    {"rightsubnet",       KW_RIGHTSUBNET},
    {"rightsendcert",     KW_RIGHTSENDCERT},
    {"server_nocert",     KW_SERVER_NOCERT},
    {"lefthostaccess",    KW_LEFTHOSTACCESS},
    {"rightidr_apn",      KW_RIGHTIDR_APN},
    {"lifebytes",         KW_LIFEBYTES},
    {"retrans_base",      KW_RETRANS_BASE},
    {"is_emergency",      KW_IS_EMERGENCY},
    {"leftrsasigkey",     KW_LEFTSIGKEY},
    {"rightsigkey",       KW_RIGHTSIGKEY},
    {"leftnexthop",       KW_LEFT_DEPRECATED},
    {"leftintsubnet",     KW_LEFTINTSUBNET},
    {"retrans_to",        KW_RETRANS_TO},
    {"inactivity",        KW_INACTIVITY},
    {"leftdns",           KW_LEFTDNS},
    {"no_initct",         KW_NO_INIT_CONTACT},
    {"installpolicy",     KW_INSTALLPOLICY},
    {"encrkeydisplay",    KW_ENCRKEYDISPLAY},
    {"esp",               KW_ESP},
    {"leftsourceip",      KW_LEFTSOURCEIP},
    {"rightprotoport",    KW_RIGHTPROTOPORT},
    {"leftforcetsi64",    KW_LEFTFORCETSI64},
    {"retrans_tries",     KW_RETRANS_TRIES},
    {"strictcrlpolicy",   KW_STRICTCRLPOLICY},
    {"rightsubnetwithin", KW_RIGHTSUBNET},
    {"virtual_private",   KW_SETUP_DEPRECATED},
    {"crluri",            KW_CRLURI},
    {"also",              KW_ALSO},
    {"postpluto",         KW_SETUP_DEPRECATED},
    {"ike",               KW_IKE},
    {"rightsourceip",     KW_RIGHTSOURCEIP},
    {"certuribase",       KW_CERTURIBASE},
    {"certreq_critical",  KW_CERTREQ_CRITICAL},
    {"rightcustcpimei",   KW_RIGHTCUSTCPIMEI},
    {"plutostart",        KW_SETUP_DEPRECATED},
    {"crlcheckinterval",  KW_SETUP_DEPRECATED},
    {"rightcustcppcscf6", KW_RIGHTCUSTCPPCSCF6},
    {"rightcert",         KW_RIGHTCERT},
    {"leftupdown",        KW_LEFTUPDOWN},
    {"ocspuri",           KW_OCSPURI},
    {"liveness_check",    KW_LIVENESS_CHECK},
    {"crluri1",           KW_CRLURI},
    {"lifepackets",       KW_LIFEPACKETS},
    {"rightnexthop",      KW_RIGHT_DEPRECATED},
    {"rightintnetmask",   KW_RIGHTINTNETMASK},
    {"rightallowany",     KW_RIGHTALLOWANY},
    {"reqid",             KW_REQID},
    {"leftikeport",       KW_LEFTIKEPORT},
    {"rightid",           KW_RIGHTID},
    {"xauth_identity",    KW_XAUTH_IDENTITY},
    {"leftimei",          KW_LEFTIMEI},
    {"leftikeportnatt",   KW_LEFTIKEPORTNATT},
    {"reauth_mbb",        KW_REAUTH_MBB},
    {"rightcertpolicy",   KW_RIGHTCERTPOLICY},
    {"ocspuri1",          KW_OCSPURI},
    {"eap",               KW_CONN_DEPRECATED},
    {"compress",          KW_COMPRESS},
    {"keepalivedelay",    KW_KEEPALIVEDELAY},
    {"nat_traversal",     KW_SETUP_DEPRECATED},
    {"mobike",	           KW_MOBIKE},
    {"rekeyfuzz",         KW_REKEYFUZZ},
    {"rightfirewall",     KW_RIGHTFIREWALL},
    {"rightforcetsifull", KW_RIGHTFORCETSIFULL},
    {"ldaphost",          KW_CA_DEPRECATED},
    {"no_eaponly",        KW_NO_EAP_ONLY},
    {"rightdns",          KW_RIGHTDNS},
    {"righthostaccess",   KW_RIGHTHOSTACCESS},
    {"device_identity",   KW_DEVICE_IDENTITY},
    {"leftsourceif",      KW_LEFTSOURCEIF},
    {"hidetos",           KW_SETUP_DEPRECATED},
    {"leftintnetmask",    KW_LEFTINTNETMASK},
    {"ocsp",              KW_OCSP},
    {"pcscf_restore",     KW_PCSCF_RESTORE},
    {"leftca",            KW_LEFTCA},
    {"rightcustcppcscf4", KW_RIGHTCUSTCPPCSCF4},
    {"force_keepalive",   KW_SETUP_DEPRECATED},
    {"mediated_by",       KW_MEDIATED_BY},
    {"leftidr_apn",       KW_LEFTIDR_APN},
    {"rightsourceif",     KW_RIGHTSOURCEIF},
    {"packetdefault",     KW_SETUP_DEPRECATED},
    {"oostimeout",        KW_OOSTIMEOUT},
    {"rightca",           KW_RIGHTCA},
    {"rekeymargin",       KW_REKEYMARGIN},
    {"interface",         KW_INTERFACE},
    {"forceencaps",       KW_FORCEENCAPS},
    {"nocrsend",          KW_SETUP_DEPRECATED},
    {"mediation",         KW_MEDIATION},
    {"pkcs11module",      KW_PKCS11_DEPRECATED},
    {"keep_alive",        KW_SETUP_DEPRECATED},
    {"fragmentation",     KW_FRAGMENTATION},
    {"pkcs11keepstate",   KW_PKCS11_DEPRECATED},
    {"xauth",             KW_XAUTH},
    {"cacert",            KW_CACERT},
    {"rightgroups",       KW_RIGHTGROUPS},
    {"keyexchange",       KW_KEYEXCHANGE},
    {"rightpcscf",        KW_RIGHTPCSCF},
    {"rightauth",         KW_RIGHTAUTH},
    {"wsharkfiledump",    KW_WSHARKFILEDUMP},
    {"ldapbase",          KW_CA_DEPRECATED},
    {"rightforcetsi64",   KW_RIGHTFORCETSI64},
    {"dpddelay",          KW_DPDDELAY},
    {"interfaces",        KW_SETUP_DEPRECATED},
    {"leftcustcpimei",    KW_LEFTCUSTCPIMEI},
    {"pkcs11proxy",       KW_PKCS11_DEPRECATED},
    {"pfs",               KW_PFS_DEPRECATED},
    {"leftcustcppcscf6",  KW_LEFTCUSTCPPCSCF6},
    {"leftcert2",         KW_LEFTCERT2},
    {"rightid2",          KW_RIGHTID2},
    {"pkcs11initargs",    KW_PKCS11_DEPRECATED},
    {"leftsubnet",        KW_LEFTSUBNET},
    {"closeaction",       KW_CLOSEACTION},
    {"leftgroups2",       KW_LEFTGROUPS2},
    {"overridemtu",       KW_SETUP_DEPRECATED},
    {"leftpcscf",         KW_LEFTPCSCF},
    {"dpd_noreply",       KW_DPD_NOREPLY},
    {"tfc",               KW_TFC},
    {"reauth",            KW_REAUTH},
    {"eap_identity",      KW_EAP_IDENTITY},
    {"wdrv_keepalive",    KW_WDRV_KEEPALIVE},
    {"mark_out",          KW_MARK_OUT},
    {"keylife",           KW_KEYLIFE},
    {"leftid",            KW_LEFTID},
    {"dpdaction",         KW_DPDACTION},
    {"ikelifetime",       KW_IKELIFETIME},
    {"hashandurl",        KW_HASHANDURL},
    {"me_peerid",         KW_ME_PEERID},
    {"mark_in",           KW_MARK_IN},
    {"ikedscp",           KW_IKEDSCP,},
    {"detach_timeout",    KW_DETACH_TIMEOUT},
    {"margintime",        KW_REKEYMARGIN},
    {"plutostderrlog",    KW_SETUP_DEPRECATED},
    {"rightupdown",       KW_RIGHTUPDOWN},
    {"leftsubnetwithin",  KW_LEFTSUBNET},
    {"skipcheckcert",     KW_SKIPCHECKCERT},
    {"charonstart",       KW_SETUP_DEPRECATED},
    {"pfsgroup",          KW_PFS_DEPRECATED},
    {"leftcustcppcscf4",  KW_LEFTCUSTCPPCSCF4},
    {"aggressive",        KW_AGGRESSIVE},
    {"uniqueids",         KW_UNIQUEIDS},
    {"marginbytes",       KW_MARGINBYTES},
    {"prepluto",          KW_SETUP_DEPRECATED},
    {"marginpackets",     KW_MARGINPACKETS},
    {"fragicmp",          KW_SETUP_DEPRECATED},
    {"cachecrls",         KW_CACHECRLS},
    {"leftca2",           KW_LEFTCA2},
    {"auto",              KW_AUTO},
    {"keyingtries",       KW_KEYINGTRIES},
    {"ah",                KW_AH},
    {"rightauth2",        KW_RIGHTAUTH2},
    {"mark",              KW_MARK},
    {"crluri2",           KW_CRLURI2},
    {"rightca2",          KW_RIGHTCA2},
    {"rightcert2",        KW_RIGHTCERT2},
    {"aaa_identity",      KW_AAA_IDENTITY},
    {"leftauth",          KW_LEFTAUTH},
    {"dpdtimeout",        KW_DPDTIMEOUT},
    {"modeconfig",        KW_MODECONFIG},
    {"ocspuri2",          KW_OCSPURI2},
    {"dumpdir",           KW_SETUP_DEPRECATED},
    {"leftid2",           KW_LEFTID2},
    {"authby",            KW_AUTHBY},
    {"fast_reauth",       KW_FAST_REAUTH},
    {"plutodebug",        KW_SETUP_DEPRECATED},
    {"skipcheckid",       KW_SKIPCHECKID},
    {"rightgroups2",      KW_RIGHTGROUPS2},
    {"klipsdebug",        KW_SETUP_DEPRECATED},
    {"charondebug",       KW_CHARONDEBUG},
    {"leftauth2",         KW_LEFTAUTH2},
    {"use_cfg_vip",       KW_USE_CFG_VIP},
    {"addrchg_reauth",    KW_ADDRCHG_REAUTH}
  };

static const short lookup[] =
  {
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,   0,  -1,  -1,  -1,   1,   2,
     -1,   3,  -1,  -1,  -1,   4,  -1,   5,  -1,  -1,
     -1,   6,  -1,   7,  -1,   8,   9,  10,  11,  12,
     13,  -1,  -1,  14,  15,  16,  -1,  17,  -1,  -1,
     -1,  -1,  18,  -1,  -1,  -1,  -1,  -1,  19,  -1,
     20,  21,  22,  23,  24,  25,  -1,  26,  -1,  27,
     28,  -1,  29,  30,  -1,  -1,  -1,  -1,  31,  -1,
     32,  33,  -1,  34,  35,  -1,  36,  37,  38,  39,
     40,  41,  -1,  42,  43,  44,  45,  -1,  46,  -1,
     47,  48,  49,  50,  51,  52,  -1,  53,  54,  55,
     -1,  56,  57,  58,  59,  60,  61,  62,  63,  -1,
     64,  -1,  65,  66,  67,  68,  69,  70,  71,  72,
     -1,  -1,  73,  -1,  -1,  74,  75,  -1,  76,  77,
     78,  79,  -1,  80,  -1,  -1,  -1,  81,  -1,  82,
     83,  84,  85,  86,  87,  88,  89,  -1,  90,  91,
     92,  93,  94,  -1,  95,  -1,  -1,  96,  97,  98,
     99, 100,  -1,  -1,  -1, 101, 102,  -1, 103, 104,
    105,  -1,  -1, 106, 107, 108, 109, 110,  -1, 111,
     -1, 112, 113, 114, 115, 116, 117,  -1, 118, 119,
     -1,  -1, 120, 121,  -1, 122, 123, 124, 125, 126,
    127, 128,  -1, 129, 130, 131, 132, 133,  -1, 134,
    135,  -1,  -1, 136, 137,  -1, 138, 139, 140, 141,
    142,  -1,  -1, 143,  -1, 144,  -1,  -1,  -1, 145,
    146, 147,  -1, 148, 149, 150,  -1, 151,  -1,  -1,
     -1, 152, 153,  -1,  -1,  -1, 154,  -1,  -1, 155,
    156,  -1,  -1,  -1,  -1, 157,  -1,  -1,  -1, 158,
    159, 160, 161,  -1,  -1, 162,  -1,  -1,  -1, 163,
    164, 165,  -1, 166,  -1, 167,  -1, 168,  -1, 169,
    170,  -1, 171,  -1,  -1,  -1, 172,  -1,  -1, 173,
    174,  -1,  -1, 175,  -1,  -1, 176, 177,  -1,  -1,
     -1,  -1,  -1, 178,  -1, 179, 180,  -1,  -1,  -1,
     -1,  -1, 181,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 182,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 183,  -1,
     -1,  -1, 184,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 185,  -1,  -1,  -1,  -1,
    186,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 187,  -1,
     -1,  -1,  -1,  -1, 188
  };

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct kw_entry *
in_word_set (str, len)
     register const char *str;
     register unsigned int len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}
