#include  "Control_Operators.h"
#include "../../Proj03.h"
#include <iostream>
using namespace std;
Control_Operators::Control_Operators(  Registers* reg,unsigned int* pc )
{
    _Reg = reg;
    _PC = pc;
}

//Branch Instructions
  //I-type
void Control_Operators::beq( int rs, int rt, const int branchaddr )
{

    if( (_Reg->Get(rs)) == (_Reg->Get(rt)))
        *_PC = *_PC + 4 + 4*branchaddr;
	else
		*_PC += 4;
}

void Control_Operators::bne(  int rs, int rt, const int branchaddr)
{

    if( (_Reg->Get(rs)) != (_Reg->Get(rt)))
        *_PC = *_PC + 4 + 4*branchaddr;
	else
		*_PC += 4;
}

//Jump Instructions
  //J-type
void Control_Operators::j( const unsigned int jumpaddr)
{
	//11110000 00000000 00000000 00000000
	unsigned int mask = 4026531840;
	unsigned int tmp;
    
	tmp = mask & (*_PC+4);
	
	unsigned int shiftaddr = jumpaddr << 2 ;
    
	*_PC = tmp | shiftaddr;
}

void Control_Operators::jal( const unsigned int jumpaddr )
{
	int t = *_PC + 4;
	_Reg->Set($ra, t);
	
	//11110000 00000000 00000000 00000000
	unsigned int mask = 4026531840;
	unsigned int tmp;

	tmp = mask & (*_PC+4);
	
	unsigned int shiftaddr = jumpaddr << 2 ;
	
	*_PC = tmp | shiftaddr;	
}
//R-type
void Control_Operators::jr( int rs)
{
	*_PC = _Reg->Get(rs);
   
}
