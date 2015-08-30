#ifndef DMemory_H
#define DMemory_H

#include "../PageTable/PageTable.h"
#include "../PageTable/TLB.h"
#include "../Disk/DDisk.h"
#include "../Cache/Cache.h"
#include <iostream>
class DMemory {
    
private :
    int _totalsize;
    int _pagesize;
    int _numofpage;
	unsigned char* _data;
    unsigned char **_page;
    
    //record for each page
    bool *valid;
    int *count;
    


public :
	DMemory();
	~DMemory();

	//operation functions
    void Initialize(int size, int pagesize);
    int getFreePage(PageTable& _DPageTable, TLB& _DTLB, DDisk& _DDisk, Cache& _DCache);
    void Incrementcount(void);
    void printDMemory(void);
    //basic get/set functions
    void setTotalSize( int size );
    int getTotalSize( void );
    void setPageSize( int pagesize );
    int getPageSize( void );
    void setNumofPage( int num );
    int getNumofPage( void );
    void setCount(int index, int val);
    int getCount(int index);
    void setValid(int index, bool val);
    bool getValid(int index);
    void setDataByte(unsigned int addr, unsigned char data);
    unsigned char getDataByte(unsigned int addr);

   
	
};

#endif