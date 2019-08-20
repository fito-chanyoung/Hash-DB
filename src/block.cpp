#include "block.h"
#include <stdlib.h>
#include <string.h>
#include <fstream>

unsigned char digest[SHA512_DIGEST_LENGTH];
Block::Block(){
	_nIndex= 0;
	transnum=0;
	//_sData=new string[transection_num];
	time_t _tTime = time(nullptr);

	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&_tTime));
	string Time(buff);
	tTime=Time;

	sPrevHash="fffffffff";
}

Block::Block(uint32_t nIndexIn, vector<string> sDataIn, int transection_num){
	_nIndex= nIndexIn;
	transnum=transection_num;
	//_sData=new string[transection_num];
	_sData=sDataIn;
	time_t _tTime = time(nullptr);

	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&_tTime));
	string Time(buff);
	tTime=Time;

	sPrevHash="fffffffff";
}
string Block::GetHash(){
	return _sHash;
}

string Block::CalMerkle(int init, int end){
//cout<<init<<" "<<end<<endl;
	string tmp=_sData[init];
	int size=tmp.size();
	if(init == end){
		//cout<<init<<" matched "<<end<<" return 1 hash code"<<endl;
		//cout<<_sData[init]<<endl;
/*		char str[size];
			for(int i=0; i<=size;i++)
				str[i]=tmp[i];
		SHA512((unsigned char*)&str, strlen(str), (unsigned char*)&digest);

		char mdString[SHA512_DIGEST_LENGTH*2+1];
		for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		     sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

		//cout<<"i hash made "<<mdString<<endl;
		return mdString;*/
			return SHA_512(tmp);
	}
	else{
		int fInit=(end+init)/2;
		int send =(end+init)/2-1;

		if((init+1) == end){
			fInit = end;
			send = init;
		}
		string tmp = CalMerkle(init,send)+CalMerkle(fInit,end);
		//cout<<"tmp "<<tmp<<endl;
		string str;
		for(int i=0; i<=256;i+= 2)
			str.push_back(tmp[i]);
		/*SHA512((unsigned char*)&str, strlen(str), (unsigned char*)&digest);

		char mdString[SHA512_DIGEST_LENGTH*2+1];
		for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		     sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
		//cout<<"return merged hash "<<mdString<<endl<<endl;
		return mdString;*/
		return SHA_512(str);
	}


}

string Block::GetMerkle(){
	if(transnum >=1){
		int tmpIndex= transnum-1;
	return CalMerkle(0,tmpIndex);
	}
	else
		return SHA_512("NULL");
	//cout<<"ret : "<<result<<endl;

}

void Block::BlockGen() {
	_merkleHash=GetMerkle();
	//cout<<"merkleroot HASH "<<_merkleHash<<endl;

	string tmp="";
	//tmp += _tTime;
	tmp += sPrevHash;
	//tmp +=_nIndex;
	tmp += _merkleHash;

	char str[tmp.size()];
		for(int i=0; i<tmp.size();i++)
			str[i]=tmp[i];
	SHA512((unsigned char*)&str, strlen(str), (unsigned char*)&digest);

    char mdString[SHA512_DIGEST_LENGTH*2+1];


    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    //cout<<"hash code_block.cpp "<<mdString<<endl;
	_sHash =mdString;
}
void Block::cData(vector<string> str){
	_sData=str;
}

string Block::CMerkle(){
	return _merkleHash;
}
void Block::CoutString(){
	cout<<_sData[0]<<endl;
}
string Block::returntTime(){
	return tTime;
}

void Block::setTime(string time){
	tTime=time;
}

void Block::reset(){
	_sData.clear();
}

vector<string> Block::get_Data(){
	return _sData;
}

void Block::push_back(string str){
	_sData.push_back(str);
	transnum++;
}

void Block::reHashing(){
	_merkleHash=GetMerkle();

	string tmp="";
	//tmp += _tTime;
	tmp += sPrevHash;
	//tmp +=_nIndex;
	tmp += _merkleHash;

	_sHash = SHA_512(tmp);
}
/*
string Block::GetreHash(){
	_merkleHash=GetMerkle();

	string tmp="";
	//tmp += _tTime;
	tmp += sPrevHash;
	tmp +=_nIndex;
	tmp += _merkleHash;

	return SHA_512(tmp);
}

void Block::reorganize(string key, string iv){
	char buff[20];
	strftime(buff, 20, "%Y.%m.%d_%H:%M:%S", localtime(&_tTime));
	string Time(buff);

	ifstream in;

	decrypt_file(Time,key,iv);
	in.open("workspace.txt");

	string tmp;
	vector<string> data;
	while(getline(in,tmp))
		data.push_back(tmp);

	system("rm workspace.txt");
	cData(data);
}
*/
