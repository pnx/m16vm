
CC = gcc
CFLAGS = -Iinclude -DMEM_SIZE=32 -DM16_DEBUG_MEM
LD = $(CC)

PROGRAMS = m16vm as

all: $(PROGRAMS)

m16vm : src/vm.o src/cpu.o src/mm.o src/instr_decode.o src/syscall.o src/program.o
	$(LD) $(LDFLAGS) -o $@ $^

as : src/as/as.o src/as/parser.o src/as/lexer.o src/as/instr_encode.o src/as/error.o
	$(LD) $(LDFLAGS) -o $@ $^

clean :
	$(RM) src/*.o
	$(RM) src/as/*.o

distclean : clean
	$(RM) $(PROGRAMS)
