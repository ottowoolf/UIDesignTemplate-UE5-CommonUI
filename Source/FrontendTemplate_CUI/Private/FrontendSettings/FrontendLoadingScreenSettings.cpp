// Never Too Late Games


#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), 
		TEXT("Forgot to assign a valid widget blueprint in the project settings as a loading screen"));
	
	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = 
	SoftLoadingScreenWidgetClass.LoadSynchronous();
	
	return LoadedLoadingScreenWidget;
}
