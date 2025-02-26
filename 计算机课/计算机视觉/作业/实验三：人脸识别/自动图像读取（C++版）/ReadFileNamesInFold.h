#ifndef _READFILENAMESINFOLD_H_
#define _READFILENAMESINFOLD_H_

//字符集也要修改为，多字节字符集
#include <afx.h>//CFindFile   //选择：在共享 DLL 中使用 MFC  若有Windows.h报错，就把这行注释掉
#include <atlstr.h>//CString
#include <iostream>//count endl
#include <vector>
using namespace std;

//OpenCV 2.4.7
#include <opencv2/opencv.hpp>
using namespace cv;

//指定图像类型
//#define TYPEJPG ".jpg"   
#define TYPEJPG ".JPG"  
#define TYPEBMP ".bmp"

typedef struct NameStruct
{
	string FileName;//保存文件名
	string FileDirectory;//保存整个地址
}NameStruct;

class CReadFileNamesInFold
{
public:
	CReadFileNamesInFold(void);
	~CReadFileNamesInFold(void);


	vector<NameStruct>FileNameVector;

	//判断文件名后缀是否为“.avi”
	bool IsVideo(LPCTSTR lpszPath);

	//判断是否为根目录如c、d、e
	bool IsRoot(LPCTSTR lpszPath);
	
	//若长度（不包括结束符）大于224，则返回-1
	//搜索文件并把文件存于vector中
	int searchfile(TCHAR *lpszPath, vector<NameStruct> &NameListVector);


	//释放整条链表
	void ReleaseVector(vector<NameStruct> &NameVector);

	void GetNamesInFold(char *folddir,vector<NameStruct> &NameListVector);

	void main_test();



};
#endif