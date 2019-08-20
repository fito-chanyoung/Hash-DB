
#include <cstdint>
#include <vector>
#include "block.h"

using namespace std;

class Blockchain {
public:
    Blockchain();
    void AddBlock(Block bNew);
    Block _GetLastBlock() const;
    //void GetData(int num,string to, string key, string iv);
    void GetData(int num);
    Block Getnode(int num);
    //void StableCheck(string key, string iv);
    void StableCheck();
    void is_Stable(bool flag);
    bool return_isGen();

    int Get_Size();
private:
    bool isGen;
    bool isChainStable;
    vector<Block> _vChain;
};
