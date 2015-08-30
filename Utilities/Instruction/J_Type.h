//J_Type Instruction
#ifndef J_TYPE_H
#define J_TYPE_H

#include "Instruction.h"

class J_Type : public Instruction {
private :
	unsigned int _address : 26;	

public :
	void setAddress( unsigned int );
	unsigned int getAddress( void ) const;

	//Override
	int type_name(void) const;
};

#endif