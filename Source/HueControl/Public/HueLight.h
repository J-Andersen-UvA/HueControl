#pragma once

#include "CoreMinimal.h"
#include "HueLight.generated.h"

USTRUCT(BlueprintType)
struct FHueLight
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LightID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Type;

    FString BridgeIP;
    FString UserName;

    // ---- Member Functions ----
    void TurnOn() const;
    void TurnOff() const;
    void SetBrightness(int32 Bri) const;
    void SetColorXY(float X, float Y) const;
    void SetColorRGB(int32 R, int32 G, int32 B) const;
    void SetCT(int32 CT) const;
    
private:
    void Send(const FString& JSON) const;
};
