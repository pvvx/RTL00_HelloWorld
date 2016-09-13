/*
 *  Test: Speed rd Flash
 */

#include <stdint.h>
#include <stdio.h>
#include "cortex.h"
#include "rtl8710.h"
#include "rom_lib.h"
#include "hal_pinmux.h"
#include "hal_platform.h"
#include "hal_spi_flash.h"
#include "hal_api.h"
#include "rtl8195a\rtl8195a_peri_on.h"
#include "hal_peri_on.h"
//#include "mask.h"
//#include "cpu/cortex/core_cm3.h"
#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address            */
#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)   /*!< Core Debug configuration struct    */
#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address                   */
#define DWT                 ((DWT_Type       *)     DWT_BASE      )   /*!< DWT configuration struct           */
#define CoreDebug_DEMCR_TRCENA_Pos         24                                             /*!< CoreDebug DEMCR: TRCENA Position */
#define CoreDebug_DEMCR_TRCENA_Msk         (1UL << CoreDebug_DEMCR_TRCENA_Pos)            /*!< CoreDebug DEMCR: TRCENA Mask */

#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */

typedef struct
{
  __IO uint32_t DHCSR;                   /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register    */
  __O  uint32_t DCRSR;                   /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register        */
  __IO uint32_t DCRDR;                   /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register            */
  __IO uint32_t DEMCR;                   /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
} CoreDebug_Type;
/** \brief  Structure type to access the Data Watchpoint and Trace Register (DWT).
 */
typedef struct
{
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  Control Register                          */
  __IO uint32_t CYCCNT;                  /*!< Offset: 0x004 (R/W)  Cycle Count Register                      */
  __IO uint32_t CPICNT;                  /*!< Offset: 0x008 (R/W)  CPI Count Register                        */
  __IO uint32_t EXCCNT;                  /*!< Offset: 0x00C (R/W)  Exception Overhead Count Register         */
  __IO uint32_t SLEEPCNT;                /*!< Offset: 0x010 (R/W)  Sleep Count Register                      */
  __IO uint32_t LSUCNT;                  /*!< Offset: 0x014 (R/W)  LSU Count Register                        */
  __IO uint32_t FOLDCNT;                 /*!< Offset: 0x018 (R/W)  Folded-instruction Count Register         */
  __I  uint32_t PCSR;                    /*!< Offset: 0x01C (R/ )  Program Counter Sample Register           */
  __IO uint32_t COMP0;                   /*!< Offset: 0x020 (R/W)  Comparator Register 0                     */
  __IO uint32_t MASK0;                   /*!< Offset: 0x024 (R/W)  Mask Register 0                           */
  __IO uint32_t FUNCTION0;               /*!< Offset: 0x028 (R/W)  Function Register 0                       */
       uint32_t RESERVED0[1];
  __IO uint32_t COMP1;                   /*!< Offset: 0x030 (R/W)  Comparator Register 1                     */
  __IO uint32_t MASK1;                   /*!< Offset: 0x034 (R/W)  Mask Register 1                           */
  __IO uint32_t FUNCTION1;               /*!< Offset: 0x038 (R/W)  Function Register 1                       */
       uint32_t RESERVED1[1];
  __IO uint32_t COMP2;                   /*!< Offset: 0x040 (R/W)  Comparator Register 2                     */
  __IO uint32_t MASK2;                   /*!< Offset: 0x044 (R/W)  Mask Register 2                           */
  __IO uint32_t FUNCTION2;               /*!< Offset: 0x048 (R/W)  Function Register 2                       */
       uint32_t RESERVED2[1];
  __IO uint32_t COMP3;                   /*!< Offset: 0x050 (R/W)  Comparator Register 3                     */
  __IO uint32_t MASK3;                   /*!< Offset: 0x054 (R/W)  Mask Register 3                           */
  __IO uint32_t FUNCTION3;               /*!< Offset: 0x058 (R/W)  Function Register 3                       */
} DWT_Type;

#define DWT_CTRL_CYCCNTENA_Pos              0                                          /*!< DWT CTRL: CYCCNTENA Position */
#define DWT_CTRL_CYCCNTENA_Msk             (0x1UL << DWT_CTRL_CYCCNTENA_Pos)           /*!< DWT CTRL: CYCCNTENA Mask */


int main(void)
{
	u32 t[10];
    int i = 333333, x = 3;
    HalPinCtrlRtl8195A(JTAG, 0, 1);
    while(i--) asm("nop");
    PERI_ON->PESOC_CLK_CTRL |= PERI_ON_CLK_CTRL_ACTCK_TIMER_EN | PERI_ON_CLK_CTRL_SLPCK_TIMER_EN | PERI_ON_CLK_CTRL_ACTCK_LOG_UART_EN | PERI_ON_CLK_CTRL_ACTCK_FLASH_EN | PERI_ON_CLK_CTRL_SLPCK_FLASH_EN;
    PERI_ON->GPIO_PULL_CTRL2 |= (PERI_ON->GPIO_PULL_CTRL2 & (~3)) | 2;
    PERI_ON->OSC32K_CTRL |= 1;
    PERI_ON->SOC_FUNC_EN |= PERI_ON_SOC_FUNC_EN_LOG_UART | PERI_ON_SOC_FUNC_EN_FLASH;
    HalPinCtrlRtl8195A(SPI_FLASH, 0, 1); //    SPI_FLASH_PIN_FCTRL(ON); // enable spi flash pins
    HalCpuClkConfig(0); // 0 - 166666666 Hz, 1 - 83333333 Hz, 2 - 41666666 Hz, 3 - 20833333 Hz, 4 - 10416666 Hz, 5 - 4000000 Hz
    HalInitPlatformLogUartV02();
	HalInitPlatformTimerV02();
	HalShowBuildInfoV02();
//	SpicLoadInitParaFromClockRtl8195AV02();
	SpicInitRtl8195AV02(2, SpicQuadBitMode);// SpicDualBitMode);
    SpicWaitBusyDoneRtl8195A();
	printf("Flash[0]: 0x%08X\r\n", *(volatile u32 *)SPI_FLASH_BASE );
//    HalDelayUs(1000000);
	printf("CPU CLK : %d Hz\r\n", HalGetCpuClk());
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // открыть доступ
	if(!(DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk)) { // уже включен?
		DWT->CYCCNT = 0; // обнулить и запустить
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // запустить счет
	}
	while(x--) {
	t[0] = DWT->CYCCNT;
	printf("t0 = %d\r\n", DWT->CYCCNT - t[0]);
	t[0] = DWT->CYCCNT;
	volatile u32 * ptr = (volatile u32 *)SPI_FLASH_BASE+0x4000;
	for(i=0; i < 16384; i++) *ptr++;
	t[1] = DWT->CYCCNT - t[0];
	printf("read(): tFlash = %d, clk/byte = %d\r\n", t[1], t[1] >> 16);
	ptr = (volatile u32 *)SPI_FLASH_BASE+0x10000;
	t[0] = DWT->CYCCNT;
	memcpy((u8 *)0x10010000,(u8 *) SPI_FLASH_BASE+0x10000, 65536);
	for(i=0; i < 16384; i++) *ptr++;
	t[2] = DWT->CYCCNT - t[0];
	printf("memcpy(): tFlash = %d, clk/byte = %d\r\n", t[2], t[2] >> 16);
	ptr = (volatile u32 *)0x10000000;
	t[0] = DWT->CYCCNT;
	for(i=0; i < 16384; i++) *ptr++;
	t[3] = DWT->CYCCNT - t[0];
	printf("read(): tRAM = %d, clk/byte = %d\r\n", t[3], t[3]>>16);
	ptr = (volatile u32 *)0x1FFF0000;
	t[0] = DWT->CYCCNT;
	for(i=0; i < 16384; i++) *ptr++;
	t[4] = DWT->CYCCNT - t[0];
	printf("read(): tTCM = %d, clk/byte = %d\r\n", t[4], t[4]>>16);
	printf("read(): tFlash/tTCM = %d, tFlash/tRAM = %d\r\n", t[1]/t[4], t[1]/t[3]);
	printf("memcpy(): tFlash/tTCM = %d, tFlash/tRAM = %d\r\n", t[2]/t[4], t[2]/t[3]);
	}
	printf("Flash[0]: 0x%08X\r\n", *(volatile u32 *)SPI_FLASH_BASE );
	printf("End");
	while(1);
}

extern uint8_t STACK_TOP;
uint32_t *cortex_vectors[] __attribute__((section(".vectors"))) = {
	(uint32_t *)&STACK_TOP,
	(uint32_t *)main
};


