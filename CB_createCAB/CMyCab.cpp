#include "CMyCab.h"

CMyCab::CMyCab(void)

{
    memset(&fh,0x0,sizeof(fh));

    memset(ObjectFilePathName,0x0,sizeof(ObjectFilePathName));
}

CMyCab::~CMyCab(void)
{

}

//�K�[����]��
void CMyCab::AddFile(const char * FilePathName)
{
    if ( fh.FileCount >= MAX_FILE_COUNT - 1 )
    {
        cout<<"�̦h�䴩"<<MAX_FILE_COUNT<<"����"<<endl;
        return;
    }
    strcpy(fh.FileName[fh.FileCount],FilePathName);
    fh.FileCount++;
}

//�]�m���]��X���
void CMyCab::SetOutPutFile(char * OutFile)
{
    memset(ObjectFilePathName,0x0,sizeof(ObjectFilePathName));
    strcpy(ObjectFilePathName,OutFile);
}

//����ɤj�p(�ǤJ�H�G�i��覡���}���ɫ��w)
long CMyCab::GetFileSize(FILE *pf)
{
    //���w�����ɮ׵���
    fseek(pf,0,/*SEEK_END*/ 2);
    return ftell(pf);
}

//�s�@���]���
void CMyCab::DoMakeCAB()
{
    if ( fh.FileCount < 1 )
    {
        cout<<"�S�����K�[�쥴�]"<<endl;
        return;
    }

    if ( strlen(ObjectFilePathName) < 1 )
    {
        cout<<"�S�����w���]�ɿ�X��m"<<endl;
        return;
    }

    FILE *pOutFile = NULL;
    FILE *pWorkFile = NULL;

    //����Ҧ��ɤj�p
    for ( int i = 0 ; i < fh.FileCount ; i++ )
    {
        pWorkFile = fopen(fh.FileName[i],"rb");
        if ( NULL == pWorkFile )
        {
            cout<<"���:"<<fh.FileName[i]<<"�L�kŪ��["<<strerror(errno)<<"]"<<endl;
            return;
        }
        fh.FileLen[i] = GetFileSize(pWorkFile);
        fclose(pWorkFile);
    }

    //�ˬd�O�_���������ɧ�
    CheckTargetPath(ObjectFilePathName);
    //�}�l�X�ּg��
    pOutFile = fopen(ObjectFilePathName,"wb");
    if ( NULL == pOutFile )
    {
        cout<<"��X���Ыإ���["<<strerror(errno)<<"]"<<endl;
        return;
    }

    //�g�J����Y
    fwrite(&fh,sizeof(fh),1,pOutFile);

    //�g�J�U���
    for ( int i = 0 ; i < fh.FileCount ; i++ )
    {
        unsigned char *pTmpData = NULL;

        pWorkFile = fopen(fh.FileName[i],"rb");

        if ( NULL == pWorkFile )
        {
            cout<<"���:"<<fh.FileName[i]<<"�L�kŪ��["<<strerror(errno)<<"]"<<endl;

            fclose(pWorkFile);

            fclose(pOutFile);

            return;
        }

        pTmpData = new unsigned char[fh.FileLen[i]];

        fread(pTmpData,fh.FileLen[i],1,pWorkFile);

        if ( ferror(pWorkFile) )
        {
            cout<<"���:"<<fh.FileName[i]<<"�L�kŪ��["<<strerror(errno)<<"]"<<endl;

            fclose(pWorkFile);

            fclose(pOutFile);

            return;
        }

        fwrite(pTmpData,fh.FileLen[i],1,pOutFile);

        if ( ferror(pOutFile) )
        {
            cout<<"���:"<<ObjectFilePathName<<"�L�k�g�J["<<strerror(errno)<<"]"<<endl;

            fclose(pWorkFile);

            fclose(pOutFile);

            return;
        }

        delete [] pTmpData;

        fclose(pWorkFile);
    }

    fclose(pOutFile);

    cout<<"���]����"<<endl;
}

//�ѥ](���F�`�ٮɶ����g���~�B�z�F,�i�H�ھڤW���ۤv�K�[)
void CMyCab::DoUnCAB(char *CabFilePathName)
{

    FILE *pCAB = NULL;

    FILE *pWork = NULL;

    pCAB = fopen(CabFilePathName,"rb");

    //Ū����Y
    memset(&fh,0x0,sizeof(fh));
    fread(&fh,sizeof(fh),1,pCAB);

    printCAB();

    //�ѥ]���Ҧ��ɩ���e�ؿ��U
    for ( int i = 0 ; i < fh.FileCount ; i++ )
    {
        unsigned char *pTmpData = NULL;

        pTmpData = new unsigned char[fh.FileLen[i]];

        fread(pTmpData,fh.FileLen[i],1,pCAB);

        //�u���ɮצW,���n�ͦ��ɪ����|�W

        char tmpFileName[MAX_PATH];

        string str="d:\\";

        string aaa;

        aaa.assign(fh.FileName[i] ,strlen(fh.FileName[i]) );

        const char *chaaaaa = aaa.replace(0, str.length(), "\\").c_str();
        //printf("%s\n",chaaaaa);
        char ptmpC[MAX_PATH];

        strcpy(ptmpC, chaaaaa);

        memset(tmpFileName,0x0,sizeof(tmpFileName));

        strcpy(tmpFileName,ptmpC+1);

        //��CAB�����|

        char tmpPathName[MAX_PATH];

        memset(tmpPathName,0x0,sizeof(tmpPathName));

        strcpy(tmpPathName,CabFilePathName);

        char* tmpC = tmpPathName + strlen(tmpPathName);

        while('\\' != *tmpC)
        {
            tmpC--;
        }

        tmpC++;

        *tmpC = '\0';

        strcat(tmpPathName,tmpFileName);

        pWork = fopen(tmpPathName,"wb");

        if (pWork == NULL)
        {
            CheckTargetPath(tmpPathName);

            pWork = fopen(tmpPathName,"wb");
        }

        fwrite(pTmpData,fh.FileLen[i],1,pWork);

        fclose(pWork);

        delete [] pTmpData;
    }

    fclose(pCAB);
}

//��ܥ��]���ɸ�T
void CMyCab::printCAB()
{
    cout<<"�ɤ���T�p�U:"<<endl;
    cout<<"����`��:"<<fh.FileCount<<endl;
    for ( int i = 0 ; i < fh.FileCount ; i++ )
    {
        cout<<fh.FileName[i]<<"\t\t\t\t"<<fh.FileLen[i]<<"�줸��"<<endl;
    }
}

//�Ыؤ��
void CMyCab::CheckTargetPath(string targetPath)
{
    //Log &log = Log::getLog("main", "CheckTargetPath");

    int e_pos = targetPath.length();

    int f_pos = targetPath.find("\\",0);

    string subdir;

    do
    {
        e_pos = targetPath.find("\\",f_pos+2);

        if(e_pos != -1)
        {

            subdir = targetPath.substr(0,e_pos);

            if(_mkdir(subdir.c_str())==0)
            {
                printf( "creat success %s",subdir.c_str());
            }
            else
            {
                printf("creat fail %s",subdir.c_str());
            }
        }

        f_pos = e_pos;
    }while(f_pos!=-1);
}
