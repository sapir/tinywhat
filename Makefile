SRCS = atmain.c
AVR_PART ?= t44
BIN = attiny44.bin

F_CPU ?= 1000000
MCU ?= attiny44a


CROSS = avr-
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy

CFLAGS ?= -Wall -Os -g -flto -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=c11
LDFLAGS ?= -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=$(MCU)
LIBS ?=


AVRDUDE ?= avrdude
AVRDUDE_FLAGS ?=
AVRDUDE_CMDS ?= $(AVRDUDE) $(AVRDUDE_FLAGS)


OBJS = $(subst .c,.o,${SRCS})
HEX = $(subst .bin,.hex,${BIN})


all: ${HEX}

clean:
	rm -f ${OBJS} ${BIN} ${HEX}

avrdude:
	$(AVRDUDE_CMD) -t

fuses:
	${AVRDUDE_CMD} -Uefuse:w:0xff:m -Uhfuse:w:0xdf:m -Ulfuse:w:0x62:m

upload: ${HEX}
	${AVRDUDE_CMD} -U $<

${HEX}: ${BIN}
	$(OBJCOPY) -O ihex -R .eeprom $< $@

${BIN}: ${OBJS}
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
