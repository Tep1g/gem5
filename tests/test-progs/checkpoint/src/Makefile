TARGET_ISA=x86

GEM5_HOME=$(realpath ../../../../)
$(info   GEM5_HOME is $(GEM5_HOME))

CXX=gcc

CFLAGS=-I$(GEM5_HOME)/include

LDFLAGS=-L$(GEM5_HOME)/util/m5/build/$(TARGET_ISA)/out -lm5

OBJECTS= ../bin/x86/linux/checkpoint
NICE_OBJECT = ../bin/x86/linux/checkpoint-nice.asm

all: checkpoint

checkpoint:
	$(CXX) -o $(OBJECTS) -O0 checkpoint.c $(CFLAGS) $(LDFLAGS)

nice:
	$(CXX) -o $(NICE_OBJECT) -S -fverbose-asm -O0 checkpoint.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS)