#include "HueControlSettings.h"
#include "HueConnection.h"
#include "HueLightManager.h"
#include "Engine/Engine.h"
#include "HueControlModule.h"
#include "Misc/CoreDelegates.h"

IMPLEMENT_MODULE(FHueControlModule, HueControl)
DEFINE_LOG_CATEGORY_STATIC(LogHueControl, Log, All);

void FHueControlModule::StartupModule()
{
    // Defer initialization until the engine is fully initialized.
    FCoreDelegates::OnPostEngineInit.AddLambda([]()
    {
        if (!GEngine)
        {
            UE_LOG(LogHueControl, Warning, TEXT("Engine not initialized yet."));
            return;
        }

        const UHueControlSettings* Settings = GetDefault<UHueControlSettings>();

        if (!Settings)
        {
            UE_LOG(LogHueControl, Warning, TEXT("HueControlSettings not found."));
            return;
        }

        if (Settings->bAutoApplyOnStartup)
        {
            if (UHueConnection* Conn = GEngine->GetEngineSubsystem<UHueConnection>())
            {
                Conn->SetBridgeIP(Settings->BridgeIP);
                Conn->SetUserName(Settings->UserName);
            }

            if (UHueLightManager* LM = GEngine->GetEngineSubsystem<UHueLightManager>())
            {
                LM->BridgeIP = Settings->BridgeIP;
                LM->UserName = Settings->UserName;
                LM->RefreshLights();
            }
        }
    });
}

void FHueControlModule::ShutdownModule()
{
}
