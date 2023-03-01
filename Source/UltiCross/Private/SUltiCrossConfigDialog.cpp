#include "UltiCrossPCH.h"
#include "SUltiCrossConfigDialog.h"
#include "SColorPicker.h"

#include "Stubs/SUTStyle.h"
#include "Stubs/SUWindowsStyle.h"

#include "Fixtures.h"

#if !UE_SERVER

static SVerticalBox::FSlot& AddSlider(FText Caption) {
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
      .Text(FText::FromString(TEXT("1")))
    ]

    // Slider
    +SHorizontalBox::Slot()
    .FillWidth(1)
    [
      SNew(SSlider)
      .IndentHandle(false)
      .Style(SUTStyle::Get(), "UT.Slider")
      .Value(1)
    ]
  ];
}

void SUltiCrossConfigDialog::Construct(const FArguments& InArgs)
{
  SUTDialogBase::Construct(SUTDialogBase::FArguments()
    .PlayerOwner(InArgs._PlayerOwner)
    .DialogTitle(InArgs._DialogTitle)
    .DialogSize(InArgs._DialogSize)
    .bDialogSizeIsRelative(InArgs._bDialogSizeIsRelative)
    .DialogPosition(InArgs._DialogPosition)
    .DialogAnchorPoint(InArgs._DialogAnchorPoint)
    .IsScrollable(false)
    .ContentPadding(InArgs._ContentPadding)
    .ButtonMask(InArgs._ButtonMask)
    .OnDialogResult(InArgs._OnDialogResult)
  );
  
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
                .Image(ExampleCrosshair)
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
          SNew(SComboBox<TSharedPtr<FString>>)
          .InitiallySelectedItem(ExampleCrosshairList[0])
          .ComboBoxStyle(SUTStyle::Get(), "UT.ComboBox")
          .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
          .OptionsSource(&ExampleCrosshairList)
          .OnGenerateWidget(this, &SUTDialogBase::GenerateStringListWidget)
          .Content()
          [
            SAssignNew(SelectedCrosshair, STextBlock)
            .Text(FText::FromString(*ExampleCrosshairList[0].Get()))
            .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
            .ColorAndOpacity(FLinearColor::Black)
          ]
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
          
          // Shape
          +SVerticalBox::Slot()
          .HAlign(HAlign_Fill)
          .AutoHeight()
          .Padding(FMargin(40.0f, 15.0f, 10.0f, 5.0f))
          [
            SNew(SHorizontalBox)
            +SHorizontalBox::Slot()
            .AutoWidth()
            [
              SNew(STextBlock)
              .MinDesiredWidth(200)
              .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small.Bold")
              .Text(FText::FromString(TEXT("Type")))
            ]

            +SHorizontalBox::Slot()
            .FillWidth(1)
            [
              SNew(SComboBox<TSharedPtr<FString>>)
              .InitiallySelectedItem(ShapeList[0])
              .ComboBoxStyle(SUTStyle::Get(), "UT.ComboBox")
              .ButtonStyle(SUTStyle::Get(), "UT.SimpleButton.Bright")
              .OptionsSource(&ShapeList)
              .OnGenerateWidget(this, &SUTDialogBase::GenerateStringListWidget)
              .Content()
              [
                SAssignNew(SelectedShape, STextBlock)
                .Text(FText::FromString(*ShapeList[0].Get()))
                .TextStyle(SUTStyle::Get(), "UT.Font.NormalText.Small")
                .ColorAndOpacity(FLinearColor::Black)
              ]
            ]
          ]
          
          // Placeholders for now
          +AddSlider(FText::FromString(TEXT("Thickness")))
          +AddSlider(FText::FromString(TEXT("Crosshair Length")))
          +AddSlider(FText::FromString(TEXT("Center Gap")))
          +AddSlider(FText::FromString(TEXT("Opacity")))
          +AddSlider(FText::FromString(TEXT("Outline Opacity")))
          +AddSlider(FText::FromString(TEXT("Dot Size")))
          +AddSlider(FText::FromString(TEXT("Dot Opacity")))

          +SVerticalBox::Slot()
          .AutoHeight()
          [
            SNew(SColorPicker)
            .DisplayInlineVersion(true)
            .UseAlpha(true)
          ]
        ]
      ]
    ];
  }
}

#endif