#pragma once

#include "UnrealTournament.h"
#include "SlateBasics.h"
#include "UltiCrosshairViewModel.h"

#if !UE_SERVER

class UUltiCrosshair;

class SUltiCrossConfigDialog : public SUTDialogBase, public FGCObject
{
  SLATE_BEGIN_ARGS(SUltiCrossConfigDialog)
  : _DialogTitle(FText::FromString(TEXT("UltiCross Configuration")))
  , _DialogSize(FVector2D(900, 700))
  , _bDialogSizeIsRelative(false)
  , _DialogPosition(FVector2D(0.5f, 0.5f))
  , _DialogAnchorPoint(FVector2D(0.5f, 0.5f))
  , _ContentPadding(FVector2D(10.0f, 5.0f))
  {}
  SLATE_ARGUMENT(TWeakObjectPtr<class UUTLocalPlayer>, PlayerOwner)
  SLATE_ARGUMENT(TWeakObjectPtr<class AUTPlayerController>, PlayerController)
  SLATE_ARGUMENT(FText, DialogTitle)
  SLATE_ARGUMENT(FVector2D, DialogSize)
  SLATE_ARGUMENT(bool, bDialogSizeIsRelative)
  SLATE_ARGUMENT(FVector2D, DialogPosition)
  SLATE_ARGUMENT(FVector2D, DialogAnchorPoint)
  SLATE_ARGUMENT(FVector2D, ContentPadding)
  SLATE_EVENT(FDialogResultDelegate, OnDialogResult)
  SLATE_END_ARGS()

public:
  SUltiCrossConfigDialog()
  : CrosshairViewModel(new FUltiCrosshairViewModel(this))
  {}

  void Construct(const FArguments& InArgs);
  void AddReferencedObjects(FReferenceCollector& Collector) override;

  TSharedRef<SWidget> BuildCustomButtonBar() override;

  void GatherCrosshairs();
  void OnDialogResult(TSharedPtr<SCompoundWidget> Widget, uint16 ButtonId);

  TSharedRef<SComboBox<UUltiCrosshair*>> ConstructCrosshairSelection();
  TSharedRef<SWidget> GenerateCrosshairListWidget(UUltiCrosshair* InItem);
  void OnCrosshairChanged(UUltiCrosshair* NewSelection, ESelectInfo::Type SelectType);

  TSharedRef<SWidget> GenerateCrosshairTypeListWidget(TSharedPtr<FUltiCrosshairTypeDescriptor> InItem);

  SVerticalBox::FSlot& AddSlider(FText Caption, TSharedRef<FConstrainedSliderDelegate> Delegate);

  TSharedRef<SWidget> ConstructPropertiesPanel();

  virtual bool bRemainOpenThroughTravel()
  {
    return true;
  }

private:
  friend class FUltiCrosshairViewModel;

  TWeakObjectPtr<AUTPlayerController> PlayerController;

  TSharedRef<FUltiCrosshairViewModel> CrosshairViewModel;

  TSharedPtr<SComboBox<TSharedPtr<FUltiCrosshairTypeDescriptor>>> CrosshairTypeComboBox;

  TArray<UUltiCrosshair*> Crosshairs;
};

#endif
