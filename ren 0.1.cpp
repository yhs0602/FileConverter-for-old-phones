//#define UNICODE 1

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#define TCHAR char
int del;
void RenameRecursive(TCHAR *path);

char cmd[256];
/*
void ren()
{
	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	TCHAR fname[MAX_PATH];
	BOOL bResult=TRUE;
	TCHAR Path[MAX_PATH];
  //struct _finddata_t file;// file검색을 위한 구조체 정의
  int flag=-1;
  int handle=-1;              // 검색 결과 보관 핸들
  //system("ren *.c *c.txt");
  //system("ren *.cpp *cpp.txt");
  //system("ren *.rc *rc.txt");
  //system("ren *.h *h.txt");
  //system("ren *.inf *inf.txt");
  //system("ren *.ini *ini.txt");
  //system("ren *.java *java.txt");
  //system("ren *.asm *asm.txt");
  system(cmd);
  handle=_findfirst("*.*",&file);
  if (!handle) {        // file이 하나도 없을 경우
     printf("file Not found !!\n");
     goto end;
  }
  do {        // 더이상 없을 때까지 계속 찾음
        //printf("found %s\n",file.name); 
        if((file.attrib&_A_SUBDIR)){      //dir이라면
                                 if(file.name[0]!='.'){
                                                       chdir(file.name);
                                                       printf(">>directory %s\n",file.name);
                                                       ren();
                                 }
        }  
        flag=_findnext(handle,&file);     // 다음 file을 찾음
  }while (flag!=-1);
  end:
      printf("return\n");
      _findclose(handle);
      chdir(".."); 
}
*/

int main(int argc, char *argv[])
{
	if(argc>1){
		strncpy(cmd,argv[1],255);
		puts(cmd);
	}
		
    del=0;
    int ans;
    printf("delete?");
    scanf("%c",&ans);
    if((char)ans=='y'||(char)ans=='Y')del=1;
    printf("file renamer\n\n");
    if(del){
             system("del /f /s *.dsp");
             system("del /f /s *.sln ");
             system("del /f /s *.dsw ");
             system("del /f /s *.opt ");
             system("del /f /s *.vcproj ");
             system("del /f /s *.wav ");
             system("del /f /s *.ico ");
             system("del /f /s /A: H *.suo ");
//             system("del /f /s /A H *.suo ");
             system("del /f /s *.bmp ");
             system("del /f /s *.dll ");
             system("del /f /s *.sys ");
             system("del /f /s *.lib ");
             system("del /f /s *.o ");
             system("del /f /s *.exe ");        
    } 
    TCHAR dir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,dir);
    RenameRecursive(dir);
    system("PAUSE");	
    return 0;
}


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
	hSrch=FindFirstFile(path,&wfd);
	if (hSrch==INVALID_HANDLE_VALUE)
	{
		puts("error");
	 	return;
	}
	_splitpath(path,drive,dir,NULL,NULL);
	//lstrcat(ext,".txt");
	while (bResult) {
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
				sprintf(newpath,"%s%s%s\\*.*",drive,dir,wfd.cFileName);
				RenameRecursive(newpath);
			}
		} else {
			sprintf(fname,"%s.txt",wfd.cFileName);
			puts(fname);
			MoveFile(wfd.cFileName,fname);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
}
