// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonTextBlock;

/**
 *
 */
UCLASS(abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDTEMPLATE_CUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

private:
	//~ Begin UUserWidget interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget interface

	//~ Begin UCommonButtonBase interface
	virtual void NativeOnCurrentTextStyleChanged() override;
	//~ End UCommonButtonBase interface

	//~ Begin UCommonButtonBase interface
	virtual void NativeOnHovered() override;
	//~ End UCommonButtonBase interface

	//~ Begin UCommonButtonBase interface
	virtual void NativeOnUnhovered() override;
	//~ End UCommonButtonBase interface


	//*** Bound Widgets ***//
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	//*** Bound Widgets ***//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};
