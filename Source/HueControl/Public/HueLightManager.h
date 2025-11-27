#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "HueLight.h"
#include "HueLightManager.generated.h"

UCLASS(BlueprintType)
class HUECONTROL_API UHueLightManager : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Populate internal map from parsed JSON
    void RegisterLight(const FHueLight& Light);

    // Accessors
    FHueLight* GetLight(const FString& LightID);
    const TMap<FString, FHueLight>& GetAllLights() const { return Lights; }

    UFUNCTION(BlueprintCallable, Category="Hue")
    FHueLight GetLightCopy(const FString& LightID) const;

    UFUNCTION(BlueprintCallable, Category="Hue")
    TArray<FHueLight> GetAllLightsBP() const;

    UFUNCTION(BlueprintCallable, Category="Hue")
    void TurnAllOn();

    UFUNCTION(BlueprintCallable, Category="Hue")
    void TurnAllOff();


    FString BridgeIP;
    FString UserName;

private:
    TMap<FString, FHueLight> Lights;
};
