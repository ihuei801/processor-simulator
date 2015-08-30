#include "IMemory.h"
#include <iostream>
#include <iomanip>
using namespace std;
IMemory::IMemory():_ins(NULL),_page(NULL)
{
    setTotalSize(0);
    setPageSize(0);
    setNumofPage(0);
}

IMemory::~IMemory()
{
	delete [] _ins;
    delete [] _page;
    
}
void IMemory::Initialize(int size, int pagesize){
    setTotalSize(size);
    setPageSize(pagesize);
    setNumofPage(size/pagesize);
    _ins = new unsigned char[ getTotalSize() ];
    _page = new unsigned char*[ getNumofPage() ];
    valid = new bool[ getNumofPage() ];
    count = new int[ getNumofPage() ];
    int i;
    for(i=0;i<getTotalSize();i++){
        _ins[i] = 0;
    }
    for(i=0;i<getNumofPage();i++){
        _page[i] = &_ins[ i * getPageSize() ];
        valid[i] = false;
        count[i] = 0;
    }

}

int IMemory::getFreePage(PageTable& _IPageTable, TLB& _ITLB, Cache& _ICache)
{
    int i,j;
    int maxcount=-1;
    int pagetoreplace;
    for(i=0;i<getNumofPage();i++){
        if(valid[i]==false){
            valid[i]=true;
            count[i]=0;
            return i;
        }
        else{
            if(count[i]>maxcount){
                maxcount = count[i];
                pagetoreplace = i;
            }
        }
    }
    unsigned int blockaddr;
    unsigned int setnum;
    unsigned int blocknum;
    unsigned int tag;
    //write cache to mem if dirty
    for(i=0;i<getPageSize()/_ICache.getBlockSize();i++){
        blockaddr = pagetoreplace * (getPageSize()/_ICache.getBlockSize())+i;
        blocknum = blockaddr % _ICache.getBlockNum();
        setnum = blocknum % _ICache.getSetNum();
        tag = blockaddr/_ICache.getSetNum();
        for(j=0;j<_ICache.getAssociativity();j++){
            if(_ICache.getBlockTag(setnum,j)==tag){
                _ICache.setBlockValid(setnum,j,false);
                _ICache.setBlockDirty(setnum,j,false);
                
            }
        }
    }

    valid[pagetoreplace] = false;
    count[pagetoreplace] = 0;
    _IPageTable.IupdateReplace(pagetoreplace);
    _ITLB.updateReplace(pagetoreplace);
    return pagetoreplace;
}
void IMemory::Incrementcount(void)
{
    int i;
    for(i=0;i<getNumofPage();i++){
        if(valid[i]==true){
            count[i]++;
        }
    
    }

}
void IMemory::printIMemory()
{
    int i,j;
    for(i=0;i<getNumofPage();i++){
        if(valid[i]==true){
            cout<<"page: "<<i<<endl;
            for(j=0;j<getPageSize();j++){
                cout<<setbase(16)<<setw(2)<<setfill('0')<<(int)_page[i][j];
                if((j+1)%4==0) cout<<endl;
            }
            cout<<endl;
        }
        
    }


}
//basic get/set functions
void IMemory::setTotalSize( int size ){
    _totalsize = size;
}
int IMemory::getTotalSize( void ){
    return _totalsize;
}
void IMemory::setPageSize( int pagesize ){
    _pagesize = pagesize;
}
int IMemory::getPageSize( void ){
    return _pagesize;
}
void IMemory::setNumofPage( int num ){
    _numofpage = num;
}
int IMemory::getNumofPage( void ){
    return _numofpage;
}
void IMemory::setCount(int index, int val){
    count[index] = val;

}
int IMemory::getCount(int index){
    return count[index];
}
void IMemory::setValid(int index, bool val)
{
    valid[index] = val;
}
bool IMemory::getValid(int index)
{
    return valid[index];
}
void IMemory::setInsByte(unsigned int addr, unsigned char data)
{
    _ins[addr] = data;

}
unsigned char IMemory::getInsByte(unsigned int addr)
{
    return _ins[addr];
}
