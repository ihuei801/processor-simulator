#ifndef CACHE_H
#define CACHE_H
using namespace std;

typedef struct Block_stru
{
	//int idx;
	bool valid;
    bool dirty;
	unsigned int tag;
	unsigned char* data;
	int count;  //number of not be used
    
}Block_type;

class Cache {
protected :
    int _totalSize;
	int _blockSize;
	int _associativity;
	int _blockNum;
	int _setNum;	//the number of sets
    int hit;
    int miss;
	Block_type* _Cacheptr;
    Block_type** _setIndex;
	
	//print info
	bool _isDirty;
	unsigned int _accessPos, _setPos;

	

public :
    Cache(){};
    ~Cache(){};
    void Initialize( int size, int blocksize, int setassociativity );
    void Incrementcount(unsigned int phyaddr);
    void printCache(void);
    //ICache operations
    bool getIns(unsigned int &ins, unsigned int phyaddr);
    //DCache operations
    bool getData(unsigned int &data, unsigned int phyaddr, int byte, int sign);
    bool storeData(unsigned int phyaddr, unsigned int data, int option);
    
    //basic get/set functions
    void setTotalSize( int );
    int getTotalSize( void ) const;
    void setBlockSize( int );
    int getBlockSize( void ) const;
    void setAssociativity( int );
    int getAssociativity( void ) const;
    void setBlockNum( int );
    int getBlockNum( void ) const;
	void setSetNum( unsigned int );
	int getSetNum( void ) const;
	void setHitCount( unsigned int );
    int getHitCount( void ) const;
	void setMissCount( unsigned int );
	int getMissCount( void ) const;
    void setBlockValid(int setnum, int blocknum , bool valid);
    bool getBlockValid(int setnum, int blocknum);
    void setBlockDirty(int setnum, int blocknum , bool dirty);
    bool getBlockDirty(int setnum, int blocknum);
    void setBlockTag(int setnum, int blocknum , unsigned int tag);
    unsigned int getBlockTag(int setnum, int blocknum);
    void setBlockCount(int setnum, int blocknum , int count);
    int getBlockCount(int setnum, int blocknum);
    void setBlockData(int setnum, int blocknum, int byte, unsigned char data);
    unsigned char getBlockData(int setnum, int blocknum, int byte);
    
};

#endif