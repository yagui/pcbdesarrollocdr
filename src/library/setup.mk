CC = avr-gcc
AR = avr-ar
OBJDUMP = avr-objdump
OBJCOPY = avr-objcopy
SIZE = avr-size
RM = rm -rf

include $(LIBCDR)/clkdef.mk
include $(LIBCDR)/target.mk

CDEFINES =
CDEFINES += -DF_CPU=$(CLK)
CDEFINES += -mmcu=$(MMCU)
#CDEFINES += -DMMCUN=$(MMCU_N)

OPTIMIZATION ?= 2

# Flags de compilacion
CFLAGS =
# Optimizacion '-Os' tamaño '-O3' velocidad. Con -O vacio el compilador elige '-01'
CFLAGS += -O$(OPTIMIZATION)
# Agrega informacion de debug al lst
CFLAGS += -g2
# All warnings
CFLAGS += -Wall
# Otros flags
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -std=gnu99
# genera los archivos de dependencias
CFLAGS += -MMD -MP -MT $(*F).o -MF dep/$(@F).d 

CCINCLUDES = -I$(LIBCDR)

LIBS := -lcdr $(LIBS)
LDFLAGS = -Wl,-Map=$(TARGET).map,--gc-sections -L$(LIBCDR) -mmcu=$(MMCU)

COMMON_OBJECTS = usart.o adc.o
COMMON_OBJECTS := $(addprefix $(LIBCDR)/, $(COMMON_OBJECTS))

AVROBJFLAGS := -j .data -j .text
BINFLASHFLAGS := $(AVROBJFLAGS) -O binary
HEXFLASHFLAGS := $(AVROBJFLAGS) -O ihex
#HEXFLASHFLAGS += -R .eeprom -R .fuse -R .lock -R .signature

# Reglas
########

# Reglas para compilar y generar el binario para subir al target
all: msg_start hex

msg_start:
	@echo "********Iniciando compilacion *********"

elf: dep $(OBJECTS) libcdr.a
	@echo "Clock = $(CLK) | Lfuse = $(LFUSE) | Hfuse = $(HFUSE)"
	$(CC) $(LDFLAGS) -o $(TARGET).elf $(OBJECTS) $(LIBS)
	@$(OBJDUMP) -h -S $(TARGET).elf > $(TARGET).lst
	@$(SIZE) -d --format=avr --mcu=$(MMCU) $(TARGET).elf
	@$(SIZE) -d $(TARGET).elf

bin: elf
	@$(OBJCOPY) $(BINFLASHFLAGS) $(TARGET).elf $(TARGET).bin

hex: elf
	@$(OBJCOPY) $(HEXFLASHFLAGS) $(TARGET).elf $(TARGET).hex

libcdr.a: $(COMMON_OBJECTS)
	@$(AR) -rcs $(LIBCDR)/libcdr.a $(COMMON_OBJECTS)
	@$(OBJDUMP) -h -S $(LIBCDR)/libcdr.a >$(LIBCDR)/libcdr.lst

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  assemble .s files to .o
#  .c or .cpp files to .s

.c.o:
	$(CC) $(CFLAGS) $(CDEFINES) $(CCINCLUDES) -c $< -o $(<:.c=.o)

# Reglas para programar el target
program: hex
	avrdude -c usbtiny -p $(TARGET_P) -U f:w:$(TARGET).hex

program_dw: bin
	avarice -w -j usb --erase --program --file $(TARGET).bin

fuse:
	avrdude -c usbtiny -p $(TARGET_P) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

fuses_read:
	@ avrdude -c usbtiny -p $(TARGET_P) -U hfuse:r:hfuse.hex:h -U lfuse:r:lfuse.hex:h
	@ avrdude -c usbtiny -p $(TARGET_P) -U efuse:r:efuse.hex:h
	@ echo "HFuse = "
	@ cat hfuse.hex
	@ echo "LFuse = " 
	@ cat lfuse.hex
	@ echo "EFuse = "
	@ cat efuse.hex

eeprom_read:
	avrdude -c usbtiny -p $(TARGET_P) -U eeprom:r:eeprom.hex:r

eeprom_write:
	avrdude -c usbtiny -p $(TARGET_P) -U eeprom:w:eeprom.hex:r

clean:
	@$(RM) $(TARGET).elf $(TARGET).hex $(TARGET).lst $(TARGET).map
	@$(RM) $(TARGET).bin
	@$(RM) *.o dep
	@$(RM) hfuse.hex lfuse.hex efuse.hex
	@$(RM) $(OBJECTS)
	@$(RM) $(COMMON_OBJECTS)
	@$(RM) $(LIBCDR)/libcdr.a
	@$(RM) $(LIBCDR)/libcdr.lst

.PHONY: clean eeprom_read eeprom_write fuse fuses_read program program_dw

#-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)
-include $(shell mkdir dep) $(wildcard dep/*)
