#include <openssl/md5.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string>
#include <stdio.h>
#include <string.h>

std::string md5(std::string str)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    int sz=str.size();
    auto ch= new char[sz+1];
    
    strcpy(ch,str.c_str());
    
    MD5((unsigned char*)ch, strlen(ch), (unsigned char*)&digest);    

    char mdString[MD5_DIGEST_LENGTH*2+1];
    mdString[MD5_DIGEST_LENGTH*2]=0;

    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);


    return mdString;
}


void convert(char* cert_filestr,char* certificateFile)
{
    X509* x509 = NULL;
    FILE* fd = NULL,*fl = NULL;

    fl = fopen(cert_filestr,"rb");
    if(fl) 
    {
        fd = fopen(certificateFile,"w+");
        if(fd) 
        {
            x509 = PEM_read_X509(fl,&x509,NULL,NULL);
            if(x509) 
            {
                 i2d_X509_fp(fd, x509);
            }
            else 
            {
                printf("failed to parse to X509 from fl");
            }
            fclose(fd);
        }
        else
        {
             printf("can't open fd");
        }
        fclose(fl);
    }
    else 
    {
         printf("can't open f");
    }
}

/*
void bar(void)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        fprintf(stderr, "unable to open: %s\n", path);
        return ;
    }

    X509 *cert = PEM_read_X509(fp, NULL, NULL, NULL);
    if (!cert) 
    {
        fprintf(stderr, "unable to parse certificate in: %s\n", path);
        fclose(fp);
        return ;
    }

    // any additional processing would go here..

    X509_free(cert);
    fclose(fp);
}

int foo()
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    char string[] = "happy";
    
    MD5((unsigned char*)&string, strlen(string), (unsigned char*)&digest);    

    char mdString[33];

    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    printf("md5 digest: %s\n", mdString);

    return 0;
}
*/


