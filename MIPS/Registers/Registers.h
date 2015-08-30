//Registers of MIPS
#ifndef REGISTER_H
#define REGISTER_H

class Registers {

private :
	int* _regs; //Array of registers
	int _size;	//Size of the register of MPIS

public :
	Registers( int = 32 );	//Default Constructor
	~Registers();			//Destructor

	void Set( int idx, int val );	//set the data into registers
	int Get( int idx ) const;		//get the value of the specific register
	int size( void ) const;			//get the value of size
};

#endif