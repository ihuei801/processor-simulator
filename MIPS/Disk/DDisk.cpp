#include "DDisk.h"


DDisk::DDisk()
{
    setSize(DDISK_SIZE);
    _data = new unsigned char[_size];
    for(int i=0;i<DDISK_SIZE;i++){
        _data[i] = 0;
    }
}

DDisk::~DDisk()
{
	delete [] _data;
}

void DDisk::setByte( unsigned int addr, unsigned char val )		//Parameters : Absolute Addr, Val
{
	if( addr <0 || addr >= _size ) {
        cerr<<"DDisk out of space"<<endl;

	}
	else
		_data[addr] = val;
}
unsigned char DDisk::getByte(unsigned int addr)
{
    if( addr < 0 || addr >= _size ) {
        cerr << "ERROR : 'InsPool::setByte()'-> The position is out of range ! " << endl;
        cerr << "Wrong Postion = " << addr << endl;
        //system("PAUSE");
        //exit(1);
    }
    else{
        return _data[addr];
    }
    
}
void DDisk::setWord( unsigned int addr, unsigned int val )		//Parameters : Absolute Addr, Val
{
    //11111111 00000000 00000000 00000000
    unsigned int mask = 4278190080;
    unsigned int tmp = 0;
    unsigned char bitSet[4];
    
    tmp = mask & val;
    tmp = tmp >> 24;
    bitSet[0] = tmp;
    
    //00000000 11111111 00000000 00000000
    mask = 16711680;
    tmp = mask & val;
    tmp = tmp >> 16;
    bitSet[1] = tmp;
    
    //00000000 00000000 11111111 00000000
    mask = 65280;
    tmp = mask & val;
    tmp = tmp >> 8;
    bitSet[2] = tmp;
    
    //00000000 00000000 00000000 11111111
    mask = 255;
    tmp = mask & val;
    bitSet[3] = tmp;
    
    for( int i=0; i<4; i++ )
		setByte( addr+i, bitSet[i] );
}

unsigned int DDisk::getWord( unsigned int addr ) 
{
	unsigned char tmp;
	unsigned int val = 0;

    
    for( int i=0; i<4; i++ ) {
		tmp = getByte(addr+i);
		val = val | tmp;
		if( i < 3 )
			val = val << 8;  //left-shift
	}

	return val;
}
void DDisk::printDDisk(void)
{
    cout<<"D Disk: "<<endl;
    for(int i=0;i<getSize();i++){
        cout<<setbase(16)<<setw(2)<<setfill('0')<<(int)_data[i];
        if((i+1)%4==0) cout<<endl;
    }
}
//basic get/set functions
void DDisk::setSize(int size)
{
    _size = size;
}

int DDisk::getSize(void)
{
    return _size;
}
