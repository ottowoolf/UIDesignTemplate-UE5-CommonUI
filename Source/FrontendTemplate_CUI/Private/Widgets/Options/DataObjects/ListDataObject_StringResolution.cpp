// Never Too Late Games


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FrontendDebugHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_StringResolution::InitStringResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)
		{
			return A.SizeSquared() < B.SizeSquared();
		});

	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{

		CurrentDisplayText = ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{

	// Resolution Value from dynamic getter: (X=3840,Y=1200)
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%ix%i"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
