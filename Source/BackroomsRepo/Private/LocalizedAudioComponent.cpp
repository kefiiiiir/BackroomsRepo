#include "LocalizedAudio/LocalizedAudioComponent.h"
#include "DefaultGameInstance.h"
#include "LocalizedAudio/LocalizedSound.h"

USoundBase* ULocalizedAudioComponent::GetLocalizedSound(FName SoundKey)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	UDefaultGameInstance* GI = World->GetGameInstance<UDefaultGameInstance>();
	if (!GI) return nullptr;

	FLocalizedSound* Entry = Sounds.Find(SoundKey);
	if (!Entry) return nullptr;

	return Entry->Get(GI->CurrentLanguage);
}
