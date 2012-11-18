# Makefile general para uso de la bilioteca.
# Copiar este Makefile dentro de cada proyecto y editar las opciones
# correspondientes

# TARGET es el nombre del proyecto y es el nombre que se usa para generar el
# binario
TARGET = main
#TARGET = helloworld

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
SRC = $(TARGET).c
INC = 

INC += $(LIBCDR)/defs.h
INC += $(LIBCDR)/board.h

include $(LIBCDR)/usart.mk
include $(LIBCDR)/adc.mk

OPTIMIZATION = 2

include $(LIBCDR)/setup.mk
