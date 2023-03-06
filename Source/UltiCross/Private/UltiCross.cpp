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
  TArray<UClass*> Classes;
  GetUltiCrosshairsClasses(Classes);

  Crosshairs.Empty();

  for (UClass* Class : Classes)
  {
    UUltiCrosshair* Crosshair = NewObject<UUltiCrosshair>(Outer, Class, NAME_None, RF_NoFlags);
    if (Crosshair)
    {
      Crosshairs.Add(Crosshair);
    }
  }
}

void FUltiCross::GetDefaultUltiCrosshairs(TArray<UUltiCrosshair*>& Crosshairs)
{
  TArray<UClass*> Classes;
  GetUltiCrosshairsClasses(Classes);

  Crosshairs.Empty();

  for (UClass* Class : Classes)
  {
    UUltiCrosshair* CDO = GetMutableDefault<UUltiCrosshair>(Class);
    if (CDO)
    {
      Crosshairs.Add(CDO);
    }
  }
}

void FUltiCross::GetUltiCrosshairsClasses(TArray<UClass*>& Classes)
{
  FName NAME_GeneratedClass(TEXT("GeneratedClass"));

  // Find all blueprint crosshair classes
  TArray<FAssetData> AssetList;
  GetAllBlueprintAssetData(UUltiCrosshair::StaticClass(), AssetList);

  for (const FAssetData& Asset : AssetList)
  {
    const FString* ClassPath = Asset.TagsAndValues.Find(NAME_GeneratedClass);
    UClass* Class = LoadObject<UClass>(NULL, **ClassPath);
    if (Class == nullptr) continue;

    Classes.Add(Class);
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
      .PlayerController(PlayerController)
      .InitiallySelected(LastSelected)
      .OnSelectionChanged_Raw(this, &FUltiCrossExecHandler::OnSelectionChanged);

    Player->OpenDialog(Dialog);

    return true;
  }

  return false;
}
