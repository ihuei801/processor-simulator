#include "Data_Operators.h"
#include "../../Proj03.h"
#include <iostream>
#include <iomanip>
using namespace std;
Data_Operators::Data_Operators( Registers* reg, DDisk* ddisk, DMemory* dmemory, Cache* dcache, PageTable* dPageTable, TLB* dTLB )
{
    _Reg = reg;
    _DDisk = ddisk;
    _DMemory = dmemory;
    _DCache = dcache;
    _DPageTable = dPageTable;
    _DTLB = dTLB;
}

unsigned int Data_Operators::translatedataaddr(unsigned int viraddr){
    unsigned int virpagenum = viraddr / _DMemory->getPageSize();
    unsigned int pageoffset = viraddr % _DMemory->getPageSize();
    unsigned int phypagenum;
    unsigned int phyaddr;
    _DTLB->Incrementcount();
    _DMemory->Incrementcount();
    if(!_DTLB->translateaddr(phypagenum, virpagenum)){
        _DTLB->setMissCount(_DTLB->getMissCount()+1);
        if(!_DPageTable->translateaddr( *_DTLB , virpagenum)){ //not in memory, page fault
            DLoadPageDisktoMem( virpagenum * _DMemory->getPageSize() ); //use virtual pagenum to load the page from disk and update pagetable, TLB
        }
        if(!_DTLB->translateaddr(phypagenum, virpagenum)){
            cerr<<"somthing wrong when dealing with DTLB miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DTLB->setHitCount(_DTLB->getHitCount()+1);

    }
    phyaddr = (phypagenum * _DMemory->getPageSize() ) + pageoffset;
    return phyaddr;
}
void Data_Operators::DLoadPageDisktoMem( unsigned int virpageaddr )
{
    
    unsigned int phypagenum = _DMemory->getFreePage(*_DPageTable,*_DTLB,*_DDisk, *_DCache); //need to update PTE and TLB if page is replaced
    unsigned int virpagenum =  virpageaddr / _DMemory->getPageSize();
    int i,j;
    for(i=0;i<_DMemory->getPageSize();i++){
        _DMemory->setDataByte(phypagenum*_DMemory->getPageSize()+i, _DDisk->getByte(virpageaddr+i));
    }
    _DMemory->setValid(phypagenum, true);
    _DMemory->setCount(phypagenum,0);
    //update pagetable,TLB
    _DPageTable->updateEntry( virpagenum, phypagenum);
    _DTLB->updateEntry( virpagenum, phypagenum, false);
}


void Data_Operators::DLoadBlockMemtoCache( unsigned int phyaddr )
{
    unsigned int blockaddr = phyaddr / _DCache->getBlockSize();
    unsigned int blocknum = blockaddr % _DCache->getBlockNum();
    unsigned int setnum = blocknum % _DCache->getSetNum();
    unsigned int phypagenum = phyaddr / _DMemory->getPageSize();
    unsigned int tag = blockaddr / _DCache->getSetNum();
    int maxcount=-1;
    int blocktoreplace;
    
    for(int i=0;i<_DCache->getAssociativity();i++){
        if(_DCache->getBlockValid(setnum,i)==false){
            for(int j=0;j<_DCache->getBlockSize();j++){
                _DCache->setBlockData(setnum,i,j, _DMemory->getDataByte( blockaddr*_DCache->getBlockSize()+j ));
            }
            _DCache->setBlockValid(setnum,i,true);
            _DCache->setBlockDirty(setnum,i,false);
            _DCache->setBlockTag(setnum,i, tag);
            _DCache->setBlockCount(setnum,i ,0);
            return;
        }
        else{
            if(_DCache->getBlockCount(setnum,i)>maxcount){
                maxcount =_DCache->getBlockCount(setnum,i);
                blocktoreplace = i;
            }
        }
    }
    //if the block to be replaced is dirty, write back to memory
    if(_DCache->getBlockDirty(setnum,blocktoreplace)==true){
        
        unsigned int tag_replace = _DCache->getBlockTag(setnum,blocktoreplace);
        unsigned int blockphyaddr_replace = (tag_replace * _DCache->getSetNum() + setnum) * _DCache->getBlockSize();
        unsigned int phypagenum_replace = blockphyaddr_replace/_DMemory->getPageSize();
        
      
        for(int j=0;j<_DCache->getBlockSize();j++){
            _DMemory->setDataByte( blockphyaddr_replace+j , _DCache->getBlockData(setnum,blocktoreplace,j));
        }
        _DTLB->writememdirty(phypagenum_replace);
        _DPageTable->writememdirty(phypagenum_replace);
    }
    
    //load data to cache
    for(int j=0;j<_DCache->getBlockSize();j++){
        _DCache->setBlockData(setnum,blocktoreplace,j,  _DMemory->getDataByte( blockaddr*_DCache->getBlockSize()+j ));
    }
    
    
    _DCache->setBlockValid(setnum,blocktoreplace,true);
    _DCache->setBlockDirty(setnum,blocktoreplace,false);
    _DCache->setBlockTag(setnum,blocktoreplace,tag);
    _DCache->setBlockCount(setnum,blocktoreplace,0);
    
}

/**lw = $t , SignExtImm($s) #load memory $s  to register $t*/
void Data_Operators::lw( int rs, int rt, const unsigned int SignExtImm )
{
    
    unsigned int viraddr = _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    unsigned int data;
   
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->getData( data,  phyaddr, 4, 0)){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        if(!_DCache->getData( data,  phyaddr, 4, 0)){
            cerr<<"somthing wrong when dealing with DCache miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }
    _Reg->Set(rt, data);
}

void Data_Operators::lh( int rs, int rt, const unsigned int SignExtImm )
{
    unsigned int viraddr =  _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    unsigned int data;
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->getData( data,  phyaddr, 2, 1)){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        if(!_DCache->getData( data,  phyaddr, 2, 1)){
            cerr<<"somthing wrong when dealing with DCache miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }
    _Reg->Set(rt, data);
}
void Data_Operators::lhu( int rs, int rt, const unsigned int SignExtImm )
{
    unsigned int viraddr =  _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    unsigned int data;
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->getData( data,  phyaddr, 2, 0)){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        if(!_DCache->getData( data,  phyaddr, 2, 0)){
            cerr<<"somthing wrong when dealing with DCache miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }
    _Reg->Set(rt, data);
}

void Data_Operators::lb(int rs, int rt, const unsigned int SignExtImm )
{
    unsigned int viraddr = _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    unsigned int data;
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->getData( data,  phyaddr, 1, 1)){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        if(!_DCache->getData( data,  phyaddr, 1, 1)){
            cerr<<"somthing wrong when dealing with DCache miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }
	_Reg->Set(rt, data);
}

void Data_Operators::lbu( int rs, int rt, const unsigned int SignExtImm )
{
    unsigned int viraddr =  _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    unsigned int data;
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->getData( data,  phyaddr, 1, 0)){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        if(!_DCache->getData( data,  phyaddr, 1, 0)){
            cerr<<"somthing wrong when dealing with DCache miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }
    _Reg->Set(rt, data);
}


void Data_Operators::lui(int rt, int imm)
{
	int result = imm << 16 ;
	_Reg->Set(rt, result);
}
 
/**sw = $t, SignExtImm($s) #store register $rt to memory $s*/
void Data_Operators::sw( int rs, int rt, const int SignExtImm )
{
    unsigned int viraddr = _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    _DCache->Incrementcount(phyaddr);
    
    if(!_DCache->storeData( phyaddr, _Reg->Get(rt), 4 )){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        if(!_DCache->storeData( phyaddr,  _Reg->Get(rt), 4)){
            cerr<<"somthing wrong when dealing with DCache miss"<<endl;
            system("pause");
            exit(1);
        }
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }

}

void Data_Operators::sb( int rs, int rt, const int SignExtImm )
{
    unsigned int viraddr = _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->storeData( phyaddr, _Reg->Get(rt), 1 )){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        _DCache->storeData( phyaddr,  _Reg->Get(rt), 1);
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }

}

void Data_Operators::sh( int rs, int rt, const int SignExtImm )
{
    unsigned int viraddr = _Reg->Get(rs) + SignExtImm;
    unsigned int phyaddr = translatedataaddr(viraddr);
    _DCache->Incrementcount(phyaddr);
    if(!_DCache->storeData( phyaddr, _Reg->Get(rt), 2 )){
        _DCache->setMissCount(_DCache->getMissCount()+1);
        DLoadBlockMemtoCache( phyaddr );
        _DCache->storeData( phyaddr,  _Reg->Get(rt), 2);
    }
    else{
        _DCache->setHitCount(_DCache->getHitCount()+1);
    }

}
