#ifndef GUI_WINDOW_H_INCLUDED
#define GUI_WINDOW_H_INCLUDED

#include<glutm/window.h>
#include<glt/gl.h>
#include<glt/rgb.h>

#include<GUI/guipanel.h>
#include<GUI/guilabel.h>
#include<GUI/guislider.h>
#include<GUI/guiscene.h>

class GuiWindow : public GlutWindow
{
public :

  GuiWindow
  (
    const std::string &title       = titleDefault,
    const int          width       = widthDefault,
    const int          height      = heightDefault,
    const int          x           = xDefault,
    const int          y           = yDefault,
    const unsigned int displayMode = displayModeDefault
  );
//  GuiWindow();

  virtual void OnOpen() ;
  virtual void OnClose() ;
  virtual void OnDisplay() ;
  virtual void OnTick() ;
  virtual void OnMouse(int button, int state, int x, int y) ;
  virtual void OnMotion(int x, int y) ;
  virtual void OnPassiveMotion(int x, int y) ;
  virtual void OnKeyboard(unsigned char key, int x, int y) ;
  virtual void OnKeyboardUp(unsigned char key, int x, int y) ;
  virtual void OnReshape(int w,int h) ;


protected :

  GuiComponent _root ;
  GltFontAscii _font ;
  GltClearColor _clear;

  int _mouse[4] ; //x,y ,dx,dy

private :

};

#endif // GUI_WINDOW_H_INCLUDED
