#include "ReadFileNamesInFold.h"


CReadFileNamesInFold::CReadFileNamesInFold(void)
{

}

CReadFileNamesInFold::~CReadFileNamesInFold(void)
{

}

void CReadFileNamesInFold::ReleaseVector(vector<NameStruct> &NameVector)
{
	vector<NameStruct>::iterator it;
	for(it=NameVector.begin();it!=NameVector.end();)
	{
		it=NameVector.erase(it);
	}

	

	////ɾ��ָ��Ԫ��
	//for (vector<int>::iterator it = v.begin(); it != v.end();)   //forѭ���в�Ҫit++
	//{
	//	if (*it == 1)
	//	{
	//		v.erase(it++);
	//	}
	//	else
	//	{
	//		it++;
	//	}
	//}



	//�ͷ��ڴ�
	//NameVector.swap(NameVector);
}

void CReadFileNamesInFold::main_test()
{
	////OpenCV 2.4.7
	//����·��
	char folddir[100]="C:\\a";
	TCHAR szPath[512];
	strcpy(szPath,folddir);
	//�������ļ����µ������ļ�����ȡ�ļ�������·�� 
	searchfile(szPath,FileNameVector);
	char savename[100];//�������ñ���ͼ����ļ���
	//ѭ����ȡ�ļ���
	for(int i=0;i<FileNameVector.size();i++)
	{
		printf("%s\n",FileNameVector[i].FileName.c_str());//��ʾ�ļ���
		Mat Img=imread(FileNameVector[i].FileDirectory,1);//����ͼ���ļ�
		imshow("a",Img);//��ʾͼ���ļ�
		//����ͼ�����ֺͱ���ͼ��
		sprintf(savename,"%d.bmp",i+1);
		imwrite(savename,Img);
		cvWaitKey(0);

	}
	//�ͷ�����
	ReleaseVector(FileNameVector);
	printf("size=%d\n",FileNameVector.size());


	//////OpenCV 2.0
	////����·��
	//char folddir[100]="C:\\a";
	//TCHAR szPath[512];
	//strcpy(szPath,folddir);
	////�������ļ����µ������ļ�����ȡ�ļ�������·�� 
	//searchfile(szPath,FileNameVector);
	//char savename[100];//�������ñ���ͼ����ļ���
	////ѭ����ȡ�ļ���
	//for(int i=0;i<FileNameVector.size();i++)
	//{
	//	printf("%s\n",FileNameVector[i].FileName.c_str());//��ʾ�ļ���
	//	IplImage* Img=cvLoadImage(FileNameVector[i].FileDirectory.c_str(),1);//����ͼ���ļ�
	//	cvShowImage("a",Img);//��ʾͼ���ļ�
	//	//����ͼ�����ֺͱ���ͼ��
	//	sprintf(savename,"%d.bmp",i+1);
	//	cvSaveImage(savename,Img);
	//	cvWaitKey(0);
	//	cvReleaseImage(&Img);
	//}
	////�ͷ�����
	//ReleaseVector(FileNameVector);
	//printf("size=%d\n",FileNameVector.size());

}

bool CReadFileNamesInFold::IsRoot(LPCTSTR lpszPath)
{
	TCHAR szRoot[4];           //ȡ��һ���ַ�
	wsprintf(szRoot, "%c:\\", lpszPath[0]); //������д��szRoot
	return (lstrcmp(szRoot, lpszPath) == 0); //0��ʾ��ͬ 
}

int CReadFileNamesInFold::searchfile(TCHAR *lpszPath, vector<NameStruct> &NameListVector)
{
	if(strlen(lpszPath) >= 224)//�����ַ���s��(unsigned int��)����,������'\0'���� 
		return -1;
	TCHAR szFind[MAX_PATH]; 
	lstrcpy(szFind, lpszPath); //��lpszPath��ֵ��szFind������szFind�ܹ����ɽ�����
	if (!IsRoot(szFind)) //�ж��Ƿ�Ϊ��Ŀ¼�������Ǹ�Ŀ¼�����"\\"
		lstrcat(szFind, "\\"); 
	lstrcat(szFind, "*.*"); //�ѡ�*.*�����ӵ�szFind����
	WIN32_FIND_DATA wfd; 

	/*
	HANDLE FindFirstFile( ����LPCTSTR lpFileName, // file name ����
	LPWIN32_FIND_DATA lpFindFileData // data buffer ����
	);
	����˵����
	�ú�����һ���ļ���(�������ļ���)ȥ����ָ���ļ� ���Ҫʹ�ø�������ȥ�����ļ��Ļ� ����ʹ��FindFirstFileEx����

	����˵����
	HANDLE hFindFile�������ļ���� ����ִ�е�ʱ���������Ǵ˾������һ�ļ� ����
	LPWIN32_FIND_DATA lpFindFileData ָ��һ�����ڱ����ļ���Ϣ�Ľṹ��

	����ֵ��
	������óɹ�����һ���������������ΪFindNextFile�� FindClose���� ����
	����ʧ�� ����ΪINVALID_HANDLE_VALUE(��-1) ,�ɵ���GetLastError����ȡ������Ϣ
	*/
	HANDLE hFind = FindFirstFile(szFind, &wfd); 
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ�� 
	{
		return -1; 
	}

	do 
	{ 
		if (wfd.cFileName[0] == '.') 
			continue; // ����������Ŀ¼ 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //�������ļ���
		{ 
			TCHAR szFile[MAX_PATH]; 
			if (IsRoot(lpszPath)) 
				wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName); 
			else 
			{
				wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName); 

				searchfile(szFile,NameListVector); // ����ҵ�����Ŀ¼��������Ŀ¼���еݹ� 
			}
		} 
		else //������ļ�
		{ 
			
			TCHAR szFile[MAX_PATH]; 
			if (IsRoot(lpszPath)) //�Ǹ��ļ��У�����
			{
				wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName); 
				
			}
			else 
			{
				wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName); 
				//ע�⣬����cFileName�����ļ���
			
				if(IsVideo(szFile))
				{
		
					NameStruct Node;
			
					//string filename="F:/1/D1.JPG";//      "F://1//D1.JPG";      //"F:/1/D1.JPG";    //"F:\\1\\D1.JPG";

					string filename;
					char *lp = (char*)(lpszPath);
					//ƴ��һ��ȫ��
					char tmp[1000];
					sprintf(tmp,"%s\\%s",lp,wfd.cFileName);
					Node.FileName=wfd.cFileName;
					Node.FileDirectory=tmp;

					NameListVector.push_back(Node);
					//cout<<Node.wholeroute<<endl;
					
					/*Mat img=imread(Node.wholeroute);
					imshow("1",img);
					cvWaitKey(300);*/
				}
			}
			// ���ļ����в��� 
		} 
	} while (FindNextFile(hFind, &wfd)); 
	FindClose(hFind); // �رղ��Ҿ�� 
}

bool CReadFileNamesInFold::IsVideo(LPCTSTR lpszPath)
{
	char *lp = (char*)strrchr(lpszPath,'.');
	int lLen = strlen(lpszPath);

	if((lp != NULL) &&   (    strcmp((const char*)lp,TYPEJPG)) || strcmp((const char*)lp,TYPEBMP)        )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CReadFileNamesInFold::GetNamesInFold(char *folddir,vector<NameStruct> &NameListVector)
{
	TCHAR szPath[512];
	strcpy(szPath,folddir);
	searchfile(szPath,FileNameVector);
}