##############################################################################
# Application specific info.  Specify the object target basename, object
# files to be created, and any precompiled libraries here.
##############################################################################
TARGET = EduCode

OBJS = ifi_startup.o \
	ifi_utilities.o \
    main.o \
    user_routines.o \
    user_routines_fast.o \
    serial_utils.o \
    oi_data.o
    
LIBS = ifi_library.lib

##############################################################################
# Compiler settings.  Specify compiler/linker flags here, as well as default
# directories for system headers/libraries.
##############################################################################
ifndef MCC18PATH
MCC18PATH = /home/WS/mcc18
endif
CFLAGS = -p=18F8520 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-
LDFLAGS = -m$(TARGET).map -o$(TARGET).cof
LIBDIRS = -L`cygpath -w $(MCC18PATH)/lib`
INCLUDEDIRS = -I`cygpath -w $(MCC18PATH)/include`
PATH += :$(MCC18PATH)/bin

##############################################################################
# Executable defines & rules - shouldn't need to change anything below here.
##############################################################################
CC  = mcc18
LD  = mplink
RM  = rm
PCC = cpp18

.SUFFIXES: .o .c .i

all: $(TARGET).hex

$(TARGET).hex: $(OBJS) $(LIBS)
	$(LD) $(TARGET).lkr $(OBJS) $(LIBS) $(LDFLAGS) $(LIBDIRS) 

.c.o:
	$(CC) $*.c -fo $@ $(INCLUDEDIRS) $(CFLAGS)

.c.i:
	$(PCC) $*.c $(INCLUDEDIRS) > $@

clean:
	@echo "Cleaning build directory..."
	-@$(RM) *.o *.hex *.cof *.cod *.lst *.map *.err
