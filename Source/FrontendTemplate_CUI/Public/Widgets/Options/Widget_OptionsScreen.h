// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

/**
 *
 */
UCLASS(abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDTEMPLATE_CUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:

	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

private:

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

};
