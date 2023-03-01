#include "../UltiCrossPCH.h"

#include "CairoCrosshairRenderer.h"

FCairoRenderContext::FCairoRenderContext(UTexture2D* Tex)
  : FRenderContext(Tex)
  , CairoSurface(nullptr)
  , CairoContext(nullptr)
{
}

void FCairoRenderContext::Begin()
{
  FRenderContext::Begin();

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, SizeX());
  CairoSurface = cairo_image_surface_create_for_data(RawData, CAIRO_FORMAT_ARGB32, SizeX(), SizeY(), stride);
  CairoContext = cairo_create(CairoSurface);
}

void FCairoRenderContext::End()
{
  cairo_destroy(CairoContext);
  CairoContext = nullptr;

  cairo_surface_destroy(CairoSurface);
  CairoSurface = nullptr;

  FRenderContext::End();
}

FCairoSession::FCairoSession(FCairoRenderContext *RenderContext)
  : Ctx(RenderContext)
{
  cairo_save(Ctx->CairoContext);
}

FCairoSession::~FCairoSession()
{
  cairo_restore(Ctx->CairoContext);
}

void FCairoSession::Clear()
{
  cairo_set_source_rgba(Ctx->CairoContext, 0, 0, 0, 0);
  cairo_set_operator(Ctx->CairoContext, CAIRO_OPERATOR_SOURCE);
  cairo_paint(Ctx->CairoContext);
}

void FCairoSession::MoveTo(const FVector2D& Point)
{
  cairo_move_to(Ctx->CairoContext, Point.X, Point.Y);
}

void FCairoSession::LineTo(const FVector2D& Point)
{
  cairo_line_to(Ctx->CairoContext, Point.X, Point.Y);
}

void FCairoSession::RelativeLineTo(const FVector2D& Vec)
{
  cairo_rel_line_to(Ctx->CairoContext, Vec.X, Vec.Y);
}

void FCairoSession::ClosePath()
{
  cairo_close_path(Ctx->CairoContext);
}

void FCairoSession::Translate(const FVector2D& V)
{
  cairo_translate(Ctx->CairoContext, V.X, V.Y);
}

void FCairoSession::Rotate(float Angle)
{
  cairo_rotate(Ctx->CairoContext, Angle);
}

void FCairoSession::RotateAround(const FVector2D& P, float Angle)
{
  Translate(P);
  Rotate(Angle);
  Translate(-1 * P);
}

void FCairoSession::SetLineWidth(float Width)
{
  cairo_set_line_width(Ctx->CairoContext, Width);
}

void FCairoSession::Fill(const FLinearColor& Color, bool bPreserve)
{
  cairo_set_source_rgba(Ctx->CairoContext, Color.R, Color.G, Color.B, Color.A);

  if (bPreserve)
  {
    cairo_fill_preserve(Ctx->CairoContext);
  }
  else
  {
    cairo_fill(Ctx->CairoContext);
  }
}

void FCairoSession::Stroke(const FLinearColor& Color, bool bPreserve)
{
  cairo_set_source_rgba(Ctx->CairoContext, Color.R, Color.G, Color.B, Color.A);

  if (bPreserve)
  {
    cairo_stroke_preserve(Ctx->CairoContext);
  }
  else
  {
    cairo_stroke(Ctx->CairoContext);
  }
}

FCairoCrosshairRenderer::FCairoCrosshairRenderer()
{
}

FRenderContext* FCairoCrosshairRenderer::CreateContext(UTexture2D* Texture)
{
  return new FCairoRenderContext(Texture);
}

void FCairoCrosshairRenderer::Clear(FRenderContext *Ctx)
{
  FCairoSession Session((FCairoRenderContext*)Ctx);
  Session.Clear();
}

void FCairoCrosshairRenderer::RenderCrosshairs(FRenderContext *Ctx, float Thickness, float Length, float Gap, const FLinearColor& Fill, const FLinearColor& Outline)
{
  FCairoRenderContext* RC = (FCairoRenderContext*)Ctx;
  FCairoSession Session(RC);

  // Make sure the desired value are not out of bounds
  Thickness = FMath::Clamp(Thickness, 1.0f, 10.0f);
  Length = FMath::Clamp(Length, 1.0f, 30.0f);
  Gap = FMath::Clamp(Gap, 0.0f, 10.0f);

  FVector2D Center(RC->SizeX() / 2.0f, RC->SizeY() / 2.0f);

  Session.SetLineWidth(2.0f);

  for (int Angle = 0; Angle < 360; Angle += 90)
  {
    // Rotate around the center.
    Session.RotateAround(Center, FMath::DegreesToRadians(Angle));

    Session.MoveTo(Center + FVector2D(-1 * Thickness, -1 * (Gap + Length)));
    Session.RelativeLineTo(FVector2D(2 * Thickness, 0));
    Session.RelativeLineTo(FVector2D(0, Length));
    Session.RelativeLineTo(FVector2D(-2 * Thickness, 0));
    Session.ClosePath();

    Session.Stroke(Outline, true);
    Session.Fill(Fill);
  }
}