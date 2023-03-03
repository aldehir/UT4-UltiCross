#pragma once

#include "Core.h"
#include "Engine.h"
#include "ModuleManager.h"
#include "ModuleInterface.h"

#include "UltiCrosshair.h"

class FUltiCrossExecHandler;

class FUltiCross : public IModuleInterface
{
public:
  static FUltiCross* Get();

  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

  TSharedPtr<FUltiCrosshairConstraints> GetConstraints();
  TSharedRef<FUltiCrosshairConstraintMap> GetConstraintMapForPath(const FString& Path);

private:
  static FUltiCross* Instance;

  FUltiCrossExecHandler *ExecHandler;
  TSharedPtr<FUltiCrosshairConstraints> Constraints;

  void DumpDebug();
};

class FUltiCrossExecHandler : public FSelfRegisteringExec
{
public:
  FUltiCrossExecHandler();

  bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);
};