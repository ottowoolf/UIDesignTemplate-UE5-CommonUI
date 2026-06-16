// Never Too Late Games


#include "FrontendSettings/FrontendGameUserSettings.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: MasterVolume(1.f)
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
