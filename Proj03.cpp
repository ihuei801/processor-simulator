/*
Computer Architecture Project #3
A single-cycle, processor simulator with memory hierarchy, TLB, virtual page table mechanism
*/

#include "Proj03.h"
#include "MIPS/MIPS.h"

int main( int argc, char *argv[] )
{
	MIPS Simulator;		//Build-up the MIPS-type simulator

	if( argc != 11 )
		Simulator.Init();
	else
		Simulator.Init( atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]),
							atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]),
								atoi(argv[9]), atoi(argv[10]) );		//Build-up the MIPS-type smulator 
	
    Simulator.LoadIns();	//Load the instructions from external resource
    Simulator.LoadData();	//Load some data from external resource
    
    cout << "Running ... ";
    
    Simulator.Run();		//Run the smulator
    
    cout << "Done" << endl;
    return 0;
    

}
