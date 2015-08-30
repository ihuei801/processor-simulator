#include "I_Type.h"
#include "Instruction.h"

void I_Type::setRS( int src )
{
	_rs = src;
}

int I_Type::getRS( void ) const
{
	return _rs;
}

void I_Type::setRT( int src )
{
	_rt = src;
}

int I_Type::getRT( void ) const
{
	return _rt;
}

void I_Type::setImmediate( int src )
{
	_immediate = src;
}

int I_Type::getImmediate( void ) const
{
	// 000000000 00000000 10000000 000000000
	unsigned int mask = 32768;
	unsigned int tmp;
	tmp = _immediate & mask; //fetch the sixteenth bit
	
	unsigned int result = 0;

	for( int i=0; i<17; i++ ) {		//sign-extension
		result = result | tmp;
		if( i < 16 )				
			result = result << 1;	//left-shift for sign bit
	}

	result = result | _immediate;

	return result;
}

unsigned int I_Type::getUnsignedImmediate( void ) const
{
	return _immediate;
}

int I_Type::type_name(void) const
{
	return I_TYPE;
}
