CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

INC = $(shell pwd)

OS_NAME:=$(shell uname -s | tr A-Z a-z)
ifeq ($(OS_NAME),darwin)
STDINCDIR := -I/opt/local/include
STDLIBDIR := -L/opt/local/lib
else
STDINCDIR :=-I/afs/cern.ch/work/c/cpena/scratch_DM/CMSSW_5_2_3/src/UserCode/CPena/src/DarkMatter/include
STDLIBDIR := 
endif

CPPFLAGS := -Wl,--no-as-needed $(shell root-config --cflags) -I$(INC)/include
LDFLAGS := $(shell root-config --glibs) $(STDLIBDIR)

CPPFLAGS += -g

TARGET = Spec

SRC = test.cc outTree_Fitting_NewScope.C

#TARGET = bkg_pre_1mu
#SRC = bkg_pre_1mu.cc src/DM_1DRatio.cc src/DM_2DRatio.cc src/DM_Base.cc

OBJ = $(SRC:.cc=.o)

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(LD) $(CPPFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

%.o : %.cc	
	$(CXX) $(CPPFLAGS) -o $@ -c $<
	@echo $@	
	@echo $<
clean :
	rm -f *.o src/*.o $(TARGET) *~

