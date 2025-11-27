#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "HueControlSettings.generated.h"

UCLASS(Config = Game, DefaultConfig)
class HUECONTROL_API UHueControlSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:

    UHueControlSettings();

    // Category under "Project Settings"
    virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
    virtual FName GetSectionName() const override { return TEXT("Hue Control"); }

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Connection")
    FString BridgeIP;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Connection")
    FString UserName;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    bool bAutoApplyOnStartup;
};
