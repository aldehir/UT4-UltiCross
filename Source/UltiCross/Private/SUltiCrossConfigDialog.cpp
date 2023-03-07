#include "UltiCrossPCH.h"
#include "SUltiCrossConfigDialog.h"

#include "SColorPicker.h"
#include "SWidgetSwitcher.h"

#include "Stubs/SUTStyle.h"
#include "Stubs/SUWindowsStyle.h"

#if !UE_SERVER


void SUltiCrossConfigDialog::Construct(const FArguments& InArgs)
{
  PlayerController = InArgs._PlayerController;
  OnSelectionChanged = InArgs._OnSelectionChanged;

  SUTDialogBase::Construct(SUTDialogBase::FArguments()
    .PlayerOwner(InArgs._PlayerOwner)
    .DialogTitle(InArgs._DialogTitle)
    .DialogSize(InArgs._DialogSize)
    .bDialogSizeIsRelative(InArgs._bDialogSizeIsRelative)
    .DialogPosition(InArgs._DialogPosition)
    .DialogAnchorPoint(InArgs._DialogAnchorPoint)
    .IsScrollable(false)
    .ContentPadding(InArgs._ContentPadding)
    .ButtonMask(UTDIALOG_BUTTON_APPLY)
    .OnDialogResult(this, &SUltiCrossConfigDialog::OnDialogResult)
  );

  GatherCrosshairs();

  if (Crosshairs.Num() > 0)
  {
    CrosshairViewModel->SetCrosshair(Crosshairs[0]);
  }

  for (UUltiCrosshair* Crosshair : Crosshairs)
  {
    if (Crosshair->CrosshairTag.Compare(InArgs._InitiallySelected) == 0)
    {
      CrosshairViewModel->SetCrosshair(Crosshair);
      break;
    }
  }

  UpdateCopyFromCrosshairs();

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
            .Padding(FMargin(32))
            [
              SNew(SBox)
              .WidthOverride(192)
              .HeightOverride(192)
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
  SAssignNew(CrosshairComboBox, SComboBox<UUltiCrosshair*>)
    .InitiallySelectedItem(CrosshairViewModel->GetCrosshair())
    .ComboBoxStyle(SUTStyle::Get(), "UT.ComboBox")
    .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
    .OptionsSource(&Crosshairs)
    .OnGenerateWidget(this, &SUltiCrossConfigDialog::GenerateCrosshairListWidget)
    .OnSelectionChanged(this, &SUltiCrossConfigDialog::OnCrosshairChanged)
    .Content()
    [
      SNew(STextBlock)
      .Text(CrosshairViewModel, &FUltiCrosshairViewModel::GetName)
      .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
      .ColorAndOpacity(FLinearColor::Black)
    ];

  return CrosshairComboBox.ToSharedRef();
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

      +AddNameEdit()

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

          +AddSlider(FText::FromString(TEXT("Thickness")), CrosshairViewModel->GetDelegate("Crosshairs.Thickness"))
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
          SNew(SVerticalBox)

          +AddSlider(FText::FromString(TEXT("Thickness")), CrosshairViewModel->GetDelegate("Circle.Thickness"))
          +AddSlider(FText::FromString(TEXT("Radius")), CrosshairViewModel->GetDelegate("Circle.Radius"))
        ]

        +SWidgetSwitcher::Slot()
        [
          SNew(SCanvas)
        ]
      ]

      +AddSlider(FText::FromString(TEXT("Outline")), CrosshairViewModel->GetDelegate("Outline"))
      +AddSlider(FText::FromString(TEXT("Dot Radius")), CrosshairViewModel->GetDelegate("DotRadius"))

      +AddSlider(FText::FromString(TEXT("Rotate")), CrosshairViewModel->GetDelegate("Transform.Rotate"))
      +AddSlider(FText::FromString(TEXT("Translate X")), CrosshairViewModel->GetDelegate("Transform.Translate.X"))
      +AddSlider(FText::FromString(TEXT("Translate Y")), CrosshairViewModel->GetDelegate("Transform.Translate.Y"))

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

TSharedRef<SWidget> SUltiCrossConfigDialog::BuildCustomButtonBar()
{
  TSharedPtr<SUniformGridPanel> LeftButtonBar;
  SAssignNew(LeftButtonBar, SUniformGridPanel)
    .SlotPadding(FMargin(0.0f, 0.0f, 10.0f, 10.0f));

  uint32 ButtonCount = 0;

  // Add Close button to the left of the dialog.
  BuildButton(LeftButtonBar, NSLOCTEXT("SUTDialogBase", "CloseButton", "CLOSE"), UTDIALOG_BUTTON_CLOSE, ButtonCount);

  return LeftButtonBar.ToSharedRef();
}

void SUltiCrossConfigDialog::OnCrosshairChanged(UUltiCrosshair* NewSelection, ESelectInfo::Type SelectType)
{
  CrosshairViewModel->SetCrosshair(NewSelection);
  OnSelectionChanged.ExecuteIfBound(NewSelection->CrosshairTag);
  UpdateCopyFromCrosshairs();
}

TSharedRef<SWidget> SUltiCrossConfigDialog::GenerateCrosshairListWidget(UUltiCrosshair* InItem)
{
  return SNew(SBox)
    .Padding(5)
    [
      SNew(STextBlock)
      .Text_UObject(InItem, &UUltiCrosshair::GetUserDefinedNameAsText)
      .TextStyle(SUTStyle::Get(), "UT.Font.ContextMenuItem")
    ];
}

TSharedRef<SWidget> SUltiCrossConfigDialog::GenerateCopyFromMenu()
{
  TSharedRef<SVerticalBox> MenuBox = SNew(SVerticalBox);

  for (UUltiCrosshair* Crosshair : CopyFromCrosshairs)
  {
    MenuBox->AddSlot()
      .HAlign(HAlign_Fill)
      .AutoHeight()
      .Padding(FMargin(0.0f, 5.0f))
      [
        SNew(SButton)
        .Text_UObject(Crosshair, &UUltiCrosshair::GetUserDefinedNameAsText)
        .TextStyle(SUTStyle::Get(), "UT.Font.ContextMenuItem")
        .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
        .ContentPadding(FMargin(5.0f))
        .OnClicked(this, &SUltiCrossConfigDialog::OnCopyFromItemClicked, Crosshair)
      ];
  }

  return SNew(SOverlay)
    +SOverlay::Slot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    [
      SNew(SImage)
      .Image(SUTStyle::Get().GetBrush("UT.HeaderBackground.SuperLight"))
    ]

    +SOverlay::Slot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    [
      SNew(SBox)
      .HeightOverride(300.0f)
      .WidthOverride(200.0f)
      [
        SNew(SScrollBox)
        .Style(SUWindowsStyle::Get(), "UT.ScrollBox.Borderless")
        .ConsumeMouseWheel(EConsumeMouseWheel::Always)  // By default, it will scroll the parent when we reached the end

        +SScrollBox::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0.0f, 5.0f))
        [
          MenuBox
        ]
      ]
    ];
}

FReply SUltiCrossConfigDialog::OnCopyFromItemClicked(UUltiCrosshair* InItem)
{
  CrosshairViewModel->CopyCrosshair(InItem);
  CopyFromComboButton->SetIsOpen(false);
  return FReply::Handled();
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
  Crosshairs.Empty();
  FUltiCross::Get()->GetUltiCrosshairs(GetPlayerOwner().Get(), Crosshairs);
}

void SUltiCrossConfigDialog::UpdateCopyFromCrosshairs()
{
  CopyFromCrosshairs.Empty();

  for (UUltiCrosshair* Crosshair : Crosshairs)
  {
    if (Crosshair->CrosshairTag.Compare(CrosshairViewModel->GetCrosshair()->CrosshairTag) == 0)
    {
      continue;
    }

    CopyFromCrosshairs.Add(Crosshair);
  }
}

void SUltiCrossConfigDialog::AddReferencedObjects(FReferenceCollector& Collector)
{
  for (int32 i = 0; i < Crosshairs.Num(); ++i)
  {
    Collector.AddReferencedObject(Crosshairs[i]);
  }
}

SVerticalBox::FSlot& SUltiCrossConfigDialog::AddNameEdit()
{
  return SVerticalBox::Slot()
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
      .Text(FText::FromString(TEXT("Name")))
    ]

    +SHorizontalBox::Slot()
    .FillWidth(1)
    [
      SNew(SHorizontalBox)

      // Name Edit Box
      +SHorizontalBox::Slot()
      .FillWidth(1)
      [
        SNew(SEditableTextBox)
        .Style(SUTStyle::Get(), "UT.EditBox.Boxed")
        .ForegroundColor(FLinearColor::Black)
        .Text(CrosshairViewModel, &FUltiCrosshairViewModel::GetName)
        .OnTextCommitted(CrosshairViewModel, &FUltiCrosshairViewModel::OnNameChanged)
      ]

      // Copy From Button
      +SHorizontalBox::Slot()
      .Padding(10.0f, 0.0f, 0.0f, 0.0f)
      .AutoWidth()
      [
        SAssignNew(CopyFromComboButton, SComboButton)
        .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
        .OnGetMenuContent(this, &SUltiCrossConfigDialog::GenerateCopyFromMenu)
        .ButtonContent()
        [
          SNew(STextBlock)
          .Text(FText::FromString(TEXT("Copy From")))
          .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
          .ColorAndOpacity(FLinearColor::Black)
        ]
      ]
    ]
  ];
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
        .OnTextCommitted(Delegate, &FConstrainedSliderDelegate::CommitText)
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
  if (ButtonId != UTDIALOG_BUTTON_APPLY)
  {
    return;
  }

  AUTHUD *HUD = nullptr;

  if (PlayerController.IsValid())
  {
    AUTPlayerController* UTPlayerController = Cast<AUTPlayerController>(PlayerController.Get());
    if (UTPlayerController != nullptr) {
      HUD = UTPlayerController->MyUTHUD;
    }
  }

  for (UUltiCrosshair* Crosshair : Crosshairs)
  {
    Crosshair->SaveCrosshair();

    // Update CDO to for new instantiations of the crosshair
    UUltiCrosshair* CDO = GetMutableDefault<UUltiCrosshair>(Crosshair->GetClass());
    CDO->CopyCrosshairParameters(Crosshair);

    // If there is a HUD, then update the instance in play
    if (HUD != nullptr) {
      if (HUD->Crosshairs.Contains(Crosshair->CrosshairTag))
      {
        UUltiCrosshair* LiveCrosshair = Cast<UUltiCrosshair>(HUD->Crosshairs[Crosshair->CrosshairTag]);
        if (LiveCrosshair != nullptr)
        {
          LiveCrosshair->CopyCrosshairParameters(Crosshair);
          LiveCrosshair->UpdateTexture();
        }
      }
    }
  }
}

#endif
