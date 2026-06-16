// Never Too Late Games


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"




#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
	InitAccessibilityCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabId(const FName& InSelectedTabId) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabId](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabId;
		});
	checkf(FoundTabCollectionPtr, TEXT("No valid tab found for the tab ID: %s"), *InSelectedTabId.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> AllChildListItems;
	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	// This is the full code for constructor data interaction helper.
	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty));*/



	// Game difficulty options
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"), FText::FromString(TEXT("Very Hard")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplyChangeImmediately(true);


		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	//Test Item
	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
		TestItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT("This is a test item to show how to use the soft image in the description. The image should be visible above this text.")));

		GameplayTabCollection->AddChildListData(TestItem);

	}

	//Test Item 2
	{
		UListDataObject_String* TestItem2 = NewObject<UListDataObject_String>();
		TestItem2->SetDataID(FName("TestItem2"));
		TestItem2->SetDataDisplayName(FText::FromString("Test Image Item 2"));
		TestItem2->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage2));
		TestItem2->SetDescriptionRichText(FText::FromString(TEXT("This is a test item to show how to use the soft image in the description. The image should be visible above this text.")));
		GameplayTabCollection->AddChildListData(TestItem2);

	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();

		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Master Volume
		{
			UListDataObject_Scalar* MasterVolume = NewObject<UListDataObject_Scalar>();
			MasterVolume->SetDataID(FName("MasterVolume"));
			MasterVolume->SetDataDisplayName(FText::FromString(TEXT("Master Volume")));

			MasterVolume->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the overall volume of the game.")));
			MasterVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MasterVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MasterVolume->SetSliderStepSize(0.1f);
			MasterVolume->SetDefaultValueFromString(LexToString(1.f));
			MasterVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MasterVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // No Decimal e.g. 50% instead of 50.00%
			MasterVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMasterVolume));
			MasterVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMasterVolume));
			MasterVolume->SetShouldApplyChangeImmediately(true);

			VolumeCategoryCollection->AddChildListData(MasterVolume);
		}
	}

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

void UOptionsDataRegistry::InitAccessibilityCollectionTab()
{
	UListDataObject_Collection* AccessibilityTabCollection = NewObject<UListDataObject_Collection>();
	AccessibilityTabCollection->SetDataID(FName("AccessibilityTabCollection"));
	AccessibilityTabCollection->SetDataDisplayName(FText::FromString(TEXT("Accessibility")));
	RegisteredOptionsTabCollections.Add(AccessibilityTabCollection);
}
