#include "HueLightManager.h"
#include "HueHttpUtils.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "UHueLightObject.h"

void UHueLightManager::Initialize(FSubsystemCollectionBase& Collection)
{
    BridgeIP = TEXT("");
    UserName = TEXT("");
}

void UHueLightManager::Deinitialize()
{
}

void UHueLightManager::RefreshLights()
{
    if (BridgeIP.IsEmpty() || UserName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("HueLightManager: BridgeIP or UserName is empty."));
        return;
    }

    FString Url = FString::Printf(TEXT("http://%s/api/%s/lights"), *BridgeIP, *UserName);

    FHueHttpUtils::Send(
        Url,
        TEXT("GET"),
        TEXT(""),
        FHttpRequestCompleteDelegate::CreateUObject(
            this,
            &UHueLightManager::OnRefreshLightsResponse
        )
    );
}

void UHueLightManager::OnRefreshLightsResponse(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bConnectedSuccessfully)
{
    if (!bConnectedSuccessfully || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HueLightManager: Request failed."));
        return;
    }

    FString Json = Response->GetContentAsString();

    TSharedPtr<FJsonObject> RootObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);

    if (!FJsonSerializer::Deserialize(Reader, RootObj) || !RootObj.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HueLightManager: Invalid JSON"));
        return;
    }

    Lights.Empty();

    for (auto& Pair : RootObj->Values)
    {
        FString LightID = Pair.Key;
        TSharedPtr<FJsonObject> LightObj = Pair.Value->AsObject();

        if (!LightObj.IsValid())
            continue;

        FHueLight Parsed;
        Parsed.LightID = LightID;
        Parsed.Name = LightObj->GetStringField(TEXT("name"));
        Parsed.Type = LightObj->GetStringField(TEXT("type"));
        Parsed.BridgeIP = BridgeIP;
        Parsed.UserName = UserName;

        this->RegisterLight(Parsed);
    }

    UE_LOG(LogTemp, Warning, TEXT("HueLightManager: Loaded %d lights."), Lights.Num());
}

void UHueLightManager::RegisterLight(const FHueLight& LightData)
{
    UHueLightObject* Obj = NewObject<UHueLightObject>(this);
    Obj->Data = LightData;

    Lights.Add(LightData.LightID, Obj);
}

TArray<UHueLightObject*> UHueLightManager::GetAllLightsBP() const
{
    TArray<UHueLightObject*> Out;
    Lights.GenerateValueArray(Out);
    return Out;
}
    
UHueLightObject* UHueLightManager::GetLight(const FString& LightID)
{
    return Lights.Contains(LightID) ? Lights[LightID] : nullptr;
}

void UHueLightManager::TurnAllOn()
{
    for (auto& Pair : Lights)
        Pair.Value->TurnOn();
}

void UHueLightManager::TurnAllOff()
{
    for (auto& Pair : Lights)
        Pair.Value->TurnOff();
}
