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
          "UnrealTournament",
          "AssetRegistry",
        });

    PrivateDependencyModuleNames.Add("UltiCrossCairo");
  }
}
