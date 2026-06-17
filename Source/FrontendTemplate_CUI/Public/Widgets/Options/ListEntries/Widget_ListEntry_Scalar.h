// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;
class UListDataObject_Scalar;

/**
 *
 */
UCLASS(abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDTEMPLATE_CUI_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:

	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

		//~ Begin UWidget_ListEntry_Base Interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	//~ End UWidget_ListEntry_Base Interface

private:
	UFUNCTION()
	void OnAnalogSliderValueChanged(float Value);

	UFUNCTION()
	void OnSliderMouseCaptureBegin();

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_SettingSlider;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	UListDataObject_Scalar* CachedOwningScalarDataObject;

};
