//CPU Construction
#ifndef MIPS_H
#define MIPS_H

#define HALT 0x3F
#include "Disk/IDisk.h"
#include "Disk/DDisk.h"
#include "Memory/IMemory.h"
#include "Memory/DMemory.h"
#include "Cache/Cache.h"
#include "PageTable/PageTable.h"
#include "PageTable/TLB.h"
#include "Registers/Registers.h"
#include "../Utilities/Translator/Translator.h"

class MIPS {
private :
    
    unsigned int _PC;		//Program Counter
    unsigned int _InsNum;	//Number of Instruction
    unsigned int _DataNum;	//Number of Data
    int _Cycle;				//Cycle Counter
    Registers _Reg;			//Register Structure of MIPS
    IDisk _IDisk;  //Disk Object
    DDisk _DDisk;  //Disk Object
    IMemory _IMemory;			//Memory Object
    DMemory _DMemory;           //Memory Object
    Cache _ICache;              //Cache Object
    Cache _DCache;              //Cache Object
    PageTable _IPageTable;      //Pagetable object
    PageTable _DPageTable;      //Pagetable object
    TLB _ITLB;                  //TLB object
    TLB _DTLB;                  //TLB object
	
    
    //Utility Functions
    void Print_Registers( ostream &output );	//Print the content of all registers
    void Print_Report( ostream &output );	//Print the number of hits and misses
    void Print_Memhie(void);
    unsigned int EndianTransform( unsigned int src );	//Transform 'Little-Endian' into 'Big-Endian'
    bool doAction( Translator src );		//Run the specific instruction
    //Basic get/set functions
    void setCycle( int );
    int getCycle( void ) const;
    void setPC( unsigned int );
    unsigned int getPC( void ) const;
    void setInsNum( unsigned int );
    unsigned int getInsNum( void ) const;
    void setDataNum( unsigned int );
    unsigned int getDataNum( void ) const;
	
public :
    
    void Init( int = 64, int = 32, int = 8, int = 16, int = 16, int = 4, int = 4, int = 16, int = 4, int = 1 );
    void LoadIns( void );	//Load Instructions from external resource to IDisk
    void LoadData( void );	//Initialize MIPS ($SP, PC.....)
    void Run( void );		//Run MIPS
    void ILoadPageDisktoMem( unsigned int virpageaddr );
    void ILoadBlockMemtoCache( unsigned int phyaddr );
};

#endif