#include "UltiCrossPCH.h"
#include "Fixtures.h"

#include "Renderer/CairoCrosshairRenderer.h"

UTexture2D *CreateExampleCrosshairTexture()
{
  UTexture2D *Tex = UTexture2D::CreateTransient(64, 64);

  FCairoCrosshairRenderer Renderer;
  FRenderContext* RenderContext = Renderer.CreateContext(Tex);

  RenderContext->Begin();
  Renderer.Clear(RenderContext);
  Renderer.RenderCrosshairs(RenderContext, 1.0, 8.0, 2.0, FLinearColor::White, FLinearColor::Black);
  RenderContext->End();

  return Tex;
}

void CreateExampleCrosshairList(TArray<TSharedPtr<FString>>& Array)
{
  Array.Add(TSharedPtr<FString>(new FString("Crosshair #1")));
  Array.Add(TSharedPtr<FString>(new FString("Crosshair #2")));
  Array.Add(TSharedPtr<FString>(new FString("Crosshair #3")));
}