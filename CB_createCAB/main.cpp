#include <iostream>
#include "CSerchAllFile.h"
#include "CMyCab.h"
using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    vector<string> vec_file;
    vector<string>::iterator itemFileName;
    char filePacketPath[MAX_PATH] = "D:\\VM_Win7\\net4_zip_test";//"D:\\sqliteadmin";
    char filePutPath[MAX_PATH] = "D:\\my.cab";
    CSerchAllFile::getInstance()->get_filelist(filePacketPath, vec_file);
    //--------------------���]�L�{--------------------
    //�n����H
    CMyCab mc;
    //�]�m��X���
    mc.SetOutPutFile(filePutPath);
    //�K�[�n���]�����
    for(itemFileName=vec_file.begin(); itemFileName<vec_file.end(); itemFileName++)
    {
        mc.AddFile((*itemFileName).c_str());
    }
    //���楴�]
    mc.DoMakeCAB();

    //--------------------�ѹL�{--------------------
    //�n����H
    CMyCab umc;
    //����ѥ]
    umc.DoUnCAB("C:\\my.cab");

    return 0;
}
