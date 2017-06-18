// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyClient.h" 
#include "MyPawn.generated.h"


UCLASS()
class UNREALCARPOLE_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Root")
	class USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Cart")
	class UStaticMeshComponent* Cart;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Sphere")
	class UStaticMeshComponent* Sphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Weight")
	class UStaticMeshComponent* Weight;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Pole")
	class UStaticMeshComponent* Pole;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "CartPolePhysics")
	class UPhysicsConstraintComponent* CartPolePhysics;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Movement")
	class UFloatingPawnMovement* Movement;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "InpudValue")
	float MoveSpeed;
	
	UPROPERTY()
	class AMyClient* TcpClient;

	void StartTCP();
};
