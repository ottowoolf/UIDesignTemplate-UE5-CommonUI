// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontendCommonRotator.generated.h"

/**
 *
 */
UCLASS(abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDTEMPLATE_CUI_API UFrontendCommonRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	void SetSelectedOptionByText(const FText& InTextOption);

};
