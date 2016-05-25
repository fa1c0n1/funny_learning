#
# some usefull variables
#
LIBDRV = ./libdrv
LIBSRC = ./libsrc
LIBDIFFER = ./libdiffer

#### target type : SP30/S80/S90/S60/S80/T80
####select target type here
TARGET_TYPE	=
TARGET_COPY = libped$(TARGET_TYPE).a
TARGET_LIB  = $(TARGET).a

TARGET_COPYDIR =E:/out/
LIBDRV_COPYDIR=E:/out/

MIPS_ARCH_LIST	= SP30 
ARM_ARCH_LIST410 = S60 S80 S90 SSBCM 
ARM_ARCH_LIST302 = T80

INCLUDE=$(LIBDRV) $(LIBSRC)
BUILD     =.
BIN_PATH  =

ARM_BIN_PATH302	=c:\gcc\build\bin
ARM_INC_PATH302 =c:\gcc\build\include
ARM_LIB_PATH302 =

ARM_BIN_PATH410 =
ARM_INC_PATH410 =
ARM_LIB_PATH410 =

MIPS_BIN_PATH	=D:\usip-hde-3.0\sde\bin

###################define CPU arch ,It is depended on terminal type#####
ARCH	=
ifeq ($(TARGET_TYPE),$(findstring $(TARGET_TYPE),$(MIPS_ARCH_LIST)))
	ARCH=MIPS
	BIN_PATH=$(MIPS_BIN_PATH)
	LIB_PATH	=
	INC_PATH	=
endif


	
ifeq ($(TARGET_TYPE),$(findstring $(TARGET_TYPE),$(ARM_ARCH_LIST410)))
	ARCH=ARM
		BIN_PATH=$(ARM_BIN_PATH410)
		LIB_PATH=$(ARM_LIB_PATH410)
		INC_PATH=$(ARM_INC_PATH410)

		ifeq (SSBCM,$(findstring SSBCM,$(TARGET_TYPE)))
		CFLAGS_ARM	= -fsigned-char -mlittle-endian -g -O1 -c -fno-zero-initialized-in-bss 
		else
		
		CFLAGS_ARM	= -fsigned-char -mlittle-endian -g -O1 -c -fno-zero-initialized-in-bss -mthumb-interwork
		endif

endif

ifeq ($(TARGET_TYPE),$(findstring $(TARGET_TYPE),$(ARM_ARCH_LIST302)))
	ARCH=ARM
		BIN_PATH=$(ARM_BIN_PATH302)
		LIB_PATH=$(ARM_LIB_PATH302)
		INC_PATH=$(ARM_INC_PATH302)
		CFLAGS_ARM	= -fsigned-char -mlittle-endian -g -O3 -c -I$(INC_PATH)

endif

#
# Tools and Options
#
################################for mips CPU###############################
AS_MIPS        = sde-gcc -xassembler-with-cpp
ASFLAGS_MIPS   = -mtune=4ksd -mips32r2 -msmartmips -c -D_32_ -D_ASSEMBLER_ -EL
CC_MIPS        = sde-gcc
CFLAGS_MIPS    = -mtune=4ksd -mips32r2 -c -D_32_ -EL 	\
			-mno-float -Wimplicit -Wformat -G0 -fomit-frame-pointer	\
			-ffreestanding -mlong-calls -g -gdwarf-2 -Os -mips16e
AR_MIPS		  = sde-ar
ARFLAGS_MIPS	  = -rcs
############################for arm########################################
AS_ARM	=	arm-elf-gcc
ASFLAGS_ARM =
CC_ARM	=	arm-elf-gcc

AR_ARM	=	arm-elf-ar
ARFLAGS_ARM	  = -r
CFLAGS_ARM += -D$(MACHINE_TYPE)_PED
###########################################################################



ifeq ($(ARCH),MIPS)
	AS =	$(AS_MIPS)
	ASFLAGS =	$(ASFLAGS_MIPS)
	CC	=	$(CC_MIPS)
	CFLAGS	=	$(CFLAGS_MIPS)
	AR	=	$(AR_MIPS)
	ARFLAGS	=	$(ARFLAGS_MIPS)
endif

ifeq ($(ARCH),ARM)
	AS =	$(AS_ARM)
	ASFLAGS =	$(ASFLAGS_ARM)
	CC	=	$(CC_ARM)
	CFLAGS	=	$(CFLAGS_ARM) 
	AR	=	$(AR_ARM)
	ARFLAGS	=	$(ARFLAGS_ARM)
endif


####define for stdarg.h function in MIPS CPU and SP30#############
STDARG_DEFINES = -Dsprintf=paxsprintf
STDARG_DEFINES += -Dvsnprintf=paxvsnprintf
STDARG_DEFINES += -Dvscnprintf=paxvscnprintf
STDARG_DEFINES += -Dsnprintf=paxsnprintf
STDARG_DEFINES += -Dscnprintf=paxscnprintf
STDARG_DEFINES += -Dvsprintf=paxvsprintf
STDARG_DEFINES += -Dvsscanf=paxvsscanf
STDARG_DEFINES += -Dsscanf=paxsscanf

ifeq ($(TARGET_TYPE),$(findstring $(TARGET_TYPE),$(MIPS_ARCH_LIST)))
CFLAGS += $(STDARG_DEFINES)
endif

##############################################################

CFLAGS += -D$(TARGET_TYPE)_PED 

ifeq ($(TRK_SELECT),TRK_FIXED)
	CFLAGS += -DTRK_FIXED_ENABLE
endif

ifeq ($(TRK_SELECT),TRK_DUKPT)
	CFLAGS += -DTRK_DUKPT_ENABLE
endif
	
	CFLAGS +=  -I$(LIBSRC) -I$(LIBDIFFER)
	
ifeq ($(ARCH),ARM)
	CFLAGS +=  -I$(LIBDRV)
endif

#
# Library file name
#
#
# Object file list
#

OBJ = ./obj
OUTPUT = ./libFile
ifeq ($(ARCH),MIPS)
VPATH += $(wildcard $(LIBSRC)) $(wildcard $(LIBDIFFER))
endif

ifeq ($(ARCH),ARM)
VPATH += $(wildcard $(LIBSRC)) $(wildcard $(LIBDIFFER)) $(wildcard $(LIBDRV))
endif

C_OBJECTS  +=  PedBendigoApi.o PedWestpac.o  PedWestpacCrc.o PedWestpacApi.o
C_OBJECTS  +=  dbdprint.o Ped.o PedApi.o PedBuff.o PedCrc16.o PedCrc32.o PedDUKPT.o
C_OBJECTS  +=  PedFile02.o PedInput.o PedMksk.o PedRsaKey.o PedTimer.o

#add for debug object
ifeq (debug,$(findstring debug,$(TARGET)))
	C_OBJECTS  +=  pedtest.o
endif

#add for SSBCM_PLUS project
ifeq (SSBCM,$(TARGET_TYPE)) 
	C_OBJECTS  +=  PedDrv.o PedDownload.o 
endif

#
# C compilation
#

C_OBJECTS_DDR = $(addprefix $(OBJ)/, $(C_OBJECTS))

maketarget: $(C_OBJECTS_DDR)
	echo $(AR) $(ARFLAGS) $(OUTPUT)/$(TARGET_LIB) $^
	$(AR) $(ARFLAGS) $(OUTPUT)/$(TARGET_LIB) $^
#rm -f $(OBJ)/*.*

$(C_OBJECTS_DDR): $(OBJ)/%.o: %.c 
#echo $(CC) $(CFLAGS) -o $@ $<
	echo CC $<
	$(CC) $(CFLAGS) -o $@ $<

	
clean:
	-rm -f $(OBJ)/*.* $(OUTPUT)/*.*

all:
	make -k maketarget TARGET_TYPE=SSBCM TRK_SELECT=TRK_FIXED	
	make -k maketarget TARGET_TYPE=S80 TRK_SELECT=TRK_FIXED
	make -k maketarget TARGET_TYPE=S90 TRK_SELECT=TRK_FIXED
	make -k maketarget TARGET_TYPE=S60 TRK_SELECT=TRK_FIXED
	make -k maketarget TARGET_TYPE=SP30 TRK_SELECT=TRK_FIXED

