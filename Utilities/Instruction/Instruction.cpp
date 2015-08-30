#include "Instruction.h"

unsigned int Instruction::getOPCode( void ) const
{
	return _OPCode;
}

void Instruction::setOPCode( unsigned int src )
{
	_OPCode = src;
}
