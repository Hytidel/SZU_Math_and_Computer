#ifndef _READFILENAMESINFOLD_H_
#define _READFILENAMESINFOLD_H_

//�ַ���ҲҪ�޸�Ϊ�����ֽ��ַ���
#include <afx.h>//CFindFile   //ѡ���ڹ��� DLL ��ʹ�� MFC  ����Windows.h�����Ͱ�����ע�͵�
#include <atlstr.h>//CString
#include <iostream>//count endl
#include <vector>
using namespace std;

//OpenCV 2.4.7
#include <opencv2/opencv.hpp>
using namespace cv;

//ָ��ͼ������
//#define TYPEJPG ".jpg"   
#define TYPEJPG ".JPG"  
#define TYPEBMP ".bmp"

typedef struct NameStruct
{
	string FileName;//�����ļ���
	string FileDirectory;//����������ַ
}NameStruct;

class CReadFileNamesInFold
{
public:
	CReadFileNamesInFold(void);
	~CReadFileNamesInFold(void);


	vector<NameStruct>FileNameVector;

	//�ж��ļ�����׺�Ƿ�Ϊ��.avi��
	bool IsVideo(LPCTSTR lpszPath);

	//�ж��Ƿ�Ϊ��Ŀ¼��c��d��e
	bool IsRoot(LPCTSTR lpszPath);
	
	//�����ȣ�������������������224���򷵻�-1
	//�����ļ������ļ�����vector��
	int searchfile(TCHAR *lpszPath, vector<NameStruct> &NameListVector);


	//�ͷ���������
	void ReleaseVector(vector<NameStruct> &NameVector);

	void GetNamesInFold(char *folddir,vector<NameStruct> &NameListVector);

	void main_test();



};
#endif