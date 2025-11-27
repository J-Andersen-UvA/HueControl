#pragma once

#include "CoreMinimal.h"
#include "HueLight.generated.h"

USTRUCT(BlueprintType)
struct FHueLight
{
    GENERATED_BODY()

public:
    // ID used by the Hue API (e.g. "1", "2", "15")
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LightID;

    // Name from the bridge
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    // Extra user-defined description
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    // Is this a color light, a plug, etc.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Type;

    // Communication helpers
    FString BridgeIP;
    FString UserName;

    // ---- Member Functions ----
    UFUNCTION(BlueprintCallable)
    void TurnOn() const;

    UFUNCTION(BlueprintCallable)
    void TurnOff() const;

    UFUNCTION(BlueprintCallable)
    void SetBrightness(int32 Bri) const;

    UFUNCTION(BlueprintCallable)
    void SetColorXY(float X, float Y) const;

    UFUNCTION(BlueprintCallable)
    void SetCT(int32 CT) const;
    
private:
    void Send(const FString& JSON) const;
};
