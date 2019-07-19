/*
 * blockchain.cpp
 *
 *  Created on: 2019. 1. 22.
 *      Author: song
 *
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

void Blockchain::GetData(int num){
	int tmp=num;
	string path = "data.txt";
	while(true){
		_vChain.at(tmp).writeData(path);
		if(_vChain[tmp].GetHash() == _vChain.back().GetHash())
			break;
		tmp++;
	}
}
Block Blockchain::Getnode(int num){
	return _vChain[num];
}
void Blockchain::isStable(bool flag){
	isChainStable=flag;
}
bool Blockchain::return_isGen(){
	return isGen;
}*/
