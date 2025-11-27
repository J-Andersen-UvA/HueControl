#pragma once

#include "CoreMinimal.h"
#include "Http.h"

class HUECONTROL_API FHueHttpUtils
{
public:
    static void Send(
        const FString& URL,
        const FString& Verb,
        const FString& Content,
        const FHttpRequestCompleteDelegate& Callback
    );
};
