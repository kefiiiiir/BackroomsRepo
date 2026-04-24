// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LocalizedAudio/LanguageTypes.h"
#include "DefaultGameInstance.generated.h"

UCLASS()
class BACKROOMSREPO_API UDefaultGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ELanguage CurrentLanguage = ELanguage::RU;
};
