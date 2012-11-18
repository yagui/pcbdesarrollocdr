# Makefile general para uso de la bilioteca.
# Copiar este Makefile dentro de cada proyecto y editar las opciones
# correspondientes

# TARGET es el nombre del proyecto y es el nombre que se usa para generar el
# binario
TARGET = lcd16x2

# PATH de la biblioteca
LIBCDR = ../library/

# MMCU es el microcontralador para el target para el avr-gcc
# Para ver el listado de posibles opciones ir a:
#      http://avr-libc.nongnu.org/user-manual/index.html
# atmega8/88/88a/88p/88pa
MMCU = atmega88a

include $(LIBCDR)/clkdef.mk
include $(LIBCDR)/target.mk

# Las siguiente dos lineas agregan todos los archivos que se encuentran en el
# directorio local, de extension .c y .h, en las correspondientes variables.
# Utilizarlas si se desea compilar todos los archivos fuentes.
#SRC = $(shell ls *.c)
#INC = $(shell ls *.h)
# Como segunda opcion, se pueden agregar a mano solo los archivos que se desean
# compilar
SRC = main.c lcd16x2.c
INC = lcd16x2.h

INC += $(LIBCDR)/defs.h
INC += $(LIBCDR)/board.h

CC = avr-gcc

CDEFINES =
CDEFINES += -DF_CPU=$(CLK)
CDEFINES += -mmcu=$(MMCU)
CDEFINES += -DMMCU=$(MMCU_N)

# Flags de compilacion
CFLAGS =
CFLAGS += -O2 # Optimizacion '-Os' tamaño '-O3' velocidad. Con -O vacio el compilador elige '-01'
CFLAGS += -g # Agrega informacion de debug ???
CFLAGS += -Wall # All warnings

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
	$(CC) $(CDEFINES) $(CFLAGS) $(CLDFLAGS) -o $(TARGET).bin -I$(LIBINCLUDE) $(SRC) 
	avr-objdump -h -S $(TARGET).bin > "$(TARGET).lst"
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
