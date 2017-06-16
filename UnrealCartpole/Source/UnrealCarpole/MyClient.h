// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "MyClient.generated.h"

#define PORT_NUM	 8080
#define MAX_MSG_LEN	 256
#define SERVER_IP	"127.0.0.1"

UCLASS()
class UNREALCARPOLE_API UMyClient : public UObject
{
	GENERATED_BODY()
public:
	UMyClient();
	~UMyClient();

	UFUNCTION()
	void SendData();

	UFUNCTION()
	void RecvData();

	SOCKET sock;
};
