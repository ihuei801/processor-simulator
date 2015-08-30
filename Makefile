#MACRO ->
#compiler
CC = g++
#cflags
CFLAGS = -c
#object
OBJS = Proj03.o MIPS/MIPS.o MIPS/Registers/Registers.o\
	MIPS/Disk/IDisk.o MIPS/Disk/DDisk.o MIPS/Memory/IMemory.o MIPS/Memory/DMemory.o\
	MIPS/ISA/Arithmetic_Operators.o MIPS/ISA/Control_Operators.o MIPS/ISA/Data_Operators.o MIPS/ISA/Logic_Operators.o \
	Utilities/Instruction/Instruction.o Utilities/Instruction/R_Type.o Utilities/Instruction/I_Type.o Utilities/Instruction/J_Type.o Utilities/Translator/Translator.o\
	MIPS/Cache/Cache.o MIPS/PageTable/TLB.o MIPS/PageTable/PageTable.o \
	
#BODY ->
#all : CMP clean

CMP : $(OBJS)
	$(CC) $(OBJS) -o $@

#clean up the unecessary files (Object files)
clean : 
	rm -f $(OBJS) CMP *.bin *.rpt *.exe *.out 
