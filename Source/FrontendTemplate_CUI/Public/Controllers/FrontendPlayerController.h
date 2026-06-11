// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 *
 */
UCLASS()
class FRONTENDTEMPLATE_CUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	//~ APlayerController interface
	virtual void OnPossess(APawn* aPawn) override;
	//~ End of APlayerController interface
};
