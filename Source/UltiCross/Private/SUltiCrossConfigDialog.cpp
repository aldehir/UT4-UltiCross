#include "UltiCrossPCH.h"
#include "SUltiCrossConfigDialog.h"
#include "SColorPicker.h"

#include "Stubs/SUTStyle.h"
#include "Stubs/SUWindowsStyle.h"

#include "UltiCrosshair.h"

#include "Fixtures.h"

#if !UE_SERVER


void SUltiCrossConfigDialog::Construct(const FArguments& InArgs)
{
  CrosshairViewModel = TSharedPtr<SUltiCrosshairViewModel>(new SUltiCrosshairViewModel(nullptr));

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

  ExampleCrosshair = new FSlateBrush();
  ExampleCrosshair->SetResourceObject(CreateExampleCrosshairTexture());
  
  CreateExampleCrosshairList(ExampleCrosshairList);
  
  ShapeList.Add(TSharedPtr<FString>(new FString("Cross")));
  ShapeList.Add(TSharedPtr<FString>(new FString("Circle")));
  
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
        SNew(SScrollBox)
        .Style(SUWindowsStyle::Get(), "UT.ScrollBox.Borderless")

        +SScrollBox::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0.0f, 15.0f, 0.0f, 15.0f))
        [
          SNew(SVerticalBox)
          
          // Placeholders for now
          +AddSlider(FText::FromString(TEXT("Thickness")), CrosshairViewModel->ThicknessDelegate)
          +AddSlider(FText::FromString(TEXT("Length")), CrosshairViewModel->LengthDelegate)
          +AddSlider(FText::FromString(TEXT("Gap")), CrosshairViewModel->GapDelegate)

          // +SVerticalBox::Slot()
          // .AutoHeight()
          // [
          //   SNew(SColorPicker)
          //   .DisplayInlineVersion(true)
          //   .UseAlpha(true)
          // ]
        ]
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
      .Text(CrosshairViewModel.ToSharedRef(), &SUltiCrosshairViewModel::GetCrosshairName)
      .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
      .ColorAndOpacity(FLinearColor::Black)
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

SVerticalBox::FSlot& SUltiCrossConfigDialog::AddSlider(FText Caption, TSharedPtr<FSliderDelegate> Delegate)
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

    // Edit Box
    +SHorizontalBox::Slot()
    .AutoWidth()
    [
      SNew(SEditableTextBox)
      .Style(SUTStyle::Get(), "UT.EditBox.Boxed")
      .ForegroundColor(FLinearColor::Black)
      .MinDesiredWidth(48.0f)
      .Text(Delegate.ToSharedRef(), &FSliderDelegate::Text)
    ]

    // Slider
    +SHorizontalBox::Slot()
    .FillWidth(1)
    [
      SNew(SSlider)
      .IndentHandle(false)
      .Style(SUTStyle::Get(), "UT.Slider")
      .Value(Delegate.ToSharedRef(), &FSliderDelegate::Get)
      .OnValueChanged(Delegate.ToSharedRef(), &FSliderDelegate::Set)
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