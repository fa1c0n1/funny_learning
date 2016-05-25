# Output file basename
OUTPUT = $(TARGET)_raw


BIN = bin
OBJ = obj
SRC =../code/src
LIB_PATH =../code/lib
CROSS = arm-elf-

# Compilation tools
CC = $(CROSS)gcc 
SIZE = $(CROSS)size
STRIP = $(CROSS)strip
OBJCOPY = $(CROSS)objcopy

CFLAGS += -fsigned-char -mlittle-endian -mstructure-size-boundary=8 -g -O2 -c -I ../code/inc -I../code/src/ipstack/include 

LIBS   += C:/Program\ Files/GNUARM/arm-elf/lib/libc.a C:/Program\ Files/GNUARM/lib/gcc/arm-elf/4.1.0/libgcc.a 
#LIBS   += C:/Program\ Files\ \(x86\)/GNUARM/arm-elf/lib/libc.a C:/Program\ Files\ \(x86\)/GNUARM/lib/gcc/arm-elf/4.1.0/libgcc.a 

LIBS   += $(LIB_PATH)/libpedSSBCM.a
#LIBS   += $(LIB_PATH)/libsslPTS_Release.a
LIBS   += $(LIB_PATH)/libRemoteDownloadSxx.a
#LIBS   += $(LIB_PATH)/libmcARM.a
LIBS   += $(LIB_PATH)/libIcon.a
LIBS   += $(LIB_PATH)/libsm_SF.a

LFLAGS  = -Wl,-TSxxMonitor.ld,-Map,bin/$(TARGET).map,--cref -nostartfiles 

#project file directory
VPATH += $(wildcard $(SRC)/*) $(wildcard $(SRC)/*/*) $(wildcard $(SRC)/*/*/*)
 
ASM_OBJECTS = init.o 

C_OBJECTS = testcase.o log.o
C_OBJECTS  +=  WlApi.o base.o system.o swi.o comm.o timer.o sofTimer.o io_manager.o  
C_OBJECTS  += main.o  kb.o adc.o dll.o SysPara.o sys_combin_api.o
C_OBJECTS  +=  bcm5892-bbl.o bcm5892-spi.o  rtc.o

C_OBJECTS  +=  filedef.o filecore.o filepublic.o
C_OBJECTS  +=  menu.o  modulecheck.o ModuleMenu.o #ssloper.o
C_OBJECTS  +=  nand.o  umc_cpuapi.o sha1.o badblock.o
C_OBJECTS  +=  Asc.o font.o LcdApi.o
C_OBJECTS  +=  des.o digit.o r_stdlib.o r_random.o nn.o md5c.o  CRC.o rsa.o HASH.o sha256.o #Md2c.o 
C_OBJECTS  +=  puk.o PedDrv.o PedDownload.o
C_OBJECTS  +=  magcard.o enmagrd.o
C_OBJECTS  +=  AppManage.o download.o  decompress.o duplicate.o uartdownload.o UsbHostDL.o
C_OBJECTS  +=  modem.o 
C_OBJECTS  +=  printer.o printer_stylus.o printerApi_stylus.o printer_stylus_bcm5892.o
C_OBJECTS  +=  bcm5892_eth.o
C_OBJECTS  +=  usb_ftdi.o usbbulk.o usbdev.o usbfat.o usbfcach.o usbhost.o 
C_OBJECTS  +=  CfgLowlevel.o CfgManage.o

C_OBJECTS  +=  icc_hard_async.o icc_hard_sync.o icc_queue.o ncn8025.o
C_OBJECTS  +=  icc_apis.o icc_core.o
C_OBJECTS	 +=  icc_device_configure.o tda8026.o

C_OBJECTS  +=  bcm5892_rfbsp.o rc663_driver.o rc663_regs_conf.o pn512_driver.o pn512_regs_conf.o as3911_driver.o as3911_regs_conf.o mifare_encrypt.o
C_OBJECTS  +=  iso14443hw_hal.o iso14443_3a.o iso14443_3b.o iso14443_4.o mifare.o emvcl.o paxcl.o paypass.o pcd_apis.o
C_OBJECTS  +=  dmu_cpuapi.o PowerManage.o

#tcpip object
C_OBJECTS  +=  bcm5892_phy.o
C_OBJECTS  +=  gprs_ppp.o  modem_ppp.o ppp.o pppoe.o  mschap.o ppp_md5.o
C_OBJECTS  +=  cbc_enc.o des_enc.o ecb_enc.o md4.o sha1dgst.o set_key.o 
C_OBJECTS  +=  dev.o inet.o ip_addr.o ip_ver.o mem_pool.o netapi.o skbuff.o  socket.o
C_OBJECTS  +=  inet_timer.o
C_OBJECTS  +=  ethernet.o ip.o arp.o icmp.o tcp.o tcp_in.o tcp_out.o udp.o dns.o dhcpc.o

#for S60H
C_OBJECTS  +=  common_s60h.o IrdaHand.o 
C_OBJECTS  +=  comm_proxy.o lan_api.o modem_proxy.o printer_proxy.o printer_test.o

C_OBJECTS += ts_driver.o ts_sample.o coordinate_convert_key.o tp_switch_interface.o atmel_mxt1066t2_ts.o gt970_ts.o crqgt4-i2c.o tp_i2c.o sitronix.o
C_OBJECTS += barcode.o 
C_OBJECTS += i2c_core.o

C_OBJECTS += jamarray.o jamcomp.o jamcrc.o jamexec.o jamexp.o jamheap.o jamjtag.o jamnote.o jamstack.o jamstub.o jamsym.o jamutil.o

C_OBJECTS += sm_chip_thk88.o
C_OBJECTS += cycle_buffer.o
C_OBJECTS += pl080-dma.o
C_OBJECTS += sp_event.o
C_OBJECTS += s25fl032.o
C_OBJECTS += pax_debug.o
C_OBJECTS += rfifo.o
C_OBJECTS += paxsec.o
ifeq (Release,$(findstring Release,$(TARGET)))
C_OBJECTS += rtm.o
else
C_OBJECTS += dtd.o
endif


# Append OBJ and BIN directories to output filename
OUTPUT := $(BIN)/$(OUTPUT)

C_OBJECTS_DDR = $(addprefix $(OBJ)/, $(C_OBJECTS))
ASM_OBJECTS_DDR = $(addprefix $(OBJ)/, $(ASM_OBJECTS))

all: $(ASM_OBJECTS_DDR) $(C_OBJECTS_DDR)
#	echo $(CC) $(LFLAGS)  -o $(OUTPUT).elf $^ $(LIBS)
	echo start link....
	$(CC) $(LFLAGS)  -o $(OUTPUT).elf $^ $(LIBS)
	$(STRIP) --strip-debug  --remove-section=.comment $^
	$(STRIP) --strip-debug  --remove-section=.comment $(OUTPUT).elf
	$(OBJCOPY) -O binary $(OUTPUT).elf $(OUTPUT).bin


$(C_OBJECTS_DDR): $(OBJ)/%.o: %.c
	echo CC $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(ASM_OBJECTS_DDR): $(OBJ)/%.o: %.S
	echo CC $<
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f $(OBJ)/*.* $(BIN)/*.*
 
