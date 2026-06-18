#pragma once
#pragma once

#include "FrontendStructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc) { EditConditionFunc = InEditConditionFunc; }

	bool IsValid() const { return EditConditionFunc != nullptr; }

	bool IsEditConditionMet() const {

		if (IsValid())
		{
			return EditConditionFunc();
		}

		return true;

	}

	FString GetDisabledReason() const { return DisabledRichReason; }
	void SetDisabledReason(const FString& InRichReason) { DisabledRichReason = InRichReason; }

	bool HasForcedDisabledValue() const { return DisabledForcedStringValue.IsSet(); }
	FString GetDisabledForcedStringValue() const { return DisabledForcedStringValue.GetValue(); }
	void SetDisabledForcedStringValue(const FString& InForcedStringValue) { DisabledForcedStringValue = InForcedStringValue; }

private:

	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;

};