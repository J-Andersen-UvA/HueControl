#include "HueLightManager.h"
#include "HueHttpUtils.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HueControlSettings.h"
#include "UHueLightObject.h"

void UHueLightManager::Initialize(FSubsystemCollectionBase& Collection)
{
    BridgeIP = TEXT("");
    UserName = TEXT("");
}

void UHueLightManager::Deinitialize()
{
    Lights.Empty();
}

void UHueLightManager::RefreshLights()
{
    if (BridgeIP.IsEmpty() || UserName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("HueLightManager: BridgeIP or UserName is empty."));
        return;
    }

    // Clear previous restriction
    AllowedLightIDs.Empty();

    // Read group name from settings or connection
    const UHueControlSettings* Settings = GetDefault<UHueControlSettings>();
    const FString GroupName = Settings->GroupName;

    if (!GroupName.IsEmpty())
    {
        const FString GroupURL = FString::Printf(
            TEXT("http://%s/api/%s/groups/%s"),
            *BridgeIP,
            *UserName,
            *GroupName
        );

        FHueHttpUtils::Send(
            GroupURL,
            TEXT("GET"),
            TEXT(""),
            FHttpRequestCompleteDelegate::CreateUObject(
                this,
                &UHueLightManager::OnGroupLightsReceived
            )
        );
    }
    else
    {
        RequestAllLights();
    }
}

void UHueLightManager::RequestAllLights()
{
    const FString Url = FString::Printf(TEXT("http://%s/api/%s/lights"), *BridgeIP, *UserName);

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
        const FString LightID = Pair.Key;

        if (AllowedLightIDs.Num() > 0 && !AllowedLightIDs.Contains(LightID))
            continue;

        TSharedPtr<FJsonObject> LightObj = Pair.Value->AsObject();
        if (!LightObj.IsValid())
            continue;

        const FString Type = LightObj->GetStringField(TEXT("type"));

        // Skip smart plugs
        if (Type.Contains("Plug") || Type.Contains("plug"))
            continue;

        FHueLight Parsed;
        Parsed.LightID = LightID;
        Parsed.Name = LightObj->GetStringField(TEXT("name"));
        Parsed.Type = LightObj->GetStringField(TEXT("type"));
        Parsed.BridgeIP = BridgeIP;
        Parsed.UserName = UserName;

        RegisterLight(Parsed);
    }

    UE_LOG(LogTemp, Warning, TEXT("HueLightManager: Loaded %d lights."), Lights.Num());
}

void UHueLightManager::OnGroupLightsReceived(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        FString Json = Response->GetContentAsString();

        TSharedPtr<FJsonObject> RootObj;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);

        if (FJsonSerializer::Deserialize(Reader, RootObj) && RootObj.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>* LightsArray;
            if (RootObj->TryGetArrayField(TEXT("lights"), LightsArray))
            {
                for (auto& V : *LightsArray)
                    AllowedLightIDs.Add(V->AsString());
            }
        }
    }

    // After group lights are known → now fetch all lights
    RequestAllLights();
}

void UHueLightManager::RegisterLight(const FHueLight& LightData)
{
    // Do NOT create a light unless bridge/account info exists
    if (LightData.BridgeIP.IsEmpty() || LightData.UserName.IsEmpty())
    {
        UE_LOG(LogTemp, Error,
            TEXT("RegisterLight() aborted — missing BridgeIP/UserName for LightID='%s'"),
            *LightData.LightID);
        return;
    }

    if (LightData.LightID.IsEmpty())
    {
        UE_LOG(LogTemp, Error,
            TEXT("RegisterLight() aborted — missing LightID."));
        return;
    }

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
