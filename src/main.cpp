//============================================================================
// Name        : 123.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Todo 4초마다 블럭 재갱신, 스트링 벡터 대신 노드에 데이터 집어넣기
#include <iostream>


#include <string>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include "block.h"


using namespace std;

#define NUM_THREADS 5

#ifndef TARGET_UID
#define TARGET_UID 0
#endif
#ifndef UID_MIN
#define UID_MIN 500
#endif
int fd[2];


class private_chain{

public:
	void aes_StringToCharArr(string str, unsigned char arr[256]);
	int aes_length(string ciphertext);
	string LoadnMerkle(int init,int end, vector<string> array);
	string Loadandcheck(string node);

	void node_pp();
	uint64_t get_nodenum();
	Block get_node();
	string get_Key();
	string get_Iv();

	private_chain(string key, string iv);

	static void *StableCheck(void *);
	static void *PrintHello(void *);
	static void *counter(void *);
	void isStable(bool flag);

	void menu();

	void excute();
	void dataout();
private:
	static void sig_handler(int signo);
	void deep_counter();
	void deep_check();
	void deep_handler();
	void deep_hello();



	pthread_t _counter;
	pthread_t _stabler;
	pthread_t _nodemaker;

	//string lastnode;
	//string last_string;
	string s;

	bool is_stable=true;
	//string transactions[5000];
	uint64_t nodenum=0;
	int line_num=0;
	//int data=0;


	//vector<string> sdata;

	string key;
	string iv;
	Block block;

};

static private_chain *inner_chain;
pthread_t sigid[3];
ifstream in;
ofstream chainlog;
ofstream chain;

pthread_mutex_t mutex_lock;

//string aes_MakeDecryptable(string str);

//int rc4_encrypt(string text,unsigned char* key, unsigned char* iv,unsigned char result[256]);
//string rc4_decrypt(unsigned char* ciphertext,int ciphertext_len,unsigned char *iv, unsigned char* key);
private_chain::private_chain(string key, string iv){
	inner_chain =this;
	string skey=key;
	string siv=iv;

	key = SHA_512(key);

	iv=SHA_512(iv);

	string key2,iv2;

	for(int i =0; i<64;i++) key2.push_back(key[i*2]);
	for(int i =0; i<32;i++) iv2.push_back(iv[i*3]);

	this->key = key2;
	this->iv = iv2;

	//cout<<key<<endl<<iv<<endl;
	nodenum=0;
	int rc=0;
	rc = pthread_create(&_nodemaker, NULL,PrintHello,NULL);
	if (rc!=0) { printf("ERROR; ...%d\n", rc);
	}
	sleep(1);
	rc = pthread_create(&_counter,NULL,counter,NULL);
	if (rc!=0) { printf("ERROR; ...%d\n", rc);;
				}
	sleep(3);
	rc = pthread_create(&_stabler,NULL,StableCheck,NULL);

	menu();

	//pthread_join(_nodemaker,NULL);
	//pthread_join(_stabler,NULL);
	//pthread_join(_counter,NULL);

}

string private_chain::LoadnMerkle(int init,int end, vector<string> array){


	//unsigned char digest[SHA512_DIGEST_LENGTH];
	string tmp=array[init];
	//int size=tmp.size();
	if(init == end){
			//cout<<size<<endl;
			//cout<<init<<" matched "<<end<<" return 1 hash code"<<endl;
			return SHA_512(tmp);
		}
		else{
			int fInit=(end+init)/2;
			int send =(end+init)/2-1;

			if((init+1) == end){
				fInit = end;
				send = init;
			}
			string tmp1 = LoadnMerkle(init,send,array)+LoadnMerkle(fInit,end,array);//crack

			string tmp2;
			for(int i =0 ;i<=256;i += 2)
				tmp2.push_back(tmp1[i]);

			//cout<<"tmp1 "<<tmp1<<endl<<"tmp "<<tmp2<<endl;
			return SHA_512(tmp2);
		}
}

string private_chain::Loadandcheck(string node){
	//cout<<"----------------loadandcheck------------"<<endl<<endl;
	if(nodenum <= 0)
		sleep(4);
	string result;
	ifstream nodefd;
	//ifstream lengfd;
	string tmp;
	vector<string> stringArray;
	int length=0;
	//unsigned char tmpChar[256];
	decrypt_file(node, key, iv);
	nodefd.open("workspace.txt");

	if(!nodefd.is_open()){
		//cout<<"couldn't find node"<<endl;
		sleep(1);
	}

	//int tmpTransnum=0;
	while(!nodefd.eof()){
		getline(nodefd,tmp);
		stringArray.push_back(tmp);
	}
	//encrypt_file(node,key,iv);
	system("rm workspace.txt");

	//lengfd.open(node+"_leng.txt");

	//cout<<"loading "<<node+".txt"<<endl;

		//cout<<"open node ";
		/*getline(nodefd, tmp);
		cout<<node<<endl;
		for(; !nodefd.eof();){
			getline(nodefd,tmp);
			if(tmp == "\0")
				length++;
		}
		length -=2;
		cout<<"length "<<length<<endl;
		length =0;
		nodefd.close();
		nodefd.open(node+".txt");
		nodefd.seekg(0, ios::beg);
		getline(nodefd,tmp);
		lengfd>>length;
		while(!nodefd.eof()){
			if(tmp !="\0"){
				while(true){
					string substr;
					getline(nodefd,substr);
					if(substr == "\0")
						break;
					tmp += 13;
					tmp += substr;
				}
			}
			//cout<<"connected "<<tmp<<endl;
			//stringArray.push_back(tmp);
			if(nodefd.eof() || lengfd.eof())
				break;
			getline(nodefd,tmp);
			lengfd>>length;
		}*/
		//cout<<"done"<<endl;
		//tmpTransnum=0;
		nodefd.close();
		//lengfd.close();
		//cout<<endl<<"currently working at ";
		//for(int i=0; i<stringArray.size();i++)
			//cout<<stringArray[i]<<endl;
		//cout<<endl;
		return LoadnMerkle(0,stringArray.size()-1,stringArray);

}

void private_chain::node_pp(){
	nodenum++;
}

uint64_t private_chain::get_nodenum(){
	return nodenum;
}

Block private_chain::get_node(){
	return block;
}

string private_chain::get_Key(){
	return key;
}

string private_chain::get_Iv(){
	return iv;
}

void private_chain::excute(){
	pthread_cancel(_nodemaker);
	pthread_cancel(_stabler);
	pthread_cancel(_counter);
}

void private_chain::dataout(){
	ifstream in,datain;
	ofstream out;

	pthread_mutex_lock(&mutex_lock);
	in.open("chainlog");
	out.open("data",ios::app |ios::ate);
	string nodename;
	string transaction;

	vector<string> data;
	while(getline(in,nodename)){
		decrypt_file(nodename,key,iv);
		datain.open("workspace.txt");
		while(getline(datain,transaction)){
			cout<<transaction<<endl;
			if(transaction.find("update") != string::npos||transaction.find("create")!= string::npos||transaction.find("insert")!= string::npos||transaction.find("delete")!= string::npos||transaction.find("alter")!= string::npos||transaction.find("drop")!= string::npos||transaction.find("rename")!= string::npos||transaction.find("distinct")!= string::npos){

			}
		}
		string command = "rm workspace.txt";
		system(command.c_str());
	}
	pthread_mutex_unlock(&mutex_lock);
}

void private_chain::isStable(bool flag){
	//cout<<"is stable changed  "<<flag<<endl;

	is_stable=flag;
}

void* private_chain::StableCheck(void *){

	inner_chain->deep_check();

}

void* private_chain::counter(void *)
{
	//cout<<"counter start"<<endl;
	sigset_t newmask;

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
	pthread_sigmask(SIG_BLOCK, &newmask, NULL);

	inner_chain->deep_counter();
}

void private_chain::sig_handler(int signo){ //toDo ->sdata의 대이터 전부를 파일 하나에 넣고 파일 암호화, 클리어
	inner_chain->deep_handler();
	/*chainlog.open("chainlog.txt",ios::app|ios::ate);
	cout<<"signal input"<<endl;
	char buff[20];
	if(nodenum > 0){
	time_t now = mychain._GetLastBlock().returntTime();
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	string Time(buff);
	chainlog<<Time<<'.'<<line_num<<endl;
	chainlog.close();
	}
	cout<<line_num<<endl;
	if(line_num ==0){
		cout<<"no input "<<endl;
		unsigned char result[256];
		encrypt_file("NULL",key,iv);
		transactions[0]=(const char*)result;
		Block newblock(nodenum,transactions,1);
		mychain.AddBlock(newblock);
		chain<<result<<endl;
	}
	else{
		cout<<"input "<<endl;
	Block newblock(nodenum,transactions,line_num);
	mychain.AddBlock(newblock);
	}
	cout<<"final hash "<<mychain._GetLastBlock().GetHash()<<endl;
	for(int i=0; i<=line_num;i++){
		transactions[i]="";
	}
	line_num=0;
	nodenum++;
	cout<<"proceessing2..."<<endl;
	//열려있던 텍스트창을 닫고 새로운 이름으로 새로 연다.
	chain.close();
	//chain_leng.close();
	time_t now = mychain._GetLastBlock().returntTime();
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	string Time(buff);
	chain.open(Time+".txt", ios::out|ios::app);
	//chain_leng.open(Time+"_leng.txt",ios::out|ios::app);
	data++;
	cout<<"end"<<endl;*/
}

void* private_chain::PrintHello(void *)
{
	//cout<<"print hello start"<<endl;
	struct sigaction act;
	sigset_t newmask;

	sigemptyset(&newmask);
	sigaddset(&newmask,SIGUSR1);
	act.sa_handler=sig_handler;

	sigaction(SIGUSR1, &act, NULL);
	pthread_sigmask(SIG_UNBLOCK, &newmask,NULL);

	//sigemptyset(&newmask);
	//sigaddset(&newmask, SIGUSR2);
	//pthread_sigmask(SIG_UNBLOCK, &newmask, NULL);

	//printf("\n%d: Hello World!\n", threadid);
	//pthread_exit(NULL);

	inner_chain->deep_hello();

}

void private_chain::deep_hello(){
	while(true){
			getline(in,s);
			if(in.eof()){
				sleep(1);
				//cout<<"eof"<<endl;
			}
			else //if(s.find("Query",20,5) != -1)
				{
				//cout<<"not eof "<<s<<endl;
				if(s[0]=='2'&& s[1]=='0'){
					//s=aes_MakeDecryptable(s);
					block.push_back(s);
					line_num++;

				}
			}

		}
}

void private_chain::deep_handler(){
	chainlog.open("chainlog",ios::app|ios::ate);
	//cout<<"signal input"<<endl;
	char buff[20];
	//if(nodenum > 0){
		//cout<<"final hash "<<block.GetHash()<<endl;
	//cout<<"proceessing2..."<<endl;
	time_t now = time(nullptr);
	strftime(buff, 20, "%Y.%m.%d_%H:%M:%S", localtime(&now));
	string Time(buff);

	chain.open(Time, ios::ate|ios::app);

	//pthread_mutex_lock(&mutex_lock);
	chainlog<<Time<<endl;
	//chainlog<<Time<<'.'<<line_num<<endl;
	chainlog.close();
	//pthread_mutex_unlock(&mutex_lock);
	//cout<<"line count "<<line_num<<endl;

	if(line_num == 0){
			block.push_back("none");
			block.setTime(Time);
			block.reHashing();
			chain << "none"<<"\n";

	}
	//cout<<"adc "<<block.get_Data()[0]<<endl;
	for(const auto &e :block.get_Data()) chain <<e<<"\n";


	//cout<<"end"<<endl;
	encrypt_file(Time,key,iv);
	//string command=  "rm "+Time;
	//system(command.c_str());

	line_num=0;
	nodenum++;
	//}
	//cout<<line_num<<endl;
	if(line_num ==0){
		//cout<<"no input "<<endl;
		//unsigned char result[256];
		chain <<"none"<<endl;
		chain.close();
		//encrypt_file(,key,iv);
		//transactions[0]=(const char*)result;
		//Block newblock(nodenum,transactions,1);
		//mychain.AddBlock(newblock);
		//chain<<result<<endl;
	}
		else{
			chain.close();
			//cout<<"input "<<endl;
		//Block newblock(nodenum,transactions,line_num);
		//mychain.AddBlock(newblock);
		}
		block.BlockGen();
		//cout<<"final hash "<<block.GetHash()<<endl;
		block.reset();

		line_num=0;
		nodenum++;
		//cout<<"proceessing2..."<<endl;
		//열려있던 텍스트창을 닫고 새로운 이름으로 새로 연다.
		//chain_leng.close();

		//strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
		//string Time(buff);
		//chain.open(Time+".txt", ios::out|ios::app);
		//chain_leng.open(Time+"_leng.txt",ios::out|ios::app);
		//data++;

}

void private_chain::deep_check(){

	ifstream log;
	log.open("chainlog");
	string nodename;
	string pre_hash ="fffffffff";
	string hesh;

	//getline(log,nodename);
	while(true){
		//pthread_mutex_lock(&mutex_lock);
		while(getline(log,nodename)){
			if(nodenum != 0){
				//cout<<"stable check node name "<<nodename<<endl;
				/*
					if(mychain.GetHash() == mychain._GetLastBlock().GetHash()){
						cout<<"clean"<<endl;
						i=0;
					}*/
					if(nodenum != 0){
						if(nodename =="" || log.eof()){
							//cout<<"dead end"<<endl;
							//log.close();
							if(pre_hash != block.GetHash())
								isStable(false);
								//break;
							//log.open("chainlog.txt");
							//log.seekg(0,ios::beg);
								//i=0;
							}
						else{
						//getline(log,nodename);
						}
						hesh =Loadandcheck(nodename);
						//cout<<"Cmerkle "<<mychain.Getnode(0).CMerkle()<<endl;
						//cout<<"Hash "<<mychain.Getnode(0).GetHash()<<endl;
						//cout<<"loadandcheck "<<hesh<<endl;
						if(pre_hash=="fffffffff")
							pre_hash=SHA_512(hesh);
						else{
							string mergedHash = pre_hash+hesh;
							pre_hash=SHA_512(mergedHash);
						}



					}

									//cout<<"node "<<i<<" currupted"<<endl;
				}

			else
				sleep(1);
		}

		if(block.GetMerkle() == hesh){
			//cout<<"getmerkle "<<block.GetMerkle()<<endl;
			//cout<<"hesh "<<hesh<<endl;
			isStable(true);
		}
		//cout<<"test 1 ";
		//log.close();
		//pthread_mutex_unlock(&mutex_lock);
		//cout<<"test 2 ";

		sleep(1);
		log.clear();
		log.seekg(0,log.beg);
		//pthread_mutex_lock(&mutex_lock);
		//log.open("chainlog");
		//cout<<"test3"<<endl;
	}
}

void private_chain::deep_counter(){
	while(true)
		{
			pthread_kill(_nodemaker,SIGUSR1);
			sleep(4);
		//write(fd[1],&data,20);
		//cout<<"datd written";
		}
}

void private_chain::menu(){
	string input;
	while(true){
		cout<<"menu"<<endl;
		cout<<"1 - end program"<<endl;
		cout<<"2 - modifiyer out"<<endl;


		cin >> input;

		switch (atoi(input.c_str())){
			case 1:
				excute();
				break;
			case 2:
				dataout();
				break;
		}
	}
}

int main (char argc, char *argv[])
{/*
	int leng=0;
	unsigned char result[256];
	in.open("/tmp/log.txt");
	while(!in.eof()){
		getline(in,s);
		if(in.eof()){
			sleep(1);
			//cout<<"eof"<<endl;
		}
		else //if(s.find("Query",20,5) != -1)
			{
			//cout<<"not eof "<<s<<endl;
			if(s[0]=='2'&& s[1]=='0'){
				//s=aes_MakeDecryptable(s);
				//encrypt_file(s,key,iv);
				transactions[line_num]=(const char*)result;
				line_num++;
				chain<<result<<endl<<endl;
				//chain_leng<<leng<<" ";
			}
		}
	}*/
/*
	string tmp;
	string array[16];
	int num=0;
	while(!in.eof()){
		getline(in,tmp);
		if(tmp == "\0")
		{}
		else{
			string sub;
			getline(in,sub);
			if(sub != "\0"){
				while(true){
				tmp += 13;
				tmp += sub;
				getline(in,sub);
				if(sub == "\0")
					break;
				}
			}
			array[num]=tmp;
			num++;
		}
	}
	Block myblock(0,array,16);
	myblock.BlockGen();
	Blockchain mychain;
	mychain.AddBlock(myblock);
	string hesh =Loadandcheck("2019-04-02 12:22:09");
	cout<<"---------------------------------------"<<endl;
	cout<<"loadandcheck "<<hesh<<endl;
	cout<<"getmerkle() "<<myblock.GetMerkle()<<endl;//새로 머클해시 계산.
	cout<<myblock.CMerkle()<<endl;//그냥 머클해시 받아옴
	cout<<"---------------------------------------"<<endl;
	string hesh1 =Loadandcheck("2019-04-02 12:22:09");
	cout<<"Cmerkle "<<mychain.Getnode(0).CMerkle()<<endl;
	cout<<"Hash "<<mychain.Getnode(0).GetHash()<<endl;
	cout<<"loadandcheck "<<hesh1<<endl;
	in.close();*/
	//cout<<Loadandcheck("2019-03-04 13:46:32")<<endl;

	/*string array[9];
	for(int i=0;i<9;i++)
		getline(in,array[i]);
	Block myblock(0,array,9);
	myblock.BlockGen();
	myblock.writeData("data.txt");
*/
	/*int size=11;
	string trans[11];
	string tmp;
	for(int i=0; i<size;){
		getline(in,tmp);
		if(tmp[0]=='2'&&tmp[1]=='0'){
			tmp=aes_MakeDecryptable(tmp);
			trans[i]=tmp;
			i++;
		}
	}
	Block myblock(0,trans,4);
	myblock.BlockGen();
	mychain.AddBlock(myblock);
	cout<<"gethash() "<<mychain._GetLastBlock().GetHash()<<endl;
	cout<<"getmerkle() "<<mychain._GetLastBlock().GetMerkle()<<endl;
	cout<<myblock.CMerkle()<<endl;
	cout<<Loadandcheck("2019-03-04 13:46:32")<<endl;
	sleep(5);
	cout<<"gethash() "<<myblock.GetHash()<<endl;
	cout<<"getmerkle() "<<myblock.GetMerkle()<<endl;
	cout<<myblock.CMerkle()<<endl;*/

/*
	uid_t ruid,euid,suid;
	int uerr;
	if (getresuid(&ruid, &euid, &suid) == -1) {
		fprintf(stderr, "Cannot obtain user identity: %m.\n");
		return EXIT_FAILURE;
    }
    if (ruid != (uid_t)TARGET_UID && ruid < (uid_t)UID_MIN) {
        fprintf(stderr, "Invalid user.\n");
        return EXIT_FAILURE;
    }
    if (seteuid((uid_t)TARGET_UID) == -1) {
            fprintf(stderr, "Insufficient user privileges.\n");
            return EXIT_FAILURE;
    }*/

	in.open("/home/song/log1.txt");
	if(!in.is_open()) return EXIT_FAILURE;
	chainlog.open("chainlog.txt", ios::ate|ios::app);

	int rc;
	char buff[20];
	time_t now = time(NULL);
	strftime(buff, 20, "%Y.%m.%d_%H:%M:%S", localtime(&now));
	string Time(buff);
	chainlog<<Time<<endl;
	//chain.open(Time,ios::out|ios::app);
	//chain_leng.open(Time+"_leng.txt",ios::out);
	chainlog.close();

	private_chain pchain("itmt&beb4589","554efi#x9&%9");

	/*rc = pthread_create(&sigid[0], NULL,chain.PrintHello(),NULL);
	if (rc!=0) { printf("ERROR; ...%d\n", rc);
	}
	sleep(1);
	rc = pthread_create(&sigid[1],NULL,chain.counter(),NULL);
	if (rc!=0) { printf("ERROR; ...%d\n", rc);;
				}
	sleep(3);
	rc = pthread_create(&sigid[2],NULL,StableCheck,NULL);
	sleep(15);*/

;

	chain.close();
	in.close();
	chainlog.close();

/*	string key1 = SHA_512("itmt&beb4589");
	string iv1 = key1;
	string key2,iv2;
	for(int i =0; i<64;i++) key2.push_back(key1[i*2]);
	for(int i =0; i<32;i++) iv2.push_back(iv1[i*3]);
	cout<<key2<<endl<<iv2<<endl;*/
	//chain_leng.close();
/*
	string text="2019-02-20T10:59:44.750160Z	    2 Connect	root@localhost on  using Socket";
	string text2="2019-02-20T10:59:44.753719Z	    2 Query	select @@version_comment limit 1";
	unsigned char result1[256];
	int leng = aes_encrypt(text,key,iv,result1);
	fstream fd;
	fd.open("test.txt");
	fd <<result1<<'.'<<leng<<endl;
	cout<<result1<<endl;
	leng = aes_encrypt(text2,key,iv,result1);
	fd<<result1<<'.'<<leng<<endl;
	cout<<result1<<endl;
	fd.close();
	fd.open("test.txt");
	string str;
	string str_leng;
	getline(fd,str);
	int encrypt_leng;
	unsigned char cipher[256];
	encrypt_leng = aes_length(str);
	aes_StringToCharArr(str,cipher);
	cout<<encrypt_leng<<endl;
	cout<<aes_decrypt(cipher,encrypt_leng,iv,key)<<endl;
	getline(fd,str);
	encrypt_leng = aes_length(str);
	aes_StringToCharArr(str,cipher);
	cout<<aes_decrypt(cipher,encrypt_leng,iv,key)<<endl;
	fd.close();*//*
	fstream fd;
	//fd.open("2019-02-20 20:57:29.txt");
	//string tmp;
	//string tmp1="2019-02-20T10:59:44.750160Z	    2 Connect	root@localhost on  using Socket";
	//unsigned char cipher[256];
	//getline(fd,tmp);
	//fd.close();
	//int leng=aes_length(tmp);
	//cout<<leng<<" "<<tmp<<endl;
	//aes_StringToCharArr(tmp,cipher);
	//cout<<leng<<" "<<cipher<<endl;
	//string tmp1="2019.02.21T23.39.35.26316_2_Select_ssn_From_DEPT_Where_(ssn == 512, @#4)";
	string tmp1="2019-02-24T06:02:09.972362Z	    2 Query	SELECT DATABASE()";
	//2019.02.24T06.02.09.97_2_Query_show_tables
	tmp1=aes_MakeDecryptable(tmp1);
	cout<<tmp1<<endl;
	unsigned char result1[256];
	int leng2 = aes_encrypt(tmp1,key,iv,result1);
	fd.open("test1.txt",ios::out);
	fstream fl;
	fl.open("test2.txt",ios::out);
	fd<<result1<<endl<<endl;
	fl<<leng2<<" ";
	cout<<"원문 "<<result1<<'.'<<leng2<<endl;
	fd.close();
	fl.close();
	fd.open("test1.txt");
	fl.open("test2.txt");
	string result;
	getline(fd,result);
	string substr;
	while(true){
		getline(fd,substr);
		if(substr =="\0")
			break;
		result+='\0';
		result+=substr;
	}
//	for(; !fd.eof();){
//		if(result[result.size()-1] >57 || result[result.size()-1]<48){
//				string subtmp;
//				cout<<"try to connect"<<endl;
//				while(true){
//					if(result[result.size()-1] <=57&&result[result.size()-1]>=48)
//						break;
//					getline(fd,subtmp);
//					result += subtmp;
//					cout<<"made "<<result<<endl;
//				}
//		}
//	}
	cout<<"a"<<result<<endl;
	int len=0;
	fl>>len;
	unsigned char lab[256];
	aes_StringToCharArr(result,lab);
	cout<<lab<<" "<<"result "<<result1<<" "<<len<<endl;
	cout<<aes_decrypt(lab,len,iv,key)<<endl;
/*
	uerr = 0;
	    if (setresuid(ruid, ruid, ruid) == -1) {
	        uerr = errno;
	        if (!uerr)
	            uerr = EINVAL;
	    }
	    if (uerr) {
	        if (uerr)
	            fprintf(stderr, "Cannot drop user privileges: %s.\n", strerror(uerr));
	        return EXIT_FAILURE;
	    }*/


	return 0;
}
/*
string rc4_MakeDecryptable(string str){
	string result= str;
	int leng=0;
	int det=0;
	for(int i =0; i<result.size();i++){
		if(result[i]==' ' && result[i+1]==' '){
			for(int j =i;j<str.size();j++){
				det++;
				if(result[j] != 32){
					char tmp[str.size()-j-det];
					leng=0;
					for(int k=j;k<str.size();k++){
						tmp[leng]=result[k];
						leng++;
					}
					for(int k=0;k<leng;k++){
					result.at(i+k)=tmp[k];
					}
					for(int k=i+leng;k<str.size()-det;k++){
						result.at(k)='\0';
					}
					leng=0;
					break;
					break;
				}
			}
		}
		if(result[i] =='.' && result[i+2] != '.'){
			char tmp2[result.size()-det-i];
			int leng=0;
			for(int j=i+8;j<result.size()-det;j++){
				tmp2[leng] = result[j];
				leng++;
			}
			det+=5;
			leng=0;
			for(int j=i+3;j<result.size()-det;j++){
				result.at(j)=tmp2[leng];
				leng++;
			}
			for(int j=0;j<det;j++)
				result.at(result.size()-det)='\0';
			leng=0;
		}
		if(result[i] == '/' ||result[i] ==':' ||result[i] == '-'||result[i] == '.')
			result.at(i)='_';
		if(result[i]==9){
			if(i == (result.size()-det))
				result.at(i)='\0';
			else
				result.at(i)=95;
		}
		if(result[i]=='('){
			result.at(i)='\0';
			result.at(i+1)=NULL;
		}
		if(result[i]==32){
			if(i == result.size()-det)
				result.at(i)='\0';
			else
				result.at(i)=95;
		}
	}
	return result;
}
*/
/*void rc4_StringToCharArr(string str, unsigned char arr[256]){
	int i=0;
	for( i; i<=str.size();i++){
			arr[i]=str[i];
	}
}
int rc4_length(string ciphertext){
	int result;
	for(int i=ciphertext.size();i>0; i--){
		if(ciphertext[i]=='.'){
			for(int j=i+1; j<ciphertext.size();j++){
				result += (ciphertext[j]-48)*pow(10,(j-ciphertext.size())*-1 -1);
			}
			break;
		}
	}
	return result;
}*/
/*int rc4_encrypt(string text,unsigned char* key, unsigned char* iv,unsigned char result[256]){
		   Message to be encrypted
		   Buffer for ciphertext. Ensure the buffer is long enough for the
		    ciphertext which may be longer than the plaintext, dependant on the
		    algorithm and mode
		  int ciphertext_len;
		   Encrypt the plaintext
		  ciphertext_len = encrypt ((unsigned char*)text.c_str(), strlen ((char *)text.c_str()), key, iv,result);
		  return ciphertext_len;
}*/
/*string rc4_decrypt(unsigned char* ciphertext,int ciphertext_len,unsigned char *iv, unsigned char* key){
	unsigned char decrypt_result[256];
	int decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,decrypt_result);
		   Add a NULL terminator. We are expecting printable text
	decrypt_result[decryptedtext_len] = '\0';
	return (const char*)decrypt_result;}//free(): invalid pointer error occurred*/
void Block::writeData(string path,string to, string key, string iv){
	ofstream fd;
	ifstream data;
	fd.open(to, ios::out|ios::ate|ios::app);

	//char buff[20];

	//strftime(buff, 20, "%Y.%m.%d_%H:%M:%S", localtime(&_tTime));
	//string Time(buff);

	decrypt_file(path,key,iv);
	data.open("workspace.txt");

	vector<string> str;
	string str2;

	for(;!data.eof();){
		getline(data,str2);
		str.push_back(str2);
	}
	data.close();
	encrypt_file(path,key,iv);
	system("rm workspace.txt");

	if(!fd.is_open())
		//cout<<"error: path: "<<path<<" open path error"<<endl;
	//if(!ld.is_open())
		//cout<<"error: leng file "<<Time<<" couln't open"<<endl;
	for(int i=0; i<str.size();i++){
		//ld>>size;
		//decrypt_file(result,key,iv);
		if(str[0] != "none"){
			fd<<str[i]<<endl;
		}
	}
	fd.close();
	//ld.close();
}
