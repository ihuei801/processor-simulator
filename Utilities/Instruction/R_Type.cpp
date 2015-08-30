#include "R_Type.h"
#include "Instruction.h"

void R_Type::setRS( unsigned int src )
{
	_rs = src;
}

unsigned int R_Type::getRS( void ) const
{
	return _rs;
}

void R_Type::setRT( unsigned int src )
{
	_rt = src;
}

unsigned int R_Type::getRT( void ) const
{
	return _rt;
}

void R_Type::setRD( unsigned int src) 
{
	_rd = src;
}

unsigned int R_Type::getRD( void ) const
{
	return _rd;
}

void R_Type::setShamt( unsigned int src )
{
	_shamt = src;
}

unsigned int R_Type::getShamt( void ) const
{
	return _shamt;
}

void R_Type::setFunct( unsigned int src )
{
	_funct = src;
}

unsigned int R_Type::getFunct( void ) const
{
	return _funct;
}

int R_Type::type_name( void ) const
{
	return R_TYPE;
}
