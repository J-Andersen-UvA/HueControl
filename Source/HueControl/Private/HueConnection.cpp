#include "HueConnection.h"
#include "HueHttpUtils.h"

void UHueConnection::Initialize(FSubsystemCollectionBase& Collection)
{
    BridgeIP = TEXT("");
    UserName = TEXT("");
}

void UHueConnection::Deinitialize()
{
}

void UHueConnection::SetUserName(const FString& InUserName)
{
    UserName = InUserName;
}

void UHueConnection::SetBridgeIP(const FString& InIP)
{
    BridgeIP = InIP;
}

void UHueConnection::CheckConnection()
{
    const FString Endpoint = FString::Printf(TEXT("http://%s/api/%s/config"), *BridgeIP, *UserName);
    SendRequest(Endpoint, TEXT("GET"));
}

void UHueConnection::GetAllLights()
{
    const FString Endpoint = FString::Printf(TEXT("http://%s/api/%s/lights"), *BridgeIP, *UserName);
    SendRequest(Endpoint, TEXT("GET"));
}

void UHueConnection::GetAllDevices()
{
    const FString Endpoint = FString::Printf(TEXT("http://%s/api/%s"), *BridgeIP, *UserName);
    SendRequest(Endpoint, TEXT("GET"));
}

void UHueConnection::SendRequest(
    const FString& Endpoint,
    const FString& Verb,
    const FString& ContentJson
)
{
    FHueHttpUtils::Send(
        Endpoint,
        Verb,
        ContentJson,
        FHttpRequestCompleteDelegate::CreateLambda(
            [](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bOK)
            {
                if (!bOK || !Resp.IsValid())
                    UE_LOG(LogTemp, Warning, TEXT("Hue request failed"));
            }
        )
    );
}
