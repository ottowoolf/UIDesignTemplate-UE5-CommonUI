// Never Too Late Games


#include "Widgets/Options/Widget_KeyRemapScreen.h"

#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"
#include "CommonRichTextBlock.h"
#include  "Framework/Application/IInputProcessor.h"
#include  "FrontendDebugHelper.h"
#include "ICommonInputModule.h"

class FKeyRemapScreenInputProprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputProprocessor(ECommonInputType InInputTypeToListedTo, ULocalPlayer* InWeakOwningLocalPlayer)
		: CachedInputTypeToListenTo(InInputTypeToListedTo),
		CachedWeakOwningLocalPlayer(InWeakOwningLocalPlayer)
	{}
	
	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;
	
	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCanceledDelegate, const FString& /*CanceledReason*/)
	FOnInputPreProcessorKeySelectCanceledDelegate OnInputPreProcessorKeySelectCanceled;
	
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, 
	TSharedRef<ICursor> Cursor) override{};
	
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());
		return true;
	}
	
	virtual bool HandleMouseButtonDownEvent( FSlateApplication& SlateApp, 
	const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		return true;
	};
	
	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("Key Remap Canceled"));
			
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get
		(CachedWeakOwningLocalPlayer.Get());
		
		check(CommonInputSubsystem);
		
		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
		
		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (InPressedKey.IsGamepadKey()|| CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT
				("Detected Gamepad key pressed for keyboard inputs. Key remap canceled."));
				return;
			}
			break;
		case ECommonInputType::Gamepad:

			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey
			 == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionData = 
				ICommonInputModule::GetSettings().GetDefaultClickAction()
				.GetRow<FCommonInputActionDataBase>(TEXT(""));
				
				check(InputActionData);
				OnInputPreProcessorKeyPressed.ExecuteIfBound
				(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());
				return;
			}
			if (!InPressedKey.IsGamepadKey() )
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT
				("Detected non Gamepad key pressed for Gamepad inputs. Key remap canceled."));
				return;
			}
			break;
		default:
			break;
		}
		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}
	
private:
	ECommonInputType CachedInputTypeToListenTo;
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
	
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(
	ECommonInputType InDesiredInputType)
{	
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	CachedInputProprocessor = MakeShared<FKeyRemapScreenInputProprocessor>
	(CachedDesiredInputType, GetOwningLocalPlayer());
	CachedInputProprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPresseddetected);
	CachedInputProprocessor->OnInputPreProcessorKeySelectCanceled.BindUObject(this, &ThisClass::OnKeySelectCanceled);
	
	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputProprocessor, -1);
	
	FString InputDeviceName;
	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse & Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	default:
		InputDeviceName = TEXT("Unknown");
		break;
	}
	
	const FString DisplayRichMessage = FString::Printf(TEXT
	("<KeyRemapDefault>Press Any</> <KeyRemapHighlight>%s</> <KeyRemapDefault>Key.</>"), *InputDeviceName);
	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
	
	
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	if (CachedInputProprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputProprocessor);
		CachedInputProprocessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPresseddetected(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget(
		[this, CanceledReason]
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(
	TFunction<void()> PreDeactivateCallback)
{
	//Delay a tick to make sure the input is processed correctly.
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([this, PreDeactivateCallback](float 
		DeltaTime)->bool	
		{
			PreDeactivateCallback();
			DeactivateWidget();
			return false;
		}
		));
}
