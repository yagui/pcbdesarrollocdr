# TARGET es el nombre del proyecto
TARGET = serial
LIBCDR = ../library/

CLK_EXT = 18432000UL #cristal externo
#CLK_EXT = 12288000UL #cristal externo
CLK_INT = 8243700UL #oscilador interno cercano a 8 Mhz

DIV8 = 0
EXTCLK = 1

# MMCU es el microcontralador para el target para el avr-gcc
# Para ver el listado de posibles opciones ir a:
#      http://avr-libc.nongnu.org/user-manual/index.html
# atmega8/88/88a/88p/88pa
MMCU = atmega88a



# TARGET_P es el microcontrolador para el target del programador
# Para ver el listado de posibles opciones hacer:
#      avrdude -p ?
ifeq ($(MMCU), atmega8)
	TARGET_P = m8

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

	MMCU_N = 0
else
 	ifeq ($(findstring atmega88,$(MMCU)), atmega88)
 		ifeq ($(MMCU), atmega88) 
 			TARGET_P = m88
 		else
 			ifeq ($(MMCU), atmega88a) 
 				TARGET_P = m88
$(info text...)
 			else
 				ifeq ($(MMCU), atmega88p) 
 					TARGET_P = m88
 				else
					TARGET_P = m88pa
 				endif
 			endif
 		endif
 
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
 
 		MMCU_N = 1
 
 	else
$(error Target no soportado)
 	endif
 
endif

SRC = main.c
INC = 

SRC += $(LIBCDR)/usart.c
INC += $(LIBCDR)/usart.h

# Flags de optimizacion '-Os' tamaño '-O3' velocidad
# Si se deja -O vacio el compilador elige '-01'
CFLAGS = -O2 -Wall

CDEFINES = -DF_CPU=$(CLK)

CLDFLAGS = 

AVROBJFLAGS = -j .text -j .data

all: hex

hex: $(SRC)
	avr-gcc $(CFLAGS) $(CDEFINES) $(CLDFLAGS) -mmcu=$(MMCU) -o $(TARGET).bin -I/usr/avr/include/ -I$(LIBCDR) $(SRC)
	avr-objcopy $(AVROBJFLAGS) -O ihex  $(TARGET).bin $(TARGET).hex
	avr-objdump -h -S $(TARGET).bin > "$(TARGET).lst"
	avr-size -d $(TARGET).bin

program: hex
	avrdude -c usbtiny -p $(TARGET_P) -U flash:w:$(TARGET).hex:i

fuses:
	avrdude -c usbtiny -p $(TARGET_P) -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m

clean:
	rm -rf $(TARGET).hex $(TARGET).bin $(TARGET).s $(TARGET).lst *.o
