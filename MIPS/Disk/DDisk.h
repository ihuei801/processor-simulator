//Stack Memory
#include "../../Proj03.h"

#ifndef DDISK_H
#define DDISK_H

#define DDISK_SIZE 1*1024  //Unit size -> Byte

class DDisk {
private :
    int _size;
	unsigned char* _data;

public :
	DDisk();
	~DDisk();
    
    
	//operation functions
	void setByte( unsigned int addr, unsigned char val );	//Absolute Parameters : Addr, Val
    unsigned char getByte(unsigned int pos);
	void setWord( unsigned int addr, unsigned int val );	//Parameters : Absolute Addr, Val
	unsigned int getWord( unsigned int addr );			//Parmeter : Absolute Addr
    void printDDisk(void);
    //basic get/set functions
    void setSize(int);
    int getSize(void);
};

#endif