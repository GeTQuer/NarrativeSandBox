using System.IO;
using UnrealBuildTool;

public class NarrativeCore : ModuleRules
{
    public NarrativeCore(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string CorePath = Path.GetFullPath(
            Path.Combine(ModuleDirectory, "../../ThirdParty/NarrativeCore")
        );

        PublicSystemIncludePaths.Add(
            Path.Combine(CorePath, "include")
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(
                Path.Combine(CorePath, "lib", "Win64", "narrative_core.lib")
            );
        }
    }
}