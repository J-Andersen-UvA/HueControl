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
        FHttpRequestCompleteDelegate::CreateLambda(
            [](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bOK)
            {
                if (!bOK || !Resp.IsValid())
                    UE_LOG(LogTemp, Warning, TEXT("Hue request failed"));
            }
        )
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

static void RGBToXY(int32 R, int32 G, int32 B, float& OutX, float& OutY)
{
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    r = (r > 0.04045f) ? FMath::Pow((r + 0.055f) / 1.055f, 2.4f) : r / 12.92f;
    g = (g > 0.04045f) ? FMath::Pow((g + 0.055f) / 1.055f, 2.4f) : g / 12.92f;
    b = (b > 0.04045f) ? FMath::Pow((b + 0.055f) / 1.055f, 2.4f) : b / 12.92f;

    float X = r * 0.664511f + g * 0.154324f + b * 0.162028f;
    float Y = r * 0.283881f + g * 0.668433f + b * 0.047685f;
    float Z = r * 0.000088f + g * 0.072310f + b * 0.986039f;

    OutX = X / (X + Y + Z);
    OutY = Y / (X + Y + Z);
}

void FHueLight::SetColorRGB(int32 R, int32 G, int32 B) const
{
    float X, Y;
    RGBToXY(R, G, B, X, Y);
    FHueLight::SetColorXY(X, Y);
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
