#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <direct.h>

using namespace std;

//最多打包文件個數
#define MAX_FILE_COUNT 1024
//最大路徑字元長度
#define MAX_PATH 260
//文件頭
struct FileHead
{
    unsigned int FileCount;//文件個數
    unsigned int FileLen[MAX_FILE_COUNT];//文件大小
    char FileName[MAX_FILE_COUNT][MAX_PATH];//檔案名
};
char gchrFileName[200],gchrPath[200];
char gchrFileExt[20];
void GetFileName(char *data);
long GetFileSize(FILE *pf);
int CheckTargetPath(string targetPath);
int main(int argc, char *argv[])
{
    int i=0;
    int intcase=-1;
    FILE *pf='\0';
    char buf[513];
    char buf1[1024];
    string strbuf;
    FILE *pWorkFile = '\0';
    FileHead fh;
    FILE *pOutFile = '\0';
    char chrFileListPath[512];
    char chrCABFilePath[512];
    char chrOuputPath[512];
    unsigned char *pTmpData = NULL;
    char tmpFileName[MAX_PATH];
    FILE *pCAB = '\0';
    char cwd[512];

    cout << "Hello world!" << endl;
    memset(cwd,0,sizeof(cwd));
    if (getcwd(cwd, sizeof(cwd)) != NULL)//抓取目前程式路徑
    {
        cout << "Current working dir: " <<cwd<<endl;
    }
    if(argc<3)//argv[0]=名稱，argv[1]=case，argv[2]=filelist path/save cab path，argv[3]= save cab path/OuputPath
    {
        cout << "parameter error" << endl;
        return -1;
    }
    else
    {
        cout << argv[1]<< endl;
        intcase=atoi(argv[1]);//0:Make Cab，1:DoUnCAB
        if((intcase==0)||(intcase==1))
        {
            memset(chrFileListPath,0,sizeof(char));
            memset(chrCABFilePath,0,sizeof(char));
            memset(chrOuputPath,0,sizeof(char));
            switch(intcase)
            {
            case 0://0:Make Cab
                 strcpy(chrFileListPath,argv[2]);
                 strcpy(chrCABFilePath,argv[3]);
                 cout << "file list: " <<chrFileListPath  << endl;
                 cout << "cab file: " <<chrCABFilePath  << endl;
                 GetFileName(chrFileListPath);
                 memset(cwd,0,sizeof(cwd));
                 strcpy(cwd,gchrPath);//所有被打包檔案的所在路徑

                break;
            case 1://1:DoUnCAB
                strcpy(chrCABFilePath,argv[2]);
                strcpy(chrOuputPath,argv[3]);
                cout << "cab file: " <<chrCABFilePath  << endl;
                cout << "ouput path: " <<chrOuputPath << endl;
                break;
            }
        }
        else
        {
            cout << "parameter error" << endl;
            return -1;
        }
    }
    if(intcase==0)//0:Make Cab
    {
        pf=fopen(chrFileListPath,"r");//pf=fopen("filelist.txt","r");
        vector<string> vec_file;
        vec_file.clear();
        memset(buf,0,sizeof(buf));
        while(fgets(buf,512,pf) != NULL)
        {
            buf[strlen(buf)-1]='\0';
            memset(buf1,0,sizeof(buf1));
            strcpy(buf1,cwd);
            strcat(buf1,buf);
            pWorkFile='\0';
            pWorkFile= fopen(buf1,"rb");
            if(pWorkFile!='\0')//確定檔案存在 & 可讀
            {
                vec_file.push_back(buf1);
                fclose(pWorkFile);
                pWorkFile='\0';
            }
        }

        //當文件內的路徑存到結構體內，並抓取檔案大小
        memset(&fh,0,sizeof(FileHead));
        fh.FileCount=0;
        for ( i = 0 ; i < vec_file.size() ; i++ )
        {
            strbuf=vec_file[i];
            strcpy(buf, strbuf.c_str());
            strcpy(fh.FileName[fh.FileCount],buf);
            pWorkFile= fopen(fh.FileName[i],"rb");
            fh.FileLen[i] = GetFileSize(pWorkFile);
            fclose(pWorkFile);
            pWorkFile = '\0';
            //printf("%s,%ld\n",fh.FileName[i],fh.FileLen[i]);
            fh.FileCount++;
        }

        //將所有指定檔案打包

        //memset(chrCABFilePath,0,sizeof(char));
        //strcpy(chrCABFilePath,"test.cab");
        //*
        pOutFile = fopen(chrCABFilePath,"wb");
        fwrite(&fh,sizeof(fh),1,pOutFile);//寫入檔頭
        for (i = 0 ; i < fh.FileCount ; i++ )
        {
            pWorkFile='\0';
            pWorkFile = fopen(fh.FileName[i],"rb");
            if ( NULL == pWorkFile )
            {
                fclose(pWorkFile);
                fclose(pOutFile);
                return -1;
            }

            pTmpData = new unsigned char[fh.FileLen[i]];
            fread(pTmpData,fh.FileLen[i],1,pWorkFile);
            if ( ferror(pWorkFile) )
            {
                fclose(pWorkFile);
                fclose(pOutFile);
                return -2;
            }

            fwrite(pTmpData,fh.FileLen[i],1,pOutFile);
            if ( ferror(pOutFile) )
            {
                fclose(pWorkFile);
                fclose(pOutFile);
                return -3;
            }
            delete [] pTmpData;
            pTmpData='\0';
            fclose(pWorkFile);
        }
        fclose(pOutFile);
        pOutFile='\0';
    }
    else
    {
        //*/
        //將打包檔解開，放到指定路徑
        //strcpy(chrCABFilePath,"D:\\Visual project code\\CB_CustomCAB\\test.cab");
        //memset(chrOuputPath,0,sizeof(char));
        //strcpy(chrOuputPath, "C:\\CAB_Buf");
        if((chdir(chrOuputPath)==0)||(CheckTargetPath(chrOuputPath)==0))
        {
            pCAB = fopen(chrCABFilePath,"rb");
            memset(&fh,0x0,sizeof(fh));
            fread(&fh,sizeof(fh),1,pCAB);//讀取文件檔頭
            for ( int i = 0 ; i < fh.FileCount ; i++ )
            {
                pTmpData = new unsigned char[fh.FileLen[i]];
                fread(pTmpData,fh.FileLen[i],1,pCAB);
                GetFileName(fh.FileName[i]);

                memset(tmpFileName,0,sizeof(char));
                sprintf(tmpFileName,"%s\\%s%s",chrOuputPath,gchrFileName,gchrFileExt);
                pWorkFile = '\0';
                pWorkFile = fopen(tmpFileName,"wb");
                fwrite(pTmpData,fh.FileLen[i],1,pWorkFile);
                fclose(pWorkFile);
                delete [] pTmpData;
                pTmpData='\0';
            }
            fclose(pCAB);
        }
    }

    return 0;
}
//獲取檔大小(傳入以二進制方式打開的檔指針)
long GetFileSize(FILE *pf)
{
    //指針移到檔案結尾
    fseek(pf,0,/*SEEK_END*/ 2);
    return ftell(pf);
}
void GetFileName(char *data)
{
    gchrPath[0]='\0';
    gchrFileName[0]='\0';
    gchrFileExt[0]='\0';
    int i,intcount;
    int j=0;
    int k=0;
    char d;
    intcount=strlen(data);//計算原本完整檔案路徑長度
    for(i=(intcount-1);i>=0;i--)
    {
        if((data[i]!='/')&&(data[i]!='\\'))
        {
            d=data[i];
            gchrPath[j]=d;
            j++;
        }
        else
        {
            break;
        }
    }
    gchrPath[j]='\0';
    for(int i=0;i<j;i++)
    {
        if(i<(j-4))
        {
            gchrFileName[i]=gchrPath[j-i-1];
            gchrFileName[i+1]='\0';
        }
        else
        {
            gchrFileExt[k]=gchrPath[j-i-1];
            k++;
            gchrFileExt[k]='\0';
        }
    }
    gchrPath[0]='\0';
    for(i=0;i<(intcount-j);i++)
    {
        gchrPath[i]=data[i];
    }
    gchrPath[(intcount-j)]='\0';
    //cout<<chrPath<<"\n";
    //cout<<chrFileName<<"\n";
}
int CheckTargetPath(string targetPath)
{
    targetPath+="\\";//Bat內的路徑沒有最後的反斜線
    int e_pos = targetPath.length();
    int f_pos = targetPath.find("\\",0);
    string subdir;
    do
    {
        e_pos = targetPath.find("\\",f_pos+2);
        if(e_pos != -1)
        {
            subdir = targetPath.substr(0,e_pos);
            if(mkdir(subdir.c_str())==0)
            {
                printf( "creat success %s\n",subdir.c_str());
            }
            else
            {
                printf("creat fail %s\n",subdir.c_str());
            }
        }
        f_pos = e_pos;
    }while(f_pos!=-1);
    return 0;
}
