// Never Too Late Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Collection;

/**
 *
 */
UCLASS()
class FRONTENDTEMPLATE_CUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	//Gets called by the options screen right after the object of type UOptionsDataRegistry is created.
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

private:

	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();
	void InitAccessibilityCollectionTab();

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;

};
