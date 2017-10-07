srcs = Split(
    '''
    lexer.c
    ''')

VariantDir('build_at', '.', duplicate=0)
VariantDir('build_test', '.', duplicate=0)

env = Environment()
env['F_CPU'] = 1000000
env['MCU'] = 'attiny44a'
env['CC'] = 'avr-gcc'
env['OBJCOPY'] = 'avr-objcopy'
env.Append(CCFLAGS='-Wall -Os -g -flto -mmcu=${MCU} -DF_CPU=${F_CPU} -std=c11')
env.Append(LINKFLAGS='-w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=${MCU}')

env.Program(
    'attiny44.bin',
    source=['atmain.c'] + srcs,
    srcdir='build_at',
)

env.Command(
    action='${OBJCOPY} -O ihex -R .eeprom ${SOURCES} ${TARGETS}',
    source='attiny44.bin',
    target='attiny44.hex',
)


test_env = Environment()
test_env.Program(
    'test',
    source=['testmain.c'] + srcs,
    srcdir='build_test',
)

# TODO:

# avrdude:
# 	$(AVRDUDE_CMD) -t

# fuses:
# 	${AVRDUDE_CMD} -Uefuse:w:0xff:m -Uhfuse:w:0xdf:m -Ulfuse:w:0x62:m

# upload: ${HEX}
# 	${AVRDUDE_CMD} -U $<
