#include "J_Type.h"
#include "Instruction.h"

void J_Type::setAddress( unsigned int src)
{
	_address = src;
}

unsigned int J_Type::getAddress( void ) const
{
	// 000000010 00000000 00000000 000000000
	unsigned int mask = 33554432;
	unsigned int tmp;
	tmp = _address & mask; //fetch the sign bit
	
	unsigned int result = 0;

	for( int i=0; i<7; i++ ) {		//sign-extension
		result = result | tmp;
		if( i < 6 )	
			result = result << 1;	//left-shift for sign bit
	}

	result = result | _address;
	
	return result;
}

int J_Type::type_name(void) const
{
	return J_TYPE;
}
