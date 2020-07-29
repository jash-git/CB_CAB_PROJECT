#include "CSerchAllFile.h"

static CSerchAllFile* instance;

CSerchAllFile::CSerchAllFile(void)
{

}




CSerchAllFile::~CSerchAllFile(void)
{

}




CSerchAllFile* CSerchAllFile::getInstance()
{
    if (instance == NULL)
    {
        instance = new CSerchAllFile();
    }
    return instance;

}




//�M���ɧ��U���ɮצW�C��(�]�A�O�M�ɧ�)

void CSerchAllFile::get_filelist(char *foldname,vector<string> &filelist)
{

    HANDLE hFind;

    WIN32_FIND_DATA fileData;

    string line;

    char fn[MAX_PATH];

    char tmpfn[MAX_PATH];

    strcpy(fn,foldname);

    //�ݭn���ɧ��W���r����i��B�z

    if(fn[strlen(fn) -1] != '\\' )
    {
        strcat(fn, "\\");
    }
    //�d�N���ǡA����fn�w�[�J"\\"

    strcpy(tmpfn,fn);

    //���[*�|�X���I

    strcat(fn, "*");

    hFind = FindFirstFile(fn, &fileData);

    FindNextFile(hFind, &fileData);

    while(FindNextFile(hFind, &fileData))

    {

        //�p�G���y�쪺��e���ɧ�

        if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)

        {
            if(fileData.cFileName[0]!='.')
            {

                char szFile[MAX_PATH];

                strcpy(szFile,tmpfn);

                strcat(szFile,fileData.cFileName);

                get_filelist(szFile,filelist);

            }

        }

        //���y����

        else
        {

            line = (string)tmpfn;

            line+=fileData.cFileName;

            /*if (line.find(".h",0)!=string::npos)

            {

            filelist.push_back(line);

            }

            else

            {

            continue;

            }*/

            filelist.push_back(line);

        }

//cout<<line<<endl;

    }

}
