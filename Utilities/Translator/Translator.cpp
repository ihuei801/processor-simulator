#include "Translator.h"
#include "../Instruction/R_Type.h"
#include "../Instruction/I_Type.h"
#include "../Instruction/J_Type.h"
#include "../../Proj03.h"

Translator::Translator() 
{
	_ins = NULL;
}

Translator::~Translator()	//Destructor
{
	delete _ins;	//release memory space
}

void Translator::OP_Code_Filter( void )
{
	//1111 1100 0000 0000 0000 0000 0000 0000 = 4227858432-> mask for OP Code (6-bit)
	unsigned int mask = 4227858432;	
	
	_OPCode = _rawData & mask;
	_OPCode = _OPCode >> 26;
}

//load raw instruction in the 'unsgined int' type
void Translator::LoadRawIns( unsigned int src )	
{
	_rawData = src;
	
	OP_Code_Filter();

	if( _ins )	//_ins != NULL
		delete _ins;	//release memory space
 
}

unsigned int Translator::getOPCode( void ) const
{
	return _OPCode;
}

void Translator::R_Filter( void )	//transform into the R-type instruction
{	
	unsigned int rs, rt, rd, shamt, funct;

	OP_Code_Filter();

	//0000 0011 1110 0000 0000 0000 0000 0000 = 65011712-> mask for RS (5-bit)
	unsigned int mask = 65011712;
	rs = _rawData & mask;
	rs = rs >> 21;

	//0000 0000 0001 1111 0000 0000 0000 0000 = 2031616-> mask for RT (5-bit)
	mask = 2031616;
	rt = _rawData & mask;
	rt = rt >> 16;

	//0000 0000 0000 0000 1111 1000 0000 0000 = 63488-> mask for RD (5-bit)
	mask = 63488;
	rd = _rawData & mask;
	rd = rd >> 11;

	//0000 0000 0000 0000 0000 0111 1100 0000 = 1984-> mask for Shamt (5-bit)
	mask = 1984;
	shamt = _rawData & mask;
	shamt = shamt >> 6;

	//0000 0000 0000 0000 0000 0000 0011 1111 = 63-> mask for Funct (6-bit)
	mask = 63;
	funct = _rawData & mask;
	
	//Set up the R-type Instruction
	_ins = new R_Type();
	R_Type* RIns = dynamic_cast<R_Type*>( _ins );
	RIns->setOPCode(getOPCode());
	RIns->setRS(rs);
	RIns->setRT(rt);
	RIns->setRD(rd);
	RIns->setShamt(shamt);
	RIns->setFunct(funct);
}

void Translator::I_Filter( void )		//I-Type Instruction Filter
{
	unsigned int rs, rt, immediate;

	OP_Code_Filter();

	//0000 0011 1110 0000 0000 0000 0000 0000 = 65011712-> mask for RS (5-bit)
	unsigned int mask = 65011712;
	rs = _rawData & mask;
	rs = rs >> 21;

	//0000 0000 0001 1111 0000 0000 0000 0000 = 2031616-> mask for RT (5-bit)
	mask = 2031616;
	rt = _rawData & mask;
	rt = rt >> 16;

	//0000 0000 0000 0000 1111 1111 1111 1111 = 65535-> mask for Immediate  (16-bit)
	mask = 65535;
	immediate = _rawData & mask;

	//Set up the I-type Instruction
	_ins = new I_Type();
	I_Type* IIns = dynamic_cast<I_Type*>( _ins );
	IIns->setOPCode(getOPCode());
	IIns->setRS(rs);
	IIns->setRT(rt);
	IIns->setImmediate(immediate);
}

void Translator::J_Filter( void )		//J-Type Instruction Filter
{
	unsigned int address;

	OP_Code_Filter();

	//0000 0011 1111 1111 1111 1111 1111 1111 = 67108863-> mask for RS (5-bit)
	unsigned int mask = 67108863;
	address = _rawData & mask;

	//Set up the J-type Instruction
	_ins = new J_Type();
	J_Type* JIns = dynamic_cast<J_Type*>( _ins );
	JIns->setOPCode(getOPCode());
	JIns->setAddress(address);
}

Instruction* Translator::getIns( int kind )		//get the instruction which has been transformed
{
	switch( kind ) {
		case R_TYPE :
			R_Filter();
			break;
		case I_TYPE :
			I_Filter();
			break;
		case J_TYPE :
			J_Filter();
			break;
		default :
			cerr << "There are some errors in the Translator::getIns() !! " << endl;
			exit(1);
	}
	
	return _ins;
}
