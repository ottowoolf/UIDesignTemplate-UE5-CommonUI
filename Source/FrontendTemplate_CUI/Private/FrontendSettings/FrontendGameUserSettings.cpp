// Never Too Late Games


#include "FrontendSettings/FrontendGameUserSettings.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: MasterVolume(1.f)
	, MusicVolume(1.f)
	, SFXVolume(1.f)
	, bAllowBackgroundAudio(false)
	, bUseHDRAudioMode(false)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendGameUserSettings::SetMasterVolume(float InVolume)
{
	MasterVolume = InVolume;

	//The actual logic for controlling the volume goes here.
}

void UFrontendGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
	//The actual logic for controlling the music volume goes here.
}

void UFrontendGameUserSettings::SetSFXVolume(float InVolume)
{
	SFXVolume = InVolume;
	//The actual logic for controlling the SFX volume goes here.
}

void UFrontendGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
	//The actual logic for allowing background audio goes here.
}

void UFrontendGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
	//The actual logic for controlling the use of HDR audio mode goes here.
}

float UFrontendGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine) {
		return	GEngine->GetDisplayGamma();
	}
	return 0.0f;
}

void UFrontendGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine) {
		GEngine->DisplayGamma = InNewGamma;
	}
}
