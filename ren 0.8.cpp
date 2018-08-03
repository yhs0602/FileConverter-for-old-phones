//#define UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <gdiplus/gdiplus.h>
using namespace Gdiplus;
int del;
void RenameRecursive(TCHAR *path);
void HandleError();

//예 제 : ImageConverter
 
TCHAR BmpPath[MAX_PATH];

BOOL GetEncCLSID(WCHAR *mime, CLSID *pClsid)

{

     UINT num,size,i;

     ImageCodecInfo *arCod;

     BOOL bFound=FALSE;

 

     GetImageEncodersSize(&num,&size);

     arCod=(ImageCodecInfo *)malloc(size);

     GetImageEncoders(num,size,arCod);

 

     for (i=0;i<num;i++) {

          if(wcscmp(arCod[i].MimeType,mime)==0) {

               *pClsid=arCod[i].Clsid;

               bFound=TRUE;

               break;

          }    

     }

     free(arCod);

     return bFound;

}


void SaveJpg(TCHAR *Path,ULONG Quality)

{

     WCHAR ImagePath[MAX_PATH];

     TCHAR NewName[MAX_PATH];

     Image *pI;

     CLSID Clsid;

     TCHAR drive[_MAX_DRIVE];

     TCHAR dir[_MAX_DIR];

     TCHAR fname[_MAX_FNAME];

     TCHAR ext[_MAX_EXT];

     EncoderParameters Para;

     MultiByteToWideChar(CP_ACP, 0, Path, -1, ImagePath, MAX_PATH);

     pI=Image::FromFile(ImagePath);
     
     GetEncCLSID(L"image/jpeg",&Clsid);
     
     _splitpath(Path,drive,dir,fname,ext);

     wsprintf(NewName,"%s%s%s_Q_%03d.jpg",drive,dir,fname,Quality);

     MultiByteToWideChar(CP_ACP, 0, NewName, -1, ImagePath, MAX_PATH);

     Para.Count=1;

     Para.Parameter[0].Guid=EncoderQuality;

     Para.Parameter[0].Type=EncoderParameterValueTypeLong;

     Para.Parameter[0].NumberOfValues=1;

     Para.Parameter[0].Value=&Quality;

     pI->Save(ImagePath,&Clsid,&Para);

     delete pI;

}


char cmd[256];
//char MyPath[MAX_PATH];
char MyDrive;
int main(int argc, char *argv[])
{
	if(argc>1){
		strncpy(cmd,argv[1],255);
		puts(cmd);
	}
	TCHAR dirt[MAX_PATH];
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,dir);
	//MessageBox(0,MyPath,"OK",MB_OK);
	MyDrive=dir[0];
	ULONG_PTR gpToken;

    GdiplusStartupInput gpsi;

    if (GdiplusStartup(&gpToken,&gpsi,NULL) != Ok) {

          MessageBox(NULL,"GDI+ 라이브러리를 초기화할 수 없습니다.","알림",MB_OK);

          return 0;

     }	

    printf("file renamer\n\n");
    puts("enter folder name");
/*
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=NULL;
	OFN.lpstrFilter=TEXT("모든 파일(*.*)\0*.*\0");
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=MAX_PATH;
	if (GetOpenFileName(&OFN)!=0) {
		wsprintf(str,TEXT("%s 파일을 선택했습니다."),OFN.lpstrFile);
		//MessageBox(hWnd,str,TEXT("파일 열기 성공"),MB_OK);
	}
*/
    gets(dirt);
    memset(dir,0,MAX_PATH);
    int len=lstrlen(dirt);
    int j=0;
    for(int i=0;i<len;++i)
    {
    	if(dirt[i]!='\"')
    	{
    		dir[j]=dirt[i];
    		++j;
		}
	}
    //if(MessageBox(0,dir,dirt,MB_YESNO)!=MB_OK)
    //	exit(-1);
    RenameRecursive(dir);
    system("PAUSE");	
    return 0;
}

#include <string>
using namespace std;

string getExt(string pathName)
{
	return pathName.substr(pathName.find_last_of(".")+1);
}

#define IMG_STR "png tiff bmp tga"
#define ANI_STR "mp4 skm avi 3gp asf dv flv gif mkv mov m4v mpg ogv rm swf webm wmv divx xvid amv"

#define INPUT_STR ""
#define OUTPUT_STR "txt jpg k3g"

void RenameRecursive(TCHAR *path)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	TCHAR fname[MAX_PATH];
	BOOL bResult=TRUE;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[MAX_PATH];
	TCHAR newpath[MAX_PATH];
	//TCHAR ext[_MAX_EXT];
	//puts(path);
	GetLastError();
	hSrch=FindFirstFile(path,&wfd);
	if (hSrch==INVALID_HANDLE_VALUE)
	{
		puts("error to acquire findfilehandle");
		HandleError();
	 	return;
	}
	_splitpath(path,drive,dir,NULL,NULL);
	//lstrcat(ext,".txt");
	while (bResult) {
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
				wsprintf(newpath,"%s%s%s\\*.*",drive,dir,wfd.cFileName);
				RenameRecursive(newpath);
			}
		} else {
			TCHAR buf[MAX_PATH+50];
			wsprintf(buf,"%s%s%s",drive,dir,wfd.cFileName);
			string sext=getExt(wfd.cFileName);
			char ext[10];
			memset(ext,0,10);
			strcpy(ext,sext.c_str());
			strlwr(ext);
			if(strstr(OUTPUT_STR,ext)==NULL)
			{
				if(strstr(IMG_STR,ext))
				{
					SaveJpg(buf,100);
				} else if(ext=="3gp")
				{
					wsprintf(fname,"%s.k3g",buf);
					if(MoveFile(buf,fname)==0)
					{
						puts("fail to rename 3gp to k3g");
						HandleError();
					}
				}else if(strstr(ANI_STR,ext))
				{
					wsprintf(buf,"%c:\\TOOL\\prism.exe -convert \"%s\" -format .3gp MPEG4 128 256 320 240 AMR 10 -hide",MyDrive,buf);
				}else{
					wsprintf(fname,"%s.txt",buf);
					//puts(fname);
					if(MoveFile(buf,fname)==0)
					{
						puts("fail to rename to txt");
					    HandleError();
					}

				}
			}
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
}
void HandleError()
{
	TCHAR errMes[1024];
	TCHAR str[1024];
	int err;
	err=GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,err,	0,errMes,1024,NULL);
	wsprintf(str,"에러코드=%d, 메시지=%s",err,errMes);
	MessageBox(0,str,"에러",MB_OK);
}
		
