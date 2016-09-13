/*
 * rom_libc_string.h
 *
 * Definitions for standard library - libc functions.
 */
#ifndef _ROM_LIBC_STRING_H_
#define	_ROM_LIBC_STRING_H_

#include <basic_types.h>

#define rtl_memchr			rtl_memchr_v1_00
#define rtl_memcmp			rtl_memcmp_v1_00
#define rtl_memcpy			rtl_memcpy_v1_00
#define rtl_memmove			rtl_memmove_v1_00
#define rtl_memset			rtl_memset_v1_00
#define rtl_strcat			rtl_strcat_v1_00
#define rtl_strchr			rtl_strchr_v1_00
#define rtl_strcmp			rtl_strcmp_v1_00
#define rtl_strcpy			rtl_strcpy_v1_00
#define rtl_strlen			rtl_strlen_v1_00
#define rtl_strncat			rtl_strncat_v1_00
#define rtl_strncmp			rtl_strncmp_v1_00
#define rtl_strncpy			rtl_strncpy_v1_00
#define rtl_strstr			rtl_strstr_v1_00
#define rtl_strsep			rtl_strsep_v1_00
#define rtl_strtok			rtl_strtok_v1_00

extern _ROM_CALL_ void * rtl_memchr_v1_00(const void * src_void , int c , size_t length);
extern _ROM_CALL_ int rtl_memcmp_v1_00(const void * m1 , const void * m2 , size_t n);
extern _ROM_CALL_ void * rtl_memcpy_v1_00(void * __restrict dst0 , const void * __restrict src0 , size_t len0);
extern _ROM_CALL_ void * rtl_memmove_v1_00( void * dst_void , const void * src_void , size_t length);
extern _ROM_CALL_ void * rtl_memset_v1_00(void * m , int c , size_t n);
extern _ROM_CALL_ char * rtl_strcat_v1_00(char *__restrict s1 , const char *__restrict s2);
extern _ROM_CALL_ char * rtl_strchr_v1_00(const char *s1 , int i);
extern _ROM_CALL_ int rtl_strcmp_v1_00(const char *s1 ,	const char *s2);
extern _ROM_CALL_ char* rtl_strcpy_v1_00(char *dst0 , const char *src0);
extern _ROM_CALL_ size_t rtl_strlen_v1_00(const char *str);
extern _ROM_CALL_ char * rtl_strncat_v1_00(char *__restrict s1 , const char *__restrict s2 , size_t n);
extern _ROM_CALL_ int rtl_strncmp_v1_00(const char *s1 , const char *s2 , size_t n);
extern _ROM_CALL_ char * rtl_strncpy_v1_00(char *__restrict dst0 , const char *__restrict src0 , size_t count);
extern _ROM_CALL_ char * rtl_strstr_v1_00(const char *searchee , const char *lookfor);
extern _ROM_CALL_ char * rtl_strsep_v1_00(register char **source_ptr , register const char *delim);
extern _ROM_CALL_ char * rtl_strtok_v1_00(register char *__restrict s , register const char *__restrict delim);

#endif /* _ROM_LIBC_STRING_H_ */
