// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Json.h"
#include "MyClient.generated.h"

#define PORT_NUM	 8080
#define MAX_MSG_LEN	 256
#define SERVER_IP	"127.0.0.1"

class AMyPawn;
UCLASS()
class UNREALCARPOLE_API AMyClient : public AActor
{
	GENERATED_BODY()
public:
	AMyClient();
	~AMyClient();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SendData(float CartPos, float Angle, float CartSpeed, float AngelChange, int Done);

	UFUNCTION()
	void RecvData();

	int isConnected;

	SOCKET sock;

	AMyPawn* myPawn;
};


