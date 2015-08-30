#ifndef IMemory_H
#define IMemory_H
#include "../PageTable/PageTable.h"
#include "../PageTable/TLB.h"
#include "../Cache/Cache.h"
#include <iostream>
class IMemory {
private :
    int _totalsize;
    int _pagesize;
    int _numofpage;
    unsigned char *_ins;		//Instructions set, word unit
    unsigned char **_page;  //pointer to the beginning of each page
    
   
//record for each page
    bool *valid;  //record page that is used or not
    int *count; //for LRU calculation
	

public :
	IMemory();
	~IMemory();
    
    //Operation Function
    void Initialize(int size, int pagesize);
    int getFreePage(PageTable& _IPageTable, TLB& _ITLB, Cache &_ICache);
    void Incrementcount(void);
    void printIMemory(void);
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
    void setInsByte(unsigned int addr, unsigned char data);
    unsigned char getInsByte(unsigned int addr);
    
    
};

#endif
