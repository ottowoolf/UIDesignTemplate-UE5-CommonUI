// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 *
 */
UCLASS()
class FRONTENDTEMPLATE_CUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()


public:
	UFrontendGameUserSettings();

	static UFrontendGameUserSettings* Get();

	//***** Gameplay Collection Tab *****//
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UFUNCTION()
	float GetMasterVolume() const { return MasterVolume; }
	UFUNCTION()
	void SetMasterVolume(float InVolume);
	//***** Audio Collection Tab *****//
private:

	//***** Gameplay Collection Tab *****//
	UPROPERTY(config)
	FString CurrentGameDifficulty;
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UPROPERTY(config)
	float MasterVolume;
	//***** Audio Collection Tab *****//

};
