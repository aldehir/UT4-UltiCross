#include "UltiCrossPCH.h"
#include "UltiCross.h"
#include "SUltiCrossConfigDialog.h"
#include "FindProperty.h"

FUltiCross* FUltiCross::Instance = nullptr;

IMPLEMENT_MODULE(FUltiCross, UltiCross);

FUltiCross* FUltiCross::Get()
{
  return FUltiCross::Instance;
}

void FUltiCross::StartupModule()
{
  UE_LOG(LogUltiCross, Log, TEXT("UltiCross Loaded"));

  if (FUltiCross::Instance == nullptr)
  {
    FUltiCross::Instance = this;
  }

  ExecHandler = new FUltiCrossExecHandler();
  Constraints = MakeShareable(new FUltiCrosshairConstraints());

  DumpDebug();
}

void FUltiCross::ShutdownModule()
{
  delete ExecHandler;
}

void FUltiCross::GetUltiCrosshairs(UObject* Outer, TArray<UUltiCrosshair*>& Crosshairs)
{
  FName NAME_GeneratedClass(TEXT("GeneratedClass"));

  // Load all Blueprint crosshairs that inherit UUlitCrosshair
  Crosshairs.Empty();
  TArray<FAssetData> AssetList;
  GetAllBlueprintAssetData(UUltiCrosshair::StaticClass(), AssetList);

  for (const FAssetData& Asset : AssetList)
  {
    const FString* ClassPath = Asset.TagsAndValues.Find(NAME_GeneratedClass);
    UClass* CrosshairClass = LoadObject<UClass>(NULL, **ClassPath);
    if (CrosshairClass == nullptr) continue;

    UUltiCrosshair* Crosshair = NewObject<UUltiCrosshair>(Outer, CrosshairClass, NAME_None, RF_NoFlags);
    if (Crosshair)
    {
      Crosshairs.Add(Crosshair);
    }
  }
}

TSharedPtr<FUltiCrosshairConstraints> FUltiCross::GetConstraints()
{
  return Constraints;
}

TSharedRef<FUltiCrosshairConstraintMap> FUltiCross::GetConstraintMapForPath(const FString& Path)
{
  return Constraints->FindChecked(Path);
}

void FUltiCross::DumpDebug()
{
  UE_LOG(LogUltiCross, Log, TEXT("UUltiCrosshair Properties:"));

  for (TFieldIterator<UProperty> PropIt(UUltiCrosshair::StaticClass()); PropIt; ++PropIt)
  {
    UProperty* Property = *PropIt;
    UE_LOG(LogUltiCross, Log, TEXT("  - Name: %s"), *Property->GetName());
    UE_LOG(LogUltiCross, Log, TEXT("    FName: %s"), *Property->GetFName().ToString());
    UE_LOG(LogUltiCross, Log, TEXT("    Path: %s"), *Property->GetPathName());

    UStructProperty* StructProperty = Cast<UStructProperty>(Property);
    if (StructProperty != nullptr)
    {
      UE_LOG(LogUltiCross, Log, TEXT("  - Children:"));

      for (TFieldIterator<UProperty> PropIt2(StructProperty->Struct); PropIt2; ++PropIt2)
      {
        UProperty *ChildProp = *PropIt2;
        UE_LOG(LogUltiCross, Log, TEXT("    - Name: %s"), *ChildProp->GetName());
        UE_LOG(LogUltiCross, Log, TEXT("      FName: %s"), *ChildProp->GetFName().ToString());
        UE_LOG(LogUltiCross, Log, TEXT("      Path: %s"), *ChildProp->GetPathName());
      }
    }
  }
}

FUltiCrossExecHandler::FUltiCrossExecHandler()
{

}

bool FUltiCrossExecHandler::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
  if (FParse::Command(&Cmd, TEXT("ULTICROSS")))
  {
    if (FParse::Command(&Cmd, TEXT("LIST")))
    {
      AUTPlayerController* PlayerController = Cast<AUTPlayerController>(GEngine->GetFirstLocalPlayerController(InWorld));
      AUTHUD* HUD = PlayerController->MyUTHUD;

      Ar.Log(TEXT("UltiCrosshairs:"));

      for (const TPair<FName, UUTCrosshair*>& Pair : HUD->Crosshairs)
      {
        UUltiCrosshair* UltiCrosshair = Cast<UUltiCrosshair>(Pair.Value);
        if (UltiCrosshair)
        {
          Ar.Logf(TEXT("  %s: Name=%s"),
            *(UltiCrosshair->CrosshairTag.ToString()), *(UltiCrosshair->CrosshairName.ToString()));
        }
      }

      return true;
    }
    else
    {
      Ar.Log(TEXT("UltiCross Activated"));

      AUTPlayerController* PlayerController = Cast<AUTPlayerController>(GEngine->GetFirstLocalPlayerController(InWorld));
      AUTHUD* HUD = Cast<AUTHUD>(PlayerController->MyUTHUD);
      UUTLocalPlayer* Player = Cast<UUTLocalPlayer>(PlayerController->Player);

      TSharedRef<class SUTDialogBase> Dialog = SNew(SUltiCrossConfigDialog)
        .PlayerOwner(Player)
        .HUD(HUD);

      Player->OpenDialog(Dialog);

      return true;
    }
  }

  return false;
}
