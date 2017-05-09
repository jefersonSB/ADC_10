#!/usr/bin/gmake -f

# Project files

MODULE1=sc_main
#MODULE2=srcs
SRC1 =./sc_main.cpp
#SRC2 =./srcs.cpp 

EXE_NAME=sc_main

# SystemC and UVM path

SYSTEMC_PATH=$(SC_PATH)
SYSTEMC_INCLUDE=$(SC_INCLUDE)
SYSTEMC_LIB_PATH=-L$(SC_LIB) -lsystemc

SYSTEMC_AMS_PATH=$(SC_AMS_PATH)
SYSTEMC_AMS_INCLUDE=$(SC_AMS_INCLUDE)
SYSTEMC_AMS_LIB_PATH=-L$(SC_AMS_LIB) -lsystemc-ams

#UVM_PATH=$(SC_UVM_PATH)
#UVM_INCLUDE=$(SC_UVM_INCLUDE)
#UVM_LIB_PATH=-L$(SC_UVM_LIB) -luvm-systemc

# Static 
CXX=g++
CFLAGS=-O0 -g -ggdb -Wno-deprecated -DDEBUG_SYSTEMC
WORK=.

PREFIX=-I$(WORK)/ -I$(SYSTEMC_INCLUDE) -I$(SYSTEMC_AMS_INCLUDE)

.PHONY: all clean 

all:
	@echo "+++++++++++++++++++++++++++++++++++++++++++++++"
	@echo "+++ Compiling +++++++++++++++++++++++++++++++++"
	$(CXX) $(CFLAGS) $(PREFIX) -c $(SRC1)
	@echo "+++ Generating executable +++++++++++++++++++++"
	$(CXX) -o $(EXE_NAME).x $(MODULE1).o $(SYSTEMC_LIB_PATH) $(SYSTEMC_AMS_LIB_PATH) 
	@echo "+++ Executing +++++++++++++++++++++++++++++++++"
	$(WORK)/$(EXE_NAME).x
	
clean:
	rm -fr *.o *.x *.deps dependencies.mk *~


