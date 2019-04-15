#include <string>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/err.h>
std::string md5(std::string str);
bool get(char* path,int offset);
char* set(char* path,int offset);
char* load(char* path);



