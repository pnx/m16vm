# ---------------------------------------------------------------------------
#  m16 Makefile.
# ---------------------------------------------------------------------------
#
#  The following debug flags exists:
#
#  * MEM_SIZE=<value>
#    Defines the memory size of the virtual machine (in bytes)
#
#  * M16_DEBUG_MEM
#    Prints memory after program execution.
#
#  * M16_DEBUG_REG
#    Prints register values after program execution.
#
#  * M16_DEBUG_INSTR
#    Outputs the instructions executed in a human-readable format.
#
CC = gcc
CFLAGS = -g -Ilib/include -DMEM_SIZE=32 -DM16_DEBUG_MEM
LD = $(CC)

PROGRAMS = m16vm m16as

all: $(PROGRAMS)

m16vm : src/vm.o src/cpu.o src/mm.o src/instr_decode.o src/syscall.o src/program.o
	$(LD) $(LDFLAGS) -o $@ $^

m16as : src/as/as.o src/as/parser.o src/as/lexer.o \
	src/as/codegen.o src/as/error.o src/as/symtab.o \
	src/as/ast.o lib/libm16.a
	$(LD) $(LDFLAGS) -o $@ $^

lib/libm16.a : lib/src/vector.o
	$(AR) rcs $@ $^

clean :
	$(RM) src/*.o
	$(RM) src/as/*.o
	$(RM) lib/*.o
	$(RM) lib/*.a

distclean : clean
	$(RM) $(PROGRAMS)
