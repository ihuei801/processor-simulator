//Instruction Set Architecture -- Control Operators
#ifndef CON_H
#define CON_H
#include "../Registers/Registers.h"

class Control_Operators
{
private :
    Registers* _Reg;
    unsigned int* _PC;
public :
	Control_Operators( Registers*, unsigned int* );
//Branch Instructions
   //I-type
	void beq( int rs, int rt, const int branchaddr );
	void bne(  int rs, int rt, const int branchaddr);
	
//Jump Instructions
    //J-type
	void j( const unsigned int jumpaddr);
	void jal( const unsigned int jumpaddr );
	//R-type
	void jr( int rs);
};

#endif