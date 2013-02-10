# Makefile general para uso de la bilioteca.
# Copiar este Makefile dentro de cada proyecto y editar las opciones
# correspondientes

# TARGET es el nombre del proyecto y es el nombre que se usa para generar el
# binario
TARGET = blinky

# PATH de la biblioteca
LIBCDR = ../library/

OBJECTS = $(TARGET).o
# Si hay algun otro archivo que deba compilarse debe agregarse el .o 
# correspondiente. Por ejemplo un archivo func.c que tiene funciones 
# encapsuladas, debe incluirse:
# OBJECTS += func.o
OBJECTS +=

# MMCU es el microcontralador para el target para el avr-gcc
# Para ver el listado de posibles opciones ir a:
#      http://avr-libc.nongnu.org/user-manual/index.html
# atmega8/88/88a/88p/88pa
MMCU = atmega88a

# TARGET_P es el microcontrolador para el target del programador
# Para ver el listado de posibles opciones hacer:
#      avrdude -p ?
STRIPMMCU = $(strip $(MMCU))
ifeq ($(STRIPMMCU), atmega8)
	MMCU_N = 0
	TARGET_P = m8
else ifeq ($(findstring atmega88,$(STRIPMMCU)), atmega88)
 	MMCU_N = 1
	ifeq ($(STRIPMMCU), atmega88) 
 		TARGET_P = m88
	else ifeq ($(STRIPMMCU), atmega88a) 
 		TARGET_P = m88
	else ifeq ($(STRIPMMCU), atmega88p) 
 		TARGET_P = m88
	else ifeq ($(STRIPMMCU), atmega88pa)
		TARGET_P = m88pa
	else
$(error Target $(MMCU) no soportado)
 	endif
else
$(error Target $(MMCU) no soportado)
endif


CLK_EXT ?= 18432000UL #cristal externo
CLK_EXT_8 ?= 2304000UL #cristal externo
#CLK_EXT = 12288000UL #cristal externo (PCB original)
#CLK_EXT_8 = 1536000UL #cristal externo (PCB original)
#CLK_INT =  8000000UL #oscilador interno cercano a 8 Mhz
#CLK_INT_8 =  1000000UL #oscilador interno cercano a 8 Mhz
CLK_INT ?=  8243700UL #si se conoce con mayor precision el oscilador interno
CLK_INT_8 ?=  1030462UL #si se conoce con mayor precision el oscilador interno

FUSE_DIV8 ?= 0 # 1 si usamos el prescaler de clock en 8. 0 sin prescaler
FUSE_CLKEXT ?= 1 # 1 si usamos el clock externo. 0 si usamos el clock interno

# F_CPU es la frecuencia de trabajo del microcontrolador.
# Es la base a partir de la cual se calculan los delays.

ifeq ($(STRIPMMCU), atmega8)
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

else ifeq ($(findstring atmega88,$(STRIPMMCU)), atmega88)
 	# 0xDF es el default  (pagina 299)
 	# ojo con el bit 5, siempre debe ser 0 -> XX0XXXXX
 	HFUSE = 0xDF
 
 	# 0x62 es el default (con oscilador interno de 1Mhz) (pagina 301)
 	# 0x6F cristal externo con DIV8
 	# 0xE2 oscilador interno sin DIV8 (8 Mhz)
	# 0xEF cristal externo sin DIV8
	ifeq ($(strip $(FUSE_DIV8)),1)
		ifeq ($(strip $(FUSE_CLKEXT)),0)
 			LFUSE = 0x62
 			CLK = $(CLK_INT_8)
 		else
 			LFUSE = 0x6F
 			CLK = $(CLK_EXT_8)
 		endif
 	else
		ifeq ($(strip $(FUSE_CLKEXT)),0)
 			LFUSE = 0xE2
 			CLK = $(CLK_INT)
 		else
 			LFUSE = 0xEF
 			CLK = $(CLK_EXT)
 		endif
 	endif
else
$(error Target $(MMCU) no soportado)
endif


OPTIMIZATION = 2

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

CFLAGS += -MD -MP -MT $(*F).o -MF $(@F).d 

AVROBJFLAGS =
AVROBJFLAGS += -O binary
AVROBJFLAGS += -j .text
AVROBJFLAGS += -j .data

LIBINCLUDE = $(LIBCDR)

COMMON_OBJECTS = usart.o adc.o
COMMON_OBJECTS := $(addprefix $(LIBCDR)/, $(COMMON_OBJECTS))

#HEXFLASHFLAGS = -R .eeprom -R .fuse -R .lock -R .signature

# Reglas
########

# Reglas para compilar y generar el binario para subir al target
all: hex

elf: $(OBJECTS) 
	@echo "Clock = $(CLK) | Lfuse = $(LFUSE) | Hfuse = $(HFUSE)"
	avr-gcc $(CFLAGS) $(OBJECTS) -o $(TARGET).elf
	avr-objdump -h -S $(TARGET).elf > $(TARGET).lst
	avr-size -d $(TARGET).elf

bin: elf
	avr-objcopy $(AVROBJFLAGS) $(TARGET).elf $(TARGET).bin

hex: elf
	avr-objcopy $(HEXFLASHFLAGS) -O ihex $(TARGET).elf $(TARGET).hex

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  assemble .s files to .o
#  .c or .cpp files to .s

.c.o:
	avr-gcc $(CFLAGS) $(CDEFINES) -I$(LIBINCLUDE) -c $< -o $(<:.c=.o)

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
	rm -rf $(TARGET).bin 
	rm -rf $(TARGET).lst 
	rm -rf $(TARGET).hex
	rm -rf $(TARGET).elf
	rm -rf *.o 
	rm -rf hfuse.hex lfuse.hex 
	rm -rf $(OBJECTS)

.PHONY: clean eeprom_read eeprom_write fuse fuses_read program program_dw
