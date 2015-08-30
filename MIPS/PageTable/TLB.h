#ifndef TLB_H
#define TLB_H
#include <iostream>
using namespace std;
typedef struct TLBentry{
    
    bool valid;
    bool dirty;
    int count;
    unsigned int virpagenum;
    unsigned int phypagenum;
    
}TLBentry;


class TLB {
private :
    int hit;
    int miss;
    int _numofentry;
    TLBentry* TLBtable;

public :
	TLB();
	~TLB();
    //TLB operation
    void Initialize( int numofentry);
    void Incrementcount(void);
    bool translateaddr(unsigned int& phypagenum, unsigned int searvirpagenum); //translate virpagenum to phypagenum, -1:not in TLB
    void updateReplace(unsigned int phypagenum);
    void updateEntry(unsigned int newvirpagenum, unsigned int newphypagenum, bool dirty);
    void printTLB(void);
    void writememdirty(int phypagenum);
    
    //basic set and get
    void setHitCount(int num);
    int getHitCount(void);
    void setMissCount(int num);
    int getMissCount(void);
    void setNumofEntry(int num);
    int getNumofEntry(void);
    void setDirty(int index);
    bool getDirty(int index);
    
};

#endif
