#include "IDisk.h"
#include <iostream>
using namespace std;
IDisk::IDisk()
{
    setSize(IDISK_SIZE);
    _ins = new unsigned char[_size];
    for(int i=0;i<_size;i++){
        _ins[i]=0;
    }
}

IDisk::~IDisk()
{
	delete [] _ins;
}

void IDisk::setByte( unsigned int pos, unsigned char val )		//Parameters : Pos, Val
{
	if( pos < 0 || pos >= _size ) {
		cerr << "ERROR : 'InsPool::setByte()'-> The position is out of range ! " << endl;
		cerr << "Wrong Postion = " << pos << endl;
		//system("PAUSE");
		//exit(1);
	}
    else{
	    _ins[pos] = val;
    }
}
unsigned char IDisk::getByte(unsigned int pos)
{
    if( pos < 0 || pos >= _size ) {
        cerr << "ERROR : 'InsPool::setByte()'-> The position is out of range ! " << endl;
        cerr << "Wrong Postion = " << pos << endl;
        //system("PAUSE");
        //exit(1);
    }
    else{
    
        return _ins[pos];
    }

}
void IDisk::setIns( unsigned int addr, unsigned int ins )   //set new instruciton into IDisk
{
    //11111111 00000000 00000000 00000000
    unsigned int mask = 4278190080;
    unsigned int tmp = 0;
    unsigned char bitSet[4];
    
    tmp = mask & ins;
    tmp = tmp >> 24;
    bitSet[0] = tmp;
    
    //00000000 11111111 00000000 00000000
    mask = 16711680;
    tmp = mask & ins;
    tmp = tmp >> 16;
    bitSet[1] = tmp;
    
    //00000000 00000000 11111111 00000000
    mask = 65280;
    tmp = mask & ins;
    tmp = tmp >> 8;
    bitSet[2] = tmp;
    
    //00000000 00000000 00000000 11111111
    mask = 255;
    tmp = mask & ins;
    bitSet[3] = tmp;
    for( int i=0; i<4; i++ )
        setByte( addr+i, bitSet[i] );
}
unsigned int IDisk::getIns( unsigned int addr )   //set new instruciton into IDisk
{
    
    unsigned int tmp=0;
    for( int i=0; i<4; i++ ){
        tmp |= getByte( addr+i );
        if(i<3){
            tmp = tmp << 8;
        }
    }
    return tmp;
}
void IDisk::printIDisk(void)
{
    cout<<"I Disk: "<<endl;
    for(int i=0;i<getSize();i++){
        cout<<setbase(16)<<setw(2)<<setfill('0')<<(int)_ins[i];
        if((i+1)%4==0) cout<<endl;
    }
}
//basic get/set functions
void  IDisk::setSize(int size)
{
    _size = size;
}

int  IDisk::getSize( void )
{
    return _size;

}
