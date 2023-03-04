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

FUltiCrossExecHandler::FUltiCrossExecHandler()
{

}

bool FUltiCrossExecHandler::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
  if (FParse::Command(&Cmd, TEXT("ULTICROSS")))
  {
    Ar.Log(TEXT("UltiCross Activated"));

    AUTBasePlayerController* PlayerController = Cast<AUTBasePlayerController>(GEngine->GetFirstLocalPlayerController(InWorld));
    UUTLocalPlayer* Player = Cast<UUTLocalPlayer>(PlayerController->Player);

    TSharedRef<class SUTDialogBase> Dialog = SNew(SUltiCrossConfigDialog)
      .PlayerOwner(Player)
      .PlayerController(PlayerController);

    Player->OpenDialog(Dialog);

    return true;
  }

  return false;
}
