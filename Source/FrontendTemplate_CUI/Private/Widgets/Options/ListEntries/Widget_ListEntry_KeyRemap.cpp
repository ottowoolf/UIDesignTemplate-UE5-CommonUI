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
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync
	(FrontendGameplayTags::Frontend_WidgetStack_Modal, 
	UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag
	(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen), [this]
	(EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
	{
		if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
         {
           UWidget_KeyRemapScreen* CreatedKeyRemapScreen = 
           CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
			
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
	Debug::Print(TEXT("Reset Key Binding Button Clicked!"));
}
