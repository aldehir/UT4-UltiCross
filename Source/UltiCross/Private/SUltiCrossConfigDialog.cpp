#include "UltiCrossPCH.h"
#include "SUltiCrossConfigDialog.h"
#include "UltiCrosshair.h"

#include "SColorPicker.h"
#include "SWidgetSwitcher.h"

#include "Stubs/SUTStyle.h"
#include "Stubs/SUWindowsStyle.h"

#if !UE_SERVER


void SUltiCrossConfigDialog::Construct(const FArguments& InArgs)
{
  HUD = InArgs._HUD;

  SUTDialogBase::Construct(SUTDialogBase::FArguments()
    .PlayerOwner(InArgs._PlayerOwner)
    .DialogTitle(InArgs._DialogTitle)
    .DialogSize(InArgs._DialogSize)
    .bDialogSizeIsRelative(InArgs._bDialogSizeIsRelative)
    .DialogPosition(InArgs._DialogPosition)
    .DialogAnchorPoint(InArgs._DialogAnchorPoint)
    .IsScrollable(false)
    .ContentPadding(InArgs._ContentPadding)
    .ButtonMask(UTDIALOG_BUTTON_CLOSE)
    .OnDialogResult(this, &SUltiCrossConfigDialog::OnDialogResult)
  );

  GatherCrosshairs();
  
  if (DialogContent.IsValid())
  {
    DialogContent->AddSlot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    .Padding(10.0, 10.0, 10.0, 10.0)
    [
      SNew(SHorizontalBox)
      +SHorizontalBox::Slot()
      .HAlign(HAlign_Fill)
      .VAlign(VAlign_Fill)
      .AutoWidth()
      [
        SNew(SVerticalBox)
        
        // Crosshair Preview
        +SVerticalBox::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Top)
        .AutoHeight()
        [
          SNew(SOverlay)
          +SOverlay::Slot()
          .HAlign(HAlign_Fill)
          .VAlign(VAlign_Fill)
          [
            SNew(SImage)
            .Image(SUTStyle::Get().GetBrush("UT.HeaderBackground.Light"))
          ]

          +SOverlay::Slot()
          .HAlign(HAlign_Fill)
          .VAlign(VAlign_Fill)
          [
            SNew(SBorder)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .Padding(FMargin(96))
            [
              SNew(SBox)
              .WidthOverride(64)
              .HeightOverride(64)
              [
                SNew(SImage)
                .Image(CrosshairViewModel->GetBrush())
              ]
            ]
          ]
        ]
        
        // Crosshair Selection
        +SVerticalBox::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .AutoHeight()
        [
          ConstructCrosshairSelection()
        ]
      ]

      +SHorizontalBox::Slot()
      .HAlign(HAlign_Fill)
      .VAlign(VAlign_Fill)
      .FillWidth(1)
      [
        ConstructPropertiesPanel()
      ]
    ];
  }
}

TSharedRef<SComboBox<UUltiCrosshair*>> SUltiCrossConfigDialog::ConstructCrosshairSelection()
{
  return SNew(SComboBox<UUltiCrosshair*>)
    .InitiallySelectedItem(CrosshairViewModel->GetCrosshair())
    .ComboBoxStyle(SUTStyle::Get(), "UT.ComboBox")
    .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
    .OptionsSource(&Crosshairs)
    .OnGenerateWidget(this, &SUltiCrossConfigDialog::GenerateCrosshairListWidget)
    .OnSelectionChanged(this, &SUltiCrossConfigDialog::OnCrosshairChanged)
    .Content()
    [
      SNew(STextBlock)
      .Text(CrosshairViewModel, &FUltiCrosshairViewModel::GetCrosshairName)
      .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
      .ColorAndOpacity(FLinearColor::Black)
    ];
}

TSharedRef<SWidget> SUltiCrossConfigDialog::ConstructPropertiesPanel()
{
  return SNew(SScrollBox)
    .Style(SUWindowsStyle::Get(), "UT.ScrollBox.Borderless")

    +SScrollBox::Slot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    .Padding(FMargin(0.0f, 15.0f, 0.0f, 15.0f))
    [
      SNew(SVerticalBox)

      +SVerticalBox::Slot()
      .HAlign(HAlign_Fill)
      .AutoHeight()
      .Padding(FMargin(40.0f, 0.0f, 10.0f, 5.0f))
      [
        SNew(SHorizontalBox)

        // Caption
        +SHorizontalBox::Slot()
        .AutoWidth()
        [
          SNew(STextBlock)
          .MinDesiredWidth(200)
          .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small.Bold")
          .Text(FText::FromString(TEXT("Type")))
        ]

        // Combo Box
        +SHorizontalBox::Slot()
        .FillWidth(1)
        [
          SAssignNew(CrosshairTypeComboBox, SComboBox<TSharedPtr<FUltiCrosshairTypeDescriptor>>)
          .InitiallySelectedItem(CrosshairViewModel->GetCrosshairTypeDescriptor())
          .ComboBoxStyle(SUTStyle::Get(), "UT.ComboBox")
          .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
          .OptionsSource(CrosshairViewModel->GetCrosshairTypeDescriptors())
          .OnGenerateWidget(this, &SUltiCrossConfigDialog::GenerateCrosshairTypeListWidget)
          .OnSelectionChanged(CrosshairViewModel, &FUltiCrosshairViewModel::OnTypeChanged)
          .Content()
          [
            SNew(STextBlock)
            .Text(CrosshairViewModel, &FUltiCrosshairViewModel::GetTypeText)
            .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
            .ColorAndOpacity(FLinearColor::Black)
          ]
        ]
      ]

      +AddSlider(FText::FromString(TEXT("Thickness")), CrosshairViewModel->GetDelegate("Thickness"))
      +AddSlider(FText::FromString(TEXT("Rotation")), CrosshairViewModel->GetDelegate("Rotation"))

      // Crosshair Type Specific Settings
      +SVerticalBox::Slot()
      .HAlign(HAlign_Fill)
      .AutoHeight()
      [
        SNew(SWidgetSwitcher)
        .WidgetIndex(CrosshairViewModel, &FUltiCrosshairViewModel::GetCrosshairTypeDescriptorIndex)

        // These slots must be in the same order as those in FUltiCrosshairViewModel::CrosshairTypes.
        +SWidgetSwitcher::Slot()
        [
          SNew(SVerticalBox)

          +AddSlider(FText::FromString(TEXT("Count")), CrosshairViewModel->GetDelegate("Crosshairs.Count"))
          +AddSlider(FText::FromString(TEXT("Length")), CrosshairViewModel->GetDelegate("Crosshairs.Length"))
          +AddSlider(FText::FromString(TEXT("Center Gap")), CrosshairViewModel->GetDelegate("Crosshairs.CenterGap"))
        ]

        +SWidgetSwitcher::Slot()
        [
          SNew(SCanvas)
        ]

        +SWidgetSwitcher::Slot()
        [
          SNew(SCanvas)
        ]

        +SWidgetSwitcher::Slot()
        [
          SNew(SCanvas)
        ]
      ]

      // Fill Color Picker
      +SVerticalBox::Slot()
      .HAlign(HAlign_Fill)
      .AutoHeight()
      .Padding(FMargin(40.0f, 15.0f, 10.0f, 5.0f))
      [
        SNew(SHorizontalBox)

        // Caption
        +SHorizontalBox::Slot()
        .AutoWidth()
        [
          SNew(STextBlock)
          .MinDesiredWidth(200)
          .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small.Bold")
          .Text(FText::FromString(TEXT("Fill Color")))
        ]

        +SHorizontalBox::Slot()
        .FillWidth(1)
        [
          SNew(SColorPicker)
          .DisplayInlineVersion(true)
          .UseAlpha(true)
          .TargetColorAttribute(CrosshairViewModel, &FUltiCrosshairViewModel::GetFillColor)
          .OnColorCommitted(CrosshairViewModel, &FUltiCrosshairViewModel::SetFillColor)
          .PreColorCommitted(CrosshairViewModel, &FUltiCrosshairViewModel::SetFillColor)
        ]
      ]

      // Outline Color Picker
      +SVerticalBox::Slot()
      .HAlign(HAlign_Fill)
      .AutoHeight()
      .Padding(FMargin(40.0f, 15.0f, 10.0f, 5.0f))
      [
        SNew(SHorizontalBox)

        // Caption
        +SHorizontalBox::Slot()
        .AutoWidth()
        [
          SNew(STextBlock)
          .MinDesiredWidth(200)
          .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small.Bold")
          .Text(FText::FromString(TEXT("Outline Color")))
        ]

        +SHorizontalBox::Slot()
        .FillWidth(1)
        [
          SNew(SColorPicker)
          .DisplayInlineVersion(true)
          .UseAlpha(true)
          .TargetColorAttribute(CrosshairViewModel, &FUltiCrosshairViewModel::GetOutlineColor)
          .OnColorCommitted(CrosshairViewModel, &FUltiCrosshairViewModel::SetOutlineColor)
          .PreColorCommitted(CrosshairViewModel, &FUltiCrosshairViewModel::SetOutlineColor)
        ]
      ]
    ];
}

void SUltiCrossConfigDialog::OnCrosshairChanged(UUltiCrosshair* NewSelection, ESelectInfo::Type SelectType)
{
  CrosshairViewModel->SetCrosshair(NewSelection);
}

TSharedRef<SWidget> SUltiCrossConfigDialog::GenerateCrosshairListWidget(UUltiCrosshair* InItem)
{
  return SNew(SBox)
    .Padding(5)
    [
      SNew(STextBlock)
      .Text(InItem->CrosshairName)
      .TextStyle(SUTStyle::Get(), "UT.Font.ContextMenuItem")
    ];
}

TSharedRef<SWidget> SUltiCrossConfigDialog::GenerateCrosshairTypeListWidget(TSharedPtr<FUltiCrosshairTypeDescriptor> InItem)
{
  return SNew(SBox)
    .Padding(5)
    [
      SNew(STextBlock)
      .Text(InItem->Text())
      .TextStyle(SUTStyle::Get(), "UT.Font.ContextMenuItem")
    ];
}

void SUltiCrossConfigDialog::GatherCrosshairs()
{
  if (HUD == nullptr)
  {
    return;
  }

  for (const TPair<FName, UUTCrosshair*>& Pair : HUD->Crosshairs)
  {
    UUltiCrosshair *UltiCrosshair = Cast<UUltiCrosshair>(Pair.Value);
    if (UltiCrosshair == nullptr)
    {
      continue;
    }

    Crosshairs.Add(UltiCrosshair);
  }

  if (Crosshairs.Num() > 0)
  {
    CrosshairViewModel->SetCrosshair(Crosshairs[0]);
  }
}

SVerticalBox::FSlot& SUltiCrossConfigDialog::AddSlider(FText Caption, TSharedRef<FConstrainedSliderDelegate> Delegate)
{
  return SVerticalBox::Slot()
  .HAlign(HAlign_Fill)
  .AutoHeight()
  .Padding(FMargin(40.0f, 15.0f, 10.0f, 5.0f))
  [
    SNew(SHorizontalBox)

    // Caption
    +SHorizontalBox::Slot()
    .AutoWidth()
    [
      SNew(STextBlock)
      .MinDesiredWidth(200)
      .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small.Bold")
      .Text(Caption)
    ]

    +SHorizontalBox::Slot()
    .FillWidth(1)
    [
      SNew(SHorizontalBox)

      // Edit Box
      +SHorizontalBox::Slot()
      .AutoWidth()
      [
        SNew(SEditableTextBox)
        .Style(SUTStyle::Get(), "UT.EditBox.Boxed")
        .ForegroundColor(FLinearColor::Black)
        .MinDesiredWidth(48.0f)
        .Text(Delegate, &FConstrainedSliderDelegate::Text)
      ]

      // Slider
      +SHorizontalBox::Slot()
      .FillWidth(1)
      [
        SNew(SSlider)
        .IndentHandle(false)
        .Style(SUTStyle::Get(), "UT.Slider")
        .Value(Delegate, &FConstrainedSliderDelegate::Get)
        .OnValueChanged(Delegate, &FConstrainedSliderDelegate::Set)
      ]
    ]
  ];
}

void SUltiCrossConfigDialog::OnDialogResult(TSharedPtr<SCompoundWidget> Widget, uint16 ButtonId)
{
  for (UUltiCrosshair* Crosshair : Crosshairs)
  {
    Crosshair->SaveConfig();
  }
}

#endif
