//Registers of MIPS

#include "../../Proj03.h"
#include "../Registers/Registers.h"

Registers::Registers( int num ) 
{
	_size = num;
	_regs = new int[num];

	for( int i=0; i<num; i++ )
		_regs[i] = 0;
}

Registers::~Registers()		//Destructor
{
	delete [] _regs;
}

void Registers::Set( int idx, int val )		//set the data into registers
{
	if( idx >= _size || idx < 0 ) {
		cerr << "Registers::Set -> The size is out of range!!! " << endl;
		exit(1);
	}

	if( idx != 0 ) 
		_regs[idx] = val;
}

int Registers::Get( int idx ) const		//get the value of the specific register
{
	return _regs[idx];
}

int Registers::size( void ) const		//get the value of size
{
	return _size;
}