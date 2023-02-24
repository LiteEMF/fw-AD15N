// *INDENT-OFF*
cd /d %~dp0

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe

if exist %OBJDUMP% (
set NAME=sdk
)

if exist %NAME%.elf (

%OBJDUMP% -section-headers %NAME%.elf
%OBJDUMP% -d -print-imm-hex -print-dbg %NAME%.elf > %NAME%.lst

%OBJCOPY% -O binary -j .app_code %NAME%.elf  %NAME%.bin
%OBJCOPY% -O binary -j .data %NAME%.elf  data.bin
%OBJCOPY% -O binary -j .debug_data %NAME%.elf  debug_data.bin
%OBJDUMP% -section-headers  %NAME%.elf
%OBJDUMP% -t %NAME%.elf > %NAME%.symbol.txt

copy /b %NAME%.bin+data.bin+debug_data.bin app.bin
copy app.bin mbox_mg/app.bin

)


cd mbox_mg
isd_download.exe -tonorflash -dev sh54 -boot 0xb00 -div8 -wait 300 -uboot uboot.boot  -uboot_compress -app app.bin 0x35f00

@REM
@rem -format vm
@rem -format all
@rem -reboot 500


@REM //烧写外置flash 命令说明：
@rem -wflash dir_song 0 [PA05_1B_NULL]
@rem //   dir_song : 要烧写的文件名（文件需在download.bat文件夹下）
@rem //   0        : 文件烧录到外置flash的起始地址
@rem //   [PA05_1B_NULL]: PA05：外置flash片选引脚（注意：不能选USBDP/USBDM）
@rem //                  1B  ：spi1 ,B端口
@rem //                  NULL: power_io & spi1_data_width，power_io连接到外置flash vcc引脚 可控制flash电源;spi1_data_width:0:单线；1：双向
@rem //                        例:NULL/PA00：power_io:无/pa0;  spi1: 双向模式  (注：power_io不能选USBDP/USBDM)
@rem //                           NUL0/B010：power_io:无/pb1;  spi1: 单线模式
@rem //                           NUL1/A081：power_io:无/pa8;  spi1: 双向模式
@rem //注意：spi端口只能选B/C（B:PA11,PA12,PA10[CLK,DO,DI]; C:PA4,PA5,PA6[CLK,DO,DI]）


@REM //生成固件升级文件
fw_add.exe -noenc -fw jl_isd.fw -add ota.bin -type 100 -out jl_isd.fw
ufw_maker.exe -fw_to_ufw jl_isd.fw
copy jl_isd.ufw update.ufw
del jl_isd.ufw


ping /n 2 127.1>null
IF EXIST null del null


