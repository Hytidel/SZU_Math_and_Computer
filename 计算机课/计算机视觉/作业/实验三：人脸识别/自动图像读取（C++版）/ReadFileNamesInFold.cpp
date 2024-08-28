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

	

	////删除指定元素
	//for (vector<int>::iterator it = v.begin(); it != v.end();)   //for循环中不要it++
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



	//释放内存
	//NameVector.swap(NameVector);
}

void CReadFileNamesInFold::main_test()
{
	////OpenCV 2.4.7
	//设置路径
	char folddir[100]="C:\\a";
	TCHAR szPath[512];
	strcpy(szPath,folddir);
	//搜索该文件夹下的所有文件，获取文件名及其路径 
	searchfile(szPath,FileNameVector);
	char savename[100];//用于设置保存图像的文件名
	//循环读取文件名
	for(int i=0;i<FileNameVector.size();i++)
	{
		printf("%s\n",FileNameVector[i].FileName.c_str());//显示文件名
		Mat Img=imread(FileNameVector[i].FileDirectory,1);//读入图像文件
		imshow("a",Img);//显示图像文件
		//设置图像名字和保存图像
		sprintf(savename,"%d.bmp",i+1);
		imwrite(savename,Img);
		cvWaitKey(0);

	}
	//释放链表
	ReleaseVector(FileNameVector);
	printf("size=%d\n",FileNameVector.size());


	//////OpenCV 2.0
	////设置路径
	//char folddir[100]="C:\\a";
	//TCHAR szPath[512];
	//strcpy(szPath,folddir);
	////搜索该文件夹下的所有文件，获取文件名及其路径 
	//searchfile(szPath,FileNameVector);
	//char savename[100];//用于设置保存图像的文件名
	////循环读取文件名
	//for(int i=0;i<FileNameVector.size();i++)
	//{
	//	printf("%s\n",FileNameVector[i].FileName.c_str());//显示文件名
	//	IplImage* Img=cvLoadImage(FileNameVector[i].FileDirectory.c_str(),1);//读入图像文件
	//	cvShowImage("a",Img);//显示图像文件
	//	//设置图像名字和保存图像
	//	sprintf(savename,"%d.bmp",i+1);
	//	cvSaveImage(savename,Img);
	//	cvWaitKey(0);
	//	cvReleaseImage(&Img);
	//}
	////释放链表
	//ReleaseVector(FileNameVector);
	//printf("size=%d\n",FileNameVector.size());

}

bool CReadFileNamesInFold::IsRoot(LPCTSTR lpszPath)
{
	TCHAR szRoot[4];           //取第一个字符
	wsprintf(szRoot, "%c:\\", lpszPath[0]); //把数据写入szRoot
	return (lstrcmp(szRoot, lpszPath) == 0); //0表示相同 
}

int CReadFileNamesInFold::searchfile(TCHAR *lpszPath, vector<NameStruct> &NameListVector)
{
	if(strlen(lpszPath) >= 224)//计算字符串s的(unsigned int型)长度,不包括'\0'在内 
		return -1;
	TCHAR szFind[MAX_PATH]; 
	lstrcpy(szFind, lpszPath); //把lpszPath赋值到szFind，并且szFind能够容纳结束符
	if (!IsRoot(szFind)) //判断是否为根目录，若不是根目录则加上"\\"
		lstrcat(szFind, "\\"); 
	lstrcat(szFind, "*.*"); //把“*.*”连接到szFind后面
	WIN32_FIND_DATA wfd; 

	/*
	HANDLE FindFirstFile( 　　LPCTSTR lpFileName, // file name 　　
	LPWIN32_FIND_DATA lpFindFileData // data buffer 　　
	);
	功能说明：
	该函数到一个文件夹(包括子文件夹)去搜索指定文件 如果要使用附加属性去搜索文件的话 可以使用FindFirstFileEx函数

	参数说明：
	HANDLE hFindFile搜索的文件句柄 函数执行的时候搜索的是此句柄的下一文件 　　
	LPWIN32_FIND_DATA lpFindFileData 指向一个用于保存文件信息的结构体

	返回值：
	如果调用成功返回一个句柄，可用来做为FindNextFile或 FindClose参数 　　
	调用失败 返回为INVALID_HANDLE_VALUE(即-1) ,可调用GetLastError来获取错误信息
	*/
	HANDLE hFind = FindFirstFile(szFind, &wfd); 
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{
		return -1; 
	}

	do 
	{ 
		if (wfd.cFileName[0] == '.') 
			continue; // 过滤这两个目录 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //属性是文件夹
		{ 
			TCHAR szFile[MAX_PATH]; 
			if (IsRoot(lpszPath)) 
				wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName); 
			else 
			{
				wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName); 

				searchfile(szFile,NameListVector); // 如果找到的是目录，则进入此目录进行递归 
			}
		} 
		else //如果是文件
		{ 
			
			TCHAR szFile[MAX_PATH]; 
			if (IsRoot(lpszPath)) //是根文件夹？？？
			{
				wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName); 
				
			}
			else 
			{
				wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName); 
				//注意，这里cFileName就是文件名
			
				if(IsVideo(szFile))
				{
		
					NameStruct Node;
			
					//string filename="F:/1/D1.JPG";//      "F://1//D1.JPG";      //"F:/1/D1.JPG";    //"F:\\1\\D1.JPG";

					string filename;
					char *lp = (char*)(lpszPath);
					//拼成一个全名
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
			// 对文件进行操作 
		} 
	} while (FindNextFile(hFind, &wfd)); 
	FindClose(hFind); // 关闭查找句柄 
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