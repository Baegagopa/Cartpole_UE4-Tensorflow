// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "MyClient.h" 
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCARPOLE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UMyGameInstance();
public:
	virtual void Init() override;

	UPROPERTY()
	class UMyClient* TcpClient;

};
