#include "UltiCrossPCH.h"
#include "Editor.h"
#include "TestCrosshair.h"
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
  if (FParse::Command(&Cmd, TEXT("ULTICROSS"))) {
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

  return false;
}
