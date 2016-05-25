VERSION = 1.01T19
FIRMWARE_VERSION=FW_VERSION_A920_PRODUCTION
MODEL = A920
APP_NAME = PAX_RPC
#DATETIME = 20`date +%y%m%d`
DATETIME = 20$(shell date +%y%m%d)
TARGET		= $(MODEL)_$(APP_NAME)_$(DATETIME)\(v$(VERSION)\)
#TARGET		= PAX_RPC

OBJ_DIR = ./obj

CROSS = arm-elf-
# Compilation tools
CC = $(CROSS)gcc 
SIZE = $(CROSS)size
STRIP = $(CROSS)strip
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
LD = $(CROSS)ld

LDSCRIPT = link/mldscript_s80

CFLAGS = -msoft-float -mlittle-endian -mstructure-size-boundary=8 -O2 -ffunction-sections -nostartfiles -c  
CFLAGS += -Iinc
ifeq ($(FIRMWARE_VERSION), FW_VERSION_A920_PCI)
CFLAGS	+= -DAPP_VERSION="\"$(VERSION)\""
else
CFLAGS	+= -DAPP_VERSION="\"$(VERSION)_$(DATETIME)\""
endif
CFLAGS	+= -DAPP_NAME="\"$(APP_NAME)\""
LDFLAGS  = -T$(LDSCRIPT) -M --cref -nostartfiles 
BIN_FLAGS = -O binary -o

SRC_ASM = $(wildcard *.s) $(wildcard */*.s) $(wildcard */*/*.s)
SRC_C = $(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) 
SRCS = $(SRC_C) $(SRC_ASM)

LIBS = lib/libS80Api.a lib/libgcc.a 

OBJS = $(SRC_ASM:%.s=%.o) $(SRC_C:%.c=%.o)
OBJ_ALL = $(OBJS)

#bin:echo_target depend $(TARGET).elf
bin:$(TARGET).elf
	$(STRIP) $(TARGET).elf $(BIN_FLAGS) $(TARGET).bin
	echo -n "PAX-S80-MAPP" >> $(TARGET).bin

$(TARGET).elf: $(OBJ_ALL) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $+ > $(TARGET).map
	$(OBJDUMP) -d -S $@ > $(TARGET).asm

%.o:%.c
#	@echo CC $<
	$(CC) $(CFLAGS) $< -o $@

%.o:%.s
#	@echo ASM $<
	$(CC) $(CFLAGS) $< -o $@

echo_target:
#@echo "asm objects: $(SRC_ASM)"
#@echo "c objects: $(SRC_C)"
#@echo "all objects: $(OBJ_ALL)"
	@echo "cflags: $(CFLAGS)"

clean:
	-rm -f *.bin
	-rm -f *.elf
	-rm -f *.map
	-rm -f *.asm
	-rm -rf $(OBJ_ALL)

rebuild:
	make clean
	make -j4
	
