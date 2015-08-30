#ifndef PageTable_H
#define PageTable_H
#include "TLB.h"
#include <iostream>
typedef struct PageTableentry{
    
    bool valid;
    bool dirty;
    unsigned int phypagenum;
    
}PageTableEntry;

class PageTable {
private :
    int hit;
    int miss;
    int _numofentry;
    PageTableEntry* PageTableptr;

public :
	PageTable();
	~PageTable();
    //I PageTable operation
    void Initialize(int numofentry);
    bool translateaddr(TLB& _TLB, unsigned int searvirpagenum); //translate virpagenum to phypagenum
    void IupdateReplace(unsigned int phypage);
    bool DupdateReplace(unsigned int &virpage,unsigned int phypagenum); //true if write
    void updateEntry(unsigned int newvirpagenum, unsigned int newphypagenum);
    void printPageTable(void);
    void writememdirty(int phypagenum);
    
    //basic set and get
    void setHitCount(int num);
    int getHitCount(void);
    void setMissCount(int num);
    int getMissCount(void);
    void setNumofEntry(int num);
    int getNumofEntry(void);
    void setDirty(int index);
    bool getDirty(int index);
    
   
      
};

#endif
