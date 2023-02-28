#include "UltiCrossPCH.h"
#include "Fixtures.h"

#include "cairo/cairo.h"

UTexture2D *CreateExampleCrosshairTexture()
{
  UTexture2D* Tex = UTexture2D::CreateTransient(32, 32);
  FTexture2DMipMap* MipMap = &Tex->PlatformData->Mips[0];
  FByteBulkData* ImageData = &MipMap->BulkData;

  uint8* data = (uint8*)ImageData->Lock(LOCK_READ_WRITE);
  FMemory::Memset(data, 0, 4 * 32 * 32);

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 32);
  cairo_surface_t *s = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, 32, 32, stride);
  cairo_t *cr = cairo_create(s);

  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
  cairo_set_line_width(cr, 1.0);

  cairo_save(cr);
  cairo_arc(cr, 16, 16, 4.0, FMath::DegreesToRadians(0), FMath::DegreesToRadians(360));

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

  ImageData->Unlock();
  Tex->UpdateResource();
  return Tex;
}

void CreateExampleCrosshairList(TArray<TSharedPtr<FString>>& Array)
{
  Array.Add(TSharedPtr<FString>(new FString("Crosshair #1")));
  Array.Add(TSharedPtr<FString>(new FString("Crosshair #2")));
  Array.Add(TSharedPtr<FString>(new FString("Crosshair #3")));
}