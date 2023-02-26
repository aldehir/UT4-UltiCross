using System;
using System.IO;
using UnrealBuildTool;

public class UltiCrossCairo : ModuleRules
{
    public UltiCrossCairo(TargetInfo Target)
    {
        Type = ModuleType.External;

        string LibDir;

        switch (Target.Platform)
        {
            case UnrealTargetPlatform.Win64:
                LibDir = Path.Combine(ModuleDirectory, "Lib", "VS" + WindowsPlatform.GetVisualStudioCompilerVersionName(), "Win64");
                break;
            default:
                return;
        }

        Definitions.Add("CAIRO_WIN32_STATIC_BUILD");

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicLibraryPaths.Add(LibDir);

        PublicAdditionalLibraries.Add("gdi32.lib");
        PublicAdditionalLibraries.Add("msimg32.lib");
        PublicAdditionalLibraries.Add("libpixman-1.a");
        PublicAdditionalLibraries.Add("libcairo.a");
    }
}
