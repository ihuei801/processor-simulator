#include "TLB.h"
#include <iostream>
using namespace std;
TLB::TLB():TLBtable(NULL)
{
    setHitCount(0);
    setMissCount(0);
    setNumofEntry(0);

}

TLB::~TLB(){
    delete [] TLBtable;
}
void TLB::Initialize(int numofentry)
{
    setNumofEntry(numofentry);
    TLBtable = new TLBentry[numofentry] ;
    for(int i=0;i<numofentry;i++){
        TLBtable[i].valid = false;
        TLBtable[i].dirty = false;
        TLBtable[i].count=0;
        TLBtable[i].virpagenum = 0;
        TLBtable[i].phypagenum = 0;
    }
}

void TLB::Incrementcount(void)
{
    //increment count
    for(int i=0;i<getNumofEntry();i++){
        if(TLBtable[i].valid==true){
            TLBtable[i].count++;
        }
    }
}

bool TLB::translateaddr(unsigned int& phypagenum, unsigned int searvirpagenum)
{
    
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(TLBtable[i].valid==true){
            if(TLBtable[i].virpagenum==searvirpagenum){
                phypagenum = TLBtable[i].phypagenum;
                TLBtable[i].count=0;
                return true;
            }
        }
    }
    return false;

}

void TLB::updateReplace(unsigned int phypage)
{
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(TLBtable[i].valid==true && TLBtable[i].phypagenum==phypage){
            TLBtable[i].valid = false;
            TLBtable[i].dirty = false;
            TLBtable[i].phypagenum = 0;
            TLBtable[i].virpagenum = 0;
            TLBtable[i].count = 0;
            return;
            
        }
    }
    
}
void TLB::updateEntry(unsigned int newvirpagenum, unsigned int newphypagenum, bool newdirty)
{
    
    int i;
    int maxcount=-1;
    int entrytoreplace;
    
    for(i=0;i<getNumofEntry();i++){
       
        if(TLBtable[i].valid==false){
            TLBtable[i].valid = true;
            TLBtable[i].dirty = newdirty;
            TLBtable[i].count = 0;
            TLBtable[i].virpagenum = newvirpagenum;
            TLBtable[i].phypagenum = newphypagenum;
            return;
        }
        else{
            if(TLBtable[i].count>maxcount){
                entrytoreplace = i;
                maxcount = TLBtable[i].count;
            }
        }
    }
    TLBtable[entrytoreplace].valid = true;
    TLBtable[entrytoreplace].dirty = newdirty;
    TLBtable[entrytoreplace].count = 0;
    TLBtable[entrytoreplace].virpagenum = newvirpagenum;
    TLBtable[entrytoreplace].phypagenum = newphypagenum;
}
void TLB::printTLB(void){
    int i;
    int j;
    for(i=0;i<getNumofEntry();i++){
        
        if(TLBtable[i].valid){
            cout<<"Entry#: "<<i;
            cout<<" virpagenum= "<<TLBtable[i].virpagenum<<" phypagenum= "<<TLBtable[i].phypagenum;
            if(TLBtable[i].dirty==true){
                cout<<" dirty ";
            }
            cout<<endl;
        }
    }
    cout<<"hit #= "<<getHitCount()<<endl;
    cout<<"miss #= "<<getMissCount()<<endl;
}

void TLB::writememdirty(int phypagenum){
    int i;
    for(i=0;i<getNumofEntry();i++){
        if(TLBtable[i].valid==true && TLBtable[i].phypagenum==phypagenum){
            TLBtable[i].dirty=true;
            break;
        }
    }
    
}

//basic set and get
void TLB::setHitCount(int num)
{
    hit = num;
}
int TLB::getHitCount(void)
{
    return hit;
}
void TLB::setMissCount(int num)
{
    miss = num;
    
}
int TLB::getMissCount(void)
{
    return miss;
}
void TLB::setNumofEntry(int num)
{
    _numofentry = num;
}
int TLB::getNumofEntry(void)
{
    return _numofentry;
}
void TLB::setDirty(int phypagenum)
{
    for(int i=0;i<getNumofEntry();i++){
        if(TLBtable[i].phypagenum==phypagenum){
            TLBtable[i].dirty = true;
            return;
        }
    }

}
bool TLB::getDirty(int phypagenum)
{
    for(int i=0;i<getNumofEntry();i++){
        if(TLBtable[i].phypagenum==phypagenum){
            if(TLBtable[i].dirty == true){
                 return true;
            }
        }
    }
    return false;

}