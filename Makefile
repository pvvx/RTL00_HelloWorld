LIBDIR = lib
FILENAME_PREFIX = test
ADDRESS = 0x10001000

#FLASHER = stlink-v2-1
#FLASHER = stlink-v2
FLASHER = Jlink
# stlink-v2-1
ifeq ($(FLASHER), Jlink)
# Jlink FLASHER_SPEED ..4000 kHz
FLASHER_SPEED = 3500
else 
ifeq ($(FLASHER),stlink-v2)
# stlink-v2 FLASHER_SPEED ..1800 kHz
FLASHER_SPEED = 1800
else
# ? FLASHER_SPEED ..500 kHz ?
FLASHER_SPEED = 500
endif
endif

CC_PARAMS = -Wall -g -Os -mlittle-endian -mlong-calls -mthumb -mcpu=cortex-m3 -mfloat-abi=soft -mthumb-interwork -ffunction-sections -ffreestanding -fsingle-precision-constant -fshort-wchar -fno-short-enums -Ddouble=float -Wstrict-aliasing=0 -Wl,-T,$(LIBDIR)/cpu/rtl8710/rtl8710.ld -L$(LIBDIR)/cpu/rtl8710 -nostartfiles -nostdlib -u cortex_vectors -Wl,--gc-sections

BASE_PARAMS = -DCORTEX_INTERRUPT_MAX=32 -I$(LIBDIR) -I$(LIBDIR)/cpu/rtl8710 -I$(LIBDIR)/cpu/cortex -I$(LIBDIR)/fwlib  -Wl,--section-start=.text=$(ADDRESS) 

#$(LIBDIR)/cpu/cortex/cortex.c

LIBC_PARAMS = -I$(LIBDIR)/libc $(LIBDIR)/libc/libc.c -DLIBC_PRINTF


FIRMWARE_PARAMS = main.c

CC = arm-none-eabi-gcc

all: firmware

firmware:
	$(CC) $(CC_PARAMS) $(BASE_PARAMS) $(LIBC_PARAMS) $(FIRMWARE_PARAMS) -lgcc -o $(FILENAME_PREFIX).elf
	arm-none-eabi-objdump -S $(FILENAME_PREFIX).elf >$(FILENAME_PREFIX).asm
	arm-none-eabi-strip $(FILENAME_PREFIX).elf
	arm-none-eabi-objcopy -O binary $(FILENAME_PREFIX).elf $(FILENAME_PREFIX).bin
	chmod 755 $(FILENAME_PREFIX).bin
	$(CC) $(CC_PARAMS) -DSeg1StartAddr=$(ADDRESS) -DBinFileName="\"$(FILENAME_PREFIX).bin\"" rtl8710_flash_boot.S -o flash.elf
	arm-none-eabi-objcopy -O binary flash.elf flash.bin
	chmod 755 flash.bin

size:
	arm-none-eabi-size -A -x $(FILENAME_PREFIX).elf

clean:
	rm -rf $(FILENAME_PREFIX).bin $(FILENAME_PREFIX).elf $(FILENAME_PREFIX).asm flash.bin flash.elf

test:
	openocd -f interface/$(FLASHER).cfg -c "adapter_khz $(FLASHER_SPEED)" -f $(LIBDIR)/cpu/rtl8710/rtl8710.ocd -f $(LIBDIR)/cpu/cortex/cortex.ocd -c "init" -c "reset halt" -c "load_image $(FILENAME_PREFIX).bin $(ADDRESS) bin" -c "cortex_bootstrap $(ADDRESS)" -c "shutdown"

flash:
	openocd -f interface/$(FLASHER).cfg -c "adapter_khz $(FLASHER_SPEED)" -f $(LIBDIR)/cpu/rtl8710/rtl8710.ocd -c "init" -c "reset halt" -c "rtl8710_flash_auto_erase 1" -c "rtl8710_flash_auto_verify 1" -c "rtl8710_flash_write flash.bin 0" -c "rtl8710_reboot" -c "reset run" -c shutdown

reset:
	openocd -f interface/$(FLASHER).cfg -c "adapter_khz $(FLASHER_SPEED)" -f $(LIBDIR)/cpu/rtl8710/rtl8710.ocd -c "init" -c "reset halt" -c "rtl8710_reboot" -c shutdown

	