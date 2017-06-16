// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "AllowWindowsPlatformTypes.h"
#include "common.h"
#include "HideWindowsPlatformTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(UnrealCartpole, Log, All);

#define MY_LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define MY_LOG_CALLONLY(Verbosity) UE_LOG(UnrealCartpole, Verbosity, TEXT("%s"), *MY_LOG_CALLINFO)
#define MY_LOG(Verbosity, Format, ...) UE_LOG(UnrealCartpole, Verbosity, TEXT("%s %s"), *MY_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__)) 