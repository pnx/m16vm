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
#    Prints memory after program execution
#
#  * M16_DEBUG_INSTR
#    Outputs the instructions executed in a human-readable format.
#
CC = gcc
CFLAGS = -Iinclude -DMEM_SIZE=32 -DM16_DEBUG_MEM
LD = $(CC)

VM = m16vm

$(VM) : src/vm.o src/cpu.o src/mm.o src/instr_decode.o src/syscall.o src/program.o
	$(LD) $(LDFLAGS)-o $@ $^

clean :
	$(RM) src/*.o

distclean : clean
	$(RM) $(VM)
