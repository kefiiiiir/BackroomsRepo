#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "LanguageTypes.h"
#include "LocalizedSound.generated.h"

USTRUCT(BlueprintType)
struct FLocalizedSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* RU = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* JP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* NL = nullptr;

	USoundBase* Get(ELanguage Lang) const
	{
		switch (Lang)
		{
		case ELanguage::RU: return RU;
		case ELanguage::JP: return JP;
		case ELanguage::NL: return NL;
		default: return RU;
		}
	}
};
