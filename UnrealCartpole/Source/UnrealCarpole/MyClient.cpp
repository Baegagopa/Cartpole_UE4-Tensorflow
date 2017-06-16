// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCarpole.h"
#include "MyClient.h"


UMyClient::UMyClient()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//윈속 초기화           

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓 생성

	SOCKADDR_IN servaddr = { 0 };//소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));//연결 요청

}

UMyClient::~UMyClient()
{
	closesocket(sock);//소켓 닫기
	WSACleanup();//윈속 해제화
}

void UMyClient::SendData()
{
	char msg[MAX_MSG_LEN] = "";

	gets_s(msg, MAX_MSG_LEN);
	send(sock, msg, sizeof(msg), 0);//송신
	if (strcmp(msg, "exit") == 0) { return; }
	recv(sock, msg, sizeof(msg), 0);//수신
	printf("수신:%s\n", msg);

}

void UMyClient::RecvData()
{

}