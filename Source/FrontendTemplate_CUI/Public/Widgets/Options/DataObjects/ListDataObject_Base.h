// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "ListDataObject_Base.generated.h"


#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; } \

/**
 *
 */
UCLASS(Abstract)
class FRONTENDTEMPLATE_CUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*, EOptionsListDataModifyReason)
	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependencyDataModified;

	LIST_DATA_ACCESSOR(FName, DataID)
		LIST_DATA_ACCESSOR(FText, DataDisplayName)
		LIST_DATA_ACCESSOR(FText, DescriptionRichText)
		LIST_DATA_ACCESSOR(FText, DisabledRichText)
		LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
		LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)

		void InitDataObject();


	// Empty in the base class, but should be overridden by child class ListDataObject_Collection. The function should return all child data a tab has.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	void SetShouldApplyChangeImmediately(bool bInShouldApplyRightAway) { bShouldApplyChangeImmediately = bInShouldApplyRightAway; }

	// Child class should ovverride to provide the implementation for resetting.
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	// Gets called from OptionsDataRegister for adding in edit conditions for the constructed list data objects.
	void AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition);

	//Gets called from OptionsDataRegister for adding in dependency data.
	void AddEditDependencyData(UListDataObject_Base* InDependencyData);

	bool IsDataCurrentlyEditable();

protected:
	// Empty in base class. The child classes should override it to handle the initialization needed accordingly
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

	// The child cklass should override it to allow the value to be set to the forced strihng value.
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const { return false; }

	// The child class should override it to specify how to set the current value to the forced value.
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) {}

	// This function will be called when the value of the dependency data has changed. The child class can override this function to handle the custom logic needed. Super call is expected!
	virtual void OnEditDependencyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;

	bool bShouldApplyChangeImmediately = false;

	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionDescriptor> EditConditionDescArray;

};
