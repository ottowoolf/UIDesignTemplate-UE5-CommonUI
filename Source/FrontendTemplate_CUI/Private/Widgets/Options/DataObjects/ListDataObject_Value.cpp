// Never Too Late Games


#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDataDynamicGetter)
{
	DataDynamicGetter = InDataDynamicGetter;
}

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDataDynamicSetter)
{
	DataDynamicSetter = InDataDynamicSetter;
}
