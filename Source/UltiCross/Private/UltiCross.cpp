#include "UltiCrossPCH.h"
#include "TestCrosshair.h"

#include "AssetRegistryModule.h"

// #pragma comment(lib, "cairo.lib")

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "cairo/cairo.h"

class FUltiCross : public IModuleInterface
{
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

private:
  UTestCrosshair *ExampleCrosshair;
  UTexture2D *ExampleTexture;
};

IMPLEMENT_MODULE(FUltiCross, UltiCross);


void generate_test_image(uint8 *data);

void FUltiCross::StartupModule()
{
  AUTHUD *hud = AUTHUD::StaticClass()->GetDefaultObject<AUTHUD>();
  UE_LOG(LogUltiCross, Log, TEXT("Loaded HUD @ %p"), hud);

  TArray<FAssetData> AssetList;
  GetAllBlueprintAssetData(UTestCrosshair::StaticClass(), AssetList);

  for (const FAssetData& Asset: AssetList)
  {
    static FName NAME_GeneratedClass(TEXT("GeneratedClass"));
    const FString* ClassPath = Asset.TagsAndValues.Find(NAME_GeneratedClass);

    UE_LOG(LogUltiCross, Log, TEXT("Found asset %s, Class Path: %s"), *Asset.AssetName.ToString(), **ClassPath);
  }

  ExampleTexture = UTexture2D::CreateTransient(32, 32);

  FTexture2DMipMap *MipMap = &ExampleTexture->PlatformData->Mips[0];
  FByteBulkData *ImageData = &MipMap->BulkData;

  uint8 *RawData = (uint8*)ImageData->Lock(LOCK_READ_WRITE);
  FMemory::Memset(RawData, 0, 4*32*32);

  generate_test_image(RawData);

  ImageData->Unlock();
  ExampleTexture->UpdateResource();

  UTestCrosshair::TextureSlots[0] = ExampleTexture;
}

void FUltiCross::ShutdownModule()
{
  UTestCrosshair::TextureSlots[0] = nullptr;
  //AUTHUD *hud = AUTHUD::StaticClass()->GetDefaultObject<AUTHUD>();
  // hud->Crosshairs.Remove(ExampleCrosshair->CrosshairTag);
}

void generate_test_image(uint8 *data) {
  UE_LOG(LogUltiCross, Log, TEXT("Generating Image with Cairo: START"));

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 32);
  cairo_surface_t *s = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, 32, 32, stride);
  cairo_t *cr = cairo_create(s);

  // cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
  cairo_set_line_width(cr, 1.0);

  cairo_save(cr);
  cairo_arc(cr, 16, 16, 4.0, 0.0, 2*M_PI);

  // Fill with white
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
  cairo_fill_preserve(cr);

  // Stroke with black
  cairo_restore(cr);
  cairo_stroke(cr);

  if (cr) {
    cairo_destroy(cr);
  }

  if (s) {
    cairo_surface_destroy(s);
  }

  UE_LOG(LogUltiCross, Log, TEXT("Generating Image with Cairo: END"));
}
