#ifndef ROM_LIBGLOSS_RETARGET_H
#define ROM_LIBGLOSS_RETARGET_H

#include <sys/stat.h>
#include <basic_types.h>

#define rtl_close		rtl_close_v1_00
#define rtl_fstat		rtl_fstat_v1_00
#define rtl_isatty	rtl_isatty_v1_00
#define rtl_lseek		rtl_lseek_v1_00
#define rtl_open		rtl_open_v1_00
#define rtl_read		rtl_read_v1_00
#define rtl_write		rtl_write_v1_00
#define rtl_sbrk		rtl_sbrk_v1_00

extern _ROM_CALL_ int rtl_close_v1_00(int fildes);
extern _ROM_CALL_ int rtl_fstat_v1_00(int fildes , struct stat *st);
extern _ROM_CALL_ int rtl_isatty_v1_00(int file);
extern _ROM_CALL_ int rtl_lseek_v1_00(int file , int ptr , int dir);
extern _ROM_CALL_ int rtl_open_v1_00(char *file , int flags , int mode);
extern _ROM_CALL_ int rtl_read_v1_00(int file , char *ptr , int len);
extern _ROM_CALL_ int rtl_write_v1_00(int file , const char *ptr , int len);
extern _ROM_CALL_ void* rtl_sbrk_v1_00(int incr);


struct _rom_libgloss_ram_map {
	int   (*libgloss_close)(int fildes);
	int   (*libgloss_fstat)(int fildes , struct stat *st);
	int   (*libgloss_isatty)(int file);
	int   (*libgloss_lseek)(int file , int ptr , int dir);
	int   (*libgloss_open)(char *file , int flags , int mode);
	int	  (*libgloss_read)(int file , char *ptr , int len);
	int	  (*libgloss_write)(int file , const char *ptr , int len);
	void* (*libgloss_sbrk)(int incr);
};

#endif	/* ROM_LIBGLOSS_RETARGET_H */
