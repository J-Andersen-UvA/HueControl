#include "HueLight.h"
#include "HueHttpUtils.h"

void FHueLight::Send(const FString& JSON) const
{
    const FString URL = FString::Printf(
        TEXT("http://%s/api/%s/lights/%s/state"),
        *BridgeIP,
        *UserName,
        *LightID
    );

    FHueHttpUtils::Send(
        URL,
        TEXT("PUT"),
        JSON,
        [](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bOK)
        {
            if (!bOK || !Resp.IsValid())
                UE_LOG(LogTemp, Warning, TEXT("Hue Light request failed"));
        }
    );
}

void FHueLight::TurnOn() const
{
    Send(TEXT("{\"on\": true}"));
}

void FHueLight::TurnOff() const
{
    Send(TEXT("{\"on\": false}"));
}

void FHueLight::SetBrightness(int32 Bri) const
{
    FString Json = FString::Printf(TEXT("{\"on\": true, \"bri\": %d}"), Bri);
    Send(Json);
}

void FHueLight::SetColorXY(float X, float Y) const
{
    FString Json = FString::Printf(
        TEXT("{\"on\": true, \"xy\": [%f, %f]}"), X, Y
    );
    Send(Json);
}

void FHueLight::SetCT(int32 CT) const
{
    FString Json = FString::Printf(
        TEXT("{\"on\": true, \"ct\": %d}"), CT
    );
    Send(Json);
}
