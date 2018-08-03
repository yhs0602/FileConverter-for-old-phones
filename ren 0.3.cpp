//#define UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <gdiplus/gdiplus.h>
using namespace Gdiplus;
//#define TCHAR char
int del;
void RenameRecursive(TCHAR *path);

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
    //del=0;
    //int ans;
    //printf("delete?");
    //scanf("%c",&ans);
    //if((char)ans=='y'||(char)ans=='Y')del=1;
    printf("file renamer\n\n");
    getch();
    puts("enter folder name");
    gets(dir);
	/*if(del){
             system("del /f /s *.dsp");
             system("del /f /s *.sln ");
             system("del /f /s *.dsw ");
             system("del /f /s *.opt ");
             system("del /f /s *.vcproj ");
             system("del /f /s *.wav ");
             system("del /f /s *.ico ");
             system("del /f /s /A: H *.suo ");
//             system("del /f /s /A H *.suo ");
             //system("del /f /s *.bmp ");
             system("del /f /s *.dll ");
             system("del /f /s *.sys ");
             system("del /f /s *.lib ");
             system("del /f /s *.o ");
             system("del /f /s *.exe ");        
    }
	*/ 

    //GetCurrentDirectory(MAX_PATH,dir);
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
			TCHAR buf[MAX_PATH+50];
			sprintf(buf,"%s%s%s",drive,dir,wfd.cFileName);
			string ext=getExt(wfd.cFileName);
			if(strstr(OUTPUT_STR,ext.c_str())==NULL)
			{
				if(strstr(IMG_STR,ext.c_str()))
				{
					SaveJpg(buf,90);
				} else if(ext=="3gp")
				{
					sprintf(fname,"%s.k3g",buf);
					if(MoveFile(buf,fname)==0)
					{
						puts("fail");
						DWORD dw = GetLastError(); 
						LPVOID lpMsgBuf;
					    FormatMessage(
    					    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
   						     FORMAT_MESSAGE_FROM_SYSTEM |
    					    FORMAT_MESSAGE_IGNORE_INSERTS,
    					    NULL,
    					    dw,
    					    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
   						     (LPTSTR) &lpMsgBuf,
   						     0, NULL );
   						MessageBox(0,(LPCTSTR)lpMsgBuf,"error",MB_OK|MB_ICONINFORMATION);
					}
				}else if(strstr(ANI_STR,ext.c_str()))
				{
					sprintf(buf,"%c:\\TOOL\\prism.exe -convert \"%s\" -format .3gp MPEG4 128 256 320 240 AMR 10 -hide",MyDrive,buf);
				}else{
					sprintf(fname,"%s.txt",buf);
					//puts(fname);
					if(MoveFile(buf,fname)==0)
					{
						puts("fail");
						DWORD dw = GetLastError(); 
						LPVOID lpMsgBuf;
					    FormatMessage(
    					    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
   						     FORMAT_MESSAGE_FROM_SYSTEM |
    					    FORMAT_MESSAGE_IGNORE_INSERTS,
    					    NULL,
    					    dw,
    					    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
   						     (LPTSTR) &lpMsgBuf,
   						     0, NULL );
   						MessageBox(0,(LPCTSTR)lpMsgBuf,"error",MB_OK|MB_ICONINFORMATION);
					}

				}
			}
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
}
/*
G:\TOOL\prism.exe -convert "C:\Users\Administrator\Downloads\국어UCC_미스터리_피의기말고사_1차편집.mp4" -format .3gp MPEG4 128 256 320 240 AMR 10 -hide
*/

//{
	/*
	Using the Prism Command-line Tool
Contents | Previous | Next  Prism Video File Converter
Help v 2.63 
 


The Prism command-line tool allows you to convert files from the command line. This can be used to let other software convert files using Prism or to automate batch conversion.

Important Note: Due to a limitation in the way Windows handles running graphical programs from the command line, Prism cannot give any feedback when an issued command has been completed, nor can it block the command line while a conversion is taking place. We are currently investigating possible ways to circumvent the Windows problem, but please note it down for now and take it into account when issuing command-line orders to Prism. 

The Prism executable is usually located as "C:\Program Files\NCH Software\Prism\prism.exe"

Usage: prism.exe [options] [files to add] 

"Options" is where you can issue a series of commands to Prism. The options are: 

-convert - convert all files loaded into Prism. 

-clear - remove all files loaded into Prism. 

-addfolder [FOLDER_NAME] - add a folder of files into Prism. 

-format [FORMAT_NAME] - specify the output file format for which to convert the loaded files. Possible format names include: 

3gp
asf
avi
dv
flv
gif
mkv
mov
mp4
m4v
mpg
ogv
rm
swf
webm
wmv
divx
xvid
amv
jpg
png
"Android 480P"
"Android 720P"
"iPhone 2G"
"iPhone 3G"
"iPhone 3GS"
"iPhone 4"
"iPhone 4S"
"iPhone 5/5c"
"iPhone 5s/6/6P"
iPhone 6s/6sP
iPod Classic
"iPod Touch 1"
"iPod Touch 2"
"iPod Touch 3"
"iPod Touch 4"
"iPod Touch 5"
"iPad 1"
"iPad 2"
"iPad 3"
"iPad 4"
"iPad Air"
"iPad Air2"
"iPad Mini"
"iPad Mini2/3"
"iPad Mini4"
"PSP"
"PS Vita"
"PS3"
"Xbox 360"
mp3
wav

-outfolder [FOLDER_NAME] - specify the output folder to store the converted files. Do NOT end the folder path with a backslash '\' character. 

-overwrite [OVERWRITE_FLAG] - specifies what to do when the converted file already exists. The flags are "PROMPT" (Prompt for What to Do), "ALWAYS" (Overwrite the File), "NEVER" (Don't Convert the Original File), "CANCEL" (Abort All Conversions), "APPENDNUMBER [format]" (Append number to filename - [format] is optional, if blank it will use the format already specified in the program). 

-settings [FORMAT_NAME] [SETTINGS_OPTIONS] - specifies the settings configuration to use for the specified format. For more detail on what parameters to use, see further down the page. 

-edit [RESIZE_TYPE] [WIDTH] [HEIGHT] [FRAMERATE] - specifies the video output options. RESIZE_TYPE - either "NONE", "CROP" or "LETTERBOX". WIDTH and HEIGHT - desired dimensions of the video. FRAMERATE - desired video frame rate (or zero if no frame rate conversion is required). 

-hide - hide the Prism window. 

-show - show the Prism window. 

-exit - exit Prism. 

"Files to Add" is where you type in the files you want to load into Prism. All typed filenames must use the full filepath of the name, and circumfixed with inverted commas. 

Examples: 

prism.exe -format .mp4

prism.exe -format "iPhone 4"

prism.exe -outfolder "C:\My Videos";

prism.exe -hide

prism.exe "C:\My Documents\video.avi"

prism.exe -convert "C:\My Videos\video.avi" -format .mp4 -overwrite ALWAYS

prism.exe -convert "C:\My Videos\video.flv" -format .mpg -overwrite APPENDNUMBER "%filename%-%number%"

prism.exe -convert "DVD:C:\My Videos\Flying Daggers" -settings .mp4 iPod 512 768 23 -overwrite APPENDNUMBER "%filename%-%number%"

prism.exe -convert "DVD:C:\My Videos\Flying Daggers" -format "iPhone 4S" -settings .mp4 H264 512 768 23 -overwrite APPENDNUMBER "%filename%-%number%"

Unless you are in the Prism folder you will need to replace "prism.exe" from above with the full path "C:\Program Files\NCH Software\Prism\prism.exe" like this:

"C:\Program Files\NCH Software\Prism\prism.exe" -convert "C:\My Videos\video.avi" -format .mp4

Settings Options: 

The following details what parameters to enter when using the "-settings" options. Note the order in which the parameters are written is the order in which they must be typed. Additionally, all parameters must be entered, otherwise the command will not take effect.

Note the possible combination of values that can be entered depend largely on the codec, so you need to open Prism and determine beforehand what values you can use for the codec in question.

Also note that when converting for preset devices, mp4 is the only supported output format, so please always use .mp4 to set preset device settings.

.avi [VIDEO_ENCODER] [AVERAGE_BITRATE] [MAXIMUM_BITRATE] [QUALITY_RATEFACTOR] [SAMPLE_RATE] [CHANNELS] [SOUND_BITRATE] 

VIDEO_ENCODER - either "MPEG4", "H264", "DVPAL" or "DVNTSC"
AVERAGE_BITRATE - average bitrate in kbits per sec (ignored when using H264 or DV)
MAXIMUM_BITRATE - maximum bitrate in kbits per sec (ignored when using H264 or DV)
QUALITY_RATEFACTOR - quality (ratefactor) in the range 51.0 to 1.0 (only used by H264)
SAMPLE_RATE - entered in Hz e.g. 8000
CHANNELS - either 1 or 2
SOUND_BITRATE - sound bitrate in kbits per sec (ignored when using DV)
Examples: 

-settings .avi MPEG4 768 1000 0.0 44100 2 128
-settings .avi DVPAL 0 0 0.0 48000 2 0
-settings .avi H264 0 0 24.0 48000 2 256

.wmv and .asf [VIDEO_ENCODER] [VIDEO_BITRATE] [SOUND_ENCODER_INDEX] [SAMPLE_RATE] [CHANNELS] [SOUND_BITRATE] 

VIDEO_ENCODER - either "WMV8" or "WMV9"
VIDEO_BITRATE - video bitrate in kbits per sec
SOUND_ENCODER_INDEX - zero based index of the sound encoder
SAMPLE_RATE - entered in Hz e.g. 48000
CHANNELS - either 1 or 2
SOUND_BITRATE - sound bitrate in kbits per sec
Examples: 

-settings .wmv WMV8 512 1 32000 2 32
-settings .asf WMV9 1024 2 44100 2 48

.mpg [VIDEO_ENCODER] [AVERAGE_BITRATE] [MAXIMUM_BITRATE] [SAMPLE_RATE] [SOUND_BITRATE] 

VIDEO_ENCODER - either "MPEG2" or "MPEG1"
AVERAGE_BITRATE - average bitrate in kbits per sec
MAXIMUM_BITRATE - maximum bitrate in kbits per sec
SAMPLE_RATE - entered in Hz e.g. 44100
SOUND_BITRATE - sound bitrate in kbits per sec
Examples: 

-settings .mpg MPEG2 4000 9000 44100 128

.3gp [VIDEO_ENCODER] [AVERAGE_BITRATE] [MAXIMUM_BITRATE] [WIDTH] [HEIGHT] [SOUND_ENCODER] [SOUND_BITRATE] 

VIDEO_ENCODER - either "H263" or "MPEG4"
AVERAGE_BITRATE - average bitrate in kbits per sec
MAXIMUM_BITRATE - maximum bitrate in kbits per sec
WIDTH - desired width of the video (ignored when using MPEG4)
HEIGHT - desired height of the video (ignored when using MPEG4)
SOUND_ENCODER - either "AMR" or "AAC"
SOUND_BITRATE - sound bitrate in kbits per sec
Examples: 

-settings .3gp H263 128 256 176 144 AMR 7.40
-settings .3gp MPEG4 768 1000 0 0 AAC 64

.mp4 [VIDEO_ENCODER] [AVERAGE_BITRATE] [MAXIMUM_BITRATE] [QUALITY_RATEFACTOR] [SOUND_BITRATE] 

VIDEO_ENCODER - either "H264", "MPEG4" or "PSP" (if converting for preset devices, do not use "MPEG4")
AVERAGE_BITRATE - average bitrate in kbits per sec (ignored if H264 or PSP)
MAXIMUM_BITRATE - maximum bitrate in kbits per sec (ignored if H264 or PSP)
QUALITY_RATEFACTOR - quality (ratefactor) in the range 51.0 to 1.0 (ignored if MPEG4)
SOUND_BITRATE - sound bitrate in kbits per sec
Examples: 

-settings .mp4 H264 0 0 23.0 32
-settings .mp4 MPEG4 768 1000 0.0 64
-settings .mp4 PSP 0 0 18.5 112

.mov [QUALITY_RATEFACTOR] [SOUND_BITRATE] 

QUALITY_RATEFACTOR - Quality (ratefactor) in the range 51.0 to 1.0
SOUND_BITRATE - sound bitrate in kbits per sec
Examples: 

-settings .mov 23.0 128
-settings .mov 30.6 64

.flv [VIDEO_ENCODER] [AVERAGE_BITRATE] [MAXIMUM_BITRATE] [QUALITY_RATEFACTOR] [SOUND_BITRATE] 

VIDEO_ENCODER - either "H264" or "FLV1"
AVERAGE_BITRATE - average bitrate in kbits per sec (ignored if H264)
MAXIMUM_BITRATE - maximum bitrate in kbits per sec (ignored if H264)
QUALITY_RATEFACTOR - quality (ratefactor) in the range 51.0 to 1.0 (ignored if FLV1)
SOUND_BITRATE - sound bitrate in kbits per sec
Examples: 

-settings .flv FLV1 128 256 0.0 32
-settings .flv H264 0 0 21.0 64

 
 
Using the Prism Command-line Tool 
Contents | Previous | Next  ⓒ NCH Software
NCH Software  
 

	*/
	/*
	Prism 명령줄 도구는, 명령줄에서 당신이 파일을 변환할 수 있게 해줍니다. 이것은 다른 소프트웨어가 Prism으로 파일을 변환하거나 자동 일괄 변환을 하는데 이용될 수 있습니다.

주의 사항: 윈도우가 명령줄에서 그래픽 프로그램 실행하는 방식에 대한 한계 때문에, Prism은 이미 명령이 실행 종료된 파일에 대해서는 피드백을 제공할 수 없고, 또한 변환이 시작된 파일의 명령줄을 차단할 수 없습니다. 저희는 현재 이런 윈도우에서의 문제를 보완하기 위해 노력 중이니, 현재 Prism에서 명령줄을 실행하실 때 이런 점에 유의하시기 바랍니다. 

Prism 실행 프로그램은 보통 "C:\Program Files\NCH Software\Prism\prism.exe"에 위치해 있습니다.

사용량: prism.exe [옵션] [추가할 파일] 

"옵션" 이 Prism이 일련의 명령들을 만들 수 있는 곳입니다. 옵션들은 다음과 같습니다: 

- 변환 -Prism으로 가져온 모든 파일을 변환 

-제거 - Prism으로 가져온 모든 파일을 제거 

-폴더 추가 [폴더_이름] Prism에 파일이 담긴 폴더 추가 

-포맷 [포맷_이름] -불러온 파일을 변환하는데 사용할 출력 파일 포맷을 지정하세요. 사용 가능한 형식명은 다음과 같습니다: 

avi
wmv
asf
mpg
3gp
mp4
mov
flv
swf
ogv
webm
rm
gif
dv
mkv
jpg
png
"앤드로이드 480P"
"앤드로이드 720P"
"iPhone 2G"
"iPhone 3G"
"iPhone 3G"
"iPhone 4"
"iPhone 4S"
"iPhone 5/5c"
"iPhone 5s/6/6P"
iPhone 6s/6sP
iPod Classic
"iPod Touch 1"
"iPod Touch 2"
"iPod Touch 3"
"iPod Touch 4"
"iPod Touch 5"
"iPad 1"
"iPad 2"
"iPad 3"
"iPad 4"
"iPad Air"
"iPad Air2"
"iPad Mini"
"iPad Mini2/3"
"iPad Mini4"
"PSP"
"PS Vita"
"PS3"
"Xbox 360"
mp3
wav

-출력 폴더 [폴더_이름] -변환한 파일을 저장할 출력 폴더를 지정하세요. 폴더 경로를 백슬래시 문자 '\'로 끝내지 마십시오. 

-덮어쓰기 [덮어쓰기_표시] -변환된 파일이 이미 존재할 때 무엇을 할지 지정합니다. 표시들은 "알림T" (무엇을 할지 알림), "항상 덮어쓰기" (파일 덮어쓰기), "절대 변환하지 않기" (원본파일을 변환하지 않음), "취소" (변환을 모두 중지), "번호첨부[포맷]" (파일이름에 번호 첨부 - [포맷]은 부가적인 것으로, 아무 표시도 없을 경우 프로그램에서 이미 지정한 포맷을 사용할 것입니다. 

-설정 [포맷_이름] [설정_옵션] -명시된 포맷으로 사용하기위해 설정 구성을 지정합니다. 어떤 파라미터를 사용해야 하는지에 대한 더 자세한 사항을 보려면 아래 웹 페이지를 참조하세요. 

-편집 [크기 조절_유형] [넓이] [높이] [프레임율] -비디오 출력 옵션을 지정 크기 조절_유형- "그대로 두기", "잘라내기" 나 "레터박스". 넓이와 높이 - 비디오의 원하는 크기 프레임율 -원하는 비디오 프레임 비율 (프레임 비율 변환이 필요없을 경우는 0). 

-숨기기 - Prism 창을 숨깁니다. 

-보여주기 - Prism 창을 보여줍니다. 

-종료 - Prism 종료 

"파일 추가" 는 Prism에 가져오기 원하는 파일에 작성하는 곳입니다. 입력하신 모든 파일 이름들은 그 이름의 전체 경로를 사용해야하고 " (따옴표)가 있어야합니다. 

예: 

prism.exe -형식 .mp4

prism.exe -형식"iPhone 4"

prism.exe -출력 폴더 "C:\My Videos";

prism.exe -숨기기

prism.exe "C:\My Documents\video.avi"

prism.exe -변환 "C:\My Videos\video.avi" -형식 .mp4 -항상 덮어쓰기

prism.exe -변환 "C:\My Videos\video.flv" -형식 .mpg -덮어쓰기 번호추가 "%filename%-%number%"

prism.exe -변환 "DVD:C:\My Videos\Flying Daggers" -설정 .mp4 iPod 512 768 23 -덮어쓰기 번호추가 "%filename%-%number%"

prism.exe -변환 "DVD:C:\My Videos\Flying Daggers" -형식 "iPhone 4S" -설정 .mp4 H264 512 768 23 -덮어쓰기 번호추가 "%filename%-%number%"

당신이 Prism 폴더 안에 있지 않는 한, 위의 "prism.exe" 를, 전체 경로 "C:\Program Files\NCH Software\Prism\prism.exe"와 함께 이렇게 교체해야 합니다.:

"C:\Program Files\NCH Software\Prism\prism.exe" -변환 "C:\My Videos\video.avi" -형식 .mp4

설정 옵션: 

아래는 "-설정" 옵션을 사용시 어떤 파라미터를 입력해야 하는지에 대한 세부 정보입니다. 파라미터가 쓰여진 순서대로 그것들을 입력해야 함을 유의하세요. 추가적으로, 파라미터들은 전부 다 입력하셔야 합니다. 그렇게 하지 않으면 명령 실행이 무효합니다.

입력할 수 있는 가능한 조합의 값은 주로 코덱에 좌우됨을 유의하세요. 그러므로 Prism을 여신 후에 어떤 값으로 코덱을 사용할 것인지를 미리 결정해야합니다.

프리셋 장치를 위해 변환할 때, 오직 mp4 만이 지원되는 출력 형식임을 유의하시고, 프리셋 장치 설정을 하실때 항상 .mp4를 사용하세요.

.avi [비디오_인코더] [평균_비트레이트] [최대_비트레이트] [품질_평가 요소] [샘플_레이트] [채널] [사운드_비트레이트] 

비디오_인코더 - "MPEG4", "H264", "DVPAL" 나 "DVNTSC"
평균­비트레이트 - 초당 kbits의 평균 비트레이트 (H264 나 DV 사용시엔 적용되지 않음)
최대­비트레이트 - 초당 kbits의 최대 비트레이트 (H264 나 DV 사용시엔 적용되지 않음)
품질_ 평가 요소 - 51.0 에서 1.0까지의 품질 평가 요소 (H264 사용시에만 적용)
샘플_레이트 - Hz로 입력 (예:8000)
채널 - 1 이나 2
사운드­비트레이트 - 초당 kbits의 사운드 비트레이트 (DV 사용시엔 적용되지 않음)
예: 

-설정 .avi MPEG4 768 1000 0.0 44100 2 128
-설정 .avi DVPAL 0 0 0.0 48000 2 0
-설정 .avi DVPAL 0 0 0.0 48000 2 0

.wmv 와 .asf [비디오_인코더] [비디오_비트레이트] [사운드_인코더_지수] [샘플_레이트] [채널] [사운드_비트레이트] 

비디오_인코더 - "WMV8" 나 "WMV9"
비디오_비트레이트 - 초당 kbits의 비디오 비트레이트
사운드_인코더_지수 - 제로 베이스 지수의 사운드 인코더
샘플_레이트 - Hz로 입력 (예:48000)
채널 - 1 이나 2
사운드_비트레이트 - 초당 kbits의 사운드 비트레이트
예: 

-설정 .wmv WMV8 512 1 32000 2 32
-설정 .asf WMV9 1024 2 44100 2 48

.mpg [비디오_인코더] [평균_비트레이트] [최대_비트레이트][샘플_레이트] [사운드_비트레이트] 

비디오_인코더 - "MPEG2" 나 "MPEG1"
평균_비트레이트 - 초당 kbits의 평균 비트레이트
최대_비트레이트 - 초당 kbits의 최대 비트레이트
샘플_레이트 - Hz로 입력 (예:44100)
사운드_비트레이트 - 초당 kbits의 사운드 비트레이트
예: 

-설정 .mpg MPEG2 4000 9000 44100 128

.3gp [비디오_인코더] [평균_비트레이트] [최대_비트레이트] [넓이] [높이] [사운드_인코더] [사운드_비트레이트] 

비디오_인코더 - "H263" 나 "MPEG4"
평균_비트레이트 - 초당 kbits의 평균 비트레이트
최대_비트레이트 - 초당 kbits의 최대 비트레이트
넓이 - 원하는 넓이의 비디오 (MPEG4 사용시엔 적용되지 않음)
높이 - 원하는 높이의 비디오 (MPEG4 사용시엔 적용되지 않음)
사운드_인코더 - "AMR" 나 "AAC"
사운드_비트레이트 - 초당 kbits의 사운드 비트레이트
예: 

-설정 .3gp H263 128 256 176 144 AMR 7.40
-설정 .3gp MPEG4 768 1000 0 0 AAC 64

.mp4 [비디오_인코더] [평균_비트레이트] [최대_비트레이트][품질_평가 요소] [사운드_비트레이트] 

비디오_인코더 - "H264", "MPEG4" 나 "PSP" (프리셋 장치를 위해 변환시, "MPEG4"를 사용하지 마세요.)
평균­비트레이트 - 초당 kbits의 평균 비트레이트 (H264 나 PSP 사용시엔 적용되지 않음)
최대­비트레이트 - 초당 kbits의 최대 비트레이트 (H264 나 PSP 사용시엔 적용되지 않음)
품질_ 평가 요소 - 51.0 에서 1.0까지의 품질 평가 요소 (MPEG4 사용시엔 적용되지 않음)
사운드_비트레이트 - 초당 kbits의 사운드 비트레이트
예: 

-설정 .mp4 H264 0 0 23.0 32
-설정 .mp4 MPEG4 768 1000 0.0 64
-설정 .mp4 PSP 0 0 18.5 112

.mov [품질_평가 요소] [사운드_비트레이트] 

품질_ 평가 요소 - 51.0 에서 1.0까지의 품질 평가 요소
사운드_비트레이트 - 초당 kbits의 사운드 비트레이트
예: 

-설정 .mov 23.0 128
-설정 .mov 30.6 64

.flv [비디오_인코더] [평균_비트레이트] [최대_비트레이트][품질_평가 요소] [사운드_비트레이트] 

비디오_인코더 - "H264" 나 "FLV1"
평균­비트레이트 - 초당 kbits의 평균 비트레이트 (H264 사용시엔 적용되지 않음)
최대­비트레이트 - 초당 kbits의 최대 비트레이트 (H264 사용시엔 적용되지 않음)
품질_ 평가 요소 - 51.0 에서 1.0까지의 품질 평가 요소 (FLV 일때는 적용되지 않음)
사운드_비트레이트 - 초당 kbits의 사운드 비트레이트
예: 

-설정 .flv FLV1 128 256 0.0 32
-설정 .flv H264 0 0 21.0 64
*/
//}
 /*

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)

{

     HDC hdc;

     PAINTSTRUCT ps;

     TCHAR *Mes="B:비트맵 읽기, P:PNG로 저장, J:Jpeg로 저장, 1~5:파라미터 목록 출력";

     OPENFILENAME OFN;

     TCHAR lpstrFile[MAX_PATH]="";

 

     switch(iMessage) {

     case WM_CREATE:

          hWndMain=hWnd;

          return 0;

     case WM_KEYDOWN:

          switch (wParam) {

          case 'B':

              memset(&OFN, 0, sizeof(OPENFILENAME));

              OFN.lStructSize = sizeof(OPENFILENAME);

              OFN.hwndOwner=hWnd;

              OFN.lpstrFilter="비트맵 파일(*.BMP)\0*.BMP\0";

              OFN.lpstrFile=lpstrFile;

              OFN.nMaxFile=MAX_PATH;

              if (GetOpenFileName(&OFN)!=0) {

                   lstrcpy(BmpPath,lpstrFile);

                   InvalidateRect(hWnd,NULL,TRUE);

              }

              break;

          case 'P':

              if (lstrlen(BmpPath) != 0) {

                   SavePng(BmpPath);

                   MessageBox(hWnd,"PNG 포맷으로 변경했습니다","알림",MB_OK);

              }

              break;

          case 'J':

              if (lstrlen(BmpPath) != 0) {

                   SaveJpg(BmpPath,0);

                   SaveJpg(BmpPath,5);

                   SaveJpg(BmpPath,10);

                   SaveJpg(BmpPath,50);

                   SaveJpg(BmpPath,100);

                   MessageBox(hWnd,"JPG 포맷으로 변경했습니다","알림",MB_OK);

              }

              break;

          case '1':

              PrintParaList(L"image/jpeg");

              break;

          case '2':

              PrintParaList(L"image/png");

              break;

          case '3':

              PrintParaList(L"image/bmp");

              break;

          case '4':

              PrintParaList(L"image/gif");

              break;

          case '5':

              PrintParaList(L"image/tiff");

              break;

          }

          return 0;

     case WM_PAINT:

          hdc=BeginPaint(hWnd, &ps);

          TextOut(hdc,0,0,Mes,lstrlen(Mes));

          OnPaint(hdc);

          EndPaint(hWnd, &ps);

          return 0;

     case WM_DESTROY:

          PostQuitMessage(0);

          return 0;

     }

     return(DefWindowProc(hWnd,iMessage,wParam,lParam));

}
 
*/

 
/*
void OnPaint(HDC hdc)

{

     Graphics G(hdc);

     WCHAR ImagePath[MAX_PATH];

 

     if (lstrlen(BmpPath) != 0) {

          MultiByteToWideChar(CP_ACP, 0, BmpPath, -1, ImagePath, MAX_PATH);

          Image I(ImagePath);

          G.DrawImage(&I,0,20);

     }

}
*/
 /*

void PrintParaList(WCHAR *MimeType)

{

     HDC hdc;

     CLSID Clsid;

     UINT size,i;

     EncoderParameters *pPara;

     WCHAR ParaGuid[39];

     TCHAR str[128];

     int y=0;

 

     if (GetEncCLSID(MimeType,&Clsid) == FALSE) {

          return;

     }

     hdc=GetDC(hWndMain);

     Bitmap *pB=new Bitmap(1,1);

     size=pB->GetEncoderParameterListSize(&Clsid);

     if (size == 0) {

          wsprintf(str, "MIME : %S, 파라미터 없음", MimeType);

          TextOut(hdc,0,y+=20,str,lstrlen(str));

     } else {

          pPara=(EncoderParameters *)malloc(size);

          pB->GetEncoderParameterList(&Clsid,size,pPara);

 

          wsprintf(str, "MIME : %S, 파라미터 개수 : %d", MimeType, pPara->Count);

          TextOut(hdc,0,y+=20,str,lstrlen(str));

          for (i=0;i<pPara->Count;i++) {

              StringFromGUID2(pPara->Parameter[i].Guid,ParaGuid,39);

              wsprintf(str,"GUID:%S, 타입:%d, 개수:%d",ParaGuid,

                   pPara->Parameter[i].Type,pPara->Parameter[i].NumberOfValues);

               TextOut(hdc,0,y+=20,str,lstrlen(str));

          }

 

          free(pPara);

     }

     delete(pB);

     ReleaseDC(hWndMain,hdc);

}
*/
 
/*
void SavePng(TCHAR *Path)

{

     WCHAR ImagePath[MAX_PATH];

     TCHAR NewName[MAX_PATH];

     Image *pI;

     CLSID Clsid;

     TCHAR drive[_MAX_DRIVE];

     TCHAR dir[_MAX_DIR];

     TCHAR fname[_MAX_FNAME];

     TCHAR ext[_MAX_EXT];

 

     MultiByteToWideChar(CP_ACP, 0, Path, -1, ImagePath, MAX_PATH);

     pI=Image::FromFile(ImagePath);

     GetEncCLSID(L"image/png",&Clsid);

     _splitpath(Path,drive,dir,fname,ext);

     wsprintf(NewName,"%s%s%s.png",drive,dir,fname);

     MultiByteToWideChar(CP_ACP, 0, NewName, -1, ImagePath, MAX_PATH);

     pI->Save(ImagePath,&Clsid,NULL);

     delete pI;

}
*/
 
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
