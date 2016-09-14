cmd /c start openocd -f interface\jlink.cfg -f ameba1.cfg
cmd /c start arm-none-eabi-gdb -x rtl_gdb_debug.txt

