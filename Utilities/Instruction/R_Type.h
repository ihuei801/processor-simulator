//R_Type Instruction
#ifndef R_TYPE_H
#define R_TYPE_H

#include "Instruction.h"

class R_Type : public Instruction {
private :
	unsigned int _rs : 5;
	unsigned int _rt : 5;
	unsigned int _rd : 5;
	unsigned int _shamt : 5;		
	unsigned int _funct : 6;

public :
	void setRS( unsigned int );
	unsigned int getRS( void ) const;
	void setRT( unsigned int );
	unsigned int getRT( void ) const;
	void setRD( unsigned int );
	unsigned int getRD( void ) const;

	void setShamt( unsigned int );
	unsigned int getShamt( void ) const;
	void setFunct( unsigned int );
	unsigned int getFunct( void ) const;

	//Override
	int type_name(void) const;
};

#endif
