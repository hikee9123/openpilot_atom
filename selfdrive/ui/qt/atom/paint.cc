#include "selfdrive/ui/qt/atom/paint.h"

#include <cmath>

#include <QDebug>

#include "selfdrive/common/timing.h"
#include "selfdrive/ui/qt/util.h"




// OnroadHud
OnPaint::OnPaint(QWidget *parent) : QWidget(parent) 
{
   state = &QUIState::ui_state;
   scene = &QUIState::ui_state.scene;
}

void OnPaint::updateState(const UIState &s)
{
   //scene = s;
}

void OnPaint::paintEvent(QPaintEvent *event) 
{
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);


  // max speed
  //QRect rc(bdr_s * 2, bdr_s * 1.5, 184, 202);

  //QRect rc(bdr_s * 2, bdr_s * 1.5 + 100, 184, 202);
 // p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  //p.setBrush(QColor(0, 0, 0, 100));
  //p.drawRoundedRect(rc, 20, 20);
  //p.setPen(Qt::NoPen);
  if( scene )
    bb_ui_draw_UI( p );
}

void OnPaint::drawText(QPainter &p, int x, int y, const QString &text, QColor qColor ) 
{
  QFontMetrics fm(p.font());
  QRect init_rect = fm.boundingRect(text);
  QRect real_rect = fm.boundingRect(init_rect, 0, text);
  real_rect.moveCenter({x, y - real_rect.height() / 2});

  p.setPen( qColor ); //QColor(0xff, 0xff, 0xff, alpha));
  p.drawText(real_rect.x(), real_rect.bottom(), text);
}

///////////////////////////////////////////////////////////////////////////////
//
//

/*
void OnPaint::ui_draw_circle_image_rotation(const UIState *s, int center_x, int center_y, int radius, const char *image, QColor color, float img_alpha, float angleSteers)
{
  const int img_size = radius * 1.5;
  float img_rotation =  angleSteers/180*3.141592;
  int ct_pos = -radius * 0.75;

  nvgBeginPath(s->vg);
  nvgCircle(s->vg, center_x, center_y + (bdr_s+7), radius);
  nvgFillColor(s->vg, color);
  nvgFill(s->vg);
  //ui_draw_image(s, {center_x - (img_size / 2), center_y - (img_size / 2), img_size, img_size}, image, img_alpha);

  nvgSave( s->vg );
  nvgTranslate(s->vg, center_x, (center_y + (bdr_s*1.5)));
  nvgRotate(s->vg, -img_rotation);  

  ui_draw_image(s, {ct_pos, ct_pos, img_size, img_size}, image, img_alpha);
  nvgRestore(s->vg); 
}
*/

//BB START: functions added for the display of various items
int OnPaint::bb_ui_draw_measure(QPainter &p,  const QString &bb_value, const QString &bb_uom, const QString &bb_label,
    int bb_x, int bb_y, int bb_uom_dx,
    QColor bb_valueColor, QColor bb_labelColor, QColor bb_uomColor,
    int bb_valueFontSize, int bb_labelFontSize, int bb_uomFontSize )
{
 


  //nvgTextAlign(s->vg, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE);
  int dx = 0;
  int nLen = bb_uom.length();
  if (nLen > 0) {
    dx = (int)(bb_uomFontSize*2.5/2);
   }


  //print value
  configFont( p, "Open Sans",  bb_valueFontSize*2, "SemiBold");
  drawText( p, bb_x-dx/2, bb_y+ (int)(bb_valueFontSize*2.5)+5, bb_value, bb_valueColor );
  //print label
  configFont( p, "Open Sans",  bb_valueFontSize*1, "Regular");
  drawText( p, bb_x, bb_y + (int)(bb_valueFontSize*2.5)+5 + (int)(bb_labelFontSize*2.5)+5, bb_label, bb_labelColor);

  //print uom
  if (nLen > 0) {

    int rx =bb_x + bb_uom_dx + bb_valueFontSize -3;
    int ry = bb_y + (int)(bb_valueFontSize*2.5/2)+25;
    configFont( p, "Open Sans",  bb_uomFontSize*2, "Regular");

    p.save();
    drawText( p, rx, ry, bb_uom, bb_uomColor);
    p.ratate( -90 );
    p.restore();
  }
  return (int)((bb_valueFontSize + bb_labelFontSize)*2.5) + 5;
}


void OnPaint::bb_ui_draw_measures_right( QPainter &p, int bb_x, int bb_y, int bb_w ) 
{
  int bb_rx = bb_x + (int)(bb_w/2);
  int bb_ry = bb_y;
  int bb_h = 5;
  QColor lab_color = QColor(255, 255, 255, 200);
  QColor uom_color = QColor(255, 255, 255, 200);
  int value_fontSize=25;
  int label_fontSize=15;
  int uom_fontSize = 15;
  int bb_uom_dx =  (int)(bb_w /2 - uom_fontSize*2.5) ;


  QString val_str;
  QString uom_str;
  //add CPU temperature
  if( true ) 
  {

    QColor val_color = QColor(255, 255, 255, 200);

    auto  maxCpuTemp = scene->deviceState.getCpuTempC();
    int   cpuPerc = scene->deviceState.getCpuUsagePercent()[0];
    float cpuTemp = maxCpuTemp[0];


      if( cpuTemp > 80) {
        val_color = QColor(255, 188, 3, 200);
      }
      if(cpuTemp > 92) {
        val_color = QColor(255, 0, 0, 200);
      }

       // temp is alway in C * 10
      val_str.sprintf("%.1f", cpuTemp );
      uom_str.sprintf("%d", cpuPerc);
      bb_h += bb_ui_draw_measure(p,  val_str, uom_str, "CPU TEMP",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );

    bb_ry = bb_y + bb_h;
  }

  //add GPU temperature
  if( 0 ) 
  {
    QColor val_color = QColor(255, 255, 255, 200);
    auto  maxGpuTemp = scene->deviceState.getGpuTempC();
    float  memTemp = scene->deviceState.getMemoryTempC();
    float gpuTemp = maxGpuTemp[0];
    
      if( gpuTemp > 80) {
        val_color = QColor(255, 188, 3, 200);
      }
      if(gpuTemp > 92) {
        val_color = QColor(255, 0, 0, 200);
      }

       // temp is alway in C * 10
      val_str.sprintf("%.1f", gpuTemp );
      uom_str.sprintf("%.1f", memTemp);       
      bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "GPU TEMP",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );

    bb_ry = bb_y + bb_h;
  } 

   //add battery temperature
  if( true )
  {
    QColor val_color = QColor(255, 255, 255, 200);
    float  batteryTemp = scene->deviceState.getBatteryTempCDEPRECATED();

    if(batteryTemp > 40) {
      val_color = QColor(255, 188, 3, 200);
    }
    if(batteryTemp > 50) {
      val_color = QColor(255, 0, 0, 200);
    }
    // temp is alway in C * 1000
    val_str.sprintf("%.1f", batteryTemp );
    uom_str = "";        
    bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "BAT TEMP",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }



  //add grey panda GPS accuracy
  if (true) {
    auto gps_ext = scene->gpsLocationExternal;
    float  gpsAccuracyUblox = gps_ext.getAccuracy();
    float  altitudeUblox = gps_ext.getAltitude();



    QColor val_color = QColor(255, 255, 255, 200);
    //show red/orange if gps accuracy is low
      if(gpsAccuracyUblox > 2.0) {
         val_color = QColor(255, 188, 3, 200);
      }
      if(gpsAccuracyUblox > 5.0) {
         val_color = QColor(255, 0, 0, 200);
      }
    // gps accuracy is always in meters
    if(gpsAccuracyUblox > 99 || gpsAccuracyUblox == 0) {
       val_str = "None";
    }else if(gpsAccuracyUblox > 9.99) {
      val_str.sprintf("%.1f", gpsAccuracyUblox );
    }
    else {
      val_str.sprintf("%.2f", gpsAccuracyUblox );
    }
    uom_str.sprintf("%.1f", altitudeUblox); 
    bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "GPS PREC",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //finally draw the frame
  bb_h += 20;


  QRect rc( bb_x, bb_y, bb_w, bb_h);
  p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 5));
  p.setBrush(QColor(0, 0, 0, 100));
  p.drawRoundedRect(rc, 20, 20);
  p.setPen(Qt::NoPen);

}


void OnPaint::bb_ui_draw_measures_left(QPainter &p, int bb_x, int bb_y, int bb_w ) 
{
  int bb_rx = bb_x + (int)(bb_w/2);
  int bb_ry = bb_y;
  int bb_h = 5;
  QColor lab_color = QColor(255, 255, 255, 200);
  QColor uom_color = QColor(255, 255, 255, 200);
  int value_fontSize=25;
  int label_fontSize=15;
  int uom_fontSize = 15;
  int bb_uom_dx =  (int)(bb_w /2 - uom_fontSize*2.5) ;


  auto radar_state = (*state->sm)["radarState"].getRadarState();  // radar
  auto lead_radar = radar_state.getLeadOne();


  QString val_str;
  QString uom_str;

  //add visual radar relative distance
  if( true )
  {
    QColor val_color = QColor(255, 255, 255, 200);

    if ( lead_radar.getStatus() ) {
      //show RED if less than 5 meters
      //show orange if less than 15 meters
      float d_rel2 = lead_radar.getDRel();
      if((int)(d_rel2) < 15) {
        val_color = QColor(255, 188, 3, 200);
      }
      if((int)(d_rel2) < 5) {
        val_color = QColor(255, 0, 0, 200);
      }
      // lead car relative distance is always in meters
      val_str.sprintf("%d", (int)d_rel2 );
    } else {
       val_str = "-";
    }

    auto lead_cam = (*state->sm)["modelV2"].getModelV2().getLeadsV3()[0];  // camera
    if (lead_cam.getProb() > 0.1) {
      float d_rel1 = lead_cam.getX()[0];

      //uom_color = QColor(255, 255, 255, 200);
      uom_str.sprintf("%d", (int)d_rel1 );
    }
    else
    {
      uom_str = "-";
    }

    bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "REL DIST",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //add visual radar relative speed
  if( true )
  {
    QColor val_color = QColor(255, 255, 255, 200);
    if ( lead_radar.getStatus() ) {
      float v_rel = lead_radar.getVRel();  
      //show Orange if negative speed (approaching)
      //show Orange if negative speed faster than 5mph (approaching fast)
      if((int)(v_rel) < 0) {
        val_color = QColor(255, 188, 3, 200);
      }
      if((int)(v_rel) < -5) {
        val_color = QColor(255, 0, 0, 200);
      }
      // lead car relative speed is always in meters
      if (scene->is_metric) {
        val_str.sprintf("%d", (int)(v_rel * 3.6 + 0.5) );
      } else {
        val_str.sprintf("%d", (int)(v_rel * 2.2374144 + 0.5));
      }
    } else {
       val_str = "-";
    }
    if (scene->is_metric) {
      uom_str = "km/h";
    } else {
      uom_str = "mph";
    }
    bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "REL SPEED",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //add  steering angle
  if( true )
  {
    float angleSteers = scene->car_state.getSteeringAngleDeg();

    QColor val_color = QColor(0, 255, 0, 200);
      //show Orange if more than 30 degrees
      //show red if  more than 50 degrees
      if(((int)(angleSteers) < -30) || ((int)(angleSteers) > 30)) {
        val_color = QColor(255, 175, 3, 200);
      }
      if(((int)(angleSteers) < -55) || ((int)(angleSteers) > 55)) {
        val_color = QColor(255, 0, 0, 200);
      }
      // steering is in degrees
      val_str.sprintf("%.1f",angleSteers);
      uom_str = "";
    bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "REAL STEER",
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //add  desired steering angle
  if( true )
  {
    float angleSteersDes = scene->controls_state.getSteeringAngleDesiredDegDEPRECATED();  

    QColor val_color = QColor(255, 255, 255, 200);
    if( scene->controls_state.getEnabled() ) {
      //show Orange if more than 6 degrees
      //show red if  more than 12 degrees
      if(((int)(angleSteersDes) < -30) || ((int)(angleSteersDes) > 30)) 
      {
        val_color = QColor(255, 255, 255, 200);
      }
      if( ((int)(angleSteersDes) < -50) || ((int)(angleSteersDes) > 50) ) 
      {
        val_color = QColor(255, 255, 255, 200);
      }
      // steering is in degrees
      val_str.sprintf("%.1f",angleSteersDes);
    } else {
      val_str = "-";
    }

    uom_str = "";
    bb_h +=bb_ui_draw_measure(p,  val_str, uom_str, "DESIR STEER",
      bb_rx, bb_ry, bb_uom_dx,
      val_color, lab_color, uom_color,
      value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }


  //finally draw the frame
  bb_h += 20;

  QRect rc( bb_x, bb_y, bb_w, bb_h);
  p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 5));
  p.setBrush(QColor(0, 0, 0, 100));
  p.drawRoundedRect(rc, 20, 20);
  p.setPen(Qt::NoPen);  
}


// tpms by neokii
QColor OnPaint::get_tpms_color(float tpms) 
{
    if(tpms < 5 || tpms > 60) // N/A
        return QColor(255, 255, 255, 200);
    if(tpms < 30)
        return QColor(255, 90, 90, 200);
    return QColor(255, 255, 255, 200);
}

QString OnPaint::get_tpms_text(float tpms) 
{
    if(tpms < 5 || tpms > 200)
        return "";

    QString str;
    str.sprintf("%.0f", round(tpms) );
    return str;
}

void OnPaint::bb_draw_tpms(QPainter &p, int viz_tpms_x, int viz_tpms_y )
{
    //const UIScene *scene = &s->scene;
    auto car_state = (*state->sm)["carState"].getCarState();
    auto tpms = car_state.getTpms();

    const float fl = tpms.getFl();
    const float fr = tpms.getFr();
    const float rl = tpms.getRl();
    const float rr = tpms.getRr();

    const int w = 58;
    const int h = 126;
    int x = viz_tpms_x;// bdr_s + 80;
    int y = viz_tpms_y - h;// s->fb_h - bdr_s - h - 60;

    const int margin = 10;


    configFont( p, "Open Sans",  60, "SemiBold");
    drawText( p, x-margin, y+45, get_tpms_text(fl) );
    drawText( p, x+w+margin, y+45, get_tpms_text(fr) );

    drawText( p, x-margin, y+h-15, get_tpms_text(rl) );
    drawText( p, x+w+margin, y+h-15, get_tpms_text(rr) );
}


//draw compass by opkr and re-designed by hoya
void OnPaint::bb_draw_compass(QPainter &p, int compass_x, int compass_y )
{

  //auto   gps_ext = scene.gpsLocationExternal;
 // float  bearingUblox = gps_ext.getBearingDeg();


 // const int radius = 130;// 85 + 40;
 // ui_draw_circle_image_rotation(s, compass_x, compass_y, radius, "direction", QColor(0, 0, 0, 0), 0.7f, -bearingUblox);
 // ui_draw_circle_image_rotation(s, compass_x, compass_y, radius, "compass", QColor(0, 0, 0, 0), 0.8f);

}


void OnPaint::bb_ui_draw_UI(QPainter &p)
{
  const int bb_dml_w = 180;
  const int bb_dml_x = (0 + bdr_s);
  const int bb_dml_y = (0 + bdr_s) + 220;

  const int bb_dmr_w = 180;
  const int bb_dmr_x = 0 + state->fb_w - bb_dmr_w - bdr_s;
  const int bb_dmr_y = (0 + bdr_s) + 220;

  // 1. kegman ui
  bb_ui_draw_measures_left(p, bb_dml_x, bb_dml_y, bb_dml_w);
  bb_ui_draw_measures_right(p, bb_dmr_x, bb_dmr_y, bb_dmr_w);

  // 2. tpms
  if( true )
  {
    int viz_tpms_x = state->fb_w - bb_dmr_w / 2 - 60;
    int viz_tpms_y = state->fb_h - bdr_s - 220;  
    bb_draw_tpms( p, viz_tpms_x, viz_tpms_y);
  }

  // 3. compass
  if( 0 )
  {
    const int compass_x = state->fb_w / 2 - 20;
    const int compass_y = state->fb_h - 40;
    bb_draw_compass( p, compass_x, compass_y );
  }
}
//BB END: functions added for the display of various items
