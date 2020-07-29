#ifndef CMYCAB_H
#define CMYCAB_H

#include <iostream>

#include <stdio.h>

#include <stdlib.h>

#include <memory.h>

#include <string.h>

//#include <error.h>

#include <direct.h>

using namespace std;




//�̦h���]���Ӽ�

#define MAX_FILE_COUNT 1024

//�̤j���|�r������

#define MAX_PATH 260




//����Y

struct FileHead

{

    unsigned int FileCount;//���Ӽ�

    unsigned int FileLen[MAX_FILE_COUNT];//���j�p

    char FileName[MAX_FILE_COUNT][MAX_PATH];//�ɮצW

};

class CMyCab
{

private:

    FileHead fh;//����Y

    char ObjectFilePathName[MAX_PATH];//�ͦ����]�ɦ�m�ΦW��

public:

    CMyCab(void);

    ~CMyCab(void);




    //�K�[����]��

    void AddFile(const char * FilePathName);

    //�]�m���]��X���

    void SetOutPutFile(char * OutFile);

    //����ɤj�p(�ǤJ�H�G�i��覡���}���ɫ��w)

    long GetFileSize(FILE *pf);

    //�s�@���]���

    void DoMakeCAB();

    //�ѥ](���F�`�ٮɶ����g���~�B�z�F,�i�H�ھڤW���ۤv�K�[)

    void DoUnCAB(char *CabFilePathName);

private:

    //��ܥ��]���ɸ�T

    void printCAB();

    //�Ыؤ��

    void CheckTargetPath(string targetPath);

};


#endif // CMYCAB_H
