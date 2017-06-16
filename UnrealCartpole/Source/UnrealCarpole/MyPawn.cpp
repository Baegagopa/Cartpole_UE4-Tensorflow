// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCarpole.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject <USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Cart = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Cart"));
	Cart->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	Cart->SetStaticMesh(StaticMesh_Cube.Object);
	Cart->SetWorldScale3D(FVector(1.0F, 1.0F, 0.1F));

	Sphere = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	Sphere->SetStaticMesh(StaticMesh_Sphere.Object);
	Sphere->SetWorldScale3D(FVector(0.1F, 0.1F, 0.1F));
	Sphere->SetRelativeLocation(FVector(0.0F, 0.0F, 10.0F));

	CartPolePhysics = CreateDefaultSubobject <UPhysicsConstraintComponent>(TEXT("CartPolePhysics"));
	CartPolePhysics->SetupAttachment(Sphere);
	CartPolePhysics->SetWorldScale3D(FVector(0.85F, 0.85F, 0.85F));

	Pole = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Pole"));
	Pole->SetupAttachment(CartPolePhysics);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Pole(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	Pole->SetStaticMesh(StaticMesh_Pole.Object);
	Pole->SetWorldScale3D(FVector(0.1F, 0.1F, 1.4F));
	Pole->SetRelativeLocation(FVector(-1.3F, 0.0F, 80.0F));
	Pole->SetSimulatePhysics(true);
	Pole->BodyInstance.bLockXRotation = true;
	Pole->BodyInstance.bLockZRotation = true;

	Weight = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Weight"));
	Weight->SetupAttachment(Pole);
	Weight->SetStaticMesh(StaticMesh_Sphere.Object);
	Weight->SetWorldScale3D(FVector(0.5F, 0.5F, 0.5F));
	Weight->SetRelativeLocation(FVector(0.0F, 0.0F, 50.0F));

	CartPolePhysics->AttachToComponent(Sphere, FAttachmentTransformRules::KeepRelativeTransform);
	CartPolePhysics->SetDisableCollision(true);
	CartPolePhysics->SetConstrainedComponents(Sphere, NAME_None, Pole, NAME_None);

	Movement = CreateDefaultSubobject <UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// 위치
	//CP_LOG(Warning, *FString::SanitizeFloat(Root->GetComponentLocation().X));
	AddMovementInput(GetActorForwardVector() * 0.8f, 0.1f, false);

	// 이동 속도
	//CP_LOG(Warning, *FString::SanitizeFloat(Root->GetComponentVelocity().X));
	float height = Weight->GetComponentLocation().Z - Cart->GetComponentLocation().Z;
	float x = Weight->GetComponentLocation().X - Cart->GetComponentLocation().X;
	float angle = atan2f(x, height);
	//CP_LOG(Warning, *FString::SanitizeFloat(angle));
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

}

