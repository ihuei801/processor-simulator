#include "DMemory.h"


DMemory::DMemory():_data(NULL),_page(NULL)
{
    setTotalSize(0);
    setPageSize(0);
    setNumofPage(0);
    
}

DMemory::~DMemory()
{
	delete [] _data;
}
//operation functions
void DMemory::Initialize(int size, int pagesize)
{
    setTotalSize(size);
    setPageSize(pagesize);
    setNumofPage(size/pagesize);
    _data = new unsigned char[ getTotalSize()];
    _page = new unsigned char*[ getNumofPage() ]; //2D array
    valid = new bool[ getNumofPage() ];
    count = new int [getNumofPage() ];
    int i;
    for(i=0;i<getNumofPage();i++){
        _page[i] = &_data[ i * getPageSize() ];
        valid[i] = false;
        count[i] = 0;
    }

}

int DMemory::getFreePage(PageTable& _DPageTable, TLB& _DTLB, DDisk& _DDisk, Cache& _DCache)
{
    int i,j,k;
    int maxnouse=-1;
    int pagetoreplace;
    unsigned int virpagenum;
    for(i=0;i<getNumofPage();i++){
        if(valid[i]==false){
            valid[i]=true;
            count[i]=0;
            return i;
        }
        else{
            if(count[i]>maxnouse){
                maxnouse = count[i];
                pagetoreplace = i;
            }
        }
    }

    unsigned int blockaddr;
    unsigned int setnum;
    unsigned int blocknum;
    unsigned int tag;
    //write cache to mem if dirty
    for(i=0;i<getPageSize()/_DCache.getBlockSize();i++){
        blockaddr = pagetoreplace * (getPageSize()/_DCache.getBlockSize())+i;
        blocknum = blockaddr % _DCache.getBlockNum();
        setnum = blocknum % _DCache.getSetNum();
        tag = blockaddr/_DCache.getSetNum();
        for(j=0;j<_DCache.getAssociativity();j++){
            if(_DCache.getBlockTag(setnum,j)==tag){
                if(_DCache.getBlockDirty(setnum,j)==true){
                    for(k=0;k<_DCache.getBlockSize();k++){
                        setDataByte(blockaddr*_DCache.getBlockSize()+k, _DCache.getBlockData(setnum,j,k));
                    }
                    _DPageTable.writememdirty( pagetoreplace );
                    _DTLB.writememdirty( pagetoreplace );
                }
                _DCache.setBlockValid(setnum,j,false);
                _DCache.setBlockDirty(setnum,j,false);
                
            }
        }
    }

    valid[pagetoreplace] = false;
    count[pagetoreplace] = 0;
   
    //write back to disk if dirty
    if(_DPageTable.DupdateReplace(virpagenum,pagetoreplace)){
        for(j=0;j<getPageSize();j++){
            _DDisk.setByte(virpagenum*getPageSize() + j, _data[pagetoreplace*getPageSize()+j]);
        }
    }
    _DTLB.updateReplace(pagetoreplace);
    return pagetoreplace;
}
void DMemory::Incrementcount(void)
{
    int i;
    for(i=0;i<getNumofPage();i++){
        if(valid[i]==true){
            count[i]++;
        }
        
    }
    
}
void DMemory::printDMemory()
{
    int i,j;
    for(i=0;i<getNumofPage();i++){
        if(valid[i]==true){
            cout<<"page: "<<i<<endl;
            for(j=0;j<getPageSize();j++){
                if((j+1)%4==0) cout<<endl;
            }
            cout<<endl;
        }
        
    }
    
}
//basic get/set functions
void DMemory::setTotalSize( int size ){
    _totalsize = size;
}
int DMemory::getTotalSize( void ){
    return _totalsize;
}
void DMemory::setPageSize( int pagesize ){
    _pagesize = pagesize;
}
int DMemory::getPageSize( void ){
    return _pagesize;
}
void DMemory::setNumofPage( int num ){
    _numofpage = num;
}
int DMemory::getNumofPage( void ){
    return _numofpage;
}
void DMemory::setCount(int index, int val){
    count[index] = val;
    
}
int DMemory::getCount(int index){
    return count[index];
}
void DMemory::setValid(int index, bool val)
{
    valid[index] = val;
}
bool DMemory::getValid(int index)
{
    return valid[index];
}

void DMemory::setDataByte(unsigned int addr, unsigned char data)
{
    _data[addr] = data;
    
}
unsigned char DMemory::getDataByte(unsigned int addr)
{
    return _data[addr];
}

