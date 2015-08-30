// Transform Instruction type --- Instruction -> ISAs
#ifndef TRANS_H
#define TRANS_H

#include "../Instruction/Instruction.h"

class Translator {
private :
    
	unsigned int _OPCode;
	Instruction* _ins;
	unsigned int _rawData;
	//transform instruction type from 'unsigned int' into 'Instruction' type
	void OP_Code_Filter( void );	//OP Code Filter
 
	void R_Filter( void );		//R-Type Instruction Filter
	void I_Filter( void );		//I-Type Instruction Filter
	void J_Filter( void );		//J-Type Instruction Filter

public :
    
	Translator();	//Constructor
	~Translator();	//Destructor
	void LoadRawIns( unsigned int );	//load raw instruction in the 'unsgined int' type
    
	Instruction * getIns( int );		//get the instruction which has been transformed
    
	unsigned int getOPCode( void ) const;	//get OP Code
};

#endif