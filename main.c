/*
 *  Test "Hello World"
 */

#include <stdint.h>
#include <stdio.h>
#include "cortex.h"
#include "rtl8710.h"
#include "rom_lib.h"
#include "hal_pinmux.h"

int main(void)
{
    int i = 6;
    HalPinCtrlRtl8195A(JTAG, 0, 1);
    HalInitPlatformLogUartV02();
	HalInitPlatformTimerV02();
	HalShowBuildInfoV02();
	do {
	    HalDelayUs(1000000);
	    HalCpuClkConfig(--i); // 0 - 166666666 Hz, 1 - 83333333 Hz, 2 - 41666666 Hz, 3 - 20833333 Hz, 4 - 10416666 Hz, 5 - 4000000 Hz
	    HalReInitPlatformLogUartV02();
		printf("Hello World : %d\r\n", i);
	    printf("CPU CLK : %d\r\n", HalGetCpuClk());
	} while(i);
	printf("End");
	while(1);
}

extern uint8_t STACK_TOP;
uint32_t *cortex_vectors[] __attribute__((section(".vectors"))) = {
	(uint32_t *)&STACK_TOP,
	(uint32_t *)main
};


