
#pragma once

#include "CoreMinimal.h"
#include "HueLight.h"
#include "UHueLightObject.generated.h"

UCLASS(BlueprintType)
class HUECONTROL_API UHueLightObject : public UObject
{
    GENERATED_BODY()

public:

    FHueLight Data;

    UFUNCTION(BlueprintCallable)
    void TurnOn();

    UFUNCTION(BlueprintCallable)
    void TurnOff();

    UFUNCTION(BlueprintCallable)
    void SetBrightness(int32 Bri);

    UFUNCTION(BlueprintCallable)
    void SetColorRGB(int32 R, int32 G, int32 B);
    
    UFUNCTION(BlueprintCallable)
    void SetColorXY(float X, float Y);

    UFUNCTION(BlueprintCallable)
    void SetCT(int32 CT);

    UFUNCTION(BlueprintPure, Category = "Hue Light")
    FString GetLightName() const { return Data.Name; }

    UFUNCTION(BlueprintPure, Category = "Hue Light")
    FString GetLightID() const { return Data.LightID; }
};
