#include "MIPS.h"
#include "../Proj03.h"
#include "ISA/Arithmetic_Operators.h"
#include "ISA/Control_Operators.h"
#include "ISA/Data_Operators.h"
#include "ISA/Logic_Operators.h"
#include "../Utilities/Instruction/Instruction.h"
#include "../Utilities/Instruction/R_Type.h"
#include "../Utilities/Instruction/I_Type.h"
#include "../Utilities/Instruction/J_Type.h"
#include <math.h>
using namespace std;
void MIPS::Init( int Imemsize, int Dmemsize, int Imempagesize, int Dmempagesize, int Icachesize, int Icacheblocksize, int Icache_setassociativity, int Dcachesize, int Dcacheblocksize, int Dcache_setassociativity )
{	

    _IMemory.Initialize(Imemsize,Imempagesize);
    _IPageTable.Initialize(_IDisk.getSize()/Imempagesize);
    //cout<<"I PTE num of entry= "<<_IDisk.getSize()/Imempagesize;
    _ITLB.Initialize((_IDisk.getSize()/Imempagesize)/4);
    //cout<<"I TLB num of entry= "<<(_IDisk.getSize()/Imempagesize)/4;
    _DMemory.Initialize(Dmemsize,Dmempagesize);
    _DPageTable.Initialize(_DDisk.getSize()/Dmempagesize);
     //cout<<"D PTE num of entry= "<<_DDisk.getSize()/Dmempagesize;
    _DTLB.Initialize((_DDisk.getSize()/Dmempagesize)/4);
     //cout<<"D TLB num of entry= "<<(_IDisk.getSize()/Imempagesize)/4;
    _ICache.Initialize(Icachesize, Icacheblocksize, Icache_setassociativity);
    _DCache.Initialize(Dcachesize, Dcacheblocksize, Dcache_setassociativity);
    
}
void MIPS::LoadIns(void)	//Load Instructions from external resource
{
    unsigned int tmp;
    ifstream inIns( "iimage.bin", ios::in | ios::binary );
    
    if( !inIns ) {
        cerr << "There are some errors in file processing -- 'iimage.bin' " << endl;
        exit(1);
    }
    
    //read the initial number of PC
    inIns.read( reinterpret_cast< char * > (&tmp), sizeof(tmp) );
    setPC( EndianTransform(tmp) );
    
    
    //read the number of Instructions
    inIns.read( reinterpret_cast< char * > (&tmp), sizeof(tmp) );
    setInsNum( EndianTransform(tmp) );
    
    unsigned int counter = 0;
    //read instrucions from external resource & set up I-Disk
    for( ; counter<getInsNum()*4; counter+=4 ) {
        inIns.read( reinterpret_cast< char * > (&tmp), sizeof(tmp) );
        _IDisk.setIns(_PC+counter, EndianTransform(tmp) );
    }
    inIns.close();
}
unsigned int MIPS::EndianTransform( unsigned int src )	//transform Little-Endian into Big-Endian
{
    unsigned int tmp;	//saving the temporary data
    
    //mask : 00000000 00000000 00000000 11111111
    unsigned int mask = 255;
    unsigned int result = src & mask;
    result = result << 8;		//left-shift
    
    src = src >> 8;
    tmp = src & mask;		//Bit Filer
    result = result | tmp;	//combine data
    result = result << 8;		//left-shift
    
    src = src >> 8;
    tmp = src & mask;		//Bit Filer
    result = result | tmp;	//combine data
    result = result << 8;		//left-shift
    
    src = src >> 8;
    tmp = src & mask;		//Bit Filer
    result = result | tmp;	//combine data
    
    return result;
}
void MIPS::LoadData( void ) //Initialize MIPS ($SP, PC.....)
{
    int tmp;
    ifstream inData( "dimage.bin", ios::in | ios::binary );
    
    if( !inData ) {
        cerr << "There are some errors in file processing -- 'dimage.bin' " << endl;
        exit(1);
    }
    
    //read initial value of $sp
    inData.read( reinterpret_cast< char * > (&tmp), sizeof(tmp) );
    _Reg.Set( $sp, EndianTransform(tmp) );
    //read the number of data
    inData.read( reinterpret_cast< char * > (&tmp), sizeof(tmp) );
    setDataNum( EndianTransform(tmp) );
    unsigned int counter = 0;
    //read data from external resource & set up D-Memory
    for( ; counter<getDataNum()*4; counter+=4 ) {
        inData.read( reinterpret_cast< char * > (&tmp), sizeof(tmp) );
        _DDisk.setWord( counter, EndianTransform(tmp) );
    }
    inData.close();
}

void MIPS::Run(void)		//Run MIPS
{
    unsigned int virpagenum;
    unsigned int phypagenum;
    unsigned int pageoffset;
    unsigned int phyaddr;
    
    unsigned int ins;
    
    Translator transMachine;
    ofstream outfile( "snapshot.rpt", ios::out );
    ofstream outreport( "report.rpt", ios::out );
    
    //First Round
    setCycle( 0 );
    Print_Registers( outfile );
    
    while( true ) {
        //cout<<"In Cycle: "<<setbase(10)<<getCycle()<<endl;
        //translation virtual address to physical address
        virpagenum = getPC() / _IMemory.getPageSize();
        pageoffset = getPC() % _IMemory.getPageSize();
        _ITLB.Incrementcount();
        _IMemory.Incrementcount();
        if(!_ITLB.translateaddr( phypagenum, virpagenum )){ //ITLB miss
            _ITLB.setMissCount(_ITLB.getMissCount()+1);
            if(!_IPageTable.translateaddr( _ITLB , virpagenum)){ //not in memory, page fault
                ILoadPageDisktoMem( virpagenum * _IMemory.getPageSize() ); //use virtual pagenum to load the page from disk and update pagetable, TLB
            }
            if(!_ITLB.translateaddr(phypagenum, virpagenum)){
                cerr<<"somthing wrong when dealing with ITLB miss"<<endl;
                system("pause");
                exit(1);
            }
        }
        else{
            _ITLB.setHitCount(_ITLB.getHitCount()+1);
        }
        phyaddr = (phypagenum * _IMemory.getPageSize() ) + pageoffset;
        
        //get instruction in ICache
        _ICache.Incrementcount(phyaddr);
        if( !_ICache.getIns( ins, phyaddr ) ){//not in Icache
            _ICache.setMissCount(_ICache.getMissCount()+1);
            ILoadBlockMemtoCache( phyaddr );
            if(!_ICache.getIns( ins, phyaddr )){
                cerr<<"somthing wrong when dealing with ICache miss"<<endl;
                system("pause");
                exit(1);
            }
        }
        else{
            _ICache.setHitCount(_ICache.getHitCount()+1);
        }
        transMachine.LoadRawIns( ins );
        
        if( transMachine.getOPCode() == HALT )
            break;
        
        if( doAction( transMachine ) )	//execute the specific instruction
            setPC( getPC()+4 );
        
        setCycle( getCycle()+1 );
        Print_Registers( outfile );
        //Print_Memhie();
    }
    Print_Report( outreport );
}
void MIPS::ILoadPageDisktoMem( unsigned int virpageaddr )
{
    unsigned int phypagenum = _IMemory.getFreePage(_IPageTable,_ITLB,_ICache); //need to update PTE and ILB if page is replaced
    unsigned int virpagenum = virpageaddr / _IMemory.getPageSize();
    int i;
    
    for(i=0;i<_IMemory.getPageSize();i++){
        _IMemory.setInsByte(phypagenum*_IMemory.getPageSize()+i, _IDisk.getByte(virpageaddr+i));
    }
    _IMemory.setValid(phypagenum, true);
    _IMemory.setCount(phypagenum,0);
    _IPageTable.updateEntry( virpagenum, phypagenum);
    _ITLB.updateEntry( virpagenum, phypagenum, false);
}
void MIPS::ILoadBlockMemtoCache( unsigned int phyaddr )
{
    unsigned int blockaddr = phyaddr / _ICache.getBlockSize();
    unsigned int blocknum = blockaddr % _ICache.getBlockNum();
    unsigned int setnum = blocknum % _ICache.getSetNum();
    unsigned int phypagennum = phyaddr / _IMemory.getPageSize();
    unsigned int tag = blockaddr / _ICache.getSetNum();
    int maxcount=-1;
    int blocktoreplace;
    
    for(int i=0;i<_ICache.getAssociativity();i++){
        if(_ICache.getBlockValid(setnum,i)==false){
            for(int j=0;j<_ICache.getBlockSize();j++){
                _ICache.setBlockData(setnum,i,j, _IMemory.getInsByte( blockaddr*_ICache.getBlockSize()+j ));
            }
            _ICache.setBlockValid(setnum,i,true);
            _ICache.setBlockDirty(setnum,i,false);
            _ICache.setBlockTag(setnum,i,tag);
            _ICache.setBlockCount(setnum,i,0);
            return;
        }
        else{
            if(_ICache.getBlockCount(setnum,i)>maxcount){
                maxcount = _ICache.getBlockCount(setnum,i);
                blocktoreplace = i;
            }
        }
    }
    
    for(int j=0;j<_ICache.getBlockSize();j++){
        _ICache.setBlockData(setnum,blocktoreplace,j, _IMemory.getInsByte( blockaddr*_ICache.getBlockSize()+j ));
    }
    
    _ICache.setBlockValid(setnum,blocktoreplace,true);
    _ICache.setBlockDirty(setnum,blocktoreplace,false);
    _ICache.setBlockTag(setnum,blocktoreplace,tag);
    _ICache.setBlockCount(setnum,blocktoreplace,0);
    
}

bool MIPS::doAction( Translator src )		//Run the specific instruction
{
    Arithmetic_Operators Ari_act(&_Reg);
    Logic_Operators Log_act(&_Reg);
    Data_Operators Dat_act(&_Reg, &_DDisk, &_DMemory, &_DCache, &_DPageTable, &_DTLB);
    Control_Operators Con_act(&_Reg,&_PC);
    
    switch( src.getOPCode() ) {
        case 0x00 :	{
            
            R_Type* Ins = dynamic_cast< R_Type* >( src.getIns( R_TYPE ) );		//change the type of instruction
            switch( Ins->getFunct() ) {
                case 0x20:
                    Ari_act.add( Ins->getRS(), Ins->getRT(),Ins->getRD());
                    break;
                case 0x22:
                    Ari_act.sub(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x24:
                    Log_act.and_t(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x25:
                    Log_act.or_t(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x26:
                    Log_act.xor_t(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x27:
                    Log_act.nor(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x28:
                    Log_act.nand(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x2A:
                    Log_act.slt(Ins->getRS(), Ins->getRT(), Ins->getRD());
                    break;
                case 0x00:
                    Log_act.sll(Ins->getRT(), Ins->getRD(), Ins->getShamt());
                    break;
                case 0x02:
                    Log_act.srl(Ins->getRT(), Ins->getRD(), Ins->getShamt());
                    break;
                case 0x03:
                    Log_act.sra(Ins->getRT(), Ins->getRD(), Ins->getShamt());
                    break;
                case 0x08:
                    Con_act.jr(Ins->getRS());
                    return false;
                    break;
                default :
                    cerr << "There are some errors in the switch 'Funct' !!!" << endl;
                    cerr << "Forb. Funct = " << Ins->getFunct() << endl;
                    system("PAUSE");
                    exit(1);
                    break;
            }
            break;
        }
        case 0x08 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Ari_act.addi(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x23 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.lw(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x21 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.lh(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x25 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.lhu(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x20 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.lb(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x24 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.lbu(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x2B : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.sw(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x29 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.sh(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x28 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.sb(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x0F : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Dat_act.lui( Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x0C : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Log_act.andi(Ins->getRS(), Ins->getRT(),Ins->getUnsignedImmediate());
            break;
        }
        case 0x0D : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Log_act.ori(Ins->getRS(), Ins->getRT(),Ins->getUnsignedImmediate());
            break;
        }
        case 0x0E : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Log_act.nori(Ins->getRS(), Ins->getRT(),Ins->getUnsignedImmediate());
            break;
        }
        case 0x0A : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Log_act.slti(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            break;
        }
        case 0x04 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Con_act.beq(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            return false;
            break;
        }
        case 0x05 : {
            I_Type* Ins = dynamic_cast< I_Type* >( src.getIns( I_TYPE ) );
            Con_act.bne(Ins->getRS(), Ins->getRT(),Ins->getImmediate());
            return false;
            break;
        }
        case 0x02 : {
            J_Type* Ins = dynamic_cast< J_Type* >( src.getIns( J_TYPE ) );
            Con_act.j(Ins->getAddress());
            return false;
            break;
        }
        case 0x03 : {
            J_Type* Ins = dynamic_cast< J_Type* >( src.getIns( J_TYPE ) );
            Con_act.jal(Ins->getAddress());
            return false;
            break;
        }
        default :
            cerr << "There are some errors in the switch 'OP Code' !!!" << endl;
            cout << "OP Code = " << src.getOPCode() << endl;
            system("PAUSE");
            exit(1);
            break;
    }
    
    return true;
}

void MIPS::Print_Registers( ostream &output )	//Print the content of all registers
{
    output << setbase(10) << "cycle " << getCycle() << endl;
    output.setf( ios::uppercase );
    for( int i=0; i<_Reg.size(); i++ ) {
        output.setf( ios::right );
        output << "$" << setw(2) << setbase(10) << setfill('0') << i << ": 0x";
        output.setf( ios::right );
        output << setbase(16) << setw(8) << _Reg.Get(i) << endl;
    }
    
    output << "PC: 0x" << setbase(16) << setw(8) << getPC() << endl;
    output << endl << endl;
}

void MIPS::Print_Report( ostream &output )	//Print the number of hits and misses
{
    output <<  "ICache :" << endl;
    output << "# hits: "<< setbase(10) << _ICache.getHitCount() << endl;
    output << "# misses: "<< setbase(10) << _ICache.getMissCount() << endl;
    output << endl;
    output <<  "DCache :" << endl;
    output << "# hits: "<< setbase(10) << _DCache.getHitCount() << endl;
    output << "# misses: "<< setbase(10) << _DCache.getMissCount() << endl;
    output << endl;
    output <<  "ITLB :" << endl;
    output << "# hits: "<< setbase(10) << _ITLB.getHitCount() << endl;
    output << "# misses: "<< setbase(10) << _ITLB.getMissCount() << endl;
    output << endl;
    output <<  "DTLB :" << endl;
    output << "# hits: "<< setbase(10) << _DTLB.getHitCount() << endl;
    output << "# misses: "<< setbase(10) << _DTLB.getMissCount() << endl;
    output << endl;
    output <<  "IPageTable :" << endl;
    output << "# hits: "<< setbase(10) << _IPageTable.getHitCount() << endl;
    output << "# misses: "<< setbase(10) << _IPageTable.getMissCount() << endl;
    output << endl;
    output <<  "DPageTable :" << endl;
    output << "# hits: "<< setbase(10) << _DPageTable.getHitCount() << endl;
    output << "# misses: "<< setbase(10) << _DPageTable.getMissCount() << endl;
    output << endl;
}
void MIPS::Print_Memhie()
{
    
    cout<<"I PageTable"<<endl;
    _IPageTable.printPageTable();
    cout<<"I TLB"<<endl;
    _ITLB.printTLB();
    //_IDisk.printIDisk();
    cout<<"I Memory"<<endl;
    _IMemory.printIMemory();
    cout<<"I Cache"<<endl;
    _ICache.printCache();
    cout<<"D PageTable"<<endl;
    _DPageTable.printPageTable();
    cout<<"D TLB"<<endl;
    _DTLB.printTLB();
    //_DDisk.printDDisk();
    cout<<"D Memory"<<endl;
    _DMemory.printDMemory();
    cout<<"D Cache"<<endl;
    _DCache.printCache();



}

void MIPS::setCycle( int src )
{
    _Cycle = src;
}

int MIPS::getCycle( void ) const
{
    return _Cycle;
}

void MIPS::setPC( unsigned int src )
{
    _PC = src;
}

unsigned int MIPS::getPC( void ) const
{
    return _PC;
}

void MIPS::setInsNum( unsigned int src )
{
    _InsNum = src;
}

unsigned int MIPS::getInsNum( void ) const
{
    return _InsNum;
}

void MIPS::setDataNum( unsigned int src )
{
    _DataNum = src;
}

unsigned int MIPS::getDataNum( void ) const
{
    return _DataNum;
}
