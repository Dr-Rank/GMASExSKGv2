using UnrealBuildTool;

public class GMCE_Motion : ModuleRules
{
    public GMCE_Motion(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "GMCCore", "GMCAbilitySystem", "GMCExtended", "GMCExtendedAnimation", "PoseSearch", "GameplayTags"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}