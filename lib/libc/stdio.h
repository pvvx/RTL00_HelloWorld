#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdint.h>
#include <stdarg.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef LLONG_MAX
#define LLONG_MAX ((long long int)(((unsigned long long int)-1) / 2))
#endif

#ifndef LLONG_MIN
#define LLONG_MIN ((long long int)(-1 * (((unsigned long long int)-1) / 2)) - 1)
#endif

typedef void FILE;

typedef int32_t ssize_t;
typedef uint32_t size_t;

ssize_t write_stdout(const void *buf, size_t count);
extern FILE *stdin, *stdout, *stderr;

int putchar(int c);
int putc(int c, FILE *stream);
int fputc(int c, FILE *stream);
int puts(const char *s);
int fputs(const char *s, FILE *stream);
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

ssize_t write(int fd, const void *buf, size_t count);

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);
void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
size_t strlen(const char *s); /**/
char *strchr(const char *s, int c); /**/
char *strstr(const char *haystack, const char *needle); /**/
int strcmp(const char *s1, const char *s2); /**/
int strncmp(const char *s1, const char *s2, size_t n); /**/
char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, size_t n); /**/
char *strncat(char *s1, const char *s2, size_t n); /**/
char *strcat(char *dst, const char *src); /**/
int strcasecmp(const char *s1, const char *s2); /**/
int strncasecmp(const char *s1, const char *s2, size_t n); /**/
int atoi(const char *nptr); /**/
long int strtol(const char *nptr, char **endptr, int base); /**/
long long int strtoll(const char *nptr, char **endptr, int base); /**/
double strtod(const char *str, char **endptr); /**/

int abs(int j);

#ifndef assert
void assert(int expression);
#endif
void abort(void);

typedef int32_t time_t;
struct tm{
	int tm_sec;         /* seconds */
	int tm_min;         /* minutes */
	int tm_hour;        /* hours */
	int tm_mday;        /* day of the month */
	int tm_mon;         /* month */
	int tm_year;        /* year */
	int tm_wday;        /* day of the week */
	int tm_yday;        /* day in the year */
	int tm_isdst;       /* daylight saving time */
}
#if defined (GCC_VERSION)
__attribute__((packed))
#endif
;

struct tm *gmtime_r(const time_t *timep, struct tm *result);
time_t mktime(struct tm *tm);

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#define isalnum(c)  (isalpha(c) || isdigit(c))
#define isalpha(c)  (isupper(c) || islower(c))
#define isascii(c)  (((c) > 0) && ((c) <= 0x7F))
#define iscntrl(c)  (((c) >= 0) && (((c) <= 0x1F) || ((c) == 0x7F)))
#define isdigit(c)  (((c) >= '0') && ((c) <= '9'))
#define isgraph(c)  (((c) != ' ') && isprint(c))
#define isprint(c)  (((c) >= ' ') && ((c) <= '~'))
#define ispunct(c)  ((((c) > ' ') && ((c) <= '~')) && !isalnum(c))
#define isspace(c)  (((c) ==  ' ') || ((c) == '\f') || ((c) == '\n') || ((c) == '\r') || ((c) == '\t') || ((c) == '\v'))
#define isupper(c)  (((c) >=  'A') && ((c) <= 'Z'))
#define islower(c)  (((c) >=  'a') && ((c) <= 'z'))
#define isxdigit(c) (isdigit(c) || (((c) >= 'A') && ((c) <= 'F')) || (((c) >= 'a') && ((c) <= 'f')))
#define isxupper(c) (isdigit(c) || (((c) >= 'A') && ((c) <= 'F')))
#define isxlower(c) (isdigit(c) || (((c) >= 'a') && ((c) <= 'f')))
#define tolower(c)  (isupper(c) ? ((c) - 'A' + 'a') : (c))
#define toupper(c)  (islower(c) ? ((c) - 'a' + 'A') : (c)) 

#endif

