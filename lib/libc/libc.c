#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#if defined (LIBC_GMTIME_R) || defined (LIBC_MKTIME)
#include <time.h>
#endif

#ifndef LIBC_FUNCTION_ATTRIBUTE
#define LIBC_FUNCTION_ATTRIBUTE
#endif

#if defined (LIBC_ALL) || defined (LIBC_PUTCHAR) || defined (LIBC_PUTC) || defined (LIBC_FPUTC) || defined (LIBC_PUTS) || defined (LIBC_FPUTS) || defined (LIBC_PRINTF) || defined (LIBC_FPRINTF) || defined (LIBC_WRITE)
#if defined (GCC_VERSION) || defined (__GNUC__)
ssize_t __attribute__((weak)) write_stdout(const void *buf __attribute__((unused)), size_t count){
	return(count);
}
ssize_t __attribute__((weak, alias("write_stdout"))) write_stderr(const void *buf, size_t count);
// FILE *stdin = NULL;
FILE *stdout = write_stdout;
FILE *stderr = write_stderr;
#else
#warning no weak functions for stdio
FILE *stdout;
FILE *stderr;
#endif
#endif

#if defined (LIBC_ALL) || defined (LIBC_SNPRINTF) || defined (LIBC_VSNPRINTF)
struct _libc_snprintf_struct{
	char *str;
	size_t loc;
	size_t maxlen;
}__attribute__((packed));
#endif

#if defined (LIBC_ALL) || defined (LIBC_SNPRINTF) || defined (LIBC_VSNPRINTF)
static int LIBC_FUNCTION_ATTRIBUTE _libc_cswrite(void *user, const void *buf, int count);
#endif
#if defined (LIBC_ALL) || defined (LIBC_PRINTF) || defined (LIBC_FPRINTF) || defined (LIBC_SNPRINTF) || defined (LIBC_VSNPRINTF)
typedef ssize_t(*stream_user_t)(void *user, const void *buf, size_t count);
#define _LIBC_PRINT_PAD_RIGHT 1
#define _LIBC_PRINT_PAD_ZERO  2
#define _LIBC_PRINT_BUF_LEN   32 // sellesse peavad kõik prinditavad numbrid ära mahtuma
typedef ssize_t(*stream_t)(const void *buf, size_t count);
static int LIBC_FUNCTION_ATTRIBUTE _libc_cvprintf(stream_t cb, void *user, const char *format, va_list ap);
static int LIBC_FUNCTION_ATTRIBUTE _libc_cprints(stream_t cb, void *user, const char *string, int width, int pad);
static int LIBC_FUNCTION_ATTRIBUTE _libc_cprinti(stream_t cb, void *user, int i, int b, int sg, int width, int pad, int letbase);
static int LIBC_FUNCTION_ATTRIBUTE _libc_cprintl(stream_t cb, void *user, long long int i, int b, int sg, int width, int pad, int letbase);
static int LIBC_FUNCTION_ATTRIBUTE _libc_cwrite(stream_t cb, void *user, const void *buf, size_t count);
#endif

#if defined (LIBC_ALL) || defined (LIBC_PUTCHAR) || defined (LIBC_PRINTF)
int LIBC_FUNCTION_ATTRIBUTE putchar(int c){
	return(fputc(c, stdout));
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_PUTC)
int LIBC_FUNCTION_ATTRIBUTE putc(int c, FILE *stream){
	return(fputc(c, stream));
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_PUTCHAR) || defined (LIBC_PUTC) || defined (LIBC_FPUTC) || defined (LIBC_PRINTF) || defined (LIBC_FPRINTF)
int LIBC_FUNCTION_ATTRIBUTE fputc(int c, FILE *stream){
	stream_t cb;
	unsigned char _c;
	cb = (stream_t)stream;
	_c = c;
	cb(&_c, 1);
	return(c);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_PUTS) || defined (LIBC_PRINTF)
int LIBC_FUNCTION_ATTRIBUTE puts(const char *s){
	return(fputs(s, stdout));
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_PUTS) || defined (LIBC_FPUTS) || defined (LIBC_PRINTF) || defined (LIBC_FPRINTF)
int LIBC_FUNCTION_ATTRIBUTE fputs(const char *s, FILE *stream){
	stream_t cb;
	int sl;
	cb = (stream_t)stream;
	for(sl = 0; s[sl]; sl++);
	cb(s, sl);
	cb("\n", 1);
	return(sl);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_PRINTF)
int LIBC_FUNCTION_ATTRIBUTE printf(const char *format, ...){
	va_list ap;
	int r;
	va_start(ap, format);
	r = _libc_cvprintf((stream_t)stdout, NULL, format, ap);
	va_end(ap);
	return(r);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_FPRINTF)
int LIBC_FUNCTION_ATTRIBUTE fprintf(FILE *stream, const char *format, ...){
	va_list ap;
	int r;
	va_start(ap, format);
	r = _libc_cvprintf((stream_t)stream, NULL, format, ap);
	va_end(ap);
	return(r);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_SNPRINTF)
int LIBC_FUNCTION_ATTRIBUTE snprintf(char *str, size_t size, const char *format, ...){
	va_list ap;
	int r;
	struct _libc_snprintf_struct s;
	s.str = str;
	s.loc = 0;
	s.maxlen = size;
	va_start(ap, format);
	r = _libc_cvprintf((stream_t)_libc_cswrite, &s, format, ap);
	va_end(ap);
	return(r);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_VSNPRINTF)
int LIBC_FUNCTION_ATTRIBUTE vsnprintf(char *str, size_t size, const char *format, va_list ap){
	int r;
	struct _libc_snprintf_struct s;
	s.str = str;
	s.loc = 0;
	s.maxlen = size;
	r = _libc_cvprintf((stream_t)_libc_cswrite, &s, format, ap);
	return(r);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_SNPRINTF) || defined (LIBC_VSNPRINTF)
static int LIBC_FUNCTION_ATTRIBUTE _libc_cswrite(void *user, const void *buf, int count){
	struct _libc_snprintf_struct *s;
	int i;
	s = (struct _libc_snprintf_struct *)user;
	for(i = 0; (i < count) && (s->loc < s->maxlen); i++, s->loc++){
		if((s->maxlen > 1) && (s->loc < (s->maxlen - 1)))s->str[s->loc] = ((char *)buf)[i];
	}
	if(s->maxlen){
		if(s->loc < (s->maxlen - 1)){
			s->str[s->loc] = 0;
		}else{
			s->str[s->maxlen - 1] = 0;
		}
	}
	return(i);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_PRINTF) || defined (LIBC_FPRINTF) || defined (LIBC_SNPRINTF) || defined (LIBC_VSNPRINTF)
static int LIBC_FUNCTION_ATTRIBUTE _libc_cwrite(stream_t cb, void *user, const void *buf, size_t count){
	stream_user_t cub;
	if(user == NULL)return(cb(buf, count));
	cub = (stream_user_t)cb;
	return(cub(user, buf, count));
}

static int LIBC_FUNCTION_ATTRIBUTE _libc_cvprintf(stream_t cb, void *user, const char *format, va_list ap){
	int width, pad, pc, l;
	const char *s, *e;
	char c;
	pc = 0;
	s = e = format;
	for( ; *format != 0; format++){
		e = format;
		if(*format == '%'){
			format++;
			width = pad = 0;
			if(*format == '\0')break;
			if(*format == '%'){
				pc += _libc_cwrite(cb, user, s, (e - s) + 1);
				s = format + 1;
				continue;
			}
			if(*format == '-'){
				format++;
				pad = _LIBC_PRINT_PAD_RIGHT;
			}
			while(*format == '0'){
				format++;
				pad |= _LIBC_PRINT_PAD_ZERO;
			}
			for( ; *format >= '0' && *format <= '9'; format++){
				width *= 10;
				width += *format - '0';
			}
			if(*format == 's'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				s = va_arg(ap, char *);
				pc += _libc_cprints(cb, user, s ? s : "(null)", width, pad);
				s = format + 1;
				continue;
			}
			l = 0;
			while(*format == 'l'){
				format++;
				l++;
			}
			if(*format == 'd'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				if(!l){
					pc += _libc_cprinti(cb, user, va_arg(ap, int), 10, 1, width, pad, 'a');
				}else if(l == 1){
					pc += _libc_cprintl(cb, user, va_arg(ap, long int), 10, 1, width, pad, 'a');
				}else{
					pc += _libc_cprintl(cb, user, va_arg(ap, long long int), 10, 1, width, pad, 'a');
				}
				s = format + 1;
				continue;
			}
			if(*format == 'b'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				if(!l){
					pc += _libc_cprinti(cb, user, va_arg(ap, unsigned int), 2, 0, width, pad, 'a');
				}else if(l == 1){
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long int), 2, 0, width, pad, 'a');
				}else{
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long long int), 2, 0, width, pad, 'a');
				}
				s = format + 1;
				continue;
			}
			if(*format == 'x'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				if(!l){
					pc += _libc_cprinti(cb, user, va_arg(ap, unsigned int), 16, 0, width, pad, 'a');
				}else if(l == 1){
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long int), 16, 0, width, pad, 'a');
				}else{
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long long int), 16, 0, width, pad, 'a');
				}
				s = format + 1;
				continue;
			}
			if(*format == 'X'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				if(!l){
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned int), 16, 0, width, pad, 'A');
				}else if(l == 1){
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long int), 16, 0, width, pad, 'A');
				}else{
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long long int), 16, 0, width, pad, 'A');
				}
				s = format + 1;
				continue;
			}
			if(*format == 'u'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				if(!l){
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned int), 10, 0, width, pad, 'a');
				}else if(l == 1){
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long int), 10, 0, width, pad, 'a');
				}else{
					pc += _libc_cprintl(cb, user, va_arg(ap, unsigned long long int), 10, 0, width, pad, 'a');
				}
				s = format + 1;
				continue;
			}
			if(*format == 'c'){
				pc += _libc_cwrite(cb, user, s, (e - s));
				c = va_arg(ap, int);
				pc += _libc_cwrite(cb, user, &c, 1);
				s = format + 1;
				continue;
			}
		}
	}
	if(e >= s){
		pc += _libc_cwrite(cb, user, s, (e - s) + 1);
	}
	return(pc);
}

static int LIBC_FUNCTION_ATTRIBUTE _libc_cprints(stream_t cb, void *user, const char *string, int width, int pad){
	int pc, sl, len;
	const char *ptr;
	char padchar;
	pc = 0;
	padchar = ' ';
	if(width > 0){
		len = 0;
		for(ptr = string; *ptr; ++ptr) ++len;
		if(len >= width)width = 0; else width -= len;
		if(pad & _LIBC_PRINT_PAD_ZERO)padchar = '0';
	}
	if(!(pad & _LIBC_PRINT_PAD_RIGHT)){
		for ( ; width > 0; width--){
			pc += _libc_cwrite(cb, user, &padchar, 1);
		}
	}
	for(sl = 0; string[sl]; sl++);
	pc += _libc_cwrite(cb, user, string, sl);
	for ( ; width > 0; --width) {
		pc += _libc_cwrite(cb, user, &padchar, 1);
	}
	return(pc);
}

static int LIBC_FUNCTION_ATTRIBUTE _libc_cprinti(stream_t cb, void *user, int i, int b, int sg, int width, int pad, int letbase){
	char print_buf[_LIBC_PRINT_BUF_LEN], *s;
	unsigned int u;
	int t, neg, pc;
	neg = pc = 0;
	u = i;
	if(i == 0){
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return(_libc_cprints(cb, user, print_buf, width, pad));
	}
	if(sg && (b == 10) && (i < 0)){
		neg = 1;
		u = -i;
	}
	s = print_buf + _LIBC_PRINT_BUF_LEN - 1;
	*s = '\0';
	while(u){
		t = u % b;
		if(t >= 10)t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}
	if(neg){
		if(width && (pad & _LIBC_PRINT_PAD_ZERO)){
			pc += _libc_cwrite(cb, user, "-", 1);
			width--;
		}else{
			*--s = '-';
		}
	}
	return(pc + _libc_cprints(cb, user, s, width, pad));
}

// li -> i: workaround et oleks hetkel alati 4 baidine (unsigned long int)
// STM32 või ARM-C3 või vastav GCC ei oska 8 baidist arvu jagada
static int LIBC_FUNCTION_ATTRIBUTE _libc_cprintl(stream_t cb, void *user, long long int li, int b, int sg, int width, int pad, int letbase){
	char print_buf[_LIBC_PRINT_BUF_LEN], *s;
	unsigned long int u;
	long int i;
	int t, neg, pc;
	neg = pc = 0;
	i = li;
	u = i;
	if(i == 0){
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return(_libc_cprints(cb, user, print_buf, width, pad));
	}
	if(sg && (b == 10) && (i < 0)){
		neg = 1;
		u = -i;
	}
	s = print_buf + _LIBC_PRINT_BUF_LEN - 1;
	*s = '\0';
	while(u){
		t = u % b;
		if(t >= 10)t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}
	if(neg){
		if(width && (pad & _LIBC_PRINT_PAD_ZERO)){
			pc += _libc_cwrite(cb, user, "-", 1);
			width--;
		}else{
			*--s = '-';
		}
	}
	return(pc + _libc_cprints(cb, user, s, width, pad));
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_WRITE)
ssize_t LIBC_FUNCTION_ATTRIBUTE write(int fd, const void *buf, size_t count){
	stream_t cb;
	if(fd == 1){
		if(stdout != NULL){
			cb = (stream_t)stdout;
			return(cb(buf, count));
		}
	}else if(fd == 2){
		if(stderr != NULL){
			cb = (stream_t)stderr;
			return(cb(buf, count));
		}else if(stdout != NULL){
			cb = (stream_t)stdout;
			return(cb(buf, count));
		}
	}
	return(0);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_MEMCPY)
void LIBC_FUNCTION_ATTRIBUTE *memcpy(void *dest, const void *src, size_t n){
	register char *_dest;
	register const char *_src;
	_dest = dest;
	_src = src;
	while(n-- > 0)*_dest++ = *_src++;
	return(dest);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_MEMMOVE)
void LIBC_FUNCTION_ATTRIBUTE *memmove(void *dest, const void *src, size_t n){
	register char *_dest;
	register const char *_src;
	_dest = dest;
	_src = src;
	if(_src < _dest){ /* Moving from low mem to hi mem; start at end.  */
		for(_src += n, _dest += n; n; n--){
			*--_dest = *--_src;
		}
	}else if(_src != _dest){ /* Moving from hi mem to low mem; start at beginning.  */
		for( ; n; n--){
			*_dest++ = *_src++;
		}
	}
	return(dest);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_MEMCMP) || defined (LIBC_MEMMEM)
int LIBC_FUNCTION_ATTRIBUTE memcmp(const void *s1, const void *s2, size_t n){
	register const unsigned char *str1;
	register const unsigned char *str2;
	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while(n-- > 0){
		if(*str1++ != *str2++)return str1[-1] < str2[-1] ? -1 : 1;
	}
	return(0);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_MEMSET)
void LIBC_FUNCTION_ATTRIBUTE *memset(void *s, int c, size_t n){
	register char *str = s;
	while(n-- > 0)*str++ = c;
	return(s);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_MEMMEM)
void LIBC_FUNCTION_ATTRIBUTE *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen){
	const char *begin;
	const char *const last_possible = (const char *)haystack + haystacklen - needlelen;
	if(needlelen == 0)return((void *)haystack);
	if(haystacklen < needlelen)return(NULL);
	for(begin = (const char *)haystack; begin <= last_possible; ++begin){
		if(begin[0] == ((const char *)needle)[0] && !memcmp((const void *)&begin[1], (const void *)((const char *)needle + 1), needlelen - 1)){
			return((void *)begin);
		}
	}
	return(NULL);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRLEN)
size_t LIBC_FUNCTION_ATTRIBUTE strlen(const char *s){
	register size_t n;
	n = 0;
	while(*s++)n++;
	return(n);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRCHR)
char LIBC_FUNCTION_ATTRIBUTE *strchr(const char *s, int c){
	for( ; *s != (char)c; s++)
	if(*s == 0)return(0);
	return((char *)s);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRSTR)
char LIBC_FUNCTION_ATTRIBUTE *strstr(const char *haystack, const char *needle){
	register char *a, *b;
	b = (char *)needle;
	if(*b == 0)return((char *)haystack);
	for( ; *haystack != 0; haystack += 1){
		if(*haystack != *b)continue;
		a = (char *)haystack;
		while(1){
			if(*b == 0)return((char *)haystack);
			if(*a++ != *b++)break;
		}
		b = (char *)needle;
	}
	return(NULL);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRCMP)
int LIBC_FUNCTION_ATTRIBUTE strcmp(const char *s1, const char *s2){
	for( ; *s1 == *s2; ++s1, ++s2)if(*s1 == 0)return(0);
	return(*(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRNCMP)
int LIBC_FUNCTION_ATTRIBUTE strncmp(const char *s1, const char *s2, size_t n){
	for( ; n > 0; s1++, s2++, n--){
		if(*s1 == 0)return(0);
		if(*s1 != *s2)return((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1);
	}
	return(0);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRCPY)
char LIBC_FUNCTION_ATTRIBUTE *strcpy(char *s1, const char *s2){
	char *s;
	s = s1;
	while((*s++ = *s2++) != 0);
	return(s1);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRNCPY)
char LIBC_FUNCTION_ATTRIBUTE *strncpy(char *s1, const char *s2, size_t n){
	char *s;
	s = s1;
	if(n != 0){
		do{
			if((*s++ = *s2++) == 0){
				while(--n != 0)*s++ = 0;
				break;
			}
		}while(--n != 0);
	}
	return(s1);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRNCAT)
char LIBC_FUNCTION_ATTRIBUTE *strncat(char *s1, const char *s2, size_t n){
	char *d = s1;
	if (n != 0) {
         	while (*d != 0)d++;
         	do {
             		if ((*d = *s2++) == 0)break;
             		d++;
         	} 
		while (--n != 0);
         	*d = 0;
     	}
     	return s1;
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRCAT)
char LIBC_FUNCTION_ATTRIBUTE *strcat(char *s1, const char *s2){
	char *d = s1;
        while (*s1 != 0)s1++;
     	while ((*s1++ = *s2++) != '\0');
     	return d;
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRCASECMP)
int LIBC_FUNCTION_ATTRIBUTE strcasecmp(const char *s1, const char *s2){
	for( ; tolower(*s1) == tolower(*s2); ++s1, ++s2)if(*s1 == 0)return(0);
	return(tolower(*(unsigned char *)s1) < tolower(*(unsigned char *)s2) ? -1 : 1);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRNCASECMP)
int LIBC_FUNCTION_ATTRIBUTE strncasecmp(const char *s1, const char *s2, size_t n){
	for( ; n > 0; s1++, s2++, n--){
		if(*s1 == 0)return(0);
		if(tolower(*s1) != tolower(*s2))return((tolower(*(unsigned char *)s1) < tolower(*(unsigned char *)s2)) ? -1 : 1);
	}
	return(0);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_ATOI)
int LIBC_FUNCTION_ATTRIBUTE atoi(const char *nptr){
	register int num, neg;
	register char c;
	num = neg = 0;
	c = *nptr;
	while((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t'))c = *++nptr;
	if(c == '-'){ /* get an optional sign */
		neg = 1;
		c = *++nptr;
	}else if(c == '+'){
		c = *++nptr;
	}

	while((c >= '0') && (c <= '9')){
		num = (10 * num) + (c - '0');
		c = *++nptr;
	}
	if(neg)return(0 - num);
	return(num);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRTOL)
long int LIBC_FUNCTION_ATTRIBUTE strtol(const char *nptr, char **endptr, int base){
	return(strtoll(nptr, endptr, base));
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRTOL) || defined (LIBC_STRTOLL)
long long int LIBC_FUNCTION_ATTRIBUTE strtoll(const char *nptr, char **endptr, int base){
	long long int acc, cutoff;
	int c, neg, any, cutlim;
	const char *s;
	s = nptr;
	do{
		c = (unsigned char)*s++;
	}while(isspace(c));
	if(c == '-'){
		neg = 1;
		c = *s++;
	}else{
		neg = 0;
		if(c == '+')c = *s++;
	}
	if(((base == 0) || (base == 16)) && (c == '0') && ((*s == 'x') || (*s == 'X'))){
		c = s[1];
		s += 2;
		base = 16;
	}
	if(base == 0)base = (c == '0') ? 8 : 10;
	cutoff = neg ? LLONG_MIN : LLONG_MAX;
	cutlim = (int)(cutoff % base);
	cutoff /= base;
	if(neg){
		if(cutlim > 0){
			cutlim -= base;
			cutoff += 1;
		}
		cutlim = -cutlim;
	}
	for(acc = 0, any = 0; ; c = (unsigned char)*s++){
		if(isdigit(c)){
			c -= '0';
		}else if(isalpha(c)){
			c -= isupper(c) ? ('A' - 10) : ('a' - 10);
		}else{
			break;
		}
		if(c >= base)break;
		if(any < 0)continue;
		if(neg){
			if((acc < cutoff) || ((acc == cutoff) && (c > cutlim))){
				any = -1;
				acc = LLONG_MIN;
				// errno = ERANGE;
			}else{
				any = 1;
				acc *= base;
				acc -= c;
			}
		}else{
			if((acc > cutoff) || ((acc == cutoff) && (c > cutlim))){
				any = -1;
				acc = LLONG_MAX;
				// errno = ERANGE;
			}else{
				any = 1;
				acc *= base;
				acc += c;
			}
		}
	}
	if(endptr != 0)*endptr = (char *)(any ? (s - 1) : nptr);
	return(acc);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_STRTOD)
#define _LIBC_HUGE_VAL    (__builtin_huge_val())
#define _LIBC_DBL_MIN_EXP (__DBL_MIN_EXP__)
#define _LIBC_DBL_MAX_EXP (__DBL_MAX_EXP__)
double LIBC_FUNCTION_ATTRIBUTE strtod(const char *str, char **endptr){
	double number, p10;
	int exponent, negative, n, num_digits, num_decimals;
	char *p;

	p = (char *)str;
	while(isspace(*p))p++;

	negative = 0;
	switch(*p){
		case '-': negative = 1; // Fall through to increment position
		case '+': p++;
	}

	number = 0.;
	exponent = num_digits = num_decimals = 0;

	while(isdigit(*p)){
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}
	// Process decimal part
	if(*p == '.'){
		p++;
		while(isdigit(*p)){
			number = number * 10. + (*p - '0');
			p++;
			num_digits++;
			num_decimals++;
		}
		exponent -= num_decimals;
	}
	if(num_digits == 0){
		// errno = ERANGE;
		return(0.0);
	}
	// Correct for sign
	if(negative)number = -number;
	// Process an exponent string
	if(*p == 'e' || *p == 'E'){
		// Handle optional sign
		negative = 0;
		switch(*++p){   
			case '-': negative = 1; // Fall through to increment pos
			case '+': p++;
		}
		// Process string of digits
		n = 0;
		while(isdigit(*p)){
			n = n * 10 + (*p - '0');
			p++;
		}
		if(negative){
			exponent -= n;
		}else{
			exponent += n;
		}
	}
	if((exponent < _LIBC_DBL_MIN_EXP) || (exponent > _LIBC_DBL_MAX_EXP)){
		// errno = ERANGE;
		return(_LIBC_HUGE_VAL);
	}
	// Scale the result
	p10 = 10.;
	n = exponent;
	if(n < 0)n = -n;
	while(n){
		if(n & 1){
			if(exponent < 0){
				number /= p10;
			}else{
				number *= p10;
			}
		}
		n >>= 1;
		p10 *= p10;
	}
	// if (number == _LIBC_HUGE_VAL)errno = ERANGE;
	if(endptr)*endptr = p;
	return(number);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_ABS)
int LIBC_FUNCTION_ATTRIBUTE abs(int j){
	if(j < 0)return(-j);
	return(j);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_ASSERT)
void LIBC_FUNCTION_ATTRIBUTE assert(int expression){
}
#endif

#if defined (LIBC_ALL) || defined(LIBC_ABORT)
void LIBC_FUNCTION_ATTRIBUTE abort(void){
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_GMTIME_R)
#define _LIBC_YEAR0           1900 /* the first year */
#define _LIBC_EPOCH_YR        1970 /* EPOCH = Jan 1 1970 00:00:00 */
#define _LIBC_SECS_DAY        (24L * 60L * 60L)
#define _LIBC_LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define _LIBC_YEARSIZE(year)  (_LIBC_LEAPYEAR(year) ? 366 : 365)
static const int _ytab[2][12] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};
struct tm * LIBC_FUNCTION_ATTRIBUTE gmtime_r(const time_t *timep, struct tm *result){
	unsigned long dayclock, dayno;
	int year;

	year = _LIBC_EPOCH_YR;
	dayclock = (unsigned long)*timep % _LIBC_SECS_DAY;
	dayno = (unsigned long)*timep / _LIBC_SECS_DAY;

	result->tm_sec = dayclock % 60;
	result->tm_min = (dayclock % 3600) / 60;
	result->tm_hour = dayclock / 3600;
	result->tm_wday = (dayno + 4) % 7; /* day 0 was a thursday */
	while(dayno >= _LIBC_YEARSIZE(year)){
		dayno -= _LIBC_YEARSIZE(year);
		year++;
	}
        result->tm_year = year - _LIBC_YEAR0;
        result->tm_yday = dayno;
        result->tm_mon = 0;
	while(dayno >= _ytab[_LIBC_LEAPYEAR(year)][result->tm_mon]){
		dayno -= _ytab[_LIBC_LEAPYEAR(year)][result->tm_mon];
		result->tm_mon++;
        }
        result->tm_mday = dayno + 1;
        result->tm_isdst = 0;
        return(result);
}
#endif

#if defined (LIBC_ALL) || defined (LIBC_MKTIME)
static const int m_to_d[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
time_t LIBC_FUNCTION_ATTRIBUTE mktime(struct tm *tm){
	int month, year;
	time_t result;

	month = tm->tm_mon;
	year = tm->tm_year + month / 12 + 1900;
	month %= 12;
	if(month < 0){
		year -= 1;
		month += 12;
	}
	result = (year - 1970) * 365 + (year - 1969) / 4 + m_to_d[month];
	result = (year - 1970) * 365 + m_to_d[month];
	if(month <= 1)year -= 1;
	result += (year - 1968) / 4;
	result -= (year - 1900) / 100;
	result += (year - 1600) / 400;
	result += tm->tm_mday;
	result -= 1;
	result *= 24;
	result += tm->tm_hour;
	result *= 60;
	result += tm->tm_min;
	result *= 60;
	result += tm->tm_sec;
	return(result);
}
#endif

