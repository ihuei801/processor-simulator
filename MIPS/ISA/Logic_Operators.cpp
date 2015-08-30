#include  "Logic_Operators.h"
#include <cmath>

Logic_Operators::Logic_Operators( Registers* reg )
{
    _Reg = reg;
}

//Logic Instructions
//R-type
void Logic_Operators::and_t( int rs, int rt, int rd)
{
	int ans = (_Reg->Get(rs) ) & ( _Reg->Get(rt) );
	_Reg->Set(rd, ans);
}

void  Logic_Operators::or_t( int rs, int rt, int rd)
{
	 int ans = (_Reg->Get(rs) )| ( _Reg->Get(rt) );
	_Reg->Set(rd, ans);
}

void  Logic_Operators::nor( int rs, int rt, int rd )
{	
	 int ans = ~ ((_Reg->Get(rs) )| ( _Reg->Get(rt) ));
	_Reg->Set(rd, ans);
}

void  Logic_Operators::xor_t( int rs, int rt, int rd )
{
	 int ans = (_Reg->Get(rs) )^ ( _Reg->Get(rt) );
	_Reg->Set(rd, ans);
}

void  Logic_Operators::nand(int rs, int rt, int rd)
{
	int ans = ~ ((_Reg->Get(rs) )& ( _Reg->Get(rt) ));
	_Reg->Set(rd, ans);
}

void  Logic_Operators::sll( int rt, int rd, const int shamt)
{
	int mov = (_Reg->Get(rt)) << shamt;
	_Reg->Set(rd, mov);
	
}

void  Logic_Operators::srl(  int rt, int rd, const int shamt )
{
	int result = _Reg->Get(rt);
	unsigned int mask = 0x7FFFFFFF;

	if( shamt == 0 ) 
		_Reg->Set(rd, result);
	else {
		result = result >> shamt;
			
		for(int i=0; i<shamt-1;i++)
			mask = mask >> 1;

		result = result & mask;
		_Reg->Set(rd, result);
	}
}

void  Logic_Operators::sra( int rt, int rd, const int shamt  )
{
	int mov = (_Reg->Get(rt)) >> shamt ;
	_Reg->Set(rd, mov);
	
}

//I-type
void  Logic_Operators::andi( int rs, int rt, const unsigned int c )
{
	int ans = (_Reg->Get(rs) ) & c ;
	_Reg->Set(rt, ans);
}

void  Logic_Operators::ori( int rs, int rt, const unsigned int c)
{
	int ans = (_Reg->Get(rs) )| c;
	_Reg->Set(rt, ans);
}

void  Logic_Operators::nori( int rs, int rt, unsigned int c)
{
	int ans = ~ ((_Reg->Get(rs) )| c);
	_Reg->Set(rt, ans);
}

//Comparision Instructions
//R-type
void  Logic_Operators::slt( int rs, int rt, int rd )
{
	int ans = ((_Reg->Get(rs) )< ( _Reg->Get(rt))) ? 1:0;
	_Reg->Set(rd, ans);
}

//I-type	
void  Logic_Operators::slti( int rs, int rt, const int c )
{
	int ans = ((_Reg->Get(rs) )< c) ? 1:0;
	_Reg->Set(rt, ans);
}

