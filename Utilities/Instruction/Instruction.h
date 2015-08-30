//Instruction Object
#ifndef INS_H
#define INS_H

#define TYPE_NUM 3
enum Type { R_TYPE, I_TYPE, J_TYPE };

class Instruction {		//The Base Class of any-type instructions
protected :
	unsigned int _OPCode : 6;

public :
	void setOPCode( unsigned int );
	unsigned int getOPCode( void ) const;
	virtual int type_name( void ) const = 0;		//pure virtual function
};

#endif