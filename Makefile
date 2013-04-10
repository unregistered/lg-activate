# DEVICE     = atmega168
DEVICE	   = attiny4313
CLOCK      = 9830400
PROGRAMMER = -c avrispmkii -P usb
OBJECTS    = main.o lgserial.o lgnetwork.o lgdb.o util.o
FUSES      = -U hfuse:w:0xdf:m -U lfuse:w:0xe0:m

# Tune the lines below only if you know what you are doing:
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-g++ -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -Wno-write-strings -D$(DEVICE)

# symbolic targets:
all:
	@echo "Which part of the project are you building? See Makefile for options. e.g. make basestation"
	@echo "To add a project, create a controller header file and controller c file and add options below"
	@echo "    See controller_example.h and controller_example.c"

# Projects
example: COMPILE += -DCONTROLLER_FILE='"controller_example.c"' -DUSE_NETWORK_SERVER
example: clean main.hex

basestation: COMPILE += -DCONTROLLER_FILE='"controller_basestation.c"' -DUSE_NETWORK_CLIENT
basestation: clean main.hex

serialtestserver: COMPILE += -DCONTROLLER_FILE='"controller_serialtestserver.c"' -DUSE_NETWORK_SERVER
serialtestserver: clean main.hex

serialtestclient: COMPILE += -DCONTROLLER_FILE='"controller_serialtestclient.c"' -DUSE_NETWORK_CLIENT
serialtestclient: clean main.hex

usbserial: COMPILE += -DCONTROLLER_FILE='"controller_usbserial.c"' -DUSE_NETWORK_CLIENT
usbserial: clean main.hex

ssd: COMPILE += -DCONTROLLER_FILE='"controller_ssd.c"' -DUSE_NETWORK_CLIENT
ssd: clean main.hex

db: COMPILE += -DCONTROLLER_FILE='"controller_db.c"' -DUSE_NETWORK_SERVER
db: clean main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	@echo CXXFLAGS=${COMPILE}
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
