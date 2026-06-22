// Never Too Late Games


#include "Widgets/Options/Widget_KeyRemapScreen.h"
#include  "Framework/Application/IInputProcessor.h"
#include  "FrontendDebugHelper.h"

class FKeyRemapScreenInputProprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputProprocessor(ECommonInputType InInputTypeToListedTo)
		: CachedInputTypeToListenTo(InInputTypeToListedTo)
	{
	}
	
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, 
	TSharedRef<ICursor> Cursor) override{};
	
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		Debug::Print(TEXT("Pressed key: ") + InKeyEvent.GetKey().GetDisplayName()
		.ToString
		());
		
		UEnum* StaticCommonInputType = StaticEnum<ECommonInputType>();
		StaticCommonInputType->GetValueAsString(CachedInputTypeToListenTo);
		
		Debug::Print(TEXT("Desired Input Key Type: ") + 
		StaticCommonInputType->GetValueAsString(CachedInputTypeToListenTo));
		return true;
	}
	
	virtual bool HandleMouseButtonDownEvent( FSlateApplication& SlateApp, 
	const FPointerEvent& MouseEvent) override
	{
		Debug::Print(TEXT("Pressed key: ") + MouseEvent.GetEffectingButton()
		.GetDisplayName()
		.ToString
		());
		
		return true;
	};
	
private:
	ECommonInputType CachedInputTypeToListenTo;
	
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(
	ECommonInputType InDesiredInputType)
{	
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	CachedInputProprocessor = MakeShared<FKeyRemapScreenInputProprocessor>(CachedDesiredInputType);
	
	FSlateApplication::Get().RegisterInputPreProcessor
	(CachedInputProprocessor, -1);
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
