using System.IO;
using UnrealBuildTool;

public class UltiCross : ModuleRules
{
  public UltiCross(TargetInfo Target)
  {
    PublicDependencyModuleNames.AddRange(
        new string[]
        {
          "Core",
          "CoreUObject",
          "Engine",
          "InputCore",
          "UnrealEd",
          "UnrealTournament",
          "AssetRegistry",
          "AppFramework",
        });

    PrivateDependencyModuleNames.AddRange(
        new string[]
        {
          "Slate",
          "SlateCore",
          "UltiCrossCairo",
        });
  }
}
