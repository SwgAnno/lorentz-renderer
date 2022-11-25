#include "guilabel.h"

GuiLabel::GuiLabel( const std::string& i_msg,
                    const GltFontAscii& i_font ) :
  GuiComponent(1,1) , _font(i_font) ,_msg(i_msg)
{

}

GuiLabel::~GuiLabel()
{
  //dtor
}

void GuiLabel::draw()
{
  draw_backgournd() ;
  draw_border();

  black.glColor() ;

// really tricky workaround to draw text to screen where i want
// with glt old GL mechanics
#ifdef GUI_DEBUG
  std::cerr << " Calling GuiLabel draw\n" ;

  const int bytesPerChar = ((8+7)>>3)*14;
  unsigned char cond ;
  glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID , &cond ) ;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  const char * const h = (const char * const) courier14Font;
  unsigned char* cara = courier14Font + strlen(h) +1 ;

  //glRasterPos3f(0.5f,0.5f,0.f) ;
  std::cerr << cond << _x << " " << _y << std::endl ;

  black.glColor() ;
  for(int i=0 ; i<20 ; i++)
    glBitmap(8 , 14 ,0,13 ,10,0, cara +bytesPerChar*(65+i) ) ;

#endif // GUI_DEBUG

  glRasterPos3f(_x , _y+_h/2, 0.f) ;
  GuiLabel::_font.print(_msg);
  GLERROR ;
}
