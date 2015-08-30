//I_Type Instruction
#ifndef I_TYPE_H
#define I_TYPE_H

#include "Instruction.h"

class I_Type : public Instruction {
private :
	unsigned int _rs : 5;
	unsigned int _rt : 5;
	unsigned int _immediate : 16;

public :
	void setRS( int );
	int getRS( void ) const;
	void setRT( int );
	int getRT( void ) const;
	void setImmediate( int );
	int getImmediate( void ) const;
	unsigned int getUnsignedImmediate( void ) const;
	
	//Override
	int type_name(void) const;
};

#endif