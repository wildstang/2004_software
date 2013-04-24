##############################################################################
# Application specific info.  Specify the object target basename, object
# files to be created, and any precompiled libraries here.
##############################################################################
TARGET = FrcCode
SIM_TARGET = RcSim

OBJS = ifi_startup.o \
       ifi_utilities.o \
       main.o \
       printf_lib.o \
       user_routines.o \
       user_routines_fast.o \
       eeprom.o \
       ws_autonomous.o \
       ws_cc.o \
       ws_calibrate.o \
       ws_crab_feedback.o \
       ws_crab_input.o \
       ws_drive_input.o \
       ws_io.o \
       ws_trig.o \
       ws_serial2.o \
       ws_general.o \
       ws_arms.o \
       ws_feedback.o \
       ws_waypoints_012.o \
       ws_waypoints_34.o \
       ws_waypoints_56.o

SIM_OBJS = RC_Simulator.go \
           main.go \
           user_routines.go \
           user_routines_fast.go \
           ws_autonomous.go \
           ws_calibrate.go \
           ws_crab_feedback.go \
           ws_crab_input.go \
           ws_drive_input.go \
           ws_io.go \
           ws_trig.go \
           ws_serial2.go \
           ws_general.go \
           sim_picdefs.go

LIBS = FRC_library.lib

##############################################################################
# Compiler settings.  Specify compiler/linker flags here, as well as default
# directories for system headers/libraries.
##############################################################################
ifndef MCC18PATH
MCC18PATH = /home/WS/mcc18
endif
CFLAGS = -p=18F8520 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-
SIM_CFLAGS =

COPTS = -D_FRC_BOARD
SIM_COPTS = -Wall -D_FRC_BOARD -D__18F8520 -D__18CXX -DCYGWIN_IFI_SIMULATOR -c -g

LDFLAGS = -m$(TARGET).map -o$(TARGET).cof
SIM_LDFLAGS = -g

LIBDIRS = -L`cygpath -w $(MCC18PATH)/lib`
SIM_LIBDIRS = 

LINKER = 18f8520user.lkr

INCLUDEDIRS = -I`cygpath -w $(MCC18PATH)/include`
SIM_INCLUDEDIRS = -I../../../simulator/include

PATH += :$(MCC18PATH)/bin

##############################################################################
# Executable defines & rules - shouldn't need to change anything below here.
##############################################################################
CC  = mcc18
GCC = gcc   
LD  = mplink
GLD = gcc 
RM  = rm
PCC = cpp18

.SUFFIXES: .o .c .i .go

#all:  $(TARGET).hex $(SIM_TARGET).exe
all:  $(TARGET).hex

$(SIM_TARGET): $(SIM_TARGET).exe

$(SIM_TARGET).exe: $(SIM_OBJS) $(SIM_LIBS)
	$(GLD) -o $(SIM_TARGET).exe $(SIM_OBJS) $(SIM_LIBS) $(SIM_LDFLAGS) $(SIM_LIBDIRS)

$(TARGET).hex: $(OBJS) $(LIBS)
	$(LD) $(LINKER) $(OBJS) $(LIBS) $(LDFLAGS) $(LIBDIRS)

.c.o:
	$(CC) $*.c -fo $@ $(INCLUDEDIRS) $(COPTS) $(CFLAGS)

.c.i:
	$(PCC) $*.c $(INCLUDEDIRS) > $@

.c.go:
	$(GCC) $*.c -o $@ $(SIM_INCLUDEDIRS) $(SIM_COPTS)

clean:
	@echo "Cleaning build directory..."
	-@$(RM) *.o *.hex *.cof *.cod *.lst *.map *.err *.go
