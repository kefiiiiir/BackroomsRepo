#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocalizedSound.h"
#include "LanguageTypes.h"
#include "LocalizedAudioComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BACKROOMSREPO_API ULocalizedAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Key = "Empty1", "Empty2", "LowEnergyLine3", etc.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FLocalizedSound> Sounds;

	UFUNCTION(BlueprintCallable)
	USoundBase* GetLocalizedSound(FName SoundKey);
};
