#include "../../Proj03.h"
#include "Cache.h"
#include <iostream>
using namespace std;
void Cache::Initialize( int size, int blocksize, int setassociativity )
{
    setTotalSize( size );
    setBlockSize( blocksize );
    setAssociativity( setassociativity );
    setBlockNum( getTotalSize() / getBlockSize() );
    setSetNum( getBlockNum()/getAssociativity() );
    setHitCount( 0 );
	setMissCount( 0 );
	_Cacheptr = new Block_type[ getBlockNum() ];
    
	//initialize every block
	for( int i=0; i<getBlockNum(); i++ ) {
		_Cacheptr[i].valid = false;
		_Cacheptr[i].dirty = false;
		_Cacheptr[i].count = 0;
		_Cacheptr[i].tag = 0;
		_Cacheptr[i].data = new unsigned char[ getBlockSize() ];
		for( int j=0; j<getBlockSize(); j++ )
			_Cacheptr[i].data[j] = 0;
	}
    
    
    _setIndex = new Block_type* [ getSetNum() ];
    //initialize setIndex
    for(int i=0;i<getSetNum();i++){
        _setIndex[i] = &_Cacheptr[ i * getAssociativity() ];
    
    }
}
void Cache::Incrementcount(unsigned int phyaddr)
{
    
    unsigned int blockoffset = phyaddr % getBlockSize();
    unsigned int blockaddr = phyaddr/getBlockSize();
    unsigned int blocknum = blockaddr % getBlockNum();
    unsigned int setnum = blocknum % getSetNum();
    //increment count
    for(int i=0;i<getAssociativity();i++){
        if(_setIndex[setnum][i].valid==true){
            _setIndex[setnum][i].count++;
        }
    }
    
}
void Cache::printCache(void){
    int i,j,k;
    for(i=0;i<getSetNum();i++){
        for(j=0;j<getAssociativity();j++){
            if(_setIndex[i][j].valid==true){
                cout<<"Set# "<<i<<" block# "<<j<<" count: "<<_setIndex[i][j].count;
                if(_setIndex[i][j].dirty==true){
                    cout<<" dirty";
                }
                cout<<"tag= "<<_setIndex[i][j].tag;
                cout<<" data=";
                for(k=0;k<getBlockSize();k++){
                    cout<<setbase(16)<<setw(2)<<setfill('0')<<(int)_setIndex[i][j].data[k];
                    if((k+1)%4==0){
                        cout<<endl;
                    }
                }
                cout<<endl;
                
               
            }
            
        }
    }
    cout<<"Cache Hit#: "<<getHitCount()<<endl;
    cout<<"Cache Miss#: "<<getMissCount()<<endl;
}
bool Cache::getIns(unsigned int &ins, unsigned int phyaddr)
{
    unsigned int blockoffset = phyaddr % getBlockSize();
    unsigned int blockaddr = phyaddr/getBlockSize();
    unsigned int blocknum = blockaddr % getBlockNum();
    unsigned int setnum = blocknum % getSetNum();
    unsigned int tag = blockaddr / getSetNum();
    unsigned int data = 0;
    
    for(int i=0;i<getAssociativity();i++){
        if(_setIndex[setnum][i].valid==true){
            if(tag==_setIndex[setnum][i].tag){
                _setIndex[setnum][i].count=0;
                for(int j=0;j<4;j++){
                     data |= _setIndex[setnum][i].data[blockoffset+j];
                    if(j<3){
                        data = data << 8;
                    }
                }
                ins = data;
                return true;
            }
        }
    }
    return false;
}


bool Cache::getData(unsigned int &data, unsigned int phyaddr, int byte, int sign)//byte: num of byte to load w:4 h:2 b:1 sign: 0 unsigned 1 signed
{
    unsigned int blockoffset = phyaddr % getBlockSize();
    unsigned int blockaddr = phyaddr/getBlockSize();
    unsigned int blocknum = blockaddr % getBlockNum();
    unsigned int setnum = blocknum % getSetNum();
    unsigned int tag= blockaddr / getSetNum();
   
    
    for(int i=0;i<getAssociativity();i++){
        if(_setIndex[setnum][i].valid==true){
            if(tag==_setIndex[setnum][i].tag){
               
                _setIndex[setnum][i].count=0;
                if(byte==4){
                    unsigned int val=0;
                    unsigned char tmp;
                    for(int j=0;j<4;j++){
                        tmp = _setIndex[setnum][i].data[blockoffset+j];
                        val = val | tmp;
                        if(j<3){
                            val = val << 8;
                        }
                    }
                    data = val;
                }
                else if(byte==2 && sign==0){   //lhu
                    unsigned short val=0;
                    unsigned char tmp;
                    for(int j=0;j<2;j++){
                        tmp = _setIndex[setnum][i].data[blockoffset+j];
                        val = val | tmp;
                        if(j<1){
                            val = val << 8;
                        }
                    }
                    data = val;
                }
                else if(byte==2 && sign==1){    //lh
                    short val=0;
                    unsigned char tmp;
                    for(int j=0;j<2;j++){
                        tmp = _setIndex[setnum][i].data[blockoffset+j];
                        val = val | tmp;
                        if(j<1){
                            val = val << 8;
                        }
                    }
                    data = val;
                }
                else if(byte==1 && sign==0){    //lbu
                    unsigned char val=0;
                    unsigned char tmp;
                    tmp = _setIndex[setnum][i].data[blockoffset];
                    val = tmp;
                    data = val;
                }
                else{//lb
                    char val=0;
                    unsigned char tmp;
                    tmp = _setIndex[setnum][i].data[blockoffset];
                    val = tmp;
                    data = val;
                
                }
                
                return true;
            }
        }
    }
    
    return false;
}

bool Cache::storeData(unsigned int phyaddr, unsigned int data, int byte)//byte: num of byte to load w:4 h:2 b:1 sign: 0 unsigned 1 signed
{
    unsigned int blockoffset = phyaddr % getBlockSize();
    unsigned int blockaddr = phyaddr/getBlockSize();
    unsigned int blocknum = blockaddr % getBlockNum();
    unsigned int setnum = blocknum % getSetNum();
    unsigned int tag= blockaddr / getSetNum();
    
    for(int i=0;i<getAssociativity();i++){
        if(_setIndex[setnum][i].valid==true){
            if(tag==_setIndex[setnum][i].tag){
                _setIndex[setnum][i].count=0;
                _setIndex[setnum][i].dirty=true;
                if(byte==4){//sw
                    //11111111 00000000 00000000 00000000
                    unsigned int mask = 4278190080;
                    unsigned int tmp = 0;
                    unsigned char bitSet[4];
                    
                    tmp = mask & data;
                    tmp = tmp >> 24;
                    bitSet[0] = tmp;
                    
                    //00000000 11111111 00000000 00000000
                    mask = 16711680;
                    tmp = mask & data;
                    tmp = tmp >> 16;
                    bitSet[1] = tmp;
                    
                    //00000000 00000000 11111111 00000000
                    mask = 65280;
                    tmp = mask & data;
                    tmp = tmp >> 8;
                    bitSet[2] = tmp;
                    
                    //00000000 00000000 00000000 11111111
                    mask = 255;
                    tmp = mask & data;
                    bitSet[3] = tmp;
                    for( int j=0; j<4; j++ )
                         _setIndex[setnum][i].data[blockoffset+j] = bitSet[j];
                }
                else if(byte==2){   //sh
                    unsigned int mask, tmp;
                    unsigned char bitSet[2];
                    
                    //00000000 00000000 11111111 00000000
                    mask = 65280;
                    tmp = mask & data;
                    tmp = tmp >> 8;
                    bitSet[0] = tmp;
                    
                    //00000000 00000000 00000000 11111111
                    mask = 255;
                    tmp = mask & data;
                    bitSet[1] = tmp;
                    for( int j=0; j<2; j++ )
                        _setIndex[setnum][i].data[blockoffset+j] = bitSet[j];
                }
                else{    //sb
                    _setIndex[setnum][i].data[blockoffset] = data;
                    
                }
                return true;
            }
        }
    }
   
    return false;
}

void Cache::setTotalSize( int src )
{
    _totalSize = src;
}

int Cache::getTotalSize( void ) const
{
    return _totalSize;
}

void Cache::setBlockSize( int src )
{
    _blockSize = src;
}

int Cache::getBlockSize( void ) const
{
    return _blockSize;
}

void Cache::setAssociativity( int src )
{
    _associativity = src;
}

int Cache::getAssociativity( void ) const
{
    return _associativity;
}

void Cache::setBlockNum( int src )
{
    _blockNum = src;
}

int Cache::getBlockNum( void ) const
{
    return _blockNum;
}
void Cache::setSetNum( unsigned int src )
{
    _setNum = src;
}

int Cache::getSetNum( void ) const
{
    return _setNum;
}
void Cache::setHitCount( unsigned int src )
{
    hit = src;
}

int Cache::getHitCount( void ) const
{
    return hit;
}

void Cache::setMissCount( unsigned int src )
{
    miss = src;
}

int Cache::getMissCount( void ) const
{
    return miss;
}
void Cache::setBlockValid(int setnum, int blocknum , bool valid)
{
    _setIndex[setnum][blocknum].valid = valid;
}
bool Cache::getBlockValid(int setnum, int blocknum)
{
    return _setIndex[setnum][blocknum].valid;
}
void Cache::setBlockDirty(int setnum, int blocknum , bool dirty)
{
    _setIndex[setnum][blocknum].dirty = dirty;
}
bool Cache::getBlockDirty(int setnum, int blocknum)
{
    return _setIndex[setnum][blocknum].dirty;
}
void Cache::setBlockTag(int setnum, int blocknum , unsigned int tag)
{
    _setIndex[setnum][blocknum].tag = tag ;
}
unsigned int Cache::getBlockTag(int setnum, int blocknum)
{
    return _setIndex[setnum][blocknum].tag;
}
void Cache::setBlockCount(int setnum, int blocknum , int count)
{
    _setIndex[setnum][blocknum].count = count;
}
int Cache::getBlockCount(int setnum, int blocknum)
{
    return _setIndex[setnum][blocknum].count;
}
void Cache::setBlockData(int setnum, int blocknum, int byte, unsigned char data)
{
    _setIndex[setnum][blocknum].data[byte] = data;
}
unsigned char Cache::getBlockData(int setnum, int blocknum, int byte)
{
    return _setIndex[setnum][blocknum].data[byte];
}
