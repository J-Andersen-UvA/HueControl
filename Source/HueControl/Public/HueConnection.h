#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "HueConnection.generated.h"

UCLASS(BlueprintType)
class HUECONTROL_API UHueConnection : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="Hue")
    void SetUserName(const FString& InUserName);

    UFUNCTION(BlueprintCallable, Category="Hue")
    void SetBridgeIP(const FString& InIP);

    UFUNCTION(BlueprintCallable, Category = "Hue")
    void SetGroupName(const FString& InGroupName);

    UFUNCTION(BlueprintCallable, Category="Hue")
    void CheckConnection();

    UFUNCTION(BlueprintCallable, Category="Hue")
    void GetAllLights();

    UFUNCTION(BlueprintCallable, Category="Hue")
    void GetAllDevices();

    UFUNCTION(BlueprintCallable, Category = "Hue")
    void GetAllDevicesForRegisteredGroup();

private:
    FString BridgeIP;
    FString UserName;
    FString GroupName;

    void SendRequest(
        const FString& Endpoint,
        const FString& Verb,
        const FString& ContentJson = TEXT("")
    );
};
