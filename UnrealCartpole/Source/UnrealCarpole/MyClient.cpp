// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCarpole.h"
#include "MyPawn.h"
#include "MyClient.h"


AMyClient::AMyClient()
{
	PrimaryActorTick.bCanEverTick = true;
	isConnected = -1;
}

AMyClient::~AMyClient()
{
	closesocket(sock);
	WSACleanup();
}

void AMyClient::BeginPlay()
{
	Super::BeginPlay();
	
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//扩加 檬扁拳           

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//家南 积己

	SOCKADDR_IN servaddr = { 0 };//家南 林家
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	servaddr.sin_port = htons(PORT_NUM);
	connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));//楷搬 夸没

	myPawn = Cast<AMyPawn>(GWorld->GetFirstPlayerController()->GetPawn());
	myPawn->StartActing();
}



void AMyClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call parent class tick function  
	if (isConnected)
		RecvData();
}

void AMyClient::SendData(float CartPos, float Angle, float CartSpeed, float AngelChange, int Done)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("CartPos", FString::SanitizeFloat(CartPos));
	JsonObject->SetStringField("Angle", FString::SanitizeFloat(Angle));
	JsonObject->SetStringField("CartSpeed", FString::SanitizeFloat(CartSpeed));
	JsonObject->SetStringField("AngelChange", FString::SanitizeFloat(AngelChange));
	JsonObject->SetStringField("Done", FString::SanitizeFloat(Done));
	
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FTCHARToUTF8 mmsg(*OutputString);
	send(sock, mmsg.Get(), mmsg.Length(), 0); //价脚
}

void AMyClient::RecvData()
{
	char msg[20];
	int data;
	ZeroMemory(msg, 20);
	recv(sock, msg, sizeof(msg), 0);

	//MY_LOG(Warning, UTF8_TO_TCHAR(msg));

	FString OutputString = msg;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(OutputString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		data = JsonObject->GetNumberField(TEXT("Move"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, JsonObject->GetStringField(TEXT("Move")));
	}

	myPawn->Acting((int)data);
}



/*
TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
JsonObject->SetStringField("Name", "Super Sword");
JsonObject->SetNumberField("Damage", 15);
JsonObject->SetNumberField("Weight", 3);

FString OutputString;
TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

TSharedPtr<FJsonObject> JsonObject2 = MakeShareable(new FJsonObject);
TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(OutputString);
if (FJsonSerializer::Deserialize(Reader, JsonObject2))
{
//MY_LOG(Warning, TEXT(JsonObject->GetStringField(TEXT("Name"))));
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "debug msg");
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, JsonObject->GetStringField(TEXT("Name")));
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, JsonObject->GetStringField(TEXT("Damage")));
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, JsonObject->GetStringField(TEXT("Weight")));
}
*/