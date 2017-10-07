env = DefaultEnvironment()
env['F_CPU'] = 1000000
env['MCU'] = 'attiny44a'
env['CC'] = 'avr-gcc'
env['OBJCOPY'] = 'avr-objcopy'
env.Append(CCFLAGS='-Wall -Os -g -flto -mmcu=${MCU} -DF_CPU=${F_CPU} -std=c11')
env.Append(LINKFLAGS='-w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=${MCU}')

Program(
    'attiny44.bin',
    source=Split(
        '''
        atmain.c
        lexer.c
        '''),
)

Command(
    action='${OBJCOPY} -O ihex -R .eeprom ${SOURCES} ${TARGETS}',
    source='attiny44.bin',
    target='attiny44.hex',
)

# TODO:

# avrdude:
# 	$(AVRDUDE_CMD) -t

# fuses:
# 	${AVRDUDE_CMD} -Uefuse:w:0xff:m -Uhfuse:w:0xdf:m -Ulfuse:w:0x62:m

# upload: ${HEX}
# 	${AVRDUDE_CMD} -U $<
