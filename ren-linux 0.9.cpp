#include <stdio.h>
#include <stdlib.h>
#include <io.h>
int del;

void find()
{
  struct _finddata_t file;// file검색을 위한 구조체 정의
  int flag=-1;
  int handle=-1;              // 검색 결과 보관 핸들
  system("ren *.c *c.txt");
  system("ren *.cpp *cpp.txt");
  system("ren *.rc *rc.txt");
  system("ren *.h *h.txt");
  system("ren *.inf *inf.txt");
  system("ren *.ini *ini.txt");
  system("ren *.java *java.txt");
  handle=_findfirst("*.*",&file);
  if (!handle) {        // file이 하나도 없을 경우
     printf("file Not found !!\n");
     goto end;
  }
  do {        // 더이상 없을 때까지 계속 찾음
        printf("found %s\n",file.name); 
        if((file.attrib&_A_SUBDIR)){      //dir이라면
                                 if(file.name[0]!='.'){
                                                       chdir(file.name);
                                                       printf(">>directory %s\n",file.name);
                                                       find();
                                 }
        }  
        flag=_findnext(handle,&file);     // 다음 file을 찾음
  }while (flag!=-1);
  end:
      printf("return\n");
      _findclose(handle);
      chdir(".."); 
}

int main(int argc, char *argv[])
{
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
    find();
    system("PAUSE");	
    return 0;
}
