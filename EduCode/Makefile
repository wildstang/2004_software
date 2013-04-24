##############################################################################
# Application specific info.  Specify the object target basename, object
# files to be created, and any precompiled libraries here.
##############################################################################
TARGET = EduCode

OBJS = ifi_startup.o \
	ifi_utilities.o \
    main.o \
    printf_lib.o \
    user_routines.o \
    user_routines_fast.o
    
LIBS = ifi_library.lib

##############################################################################
# Compiler settings.  Specify compiler/linker flags here, as well as default
# directories for system headers/libraries.
##############################################################################
MCC18PATH = /home/davidf/wildstang/mcc18
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

.SUFFIXES: .o .c

all: $(TARGET).hex

$(TARGET).hex: $(OBJS) $(LIBS)
	$(LD) $(TARGET).lkr $(OBJS) $(LIBS) $(LDFLAGS) $(LIBDIRS) 

.c.o:
	$(CC) $*.c -fo $@ $(INCLUDEDIRS) $(CFLAGS)

clean:
	@echo "Cleaning build directory..."
	-@$(RM) *.o *.hex *.cof *.cod *.lst *.map
