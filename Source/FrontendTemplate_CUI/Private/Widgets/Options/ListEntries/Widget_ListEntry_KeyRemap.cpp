// Never Too Late Games


#include "Widgets/Options/ListEntries/Widget_ListEntry_KeyRemap.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include  "FrontendDebugHelper.h"
#include "FrontendFunctionLibrary.h"
#include  "FrontendGameplayTags.h"
#include  "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Options/Widget_KeyRemapScreen.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CommonButton_RemapKey->OnClicked().AddUObject(this, 
	&ThisClass::OnRemapKeyButtonClicked);
	
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, 
	&ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	
	SelectThisEntryWidget();
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync
	(FrontendGameplayTags::Frontend_WidgetStack_Modal, 
	UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag
	(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen), [this]
	(EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
	{
		if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
         {
           UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
			CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
			CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);

			if (CreatedKeyRemapScreen)
			{
			CreatedKeyRemapScreen->SetDesiredInputTypeToFilter
			(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
			}
         }
	}
	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();
	
	if (!CachedOwningKeyRemapDataObject) {return;}
	
	//Check if the current key is already the default key. Display screen to notify this tom the player.
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
	EConfirmScreenType::Ok,
	FText::FromString(TEXT("Reset Key Mapping")),
	FText::FromString(TEXT("The key binding for ") + 
	CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + 
	TEXT(" is already set to default.")),
	[](EConfirmScreenButtonType ClickedButton){}
	);
		return;
	}
	
	//Reset key binding back to default
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
	EConfirmScreenType::YesNo,
	FText::FromString(TEXT("Reset Key Mapping")),
	FText::FromString(TEXT("Are you sure you want to reset the key binding for ") + 
	CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + 
	TEXT(" back to default?")),
	[this](EConfirmScreenButtonType ClickedButton)
	{
		if (ClickedButton == EConfirmScreenButtonType::Confirmed)
		{
			CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
		}
	}	
	);
	
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}  
	
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(
	const FString& CanceledReason)
{
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
	EConfirmScreenType::Ok,
	FText::FromString(TEXT("Key Remap")),
	FText::FromString(CanceledReason),
	[](EConfirmScreenButtonType){}
	);
}
