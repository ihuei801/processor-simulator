//Pool of Instructions
#ifndef IDISK_H
#define IDISK_H

#include "../../Proj03.h"

#define IDISK_SIZE 1*1024  //Unit size -> Byte

class IDisk {
private :
    int _size;
	unsigned char *_ins;		//Instructions set

	
public :
	IDisk();
	~IDisk();
    void setIns( unsigned int addr, unsigned int ins );
    unsigned int getIns(unsigned int addr);
    void printIDisk(void);
    //Basic Get/Set Function
	void setByte( unsigned int pos, unsigned char val );	//Parameters : Absolute Pos, Val
    unsigned char getByte(unsigned int pos);
    //basid get/set functions
    void setSize(int size);
    int getSize( void );

};

#endif
