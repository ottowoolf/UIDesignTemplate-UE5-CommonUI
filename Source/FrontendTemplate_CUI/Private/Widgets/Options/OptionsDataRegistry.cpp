// Never Too Late Games


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Internationalization/StringTableRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "FrontendDebugHelper.h"


#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTables/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwningLocalPlayer);
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
		// Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the overall volume of the game's music.")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.1f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // No Decimal e.g. 50% instead of 50.00%
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplyChangeImmediately(true);
			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		// SFX Volume
		{
			UListDataObject_Scalar* SFXVolume = NewObject<UListDataObject_Scalar>();
			SFXVolume->SetDataID(FName("SFXVolume"));
			SFXVolume->SetDataDisplayName(FText::FromString(TEXT("SFX Volume")));
			SFXVolume->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the overall volume of the game's sound effects.")));
			SFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SFXVolume->SetSliderStepSize(0.1f);
			SFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // No Decimal e.g. 50% instead of 50.00%
			SFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSFXVolume));
			SFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSFXVolume));
			SFXVolume->SetShouldApplyChangeImmediately(true);
			VolumeCategoryCollection->AddChildListData(SFXVolume);
		}



	}

	// Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		// Allow background audio
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
			AllowBackgroundAudio->SetDescriptionRichText(FText::FromString(TEXT("Allows the game to continue playing audio when it is running in the background.")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeImmediately(true);


			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		// Use HDR Audio
		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
			UseHDRAudioMode->SetDescriptionRichText(FText::FromString(TEXT("Enables or disables HDR audio processing, which can enhance the audio quality and spatialization for supported audio hardware.")));
			UseHDRAudioMode->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			UseHDRAudioMode->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplyChangeImmediately(true);
			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	// Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]() -> bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
				return !bIsInEditor;
			}
		);

		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This option is only available in packaged builds.</>"));

		// Window Mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));

			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));

			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::Fullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		// Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
			ScreenResolution->InitStringResolutionValues();

			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeImmediately(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowedModeEditCondition;
			WindowedModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]() -> bool
				{ const bool bIsBorderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;
			return !bIsBorderlessWindow;
				}
			);

			WindowedModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled> Screen resolution cannot be changed in borderless window mode. The maximum allowed resolution is enforced.</>"));
			WindowedModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowedModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);


			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	// Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));
		GraphicsCategoryCollection->SetDescriptionRichText(FText::FromString(TEXT("Graphics settings can have a significant impact on game performance. Adjust these settings to find the right balance between visual quality and performance for your system.")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		// Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
			DisplayGamma->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the display brightness, to affect the overall visual clarity of the game.")));

			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); //The default value in unreal is 2.2, so we set the output range to be around that default value.
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetSliderStepSize(0.1f);
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInteger* CreatedQualityPreset = nullptr;

		// Quality Preset
		{
			UListDataObject_StringInteger* QualityPreset = NewObject<UListDataObject_StringInteger>();
			QualityPreset->SetDataID(FName("QualityPreset"));
			QualityPreset->SetDataDisplayName(FText::FromString(TEXT("Quality Preset")));
			QualityPreset->SetDescriptionRichText(FText::FromString(TEXT("Sets the graphics quality preset for the game.")));
			QualityPreset->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			QualityPreset->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			QualityPreset->AddIntegerOption(2, FText::FromString(TEXT("High")));
			QualityPreset->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			QualityPreset->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			QualityPreset->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			QualityPreset->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			QualityPreset->SetDefaultValueFromString(LexToString(2)); // Default to High
			QualityPreset->SetShouldApplyChangeImmediately(true);

			GraphicsCategoryCollection->AddChildListData(QualityPreset);

			CreatedQualityPreset = QualityPreset;
		}

		// Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("Resolution Scale")));
			ResolutionScale->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the resolution scale, which can impact performance by rendering the game at a lower resolution and upscaling it to fit the screen.")));

			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetDefaultValueFromString(LexToString(1.f));
			ResolutionScale->SetShouldApplyChangeImmediately(true);

			ResolutionScale->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(ResolutionScale);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		// Global Illumination
		{
			UListDataObject_StringInteger* GlobalIllumination = NewObject<UListDataObject_StringInteger>();
			GlobalIllumination->SetDataID(FName("GlobalIllumination"));
			GlobalIllumination->SetDataDisplayName(FText::FromString(TEXT("Global Illumination")));
			GlobalIllumination->SetDescriptionRichText(FText::FromString(TEXT("Sets the global illumination quality used for indirect lighting in the game.")));
			GlobalIllumination->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIllumination->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			GlobalIllumination->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIllumination->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			GlobalIllumination->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			GlobalIllumination->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIllumination->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIllumination->SetShouldApplyChangeImmediately(true);

			GlobalIllumination->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(GlobalIllumination);

			GraphicsCategoryCollection->AddChildListData(GlobalIllumination);

		}

		//Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
			ShadowQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the shadow quality for the game.")));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplyChangeImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		// Anti-Aliasing Quality
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("Anti-Aliasing Quality")));
			AntiAliasingQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the anti-aliasing quality for the game.")));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			AntiAliasingQuality->SetDefaultValueFromString(LexToString(2)); // Default to High
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality)); \
				AntiAliasingQuality->SetShouldApplyChangeImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		// View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance Quality")));
			ViewDistanceQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the view distance quality for the game.")));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ViewDistanceQuality->SetDefaultValueFromString(LexToString(2)); // Default to High
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplyChangeImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		// Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
			TextureQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the texture quality for the game.")));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			TextureQuality->SetDefaultValueFromString(LexToString(2)); // Default to High
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplyChangeImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		// VFX Quality
		{
			UListDataObject_StringInteger* VFXQuality = NewObject<UListDataObject_StringInteger>();
			VFXQuality->SetDataID(FName("VFXQuality"));
			VFXQuality->SetDataDisplayName(FText::FromString(TEXT("VFX Quality")));
			VFXQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the visual effects quality for the game.")));
			VFXQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			VFXQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			VFXQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			VFXQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			VFXQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			VFXQuality->SetDefaultValueFromString(LexToString(2)); // Default to High
			VFXQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VFXQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VFXQuality->SetShouldApplyChangeImmediately(true);

			VFXQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(VFXQuality);

			GraphicsCategoryCollection->AddChildListData(VFXQuality);
		}

		// Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
			ReflectionQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the reflection quality for the game.")));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ReflectionQuality->SetDefaultValueFromString(LexToString(2)); // Default to High
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplyChangeImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		// Post-Processing Quality
		{
			UListDataObject_StringInteger* PostProcessQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessQuality->SetDataID(FName("PostProcessQuality"));
			PostProcessQuality->SetDataDisplayName(FText::FromString(TEXT("Post-Processing Quality")));
			PostProcessQuality->SetDescriptionRichText(FText::FromString(TEXT("Sets the post-processing quality for the game.")));
			PostProcessQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			PostProcessQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			PostProcessQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			PostProcessQuality->SetDefaultValueFromString(LexToString(2)); // Default to High
			PostProcessQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessQuality->SetShouldApplyChangeImmediately(true);

			PostProcessQuality->AddEditDependencyData(CreatedQualityPreset);
			CreatedQualityPreset->AddEditDependencyData(PostProcessQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessQuality);
		}

		RegisteredOptionsTabCollections.Add(VideoTabCollection);
	}

	// Advanced Graphics Category
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced Graphics")));
		AdvancedGraphicsCategoryCollection->SetDescriptionRichText(FText::FromString(TEXT("Adjust these settings to find the right balance between visual quality and performance for your system.")));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		// Vertical Sync
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
			VerticalSync->SetDescriptionRichText(FText::FromString(TEXT("Enables or disables vertical sync, <Disabled>Warning:</> This may introduce input lag!.")));
			VerticalSync->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			VerticalSync->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplyChangeImmediately(true);

			FOptionsDataEditConditionDescriptor FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc([CreatedWindowMode]() -> bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;

				}

			);

			FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>V-Sync can only be changed in fullscreen mode.</>"));
			FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullScreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);

		}

		// Frame Rate Limit
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
			FrameRateLimit->SetDescriptionRichText(FText::FromString(TEXT("Set a custom frame rate limit for the game.")));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("Unlimited")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));

			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplyChangeImmediately(true);


			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}

	}
}

void UOptionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();
	check(EIUserSettings);

	//Keyboard Mouse Category
	{
		UListDataObject_Collection* KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
		KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Keyboard & Mouse")));

		ControlTabCollection->AddChildListData(KeyboardMouseCategoryCollection);

		// Keyboard & Mouse Inputs
		{

			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;
			
			for (const TPair<FString, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{

						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							/*Debug::Print(
								TEXT("Mapping ID: ") + KeyMapping.GetMappingName().ToString() +
								TEXT(" Display Name: ") + KeyMapping.GetDisplayName().ToString() +
								TEXT(" Bound Key: ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
							);*/

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
							KeyRemapDataObject->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(
			FrontendGameplayTags::Frontend_Image_Controller));
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);

							KeyboardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);

						};
					}
				}
			}
		}

	}
	
	// Gamepad category
	UListDataObject_Collection* GamepadCategoryCollection = NewObject<UListDataObject_Collection>();
	GamepadCategoryCollection->SetDataID(FName("GamepadCategoryCollection"));
	GamepadCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Gamepad")));

	ControlTabCollection->AddChildListData(GamepadCategoryCollection);
		
		//Gamepad Inputs
		{

			FPlayerMappableKeyQueryOptions GampadOnly;
			GampadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			GampadOnly.bMatchBasicKeyTypes = true;
				
			for (const TPair<FString, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{

						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GampadOnly))
						{
							/*Debug::Print(
								TEXT("Mapping ID: ") + KeyMapping.GetMappingName().ToString() +
								TEXT(" Display Name: ") + KeyMapping.GetDisplayName().ToString() +
								TEXT(" Bound Key: ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
							);*/

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
							KeyRemapDataObject->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(
			FrontendGameplayTags::Frontend_Image_Controller));
							KeyRemapDataObject->InitKeyRemapData
							(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);

							GamepadCategoryCollection->AddChildListData
							(KeyRemapDataObject);

						};
					}
				}
			}
		}

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

void UOptionsDataRegistry::InitAccessibilityCollectionTab()
{
	UListDataObject_Collection* AccessibilityTabCollection = NewObject<UListDataObject_Collection>();
	AccessibilityTabCollection->SetDataID(FName("AccessibilityTabCollection"));
	AccessibilityTabCollection->SetDataDisplayName(FText::FromString(TEXT("Accessibility")));
	RegisteredOptionsTabCollections.Add(AccessibilityTabCollection);
}
