#include "UltiCrossPCH.h"
#include "Editor.h"
#include "TestCrosshair.h"
#include "UltiCrosshair.h"
#include "SUltiCrossConfigDialog.h"

#include "AssetRegistryModule.h"

#include "cairo/cairo.h"

class FUltiCrossExecHandler : public FSelfRegisteringExec
{
public:
  FUltiCrossExecHandler();

  bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);
};

class FUltiCross : public IModuleInterface
{
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

private:
  UTestCrosshair *ExampleCrosshair;
  UTexture2D *ExampleTexture;

  FUltiCrossExecHandler *ExecHandler;
};

IMPLEMENT_MODULE(FUltiCross, UltiCross);

void FUltiCross::StartupModule()
{
  UE_LOG(LogUltiCross, Log, TEXT("UltiCross Loaded"));

  ExecHandler = new FUltiCrossExecHandler();
}

void FUltiCross::ShutdownModule()
{
  delete ExecHandler;
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
          Ar.Logf(TEXT("  %s: Name=%s Thickness=%f Length=%f Gap=%f"),
            *(UltiCrosshair->CrosshairTag.ToString()), *(UltiCrosshair->CrosshairName.ToString()),
            UltiCrosshair->Thickness, UltiCrosshair->Length, UltiCrosshair->Gap);
        }
      }

      return true;
    }
    else
    {
      Ar.Log(TEXT("UltiCross Activated"));

      UUTLocalPlayer* Player = Cast<UUTLocalPlayer>(GEngine->GetLocalPlayerFromControllerId(InWorld, 0));
      if (Player)
      {
        UE_LOG(LogUltiCross, Log, TEXT("Found Local Player @ %p: %s"), Player, *Player->GetNickname());
      }

      TSharedRef<class SUTDialogBase> Dialog = SNew(SUltiCrossConfigDialog)
        .PlayerOwner(Player);

      Player->OpenDialog(Dialog);

      return true;
    }
  }

  return false;
}
