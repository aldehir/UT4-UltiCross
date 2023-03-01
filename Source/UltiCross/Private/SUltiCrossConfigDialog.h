#pragma once

#include "UnrealTournament.h"
#include "SlateBasics.h"
#include "SUltiCrossConfigViewModel.h"

#if !UE_SERVER

class UUltiCrosshair;

class SUltiCrossConfigDialog : public SUTDialogBase
{
  SLATE_BEGIN_ARGS(SUltiCrossConfigDialog)
  : _DialogTitle(FText::FromString(TEXT("UltiCross Configuration")))
  , _DialogSize(FVector2D(900, 700))
  , _bDialogSizeIsRelative(false)
  , _DialogPosition(FVector2D(0.5f, 0.5f))
  , _DialogAnchorPoint(FVector2D(0.5f, 0.5f))
  , _ContentPadding(FVector2D(10.0f, 5.0f))
  , _ButtonMask(UTDIALOG_BUTTON_OK)
  {}
  SLATE_ARGUMENT(TWeakObjectPtr<class UUTLocalPlayer>, PlayerOwner)
  SLATE_ARGUMENT(TWeakObjectPtr<class AUTHUD>, HUD)
  SLATE_ARGUMENT(FText, DialogTitle)
  SLATE_ARGUMENT(FVector2D, DialogSize)
  SLATE_ARGUMENT(bool, bDialogSizeIsRelative)
  SLATE_ARGUMENT(FVector2D, DialogPosition)
  SLATE_ARGUMENT(FVector2D, DialogAnchorPoint)
  SLATE_ARGUMENT(FVector2D, ContentPadding)
  SLATE_ARGUMENT(uint16, ButtonMask)
  SLATE_EVENT(FDialogResultDelegate, OnDialogResult)
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

  void GatherCrosshairs();

  TSharedRef<SComboBox<UUltiCrosshair*>> ConstructCrosshairSelection();
  TSharedRef<SWidget> GenerateCrosshairListWidget(UUltiCrosshair* InItem);
  void OnCrosshairChanged(UUltiCrosshair* NewSelection, ESelectInfo::Type SelectType);

public:
  virtual bool bRemainOPenThroughTravel()
  {
    return true;
  }

private:
  TWeakObjectPtr<class AUTHUD> HUD;

  TArray<UUltiCrosshair*> Crosshairs;
  TSharedPtr<SUltiCrossConfigViewModel> CrosshairViewModel;
  TSharedPtr<STextBlock> CrosshairTextBlock;

  FSlateBrush* ExampleCrosshair;
  TArray<TSharedPtr<FString>> ExampleCrosshairList;

  TArray<TSharedPtr<FString>> ShapeList;
  TSharedPtr<STextBlock> SelectedShape;
};

#endif