/*
 * myAES.h
 *
 *  Created on: 2019. 1. 26.
 *      Author: song
 */

#ifndef MYAES_H_
#define MYAES_H_

#include <iostream>
#include <string>
#include <stdlib.h>

#include <openssl/aes.h>
#include <openssl/sha.h>
using namespace std;
/*
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>*/
#include <string.h>

void encrypt_file(string file, string key, string iv);


void decrypt_file(string file, string key, string iv);

string SHA_512(string str);


#endif /* MYRC4_H_ */
