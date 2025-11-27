#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "HttpFwd.h"
#include "HueLight.h"
#include "HueLightManager.generated.h"

class UHueLightObject;

UCLASS(BlueprintType)
class HUECONTROL_API UHueLightManager : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void RegisterLight(const FHueLight& LightData);

    UFUNCTION(BlueprintCallable, Category = "Hue")
    void RefreshLights();
    void OnRefreshLightsResponse(
        FHttpRequestPtr Request,
        FHttpResponsePtr Response,
        bool bConnectedSuccessfully);

    UFUNCTION(BlueprintCallable, Category="Hue")
    UHueLightObject* GetLight(const FString& LightID);

    UFUNCTION(BlueprintCallable, Category="Hue")
    TArray<UHueLightObject*> GetAllLightsBP() const;

    UFUNCTION(BlueprintCallable, Category="Hue")
    void TurnAllOn();

    UFUNCTION(BlueprintCallable, Category="Hue")
    void TurnAllOff();

    FString BridgeIP;
    FString UserName;

private:
    TMap<FString, UHueLightObject*> Lights;
};
