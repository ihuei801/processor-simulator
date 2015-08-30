//Instruction Set Architecture -- Arithmetic Operators
#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "../Registers/Registers.h"


class Arithmetic_Operators{
private :
    Registers* _Reg;
public :
	Arithmetic_Operators( Registers* );
//Arithmetic Instructions
    
	// R-type
	void add( int rs, int rt, int rd );
	void sub( int rs, int rt, int rd );
	//I-type
	void addi( int rs, int rt, int SignExtImm ); //Adds a register and a sign-extended immediate value and stores the result in a register
};

#endif