// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 *
 */
UCLASS()
class FRONTENDTEMPLATE_CUI_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitStringResolutionValues();

	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }

protected:

	//~Begin UListDataObject_String Interface
	virtual void OnDataObjectInitialized() override;
	//~End UListDataObject_String Interface

private:
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;

};
