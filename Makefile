# ---------------------------------------------------------------------------
#  m16 Makefile.
# ---------------------------------------------------------------------------
#
#  The following debug flags exists:
#
#  * MEM_SIZE=<value>
#    Defines the memory size of the virtual machine (in bytes)
#
#  * M16_DEBUG_INSTR
#    Outputs the instructions executed in a human-readable format.
#
#  Example: make EXT_CFLAGS="-DM16_DEBUG_INSTR -DMEM_SIZE=512"
CC 	= gcc
CFLAGS 	= -Ilib/include $(EXT_CFLAGS)
LD 	= $(CC)

ifndef VERBOSE
	QUIET_CC = @echo " CC" $@;
	QUIET_LD = @echo " LD" $@;
	QUIET_AR = @echo " AR" $@;
endif

PROGRAMS = m16vm m16as

all: $(PROGRAMS)

m16vm : vm/vm.o vm/cpu.o vm/mm.o vm/instr_decode.o \
	vm/syscall.o vm/program.o vm/debug.o lib/libm16.a
	$(QUIET_LD)$(LD) $(LDFLAGS) -o $@ $^

m16as : as/as.o as/parser.o as/lexer.o as/lexer/number.o \
	as/codegen.o as/symtab.o \
	as/ast.o lib/libm16.a
	$(QUIET_LD)$(LD) $(LDFLAGS) -o $@ $^

lib/libm16.a : lib/vector.o lib/error.o
	$(QUIET_AR)$(AR) rcs $@ $^

%.o : %.c
	$(QUIET_CC)$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) as/*.o
	$(RM) as/lexer/*.o
	$(RM) vm/*.o
	$(RM) lib/*.o
	$(RM) lib/*.a

distclean : clean
	$(RM) $(PROGRAMS)
