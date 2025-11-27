#include "HueLightManager.h"

void UHueLightManager::Initialize(FSubsystemCollectionBase& Collection)
{
    BridgeIP = TEXT("192.168.0.229");
    UserName = TEXT("");
}

void UHueLightManager::Deinitialize()
{
}

void UHueLightManager::RegisterLight(const FHueLight& Light)
{
    Lights.Add(Light.LightID, Light);
}

FHueLight* UHueLightManager::GetLight(const FString& LightID)
{
    return Lights.Find(LightID);
}

void UHueLightManager::TurnAllOn()
{
    for (auto& Pair : Lights)
        Pair.Value.TurnOn();
}

void UHueLightManager::TurnAllOff()
{
    for (auto& Pair : Lights)
        Pair.Value.TurnOff();
}
