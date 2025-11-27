#include "UHueLightObject.h"
#include "HueHttpUtils.h"

void UHueLightObject::TurnOn() { Data.TurnOn(); }
void UHueLightObject::TurnOff() { Data.TurnOff(); }
void UHueLightObject::SetBrightness(int32 Bri) { Data.SetBrightness(Bri); }
void UHueLightObject::SetColorXY(float X, float Y) { Data.SetColorXY(X, Y); }
void UHueLightObject::SetCT(int32 CT) { Data.SetCT(CT); }
void UHueLightObject::SetColorRGB(int32 R, int32 G, int32 B) { Data.SetColorRGB(R, G, B); }
