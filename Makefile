PROJECT = UART_FreeRTOS

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary -S
MCU  = cortex-m4

STMU = ST-LINK_CLI
STMUFLAGS = -c SWD -Halt -ME -P $(PROJECT).bin 0x08000000 -V -Rst

OPENOCD      = openocd
OPENOCDFLAGS = -f ./openocd/stm32f3discovery.cfg

GDB = $(TRGT)gdb

LDSCRIPT = ./linker/stm32f3-linker.ld

DDEFS = -DSTM32F30X -DUSE_STDPERIPH_DRIVER

# List C source files here
CMSISDIR   	= ./inc/device/CMSIS
DEVICESRCDIR   	= ./src/device
DEVICEINCDIR	= ./inc/device
PERIPHSRCDIR	= ./src/device/periph
PERIPHINCDIR 	= ./inc/device/periph
DISCOVERYINCDIR	= ./inc/device/periph/discovery
DISCOVERYSRCDIR	= $(PERIPHSRCDIR)/discovery

# FreeRTOS
FREERTOSDIR		= ../../FreeRTOS/Source
FREERTOSINCDIR  = $(FREERTOSDIR)/include

FREERTOSMCU		= ARM_CM3
FREERTOSHEAP	= 1

DRIVERSRCDIR	= ./src/drivers

# sources
SRC = $(wildcard src/*.c)
SRC += $(wildcard $(FREERTOSDIR)/*.c)
SRC += $(FREERTOSDIR)/portable/GCC/$(FREERTOSMCU)/port.c
SRC += $(FREERTOSDIR)/portable/MemMang/heap_$(FREERTOSHEAP).c
SRC += $(wildcard $(DRIVERSRCDIR)/*.c)
SRC += $(DEVICESRCDIR)/stm32f30x_it.c
SRC += $(DEVICESRCDIR)/system_stm32f30x.c
SRC += $(DISCOVERYSRCDIR)/stm32f3_discovery.c
## used parts of the STM-Library
#SRC += $(PERIPHSRCDIR)/stm32f30x_adc.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_can.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_comp.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_crc.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_dac.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_dbgmcu.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_dma.c
SRC += $(PERIPHSRCDIR)/stm32f30x_exti.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_flash.c
SRC += $(PERIPHSRCDIR)/stm32f30x_gpio.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_i2c.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_iwdg.c
SRC += $(PERIPHSRCDIR)/stm32f30x_misc.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_opamp.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_pwr.c
SRC += $(PERIPHSRCDIR)/stm32f30x_rcc.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_rtc.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_spi.c
SRC += $(PERIPHSRCDIR)/stm32f30x_syscfg.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_tim.c
SRC += $(PERIPHSRCDIR)/stm32f30x_usart.c
#SRC += $(PERIPHSRCDIR)/stm32f30x_wwdg.c
## USB
#SRC += $(PERIPHSRCDIR)/usb_core.c
#SRC += $(PERIPHSRCDIR)/usb_init.c
#SRC += $(PERIPHSRCDIR)/usb_int.c
#SRC += $(PERIPHSRCDIR)/usb_mem.c
#SRC += $(PERIPHSRCDIR)/usb_regs.c
#SRC += $(PERIPHSRCDIR)/usb_sil.c
# List ASM source files here
ASRC = ./startup/startup_stm32f30x.s

# List all user directories here
UINCDIR = $(CMSISDIR) \
          $(PERIPHINCDIR) \
          $(DISCOVERYINCDIR) \
	  $(DEVICEINCDIR) \
	  $(FREERTOSINCDIR) \
	  $(FREERTOSDIR)/portable/GCC/$(FREERTOSMCU) \
	  $(DRIVERINCDIR) \
          ./inc

# Define optimisation level here
OPT = -Os

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

DEFS    = $(DDEFS) $(UDEFS) -DRUN_FROM_FLASH=1

ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU)

ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -Wstrict-prototypes -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
LDFLAGS = $(MCFLAGS) -mthumb -nostartfiles -specs=nano.specs -specs=nosys.specs -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)

# Generate dependency information
CPFLAGS += -MD -MP -MF dep/$(@F).d

#
# makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin
	$(TRGT)size $(PROJECT).elf

%o: %c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

%o: %s
	$(AS) -c $(ASFLAGS) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN) $< $@

upload: $(PROJECT).bin
	$(STMU) $(STMUFLAGS)

debug: $(PROJECT).elf
	$(OPENOCD) $(OPENOCDFLAGS) &
	$(GDB) $(PROJECT).elf

clean:
	rm -f $(OBJS)
	rm $(PROJECT).elf
	rm $(PROJECT).map
	rm $(PROJECT).bin
	rm $(PROJECT).hex
	rm $(ASRC:.s=.lst)
	rm $(SRC:.c=.lst)
	rm ./dep/*
	rm $(SRC:.c=.ini)

# *** EOF ***
