#include "selfdrive/ui/qt/atom/paint.h"

#include <cmath>

#include <QDebug>

#include "selfdrive/common/timing.h"
#include "selfdrive/ui/qt/util.h"




// OnroadHud
OnPaint::OnPaint(QWidget *parent) : QWidget(parent) 
{

}



void OnPaint::paintEvent(QPaintEvent *event) 
{
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);


  // max speed
  //QRect rc(bdr_s * 2, bdr_s * 1.5, 184, 202);

  QRect rc(bdr_s * 2, bdr_s * 1.5 + 100, 184, 202);
  p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  p.setBrush(QColor(0, 0, 0, 100));
  p.drawRoundedRect(rc, 20, 20);
  p.setPen(Qt::NoPen);
}