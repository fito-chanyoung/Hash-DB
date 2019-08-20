/*
 * myAES.cpp
 *
 *  Created on: 2019. 1. 26.
 *      Author: song
 */

#include "myAES.h"
/*
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}*/
void encrypt_file(string file, string key, string iv)
{

	string command;
	command = "openssl aes-256-cbc -e -salt -in "+file+" -out "+file+".enc -K ";
	command.append(key);
	command.append(" -iv ");
	command.append(iv);
	cout<<command<<endl;
	system(command.c_str());

	command = "rm "+file;
	system(command.c_str());

}

void decrypt_file(string file, string key,string iv)
{
	string command;
	command = "openssl aes-256-cbc -d -salt -in "+file+".enc -out workspace.txt "+"-K "+key+" -iv "+iv;
	cout<<command<<endl;
	system(command.c_str());

}

string SHA_512(string str){
	unsigned char digest[SHA512_DIGEST_LENGTH];
	SHA512((unsigned char*)&str, strlen(str.c_str()), (unsigned char*)&digest);

	char mdString[SHA512_DIGEST_LENGTH*2+1];
	for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
	     sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

	//cout<<"i hash made "<<mdString<<endl;
	return mdString;
}
/*
unsigned char* encrypt(RC4_KEY key, int block_size, unsigned char *plain_txt, unsigned char *ciphertext)
{
  RC4(&key, block_size,plain_txt,ciphertext);

  return ciphertext;
}

unsigned char* decrypt(RC4_KEY key, int block_size, unsigned char *ciphertext, unsigned char *plain_txt)
{
  RC4(&key, block_size, ciphertext, plain_txt);

  return plain_txt;
}*/

