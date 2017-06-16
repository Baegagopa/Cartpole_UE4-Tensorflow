// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCarpole.h"
#include "MyClient.h"


UMyClient::UMyClient()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//���� �ʱ�ȭ           

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���� ����

	SOCKADDR_IN servaddr = { 0 };//���� �ּ�
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));//���� ��û

}

UMyClient::~UMyClient()
{
	closesocket(sock);//���� �ݱ�
	WSACleanup();//���� ����ȭ
}

void UMyClient::SendData()
{
	char msg[MAX_MSG_LEN] = "";

	gets_s(msg, MAX_MSG_LEN);
	send(sock, msg, sizeof(msg), 0);//�۽�
	if (strcmp(msg, "exit") == 0) { return; }
	recv(sock, msg, sizeof(msg), 0);//����
	printf("����:%s\n", msg);

}

void UMyClient::RecvData()
{

}