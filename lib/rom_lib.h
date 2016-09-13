#ifndef __PLATFORM_ROMLIB_H__
#define __PLATFORM_ROMLIB_H__

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "strproc.h"
	#include "basic_types.h"
//#include "rtl_std_lib/include/rt_lib_rom.h"
//	#include "fwlib/hal_misc.h"
//	#include "rtl_std_lib/include/rtl_lib.h"

#define _ROM_CALL_

extern uint32_t HalDelayUs(uint32_t us);
extern uint32_t DiagPrintf(const char *fmt, ...);
extern uint32_t HalGetCpuClk(VOID);

extern void HalInitLogUart(void);
extern void HalLogUartInit(void);

extern u32 DiagPrintf(IN  const char *fmt, ...);

extern u32 DiagSPrintf(IN  u8 *buf, IN  const char *fmt, ...);

extern int prvDiagPrintf(IN  const char *fmt, ...);

extern int prvDiagSPrintf(IN char *buf, IN  const char *fmt, ...);

extern u8 __ram_start_table_start__[];
extern VOID HalCpuClkConfig(u8  CpuType);
extern VOID VectorTableInitRtl8195A(u32 StackP);
extern VOID HalInitPlatformLogUartV02(VOID);
extern VOID HalReInitPlatformLogUartV02(VOID);
extern VOID HalInitPlatformTimerV02(VOID);

extern u8 HalPinCtrlRtl8195A(
    IN u32  Function,
    IN u32  PinLocation,
    IN BOOL   Operation
    );

extern void HalShowBuildInfoV02(void);
extern VOID SpicInitRtl8195A(u8 InitBaudRate, u8 SpicBitMode);
extern void UartLogIrqHandle(void);
extern u8 GetRomCmdNum(void); // = 6

typedef struct _COMMAND_TABLE_ {
    const   u8* cmd;
    u16     ArgvCnt;
    u32     (*func)(u16 argc, u8* argv[]);
    const   u8* msg;
}COMMAND_TABLE, *PCOMMAND_TABLE;

extern COMMAND_TABLE  UartLogRomCmdTable[];


	
	#define printf                  	DiagPrintf
//	#define printf 							prvDiagPrintf
	#define sprintf(fmt, arg...)		DiagSPrintf((u8*)fmt, ##arg)
//	#define sprintf						rtl_sprintf
//	#define sprintf							prvDiagSPrintf
	#define snprintf					DiagSnPrintf

#define memset	rtl_memset_v1_00
#define memchr	rtl_memchr_v1_00
#define memmove	rtl_memmove_v1_00
#define memcpy	rtl_memcpy_v1_00
extern _ROM_CALL_ void * rtl_memset_v1_00(void * m , int c , size_t n);
extern _ROM_CALL_ void * rtl_memchr_v1_00(const void * src_void , int c , size_t length);
extern _ROM_CALL_ void * rtl_memmove_v1_00( void * dst_void , const void * src_void , size_t length);
extern _ROM_CALL_ void * rtl_memcpy_v1_00(void * __restrict dst0 , const void * __restrict src0 , size_t len0);

#define strcmp	rtl_strcmp_v1_00
extern _ROM_CALL_ int rtl_strcmp_v1_00(const char *s1 ,	const char *s2);

	#define strcat						rtl_strcat
	#define strchr						rtl_strchr
	#define strcpy						rtl_strcpy
	#define strlen(str)					rtl_strlen((const char *)str)
	#define strncat						rtl_strncat
	#define strncmp(s1, s2, n)			rtl_strncmp((const char *)s1, (const char *)s2, n)
	#define strncpy						rtl_strncpy
	#define strstr						rtl_strstr
	#define strsep						rtl_strsep
	#define strtok						rtl_strtok

	#define atoi(str)               	prvAtoi(str)	
	#define strpbrk(cs, ct)				_strpbrk(cs, ct)		// for B-cut ROM
	#define sscanf						_sscanf

//	#define sscanf		rtl_sscanf
	#define strnlen		rtl_strnlen
//	#define strlen		rtl_strlen



	#define strstr		rtl_strstr
	#define vsnprintf	rtl_vfprintf_r
/*
#define fflush_r 			rtl_fflush_r_v1_00
#define vfprintf_r		rtl_vfprintf_r_v1_00

extern _ROM_CALL_ int rtl_fflush_r_v1_00(struct _reent *ptr, register FILE * fp);
extern _ROM_CALL_ int rtl_vfprintf_r_v1_00(struct _reent *, FILE *, const char *, va_list);
*/
	
#endif // __PLATFORM_ROMLIB_H__
