// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCarpole.h"
#include "MyGameInstance.h"


UMyGameInstance::UMyGameInstance()
{
	MY_LOG(Warning, TEXT("Constructor Call Start"));
	UE_LOG(LogClass, Warning, TEXT("%s"), TEXT("Game Instance Constructor Call!"));
	MY_LOG(Warning, TEXT("Constructor Call Start"));

	
}

void UMyGameInstance::Init()
{
	Super::Init();
	TcpClient = CreateDefaultSubobject<UMyClient>(TEXT("MyTcpClient"));
	MY_LOG(Warning, TEXT("Game Instance Init!"));
}
