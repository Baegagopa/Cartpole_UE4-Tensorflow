// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCarpole.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Root = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Root"));
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
	Pole->SetSimulatePhysics(false);
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

	CartpoleInfo.CartPos = 0;
	CartpoleInfo.Angle = 0;
	CartpoleInfo.CartSpeed = 0;
	CartpoleInfo.AngelChange = 0;
	CartpoleInfo.Done = 0;
	oldAngle = 0;
	cntStep = 0;
	isStarted = false;
	isTrigActing = true;
	isTrigReset = true;
	Movement = CreateDefaultSubobject <UFloatingPawnMovement>(TEXT("Movement"));

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerController = this->GetWorld()->GetFirstPlayerController();
	if (this->PlayerController && this->ViewTarget)
	{
		this->PlayerController->SetViewTarget(this->ViewTarget);
	}

	myClient = GWorld->SpawnActor<AMyClient>();// tcp

	rootOrignPos = Root->GetComponentLocation();
	poleOrignPos = Pole->GetComponentLocation();

}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (!isTrigActing)
	{
		ActingHandling();
	}
	if (!isTrigReset)
	{
		ResetEpisode();
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);
}

void AMyPawn::ResetEpisode()
{
	isTrigReset = true;//

	CartpoleInfo.CartPos = 0;
	CartpoleInfo.Angle = 0;
	CartpoleInfo.CartSpeed = 0;
	CartpoleInfo.AngelChange = 0;
	CartpoleInfo.Done = 0;

	oldAngle = 0;
	cntStep  = 0;
	
	Root->SetWorldLocation(rootOrignPos, false, nullptr, ETeleportType::TeleportPhysics);
	Pole->SetWorldLocation(poleOrignPos, false, nullptr, ETeleportType::TeleportPhysics);

	Root->SetPhysicsLinearVelocity(FVector(0.0F, 0.0F, 0.0F));

	Pole->SetWorldRotation(FQuat(0.0F, 0.0F, 0.0F, 0.0F));
	Pole->SetPhysicsAngularVelocity(FVector(0.0F, 0.0F, 0.0F));
	Pole->SetPhysicsLinearVelocity(FVector(0.0F, 0.0F, 0.0F));

	isStarted = false;
	isTrigActing = true;
	StartActing();
}

void AMyPawn::StartActing()
{
	Pole->SetSimulatePhysics(true);
	//Acting(1);
	ActingHandling();
	isStarted = true;
}

void AMyPawn::Acting(int dir)
{
	if (isStarted)
	{
		if(dir == 0)
			AddMovementInput(GetActorForwardVector(), moveSpeed, false);
		else
			AddMovementInput(GetActorForwardVector(), -moveSpeed, false);
	}
	isTrigActing = false;
	
}

void AMyPawn::ActingHandling()
{
	isTrigActing = true;
	float height = Weight->GetComponentLocation().Z - Cart->GetComponentLocation().Z;
	float x = Weight->GetComponentLocation().X - Cart->GetComponentLocation().X;
	float angle = atan2f(x, height);

	CartpoleInfo.CartPos = Root->GetComponentLocation().X * 0.01F;
	CartpoleInfo.Angle = angle;
	CartpoleInfo.CartSpeed = Root->GetComponentVelocity().X * 0.01F;
	CartpoleInfo.AngelChange = angle - oldAngle;
	CartpoleInfo.Done = 0;
	oldAngle = angle;


	float degree = FMath::RadiansToDegrees(angle);
	if (degree < failAngle && degree > -failAngle && cntStep < maxCnt)
	{

		MY_LOG(Warning, TEXT("%f , %f, %f, %f  T"),
			CartpoleInfo.CartPos * 100.0F,
			degree,
			CartpoleInfo.CartSpeed *100.0F,
			CartpoleInfo.AngelChange);

		myClient->SendData(
			CartpoleInfo.CartPos,
			CartpoleInfo.Angle,
			CartpoleInfo.CartSpeed,
			CartpoleInfo.AngelChange,
			CartpoleInfo.Done);
	}
	else
	{
		MY_LOG(Warning, TEXT("%f , %f, %f, %f  F"),
			CartpoleInfo.CartPos*100.0F,
			degree,
			CartpoleInfo.CartSpeed * 100.0F,
			CartpoleInfo.AngelChange);
		CartpoleInfo.Done = 1;

		myClient->SendData(
			CartpoleInfo.CartPos,
			CartpoleInfo.Angle,
			CartpoleInfo.CartSpeed,
			CartpoleInfo.AngelChange,
			CartpoleInfo.Done);

		isTrigReset = false;//ResetEpisode();
	}
}