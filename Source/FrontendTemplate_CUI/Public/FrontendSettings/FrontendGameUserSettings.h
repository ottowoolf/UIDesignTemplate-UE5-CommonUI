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

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetSFXVolume() const { return SFXVolume; }
	UFUNCTION()
	void SetSFXVolume(float InVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }
	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION()
	bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }
	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsAllowed);
	//***** Audio Collection Tab *****//

	//***** Video Collection Tab *****//
	UFUNCTION()
	float GetCurrentDisplayGamma() const;
	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);
	//***** Video Collection Tab *****//

private:

	//***** Gameplay Collection Tab *****//
	UPROPERTY(config)
	FString CurrentGameDifficulty;
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UPROPERTY(config)
	float MasterVolume;

	UPROPERTY(config)
	float MusicVolume;

	UPROPERTY(config)
	float SFXVolume;

	UPROPERTY(config)
	bool bAllowBackgroundAudio;

	UPROPERTY(config)
	bool bUseHDRAudioMode;



	//***** Audio Collection Tab *****//

};
