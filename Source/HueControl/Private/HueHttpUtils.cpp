#include "HueHttpUtils.h"

void FHueHttpUtils::Send(
    const FString& URL,
    const FString& Verb,
    const FString& Content,
    const FHttpRequestCompleteDelegate& Callback
)
{
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->SetURL(URL);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    if (!Content.IsEmpty())
        Request->SetContentAsString(Content);

    Request->OnProcessRequestComplete() = Callback;
    Request->ProcessRequest();
}
