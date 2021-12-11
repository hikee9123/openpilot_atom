#pragma once

#include <QStackedLayout>
#include <QWidget>


#include "selfdrive/ui/ui.h"



class OnPaint : public QWidget 
{
  Q_OBJECT

public:
  OnPaint(QWidget *parent);


private:
  void paintEvent(QPaintEvent *event) override;

};
