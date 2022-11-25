#include "guiwindow.h"


GuiWindow::GuiWindow(const std::string &title,
                     const int width, const int height,
                     const int x, const int y,
                     const unsigned int displayMode)
                    :
  GlutWindow(title, width, height, x, y, displayMode) ,
  _root(width, height)
{

#ifdef GUI_DEBUG
  std::cerr << "GuiWindow constructor\n" ;
#endif // GUI_DEBUG

  _mouse[0] = 0 ;
  _mouse[1] = 0 ;
  _mouse[2] = 0 ;
  _mouse[3] = 0 ;
}

//GuiWindow::GuiWindow() : GlutWindow()
//  {}

void GuiWindow::OnOpen()
{
  _clear.set(yellow);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  _font.init(courier14Font) ;

  float h_div[3] = {2.f/10, 7.f/10, 1.f/10} ;
  gui_p h_box( new GuiPanel(false , 3 , h_div )) ;

  gui_p x_box( new GuiPanel(true , 5)) ;

  gui_p lab1( new GuiLabel("Bada", _font )) ;
  gui_p lab2( new GuiLabel("Bam", _font )) ;
  gui_p lab3( new GuiLabel("bip", _font )) ;
  gui_p lab4( new GuiLabel("bop", _font )) ;
  x_box->add(lab1,0) ;
  x_box->add(lab2,1) ;
  x_box->add(lab3,3) ;
  x_box->add(lab4,4) ;

  h_box->add(x_box,0) ;

  _root.add(h_box) ;
}

void GuiWindow::OnClose()
{

}

void GuiWindow::OnDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) ;

  _root.draw() ;

}

void GuiWindow::OnTick()
{
#ifdef TICK_DEBUG
  std::cerr << "Tick GuiWindow" << std::endl ;
#endif // TICK_DEBUG
  _root.OnTick() ;

  postRedisplay();
}

void GuiWindow::OnMouse(int button, int state, int x, int y)
{
  _mouse[0] = x ;
  _mouse[1] = y ;

  //map screen coordinate do gl coordinate
  float gl_x = -1.f + 2*static_cast<float>(x) / _root.px_w() ;
  float gl_y =  1.f - 2*static_cast<float>(y) / _root.px_h() ;
#ifdef  INPUT_DEBUG
  std::cerr << "Mouse event: b" << button << " s"
                                << state  << " p: "
                                << gl_x   << "\t"
                                << gl_y   << std::endl ;
#endif // INPUT_DEBUG

  _root.OnMouse( button, state ,
                 gl_x , gl_y ) ;

}

void GuiWindow::OnMotion(int x, int y)
{
  _mouse[2] = x - _mouse[0] ;
  _mouse[3] = y - _mouse[1] ;
  _mouse[0] = x ;
  _mouse[1] = y ;


  _root.OnMotion(_mouse[2], _mouse[3]) ;

}

void GuiWindow::OnPassiveMotion(int x, int y)
{
  _mouse[0] = x ;
  _mouse[1] = y ;
}

void GuiWindow::OnKeyboard(unsigned char key, int x, int y)
{
#ifdef INPUT_DEBUG
  std::cerr << "Key input "
            << key << " "
            << x << " "
            << y << "\n" ;
#endif // DEBUG
  _root.OnKeyboard(key, x, y);

}

void GuiWindow::OnKeyboardUp(unsigned char key, int x, int y)
{
#ifdef INPUT_DEBUG
  std::cerr << "KeyUp input  "
            << key << " "
            << x << " "
            << y << "\n" ;
#endif // DEBUG
  _root.OnKeyboardUp(key, x, y);

}

void GuiWindow::OnReshape(int w,int h){
  GlutWindow::OnReshape( w, h) ;
  _root.resize(w,h) ;
}
