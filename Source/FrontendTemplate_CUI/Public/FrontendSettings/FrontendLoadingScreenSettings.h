// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class FRONTENDTEMPLATE_CUI_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 3.f;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	bool bShouldLoadingScreenInEditor = true;
	
};
