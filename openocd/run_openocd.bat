taskkill /F /IM openocd.exe
@rem openocd -f interface\cmsis-dap.cfg -f ..\..\..\component\soc\realtek\8195a\misc\gcc_utility\openocd\ameba1.cfg
openocd -f interface\jlink.cfg -f ameba1.cfg
