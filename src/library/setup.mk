CC = avr-gcc

CDEFINES =
CDEFINES += -DF_CPU=$(CLK)
CDEFINES += -mmcu=$(MMCU)
CDEFINES += -DMMCUN=$(MMCU_N)

# Flags de compilacion
CFLAGS =
# Optimizacion '-Os' tamaño '-O3' velocidad. Con -O vacio el compilador elige '-01'
CFLAGS += -O$(OPTIMIZATION)
# Agrega informacion de debug al lst
CFLAGS += -g
# All warnings
CFLAGS += -Wall

CLDFLAGS =

AVROBJFLAGS = 
AVROBJFLAGS += -j .text
AVROBJFLAGS += -j .data

LIBINCLUDE = $(LIBCDR)

# Reglas
########

# Reglas para compilar y generar el binario para subir al target
all: hex

bin: $(SRC) $(INC)
	@echo "Clock = $(CLK) | Lfuse = $(LFUSE) | Hfuse = $(HFUSE)"
	$(CC) $(CDEFINES) $(CFLAGS) $(CLDFLAGS) -o $(TARGET).bin -I$(LIBINCLUDE) $(SRC) 
	avr-objdump -h -S $(TARGET).bin > $(TARGET).lst
	avr-size -d $(TARGET).bin

hex: bin
	avr-objcopy $(AVROBJFLAGS) -O ihex $(TARGET).bin $(TARGET).hex

# Reglas para programar el target
program: hex
	avrdude -c usbtiny -p $(TARGET_P) -U f:w:$(TARGET).hex

program_dw: bin
	avarice -w -j usb --erase --program --file $(TARGET).bin

fuse:
	avrdude -c usbtiny -p $(TARGET_P) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

fuses_read:
	@ avrdude -c usbtiny -p $(TARGET_P) -U hfuse:r:hfuse.hex:h -U lfuse:r:lfuse.hex:h
#	@ avrdude -c usbtiny -p $(TARGET_P) -U efuse:r:efuse.hex:h
	@ echo "HFuse = "
	@ cat hfuse.hex
	@ echo "LFuse = " 
	@ cat lfuse.hex
#	@ echo "EFuse = "
#	@ cat efuse.hex

eeprom_read:
	avrdude -c usbtiny -p $(TARGET_P) -U eeprom:r:eeprom.hex:r

eeprom_write:
	avrdude -c usbtiny -p $(TARGET_P) -U eeprom:w:eeprom.hex:r
clean:
	rm -rf $(TARGET).bin $(TARGET).lst *.o hfuse.hex lfuse.hex $(TARGET).hex

.PHONY: clean eeprom_read eeprom_write fuse fuses_read program program_dw

