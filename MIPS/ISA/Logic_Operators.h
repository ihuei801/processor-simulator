//Instruction Set Architecture -- Logic_Operators
#ifndef LOGIC_H
#define LOGIC_H
#include "../Registers/Registers.h"

class Logic_Operators{
private :
    Registers* _Reg;
public :
	Logic_Operators( Registers* );
//Logic Instructions
  //R-type
	void and_t( int rs, int rt, int rd);
	void or_t( int rs, int rt, int rd);
	void nor( int rs, int rt, int rd );
	void xor_t( int rs, int rt, int rd );
	void nand( int rs, int rt, int rd );
	void sll( int rt, int rd, const int shamt );
	void srl( int rt, int rd, const int shamt );
	void sra( int rt, int rd, const int shamt );

  //I-type
	void andi( int rs, int rt, const unsigned int c );
	void ori( int rs, int rt, const unsigned int c);
	void nori( int rs, int rt, const unsigned int c);

//Comparision Instructions
  //R-type
	void slt( int rs, int rt, int rd );
  //I-type	
	void slti( int rs, int rt, const int c );

};

#endif
