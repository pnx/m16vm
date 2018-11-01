
CC = gcc
CFLAGS = -Iinclude -DMEM_SIZE=32
LD = $(CC)

VM = m16vm

$(VM) : src/vm.o src/cpu.o src/mm.o src/instr_decode.o src/syscall.o src/program.o
	$(LD) $(LDFLAGS)-o $@ $^

clean :
	$(RM) src/*.o

distclean : clean
	$(RM) $(VM)
