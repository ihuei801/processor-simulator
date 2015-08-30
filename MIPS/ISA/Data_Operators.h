//Instruction Set Architecture -- Data Store&Move Operators
#ifndef DATA_H
#define DATA_H

#include "../Registers/Registers.h"
#include "../Disk/DDisk.h"
#include "../Memory/DMemory.h"
#include "../Cache/Cache.h"
#include "../PageTable/PageTable.h"
#include "../PageTable/TLB.h"

class Data_Operators{
private :
    Registers* _Reg;
    DDisk* _DDisk;
    DMemory* _DMemory;
	Cache* _DCache;
    PageTable* _DPageTable;
    TLB* _DTLB;

public :
	Data_Operators( Registers*, DDisk*, DMemory*, Cache*, PageTable*, TLB* );
    unsigned int translatedataaddr(unsigned int viraddr);
    void DLoadPageDisktoMem( unsigned int virpageaddr );
    void DLoadBlockMemtoCache( unsigned int phyaddr );
//Load Instructions
  //I-type
    void lw( int rs, int rt, const unsigned int SignExtImm );
    void lh( int rs, int rt, const unsigned int SignExtImm );
    void lhu( int rs, int rt, const unsigned int SignExtImm );
	void lb( int rs, int rt, const unsigned int SignExtImm );
	void lbu( int rs, int rt, const unsigned int SignExtImm );
	void lui( int rt, int imm );      //The immediate value is shifted left 16 bits and stored in the register. The lower 16 bits are zeroes
	

//Store Instructions
  //I-type
	void sb( int rs, int rt, const int SignExtImm );
	void sh( int rs, int rt, const int SignExtImm );
	void sw( int rs, int rt, const int SignExtImm );
};

#endif