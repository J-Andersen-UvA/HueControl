using UnrealBuildTool;

public class HueControl : ModuleRules
{
    public HueControl(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Slate", "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
    }
}
