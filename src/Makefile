# Makefile general para uso de la bilioteca.
# Copiar este Makefile dentro de cada proyecto y editar las opciones
# correspondientes

# TARGET es el nombre del proyecto y es el nombre que se usa para generar el
# binario
TARGET = main

# PATH de la biblioteca
LIBCDR = ../library/

# MMCU es el microcontralador para el target para el avr-gcc
# Para ver el listado de posibles opciones ir a:
#      http://avr-libc.nongnu.org/user-manual/index.html
# atmega8/88/88a/88p/88pa
MMCU = atmega88a

CLK_EXT = 18432000UL #cristal externo
#CLK_EXT = 12288000UL #cristal externo (PCB original)
#CLK_INT =  8000000UL #oscilador interno cercano a 8 Mhz
CLK_INT =  8243700UL #si se conoce con mayor precision el oscilador interno

DIV8 = 0   # 1 si usamos el prescaler de clock en 8. 0 sin prescaler
EXTCLK = 1 # 1 si usamos el clock externo. 0 si usamos el clock interno

# F_CPU es la frecuencia de trabajo del microcontrolador.
# Es la base a partir de la cual se calculan los delays.

ifeq ($(MMCU), atmega8)
	# 0xD9 es el default  (pagina 223)
	# ojo con el bit 5, siempre debe ser 0 -> XX0XXXXX
	HFUSE = 0xD9

	# (pagina 224)
	# 0xE1 es el default (oscilador interno de 1 Mhz) CLK = CLK_INT/8
	# 0xE2 (oscilador interno de 2 Mhz) CLK = CLK_INT/4
	# 0xE3 (oscilador interno de 4 Mhz) CLK = CLK_INT/2
	# 0xE4 (oscilador interno de 8 Mhz) CLK = CLK_INT
	LFUSE = 0xE4

	CLK = CLK_INT

else
 	ifeq ($(findstring atmega88,$(MMCU)), atmega88)
 		# 0xDF es el default  (pagina 299)
 		# ojo con el bit 5, siempre debe ser 0 -> XX0XXXXX
 		HFUSE = 0xDF
 
 		# 0x62 es el default (con oscilador interno de 1Mhz) (pagina 301)
 		# 0x6F cristal externo con DIV8
 		# 0xE2 oscilador interno sin DIV8 (8 Mhz)
		# 0xEF cristal externo sin DIV8
 		ifeq ($(DIV8),1)
 			ifeq ($(CLKEXT),0)
 				LFUSE = 0x62
 				CLK = $(CLK_INT)/8
 			else
 				LFUSE = 0x6F
 				CLK = $(CLK_EXT)/8
 			endif
 		else
 			ifeq ($(CLKEXT),0)
 				LFUSE = 0xE2
 				CLK = $(CLK_INT)
 			else
 				LFUSE = 0xEF
 				CLK = $(CLK_EXT)
 			endif
 		endif
 	else
		$(error Target no soportado)
 	endif
endif

# TARGET_P es el microcontrolador para el target del programador
# Para ver el listado de posibles opciones hacer:
#      avrdude -p ?
ifeq ($(MMCU), atmega8)
	MMCU_N = 0
	TARGET_P = m8
else
 	ifeq ($(findstring atmega88,$(MMCU)), atmega88)
 		MMCU_N = 1
 		ifeq ($(MMCU), atmega88) 
 			TARGET_P = m88
 		else
 			ifeq ($(MMCU), atmega88a) 
 				TARGET_P = m88
 			else
 				ifeq ($(MMCU), atmega88p) 
 					TARGET_P = m88
 				else
					TARGET_P = m88pa
 				endif
 			endif
 		endif
 	else
		$(error Target no soportado)
 	endif
endif


# Las siguiente dos lineas agregan todos los archivos que se encuentran en el
# directorio local, de extension .c y .h, en las correspondientes variables.
# Utilizarlas si se desea compilar todos los archivos fuentes.
#SRC = $(shell ls *.c)
#INC = $(shell ls *.h)
# Como segunda opcion, se pueden agregar a mano solo los archivos que se desean
# compilar
SRC = $(TARGET).c
INC = 

INC += $(LIBCDR)/defs.h
INC += $(LIBCDR)/board.h

SRC += $(LIBCDR)/usart.c
INC += $(LIBCDR)/usart.h

SRC += $(LIBCDR)/adc.c
INC += $(LIBCDR)/adc.h


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
