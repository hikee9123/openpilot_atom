#include "selfdrive/ui/paint.h"

#include <cassert>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define NANOVG_GL3_IMPLEMENTATION
#define nvgCreate nvgCreateGL3
#else
#include <GLES3/gl3.h>
#define NANOVG_GLES3_IMPLEMENTATION
#define nvgCreate nvgCreateGLES3
#endif

#define NANOVG_GLES3_IMPLEMENTATION
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

#include "selfdrive/common/util.h"
#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/ui.h"

#include "selfdrive/ui/qt/atom/navi.h"
#include "selfdrive/ui/qt/atom/dashcam.h"
#include "selfdrive/ui/qt/atom/kegman_ui.h"



void ui_draw_image(const UIState *s, const Rect &r, const char *name, float alpha) {
  nvgBeginPath(s->vg);
  NVGpaint imgPaint = nvgImagePattern(s->vg, r.x, r.y, r.w, r.h, 0, s->images.at(name), alpha);
  nvgRect(s->vg, r.x, r.y, r.w, r.h);
  nvgFillPaint(s->vg, imgPaint);
  nvgFill(s->vg);
}

void ui_draw_rect(NVGcontext *vg, const Rect &r, NVGcolor color, int width, float radius) {
  nvgBeginPath(vg);
  radius > 0 ? nvgRoundedRect(vg, r.x, r.y, r.w, r.h, radius) : nvgRect(vg, r.x, r.y, r.w, r.h);
  nvgStrokeColor(vg, color);
  nvgStrokeWidth(vg, width);
  nvgStroke(vg);
}

static inline void fill_rect(NVGcontext *vg, const Rect &r, const NVGcolor *color, const NVGpaint *paint, float radius) {
  nvgBeginPath(vg);
  radius > 0 ? nvgRoundedRect(vg, r.x, r.y, r.w, r.h, radius) : nvgRect(vg, r.x, r.y, r.w, r.h);
  if (color) nvgFillColor(vg, *color);
  if (paint) nvgFillPaint(vg, *paint);
  nvgFill(vg);
}
void ui_fill_rect(NVGcontext *vg, const Rect &r, const NVGcolor &color, float radius) {
  fill_rect(vg, r, &color, nullptr, radius);
}
void ui_fill_rect(NVGcontext *vg, const Rect &r, const NVGpaint &paint, float radius) {
  fill_rect(vg, r, nullptr, &paint, radius);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

void ui_draw(UIState *s, int w, int h) {
  // Update intrinsics matrix after possible wide camera toggle change
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  nvgBeginFrame(s->vg, s->fb_w, s->fb_h, 1.0f);

  //BB START: add new measures panel  const int bb_dml_w = 180;
  bb_ui_draw_UI(s);
  //BB END: add new measures panel    
  update_dashcam(s);

  ui_main_navi( s );
  nvgEndFrame(s->vg);
  glDisable(GL_BLEND);
}

void ui_nvg_init(UIState *s) {
  // on EON, we enable MSAA
  s->vg = Hardware::EON() ? nvgCreate(0) : nvgCreate(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
  assert(s->vg);

  // init fonts
  std::pair<const char *, const char *> fonts[] = {
      {"sans-regular", "../assets/fonts/opensans_regular.ttf"},
      {"sans-semibold", "../assets/fonts/opensans_semibold.ttf"},
      {"sans-bold", "../assets/fonts/opensans_bold.ttf"},
  };
  for (auto [name, file] : fonts) {
    int font_id = nvgCreateFont(s->vg, name, file);
    assert(font_id >= 0);
  }

  // init images
  std::vector<std::pair<const char *, const char *>> images = {
    {"traf_turn", "../assets/img_trafficSign_turn.png"}, 
    {"compass", "../assets/addon/Image/img_compass.png"},
    {"direction", "../assets/addon/Image/img_direction.png"},
    {"tire_pressure", "../assets/addon/Image/img_tire_pressure.png"},

    {"speed_30", "../assets/addon/navigation/img_30_speedahead.png"},
    {"speed_40", "../assets/addon/navigation/img_40_speedahead.png"},
    {"speed_50", "../assets/addon/navigation/img_50_speedahead.png"},
    {"speed_60", "../assets/addon/navigation/img_60_speedahead.png"},
    {"speed_70", "../assets/addon/navigation/img_70_speedahead.png"},
    {"speed_80", "../assets/addon/navigation/img_80_speedahead.png"},
    {"speed_90", "../assets/addon/navigation/img_90_speedahead.png"},
    {"speed_100", "../assets/addon/navigation/img_100_speedahead.png"},
    {"speed_110", "../assets/addon/navigation/img_110_speedahead.png"},
    {"speed_var", "../assets/addon/navigation/img_var_speedahead.png"}, 
    {"img_space", "../assets/addon/navigation/img_space.png"},
    {"car_left",  "../assets/addon/navigation/img_car_left.png"},
    {"car_right", "../assets/addon/navigation/img_car_right.png"},
    {"speed_bump", "../assets/addon/navigation/img_speed_bump.png"},
    {"bus_only", "../assets/addon/navigation/img_bus_only.png"},
  };
  for (auto [name, file] : images) {
    s->images[name] = nvgCreateImage(s->vg, file, 1);
    if( s->images[name] == 0 )
    {
      printf("images is NULL FileName = %s\n", file);
    }
    assert(s->images[name] != 0);
  }
}
