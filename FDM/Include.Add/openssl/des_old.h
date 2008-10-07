/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
      

#ifndef HEADER_DES_H
#define HEADER_DES_H

#include <openssl/e_os2.h>	

#ifdef OPENSSL_NO_DES
#error DES is disabled.
#endif

#ifndef HEADER_NEW_DES_H
#error You must include des.h, not des_old.h directly.
#endif

#ifdef _KERBEROS_DES_H
#error <openssl/des_old.h> replaces <kerberos/des.h>.
#endif

#include <openssl/symhacks.h>

#ifdef OPENSSL_BUILD_SHLIBCRYPTO
# undef OPENSSL_EXTERN
# define OPENSSL_EXTERN OPENSSL_EXPORT
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _
#undef _
#endif

typedef unsigned char _ossl_old_des_cblock[8];
typedef struct _ossl_old_des_ks_struct
	{
	union	{
		_ossl_old_des_cblock _;
		
		DES_LONG pad[2];
		} ks;
	} _ossl_old_des_key_schedule[16];

#ifndef OPENSSL_DES_LIBDES_COMPATIBILITY
#define des_cblock DES_cblock
#define const_des_cblock const_DES_cblock
#define des_key_schedule DES_key_schedule
#define des_ecb3_encrypt(i,o,k1,k2,k3,e)\
	DES_ecb3_encrypt((i),(o),&(k1),&(k2),&(k3),(e))
#define des_ede3_cbc_encrypt(i,o,l,k1,k2,k3,iv,e)\
	DES_ede3_cbc_encrypt((i),(o),(l),&(k1),&(k2),&(k3),(iv),(e))
#define des_ede3_cbcm_encrypt(i,o,l,k1,k2,k3,iv1,iv2,e)\
	DES_ede3_cbcm_encrypt((i),(o),(l),&(k1),&(k2),&(k3),(iv1),(iv2),(e))
#define des_ede3_cfb64_encrypt(i,o,l,k1,k2,k3,iv,n,e)\
	DES_ede3_cfb64_encrypt((i),(o),(l),&(k1),&(k2),&(k3),(iv),(n),(e))
#define des_ede3_ofb64_encrypt(i,o,l,k1,k2,k3,iv,n)\
	DES_ede3_ofb64_encrypt((i),(o),(l),&(k1),&(k2),&(k3),(iv),(n))
#define des_options()\
	DES_options()
#define des_cbc_cksum(i,o,l,k,iv)\
	DES_cbc_cksum((i),(o),(l),&(k),(iv))
#define des_cbc_encrypt(i,o,l,k,iv,e)\
	DES_cbc_encrypt((i),(o),(l),&(k),(iv),(e))
#define des_ncbc_encrypt(i,o,l,k,iv,e)\
	DES_ncbc_encrypt((i),(o),(l),&(k),(iv),(e))
#define des_xcbc_encrypt(i,o,l,k,iv,inw,outw,e)\
	DES_xcbc_encrypt((i),(o),(l),&(k),(iv),(inw),(outw),(e))
#define des_cfb_encrypt(i,o,n,l,k,iv,e)\
	DES_cfb_encrypt((i),(o),(n),(l),&(k),(iv),(e))
#define des_ecb_encrypt(i,o,k,e)\
	DES_ecb_encrypt((i),(o),&(k),(e))
#define des_encrypt1(d,k,e)\
	DES_encrypt1((d),&(k),(e))
#define des_encrypt2(d,k,e)\
	DES_encrypt2((d),&(k),(e))
#define des_encrypt3(d,k1,k2,k3)\
	DES_encrypt3((d),&(k1),&(k2),&(k3))
#define des_decrypt3(d,k1,k2,k3)\
	DES_decrypt3((d),&(k1),&(k2),&(k3))
#define des_xwhite_in2out(k,i,o)\
	DES_xwhite_in2out((k),(i),(o))
#define des_enc_read(f,b,l,k,iv)\
	DES_enc_read((f),(b),(l),&(k),(iv))
#define des_enc_write(f,b,l,k,iv)\
	DES_enc_write((f),(b),(l),&(k),(iv))
#define des_fcrypt(b,s,r)\
	DES_fcrypt((b),(s),(r))
#if 0
#define des_crypt(b,s)\
	DES_crypt((b),(s))
#if !defined(PERL5) && !defined(__FreeBSD__) && !defined(NeXT) && !defined(__OpenBSD__)
#define crypt(b,s)\
	DES_crypt((b),(s))
#endif
#endif
#define des_ofb_encrypt(i,o,n,l,k,iv)\
	DES_ofb_encrypt((i),(o),(n),(l),&(k),(iv))
#define des_pcbc_encrypt(i,o,l,k,iv,e)\
	DES_pcbc_encrypt((i),(o),(l),&(k),(iv),(e))
#define des_quad_cksum(i,o,l,c,s)\
	DES_quad_cksum((i),(o),(l),(c),(s))
#define des_random_seed(k)\
	_ossl_096_des_random_seed((k))
#define des_random_key(r)\
	DES_random_key((r))
#define des_read_password(k,p,v) \
	DES_read_password((k),(p),(v))
#define des_read_2passwords(k1,k2,p,v) \
	DES_read_2passwords((k1),(k2),(p),(v))
#define des_set_odd_parity(k)\
	DES_set_odd_parity((k))
#define des_check_key_parity(k)\
	DES_check_key_parity((k))
#define des_is_weak_key(k)\
	DES_is_weak_key((k))
#define des_set_key(k,ks)\
	DES_set_key((k),&(ks))
#define des_key_sched(k,ks)\
	DES_key_sched((k),&(ks))
#define des_set_key_checked(k,ks)\
	DES_set_key_checked((k),&(ks))
#define des_set_key_unchecked(k,ks)\
	DES_set_key_unchecked((k),&(ks))
#define des_string_to_key(s,k)\
	DES_string_to_key((s),(k))
#define des_string_to_2keys(s,k1,k2)\
	DES_string_to_2keys((s),(k1),(k2))
#define des_cfb64_encrypt(i,o,l,ks,iv,n,e)\
	DES_cfb64_encrypt((i),(o),(l),&(ks),(iv),(n),(e))
#define des_ofb64_encrypt(i,o,l,ks,iv,n)\
	DES_ofb64_encrypt((i),(o),(l),&(ks),(iv),(n))
		

#define des_ecb2_encrypt(i,o,k1,k2,e) \
	des_ecb3_encrypt((i),(o),(k1),(k2),(k1),(e))

#define des_ede2_cbc_encrypt(i,o,l,k1,k2,iv,e) \
	des_ede3_cbc_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(e))

#define des_ede2_cfb64_encrypt(i,o,l,k1,k2,iv,n,e) \
	des_ede3_cfb64_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(n),(e))

#define des_ede2_ofb64_encrypt(i,o,l,k1,k2,iv,n) \
	des_ede3_ofb64_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(n))

#define des_check_key DES_check_key
#define des_rw_mode DES_rw_mode
#else 

#define des_cblock _ossl_old_des_cblock
#define des_key_schedule _ossl_old_des_key_schedule
#define des_ecb3_encrypt(i,o,k1,k2,k3,e)\
	_ossl_old_des_ecb3_encrypt((i),(o),(k1),(k2),(k3),(e))
#define des_ede3_cbc_encrypt(i,o,l,k1,k2,k3,iv,e)\
	_ossl_old_des_ede3_cbc_encrypt((i),(o),(l),(k1),(k2),(k3),(iv),(e))
#define des_ede3_cfb64_encrypt(i,o,l,k1,k2,k3,iv,n,e)\
	_ossl_old_des_ede3_cfb64_encrypt((i),(o),(l),(k1),(k2),(k3),(iv),(n),(e))
#define des_ede3_ofb64_encrypt(i,o,l,k1,k2,k3,iv,n)\
	_ossl_old_des_ede3_ofb64_encrypt((i),(o),(l),(k1),(k2),(k3),(iv),(n))
#define des_options()\
	_ossl_old_des_options()
#define des_cbc_cksum(i,o,l,k,iv)\
	_ossl_old_des_cbc_cksum((i),(o),(l),(k),(iv))
#define des_cbc_encrypt(i,o,l,k,iv,e)\
	_ossl_old_des_cbc_encrypt((i),(o),(l),(k),(iv),(e))
#define des_ncbc_encrypt(i,o,l,k,iv,e)\
	_ossl_old_des_ncbc_encrypt((i),(o),(l),(k),(iv),(e))
#define des_xcbc_encrypt(i,o,l,k,iv,inw,outw,e)\
	_ossl_old_des_xcbc_encrypt((i),(o),(l),(k),(iv),(inw),(outw),(e))
#define des_cfb_encrypt(i,o,n,l,k,iv,e)\
	_ossl_old_des_cfb_encrypt((i),(o),(n),(l),(k),(iv),(e))
#define des_ecb_encrypt(i,o,k,e)\
	_ossl_old_des_ecb_encrypt((i),(o),(k),(e))
#define des_encrypt(d,k,e)\
	_ossl_old_des_encrypt((d),(k),(e))
#define des_encrypt2(d,k,e)\
	_ossl_old_des_encrypt2((d),(k),(e))
#define des_encrypt3(d,k1,k2,k3)\
	_ossl_old_des_encrypt3((d),(k1),(k2),(k3))
#define des_decrypt3(d,k1,k2,k3)\
	_ossl_old_des_decrypt3((d),(k1),(k2),(k3))
#define des_xwhite_in2out(k,i,o)\
	_ossl_old_des_xwhite_in2out((k),(i),(o))
#define des_enc_read(f,b,l,k,iv)\
	_ossl_old_des_enc_read((f),(b),(l),(k),(iv))
#define des_enc_write(f,b,l,k,iv)\
	_ossl_old_des_enc_write((f),(b),(l),(k),(iv))
#define des_fcrypt(b,s,r)\
	_ossl_old_des_fcrypt((b),(s),(r))
#define des_crypt(b,s)\
	_ossl_old_des_crypt((b),(s))
#if 0
#define crypt(b,s)\
	_ossl_old_crypt((b),(s))
#endif
#define des_ofb_encrypt(i,o,n,l,k,iv)\
	_ossl_old_des_ofb_encrypt((i),(o),(n),(l),(k),(iv))
#define des_pcbc_encrypt(i,o,l,k,iv,e)\
	_ossl_old_des_pcbc_encrypt((i),(o),(l),(k),(iv),(e))
#define des_quad_cksum(i,o,l,c,s)\
	_ossl_old_des_quad_cksum((i),(o),(l),(c),(s))
#define des_random_seed(k)\
	_ossl_old_des_random_seed((k))
#define des_random_key(r)\
	_ossl_old_des_random_key((r))
#define des_read_password(k,p,v) \
	_ossl_old_des_read_password((k),(p),(v))
#define des_read_2passwords(k1,k2,p,v) \
	_ossl_old_des_read_2passwords((k1),(k2),(p),(v))
#define des_set_odd_parity(k)\
	_ossl_old_des_set_odd_parity((k))
#define des_is_weak_key(k)\
	_ossl_old_des_is_weak_key((k))
#define des_set_key(k,ks)\
	_ossl_old_des_set_key((k),(ks))
#define des_key_sched(k,ks)\
	_ossl_old_des_key_sched((k),(ks))
#define des_string_to_key(s,k)\
	_ossl_old_des_string_to_key((s),(k))
#define des_string_to_2keys(s,k1,k2)\
	_ossl_old_des_string_to_2keys((s),(k1),(k2))
#define des_cfb64_encrypt(i,o,l,ks,iv,n,e)\
	_ossl_old_des_cfb64_encrypt((i),(o),(l),(ks),(iv),(n),(e))
#define des_ofb64_encrypt(i,o,l,ks,iv,n)\
	_ossl_old_des_ofb64_encrypt((i),(o),(l),(ks),(iv),(n))
		

#define des_ecb2_encrypt(i,o,k1,k2,e) \
	des_ecb3_encrypt((i),(o),(k1),(k2),(k1),(e))

#define des_ede2_cbc_encrypt(i,o,l,k1,k2,iv,e) \
	des_ede3_cbc_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(e))

#define des_ede2_cfb64_encrypt(i,o,l,k1,k2,iv,n,e) \
	des_ede3_cfb64_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(n),(e))

#define des_ede2_ofb64_encrypt(i,o,l,k1,k2,iv,n) \
	des_ede3_ofb64_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(n))

#define des_check_key DES_check_key
#define des_rw_mode DES_rw_mode
#endif

const char *_ossl_old_des_options(void);
void _ossl_old_des_ecb3_encrypt(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,
	_ossl_old_des_key_schedule ks1,_ossl_old_des_key_schedule ks2,
	_ossl_old_des_key_schedule ks3, int enc);
DES_LONG _ossl_old_des_cbc_cksum(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,
	long length,_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec);
void _ossl_old_des_cbc_encrypt(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,long length,
	_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec,int enc);
void _ossl_old_des_ncbc_encrypt(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,long length,
	_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec,int enc);
void _ossl_old_des_xcbc_encrypt(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,long length,
	_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec,
	_ossl_old_des_cblock *inw,_ossl_old_des_cblock *outw,int enc);
void _ossl_old_des_cfb_encrypt(unsigned char *in,unsigned char *out,int numbits,
	long length,_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec,int enc);
void _ossl_old_des_ecb_encrypt(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,
	_ossl_old_des_key_schedule ks,int enc);
void _ossl_old_des_encrypt(DES_LONG *data,_ossl_old_des_key_schedule ks, int enc);
void _ossl_old_des_encrypt2(DES_LONG *data,_ossl_old_des_key_schedule ks, int enc);
void _ossl_old_des_encrypt3(DES_LONG *data, _ossl_old_des_key_schedule ks1,
	_ossl_old_des_key_schedule ks2, _ossl_old_des_key_schedule ks3);
void _ossl_old_des_decrypt3(DES_LONG *data, _ossl_old_des_key_schedule ks1,
	_ossl_old_des_key_schedule ks2, _ossl_old_des_key_schedule ks3);
void _ossl_old_des_ede3_cbc_encrypt(_ossl_old_des_cblock *input, _ossl_old_des_cblock *output, 
	long length, _ossl_old_des_key_schedule ks1, _ossl_old_des_key_schedule ks2, 
	_ossl_old_des_key_schedule ks3, _ossl_old_des_cblock *ivec, int enc);
void _ossl_old_des_ede3_cfb64_encrypt(unsigned char *in, unsigned char *out,
	long length, _ossl_old_des_key_schedule ks1, _ossl_old_des_key_schedule ks2,
	_ossl_old_des_key_schedule ks3, _ossl_old_des_cblock *ivec, int *num, int enc);
void _ossl_old_des_ede3_ofb64_encrypt(unsigned char *in, unsigned char *out,
	long length, _ossl_old_des_key_schedule ks1, _ossl_old_des_key_schedule ks2,
	_ossl_old_des_key_schedule ks3, _ossl_old_des_cblock *ivec, int *num);

void _ossl_old_des_xwhite_in2out(_ossl_old_des_cblock (*des_key), _ossl_old_des_cblock (*in_white),
	_ossl_old_des_cblock (*out_white));

int _ossl_old_des_enc_read(int fd,char *buf,int len,_ossl_old_des_key_schedule sched,
	_ossl_old_des_cblock *iv);
int _ossl_old_des_enc_write(int fd,char *buf,int len,_ossl_old_des_key_schedule sched,
	_ossl_old_des_cblock *iv);
char *_ossl_old_des_fcrypt(const char *buf,const char *salt, char *ret);
char *_ossl_old_des_crypt(const char *buf,const char *salt);
#if !defined(PERL5) && !defined(NeXT)
char *_ossl_old_crypt(const char *buf,const char *salt);
#endif
void _ossl_old_des_ofb_encrypt(unsigned char *in,unsigned char *out,
	int numbits,long length,_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec);
void _ossl_old_des_pcbc_encrypt(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,long length,
	_ossl_old_des_key_schedule schedule,_ossl_old_des_cblock *ivec,int enc);
DES_LONG _ossl_old_des_quad_cksum(_ossl_old_des_cblock *input,_ossl_old_des_cblock *output,
	long length,int out_count,_ossl_old_des_cblock *seed);
void _ossl_old_des_random_seed(_ossl_old_des_cblock key);
void _ossl_old_des_random_key(_ossl_old_des_cblock ret);
int _ossl_old_des_read_password(_ossl_old_des_cblock *key,const char *prompt,int verify);
int _ossl_old_des_read_2passwords(_ossl_old_des_cblock *key1,_ossl_old_des_cblock *key2,
	const char *prompt,int verify);
void _ossl_old_des_set_odd_parity(_ossl_old_des_cblock *key);
int _ossl_old_des_is_weak_key(_ossl_old_des_cblock *key);
int _ossl_old_des_set_key(_ossl_old_des_cblock *key,_ossl_old_des_key_schedule schedule);
int _ossl_old_des_key_sched(_ossl_old_des_cblock *key,_ossl_old_des_key_schedule schedule);
void _ossl_old_des_string_to_key(char *str,_ossl_old_des_cblock *key);
void _ossl_old_des_string_to_2keys(char *str,_ossl_old_des_cblock *key1,_ossl_old_des_cblock *key2);
void _ossl_old_des_cfb64_encrypt(unsigned char *in, unsigned char *out, long length,
	_ossl_old_des_key_schedule schedule, _ossl_old_des_cblock *ivec, int *num, int enc);
void _ossl_old_des_ofb64_encrypt(unsigned char *in, unsigned char *out, long length,
	_ossl_old_des_key_schedule schedule, _ossl_old_des_cblock *ivec, int *num);

void _ossl_096_des_random_seed(des_cblock *key);  

#define _KERBEROS_DES_H

#define KRBDES_ENCRYPT DES_ENCRYPT
#define KRBDES_DECRYPT DES_DECRYPT

#ifdef KERBEROS
#  define ENCRYPT DES_ENCRYPT
#  define DECRYPT DES_DECRYPT
#endif

#ifndef NCOMPAT
#  define C_Block des_cblock
#  define Key_schedule des_key_schedule
#  define KEY_SZ DES_KEY_SZ
#  define string_to_key des_string_to_key
#  define read_pw_string des_read_pw_string
#  define random_key des_random_key
#  define pcbc_encrypt des_pcbc_encrypt
#  define set_key des_set_key
#  define key_sched des_key_sched
#  define ecb_encrypt des_ecb_encrypt
#  define cbc_encrypt des_cbc_encrypt
#  define ncbc_encrypt des_ncbc_encrypt
#  define xcbc_encrypt des_xcbc_encrypt
#  define cbc_cksum des_cbc_cksum
#  define quad_cksum des_quad_cksum
#  define check_parity des_check_key_parity
#endif

#define des_fixup_key_parity DES_fixup_key_parity

#ifdef  __cplusplus
}
#endif 

#include <openssl/ui_compat.h>

#endif
