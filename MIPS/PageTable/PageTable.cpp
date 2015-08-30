#include "PageTable.h"
#include <iostream>
using namespace std;
PageTable::PageTable():PageTableptr(NULL)
{
    setHitCount(0);
    setMissCount(0);
    setNumofEntry(0);

}

PageTable::~PageTable(){
    delete [] PageTableptr;
}
void PageTable::Initialize(int numofentry)
{
    setNumofEntry(numofentry);
    PageTableptr = new PageTableEntry[numofentry];
    for(int i=0;i<numofentry;i++){
        PageTableptr[i].valid=false;
        PageTableptr[i].dirty=false;
        PageTableptr[i].phypagenum=0;
    }

}

//IPageTable operations
bool PageTable::translateaddr(TLB& _TLB, unsigned int searvirpagenum){
    unsigned int phypagenum;
    bool dirty;
    if(PageTableptr[searvirpagenum].valid==true){
        phypagenum = PageTableptr[searvirpagenum].phypagenum;
        dirty = PageTableptr[searvirpagenum].dirty;
        setHitCount(getHitCount()+1);
        _TLB.updateEntry(searvirpagenum,phypagenum,dirty);
        return true;
    }
    else{
        setMissCount(getMissCount()+1);
        return false;
    }
}
void PageTable::IupdateReplace(unsigned int phypage)
{
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(PageTableptr[i].valid==true && PageTableptr[i].phypagenum==phypage){
            PageTableptr[i].valid=false;
            PageTableptr[i].dirty=false;
            PageTableptr[i].phypagenum = 0;
            return;
        }
    }
}
bool PageTable::DupdateReplace(unsigned int& virpagenum, unsigned int phypage)
{
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(PageTableptr[i].valid==true && PageTableptr[i].phypagenum==phypage){
            PageTableptr[i].valid=false;
            PageTableptr[i].phypagenum = 0;
            virpagenum = i;
            if(PageTableptr[i].dirty==true){
                PageTableptr[i].dirty=false;
                return true;
            }
            else return false;
        }
    }

}

void PageTable::updateEntry(unsigned int virpage, unsigned int newphypagenum)
{
    PageTableptr[virpage].valid = true;
    PageTableptr[virpage].dirty = false;
    PageTableptr[virpage].phypagenum = newphypagenum;
}

void PageTable::printPageTable(void)
{
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(PageTableptr[i].valid==true){
            cout<<"Entry#: "<<i;
            cout<<" phypagenum= "<<PageTableptr[i].phypagenum;
            if(PageTableptr[i].dirty==true) cout<<" dirty ";
            cout<<endl;
        }
        
    }
    cout<<"hit #= "<<getHitCount()<<endl;
    cout<<"miss #= "<<getMissCount()<<endl;

}
void PageTable::writememdirty(int phypagenum){
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(PageTableptr[i].valid==true && PageTableptr[i].phypagenum==phypagenum){
            PageTableptr[i].dirty=true;
            break;
        }
    }
}

//basic set and get
void PageTable::setHitCount(int num)
{
    hit = num;
}
int PageTable::getHitCount(void)
{
    return hit;
    
}
void PageTable::setMissCount(int num)
{
    miss = num;
    
}
int PageTable::getMissCount(void)
{
    return miss;
    
}
void PageTable::setNumofEntry(int num)
{
    _numofentry = num;
}
int PageTable::getNumofEntry(void)
{
    return _numofentry;
}
void PageTable::setDirty(int index)
{
    PageTableptr[index].dirty = true;
}
bool PageTable::getDirty(int index)
{
    return PageTableptr[index].dirty;
}
