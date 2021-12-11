#pragma once

#include <QStackedLayout>
#include <QWidget>


#include "selfdrive/ui/ui.h"



class OnPaint : public QWidget 
{
  Q_OBJECT

public:
  explicit OnPaint(QWidget *parent);
  void updateState(const UIState &s);

private:
  void paintEvent(QPaintEvent *event) override;
  void drawText(QPainter &p, int x, int y, const QString &text, QColor qColor = QColor(255,255,255,255) );
  
private:
  UIState  *scene;

private:
  //void ui_draw_circle_image_rotation(const UIState *s, int center_x, int center_y, int radius, const char *image, NVGcolor color, float img_alpha, float angleSteers = 0);
  int  bb_ui_draw_measure(QPainter &p,  const QString &bb_value, const QString &bb_uom, const QString &bb_label,
    int bb_x, int bb_y, int bb_uom_dx,
    QColor bb_valueColor, QColor bb_labelColor, QColor bb_uomColor,
    int bb_valueFontSize, int bb_labelFontSize, int bb_uomFontSize );


  void bb_ui_draw_measures_right(QPainter &p, int bb_x, int bb_y, int bb_w );
  void bb_ui_draw_measures_left(QPainter &p, int bb_x, int bb_y, int bb_w );

  QColor get_tpms_color(float tpms);
  QString get_tpms_text(float tpms);

  void  bb_draw_tpms(QPainter &p, int viz_tpms_x, int viz_tpms_y );
  void  bb_draw_compass(QPainter &p, int compass_x, int compass_y );

  void  bb_ui_draw_UI(QPainter &p);
};
