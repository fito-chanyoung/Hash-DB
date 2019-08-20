/*
 * blockchain.cpp
 *
 *  Created on: 2019. 1. 22.
 *      Author: song
 */
#include "blockchain.h"
#include <unistd.h>

Blockchain::Blockchain() {
	isGen=true;
	isChainStable =true;
}


void Blockchain::AddBlock(Block bNew) {
	if(!isGen){
		 bNew.sPrevHash = _GetLastBlock().GetHash();
		//StableCheck();
	}
	else
		bNew.sPrevHash="fffffffffffffff";
	if(isChainStable){
		bNew.BlockGen();
		_vChain.push_back(bNew);
	}
	isGen=false;
}

Block Blockchain::_GetLastBlock() const{
	return _vChain.back();
}
/*
void Blockchain::GetData(int num,string to, string key, string iv){
	int tmp=num;
	string path = "data.txt";
	while(true){
		_vChain.at(tmp).writeData(path,to,key,iv);
		if(_vChain[tmp].GetHash() == _vChain.back().GetHash())
			break;
		tmp++;
	}
}

void Blockchain::StableCheck(string key, string iv){

	for(int i=0;i<_vChain.size();i++){
		if(_vChain[i].GetHash() != _vChain[i].GetreHash()){
			is_Stable(false);

			cout<<"ALTER node "<<i<<" broken"<<endl;

			_vChain[i].reorganize(key,iv);
		}
	}
}
*/
Block Blockchain::Getnode(int num){
	return _vChain[num];
}

void Blockchain::is_Stable(bool flag){
	isChainStable=flag;
}

bool Blockchain::return_isGen(){
	return isGen;
}

int Blockchain::Get_Size(){
	return _vChain.size();
}


