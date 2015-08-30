#include "Arithmetic_Operators.h"
Arithmetic_Operators::Arithmetic_Operators( Registers* reg )
{
    _Reg = reg;
}

// R-type
void Arithmetic_Operators::add( int rs, int rt, int rd)
{
	int sum = _Reg->Get(rs) + _Reg->Get(rt);	
	_Reg->Set(rd, sum);
}

void Arithmetic_Operators::sub(  int rs, int rt, int rd  )
{
	int ans = _Reg->Get(rs) + ( - (_Reg->Get(rt)) );
	_Reg->Set(rd, ans);
}

//I-type
void Arithmetic_Operators::addi( int rs, int rt, int SignExtImm )
{
	int sum = _Reg->Get(rs) + SignExtImm ;
	_Reg->Set(rt, sum);
}
