/*
 * rtl_lib.h
 *
 * Definitions for RTL library functions
 */
 
#ifndef _RTL_LIB_ROM_H_
#define	_RTL_LIB_ROM_H_


#include <basic_types.h>
#include <diag.h>

#include <reent.h>

#include "../libc/rom/string/rom_libc_string.h"
#include "../libgloss/rtl8195a/rom/rom_libgloss_retarget.h"

#ifndef _PTR
#define	_PTR		void *
#endif

#ifndef _AND
#define	_AND		,
#endif

#ifndef _NOARGS
#define	_NOARGS		void
#endif

#ifndef _CONST
#define	_CONST		const
#endif

#ifndef _VOLATILE
#define	_VOLATILE	volatile
#endif

#ifndef _SIGNED
#define	_SIGNED		signed
#endif

#ifndef _DOTS
#define	_DOTS		, ...
#endif

#ifndef _VOID
#define _VOID 		void
#endif


//
// RTL library functions in ROM
// 

#define rtl_memset	rtl_memset_v1_00
#define rtl_memchr	rtl_memchr_v1_00
#define rtl_memmove	rtl_memmove_v1_00
#define rtl_strcmp	rtl_strcmp_v1_00
#define rtl_memcpy	rtl_memcpy_v1_00



extern _ROM_CALL_ void * rtl_memset_v1_00(void * m , int c , size_t n);
extern _ROM_CALL_ void * rtl_memchr_v1_00(const void * src_void , int c , size_t length);
extern _ROM_CALL_ void * rtl_memmove_v1_00( void * dst_void , const void * src_void , size_t length);
extern _ROM_CALL_ int rtl_strcmp_v1_00(const char *s1 ,	const char *s2);
extern _ROM_CALL_ void * rtl_memcpy_v1_00(void * __restrict dst0 , const void * __restrict src0 , size_t len0);


//
// rtl eabi functions
// 

#define rtl_itod 		rtl_itod_v1_00
#define rtl_dtoi		rtl_dtoi_v1_00
#define rtl_uitof 	rtl_uitof_v1_00
#define rtl_uitod 	rtl_uitod_v1_00



#define rtl_dcmpeq 	rtl_dcmpeq_v1_00
#define rtl_dcmplt 	rtl_dcmplt_v1_00
#define rtl_dcmpgt 	rtl_dcmpgt_v1_00


#define rtl_dadd 		rtl_dadd_v1_00
#define rtl_dsub 		rtl_dsub_v1_00
#define rtl_dmul 		rtl_dmul_v1_00
#define rtl_ddiv 		rtl_ddiv_v1_00

extern _ROM_CALL_ double rtl_itod_v1_00(int lval);
extern _ROM_CALL_ int rtl_dtoi_v1_00(double d);
extern _ROM_CALL_ float rtl_uitof_v1_00(unsigned int lval);
extern _ROM_CALL_ double rtl_uitod_v1_00(unsigned int lval);


extern _ROM_CALL_ int 	  rtl_dcmpeq_v1_00(double a, double b);
extern _ROM_CALL_ int 	  rtl_dcmplt_v1_00(double a, double b);
extern _ROM_CALL_ int 	  rtl_dcmpgt_v1_00(double a, double b);


extern _ROM_CALL_ double rtl_dadd_v1_00(double a, double b);
extern _ROM_CALL_ double rtl_dsub_v1_00(double a, double b);
extern _ROM_CALL_ double rtl_dmul_v1_00(double a, double b);
extern _ROM_CALL_ double rtl_ddiv_v1_00(double a, double b);


//
// mprec
//

#include <reent.h>


typedef struct _Bigint _Bigint;


#define rtl_Balloc	rtl_Balloc_v1_00
#define rtl_Bfree 	rtl_Bfree_v1_00
#define rtl_d2b       rtl_d2b_v1_00
#define rtl_i2b		rtl_i2b_v1_00
#define rtl_pow5mult	rtl_pow5mult_v1_00
#define rtl_multadd	rtl_multadd_v1_00
#define rtl_mult		rtl_mult_v1_00
#define rtl_hi0bits	rtl_hi0bits_v1_00
#define rtl_lshift	rtl_lshift_v1_00
#define rtl_cmp		rtl_cmp_v1_00
#define rtl_diff		rtl_diff_v1_00


extern _ROM_CALL_ _Bigint * rtl_Balloc_v1_00(struct _reent *ptr, int k);

extern _ROM_CALL_ void rtl_Bfree_v1_00(struct _reent *ptr, _Bigint * v);

extern _ROM_CALL_ _Bigint * rtl_d2b_v1_00(struct _reent * ptr, double _d, int *e, int *bits);
extern _ROM_CALL_ _Bigint * rtl_i2b_v1_00(struct _reent *ptr, int i );
extern _ROM_CALL_ _Bigint * rtl_pow5mult_v1_00(struct _reent * ptr, _Bigint *b, int k);
extern _ROM_CALL_ _Bigint * rtl_multadd_v1_00(struct _reent *ptr, _Bigint * b, int m, int a);
extern _ROM_CALL_ _Bigint * rtl_mult_v1_00(struct _reent *ptr, _Bigint *a, _Bigint *b);
extern _ROM_CALL_ int rtl_hi0bits_v1_00(register __ULong x);
extern _ROM_CALL_ _Bigint *rtl_lshift_v1_00(struct _reent *ptr, _Bigint *b, int k);
extern _ROM_CALL_ int rtl_cmp_v1_00(_Bigint *a, _Bigint *b);
extern _ROM_CALL_ _Bigint *rtl_diff_v1_00(struct _reent* ptr, _Bigint *a, _Bigint *b);

//
// dtoa
//

#define rtl_dtoa_r	rtl_dtoa_r_v1_00

extern char * rtl_dtoa_r_v1_00(struct _reent *ptr, double _d, int mode, int ndigits, int *decpt, int *sign, char **rve);

//
// mallocr
//
#include <sys/config.h>
#include <reent.h>



#define __rom_mallocr_init	__rom_mallocr_init_v1_00

#define rtl_calloc_r		rtl_calloc_r_v1_00
#define rtl_cfree_r 		rtl_cfree_r_v1_00
#define rtl_malloc_r		rtl_malloc_r_v1_00
#define rtl_free_r		rtl_free_r_v1_00
#define rtl_realloc_r		rtl_realloc_r_v1_00
#define rtl_memalign_r 	rtl_memalign_r_v1_00
#define rtl_valloc_r      rtl_valloc_r_v1_00
#define rtl_pvalloc_r     rtl_pvalloc_r_v1_00


extern _ROM_CALL_ void __rom_mallocr_init_v1_00(void);


#define RARG struct _reent *reent_ptr,
extern _ROM_CALL_ void* rtl_calloc_r_v1_00(RARG size_t n, size_t elem_size);
extern _ROM_CALL_ void rtl_cfree_r_v1_00(void *mem);
extern _ROM_CALL_ void* rtl_malloc_r_v1_00(RARG size_t bytes);
extern _ROM_CALL_ void rtl_free_r_v1_00(RARG void* mem);
extern _ROM_CALL_ void* rtl_realloc_r_v1_00(RARG void* oldmem, size_t bytes);
extern _ROM_CALL_ void* rtl_memalign_r_v1_00(RARG size_t alignment, size_t bytes);
extern _ROM_CALL_ void* rtl_valloc_r_v1_00(RARG size_t bytes);
extern _ROM_CALL_ void* rtl_pvalloc_r_v1_00(RARG size_t bytes);


//
// stdio
//
extern int rtl_errno;

#ifndef _READ_WRITE_RETURN_TYPE
#define _READ_WRITE_RETURN_TYPE 	_ssize_t
#endif

#ifndef _READ_WRITE_BUFSIZE_TYPE
#define _READ_WRITE_BUFSIZE_TYPE 	int
#endif

#define rtl_sread		rtl_sread_v1_00
#define rtl_swrite	rtl_swrite_v1_00
#define rtl_seofread	rtl_seofread_v1_00
#define rtl_sseek		rtl_sseek_v1_00
#define rtl_sclose    rtl_sclose_v1_00
#define rtl_sbrk_r 	rtl_sbrk_r_v1_00

extern _ROM_CALL_ _READ_WRITE_RETURN_TYPE rtl_sread_v1_00(
       struct _reent *ptr,
       void *cookie,
       char *buf,
       _READ_WRITE_BUFSIZE_TYPE n);

extern _ROM_CALL_ _READ_WRITE_RETURN_TYPE rtl_swrite_v1_00(
       struct _reent *ptr,
       void *cookie,
       char const *buf,
       _READ_WRITE_BUFSIZE_TYPE n);

extern _ROM_CALL_ _READ_WRITE_RETURN_TYPE rtl_seofread_v1_00(
       struct _reent *_ptr,
       _PTR cookie,
       char *buf,
       _READ_WRITE_BUFSIZE_TYPE len);

extern _ROM_CALL_ _fpos_t rtl_sseek_v1_00(
       struct _reent *ptr _AND
       void *cookie _AND
       _fpos_t offset _AND
       int whence);

extern _ROM_CALL_ int rtl_sclose_v1_00(
       struct _reent *ptr _AND
       void *cookie);

extern _ROM_CALL_ void * rtl_sbrk_r_v1_00(
     struct _reent *ptr,
     ptrdiff_t incr);

//
// vfprintf
//

#include <stdio.h>
#include <stdarg.h>

#define rtl_fflush_r 			rtl_fflush_r_v1_00
#define rtl_vfprintf_r		rtl_vfprintf_r_v1_00

extern _ROM_CALL_ int rtl_fflush_r_v1_00(struct _reent *ptr, register FILE * fp);
extern _ROM_CALL_ int rtl_vfprintf_r_v1_00(struct _reent *, FILE *, const char *, va_list);


#endif /* _RTL_LIB_ROM_H_ */
