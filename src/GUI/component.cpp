#include "component.h"

// static float px_ux = 0, px_uy = 0;
//global variable for small graphics adjustment
//to be handled/updated by Window class


GuiComponent::GuiComponent(const float& i_x,
                           const float& i_y,
                           const float& i_w,
                           const float& i_h,
                           GuiComponent* i_parent) :
 _focus(false), _parent(i_parent)
{
  _x = _parent->_x + i_x *_parent->_w ;
  _y = _parent->_y + i_y *_parent->_h ;
  _w = _parent->_w * i_w ;
  _h = _parent->_h * i_h ;
}

GuiComponent::GuiComponent(const float& i_x,
                           const float& i_y,
                           const int& i_w,
                           const int& i_h,
                           GuiComponent* i_parent) :
 _focus(false), _parent(i_parent)
{
  _x = _parent->x() + i_x *_parent->w() ;
  _y = _parent->y() + i_y *_parent->h() ;

  i_parent = root() ; //reusing variable

  _w = static_cast<float>(i_w) / i_parent->px_w() ;
  _h = static_cast<float>(i_h) / i_parent->px_h() ;
}

GuiComponent::GuiComponent(const int& i_x,
                           const int& i_y,
                           const int& i_w,
                           const int& i_h,
                           GuiComponent* i_parent) :
  _focus(false), _parent(i_parent)
{
  i_parent = root() ; //reusing variable

  _x = static_cast<float>(i_x) / i_parent->px_w() ;
  _y = static_cast<float>(i_y) / i_parent->px_h() ;
  _w = static_cast<float>(i_w) / i_parent->px_w() ;
  _h = static_cast<float>(i_h) / i_parent->px_h() ;
}

GuiComponent::GuiComponent(const int& i_w, const int& i_h) :
  _px_w(i_w), _px_h(i_h),
  _x(-1.f), _y(-1.f),
  _w(2.f), _h(2.f),
  _focus(false), _parent(this)
  {}

void GuiComponent::fitInto(const gui_p& n_p)
{

#ifdef GUI_DEBUG
  std::cerr << "fitting "
            << n_p->_x << "\t" << n_p->_y << "\t" << n_p->_w << "\t" << n_p->_h << "\n" ;
#endif // GUI_DEBUG

  float n_x = n_p->_x + ( _x - _parent->_x ) / _parent->_w * n_p->_w ;
  float n_y = n_p->_y + ( _y - _parent->_y ) / _parent->_h * n_p->_h ;
  float n_w = n_p->_w / _parent->_w * _w ;
  float n_h = n_p->_h / _parent->_h * _h;

  for(auto i: _children)
  {
    i->resize( n_x, n_y, n_w, n_h);
  }

  _x = n_x ;
  _y = n_y ;
  _w = n_w ;
  _h = n_h ;
}

void GuiComponent::fitInto(GuiComponent* n_p)
{
#ifdef GUI_DEBUG
  std::cerr << "fitting "
            << n_p->_x << "\t" << n_p->_y << "\t" << n_p->_w << "\t" << n_p->_h << "\n" ;
#endif // GUI_DEBUG

  //children have to relate to old relative measure
  float n_x = n_p->_x + ( _x - _parent->_x ) / _parent->_w * n_p->_w ;
  float n_y = n_p->_y + ( _y - _parent->_y ) / _parent->_h * n_p->_h ;
  float n_w = n_p->_w / _parent->_w * _w ;
  float n_h = n_p->_h / _parent->_h * _h;

  //but have to adapt to new pixel measure
  _parent = n_p->_parent ;

  for(auto i: _children)
  {
    i->resize( n_x, n_y, n_w, n_h);
  }

  _x = n_x ;
  _y = n_y ;
  _w = n_w ;
  _h = n_h ;
}

void GuiComponent::resize( const float& p_x,  const float& p_y, const float& p_w, const float& p_h)
{
  float n_x = p_x + ( _x - _parent->_x ) / _parent->_w * p_w ;
  float n_y = p_y + ( _y - _parent->_y ) / _parent->_h * p_h ;
  float n_w = p_w / _parent->_w * _w ;
  float n_h = p_h / _parent->_h * _h;

//recursivly adjust children then change measure
  for(auto i: _children)
  {
    i->resize( n_x, n_y, n_w, n_h);
  }

  _x = n_x ;
  _y = n_y ;
  _w = n_w ;
  _h = n_h ;
}

void GuiComponent::resize( const int& w, const int& h)
{
  _px_w = w ;
  _px_h = h ;

  this->resize(_x, _y, _w, _h) ; //Nothing has changed
}

void GuiComponent::draw()
{

#ifdef GUI_DEBUG
std::cerr << "Drawing backgourd"
          << _x << "\t" << _y << "\t" << _w << "\t" << _h << "\n" ;
#endif // GUI_DEBUG

  if(_children.size() == 0)
    {
      draw_border();
#ifdef GUI_DEBUG
    draw_cross() ;
#endif // GUI_DEBUG
    }

  GLERROR;

  draw_children() ;

}

void GuiComponent::draw_children()
{
  for(auto i : _children)
    i->draw();
}

void GuiComponent::draw_border()
{
  white.glColor() ;
  glBegin(GL_LINES) ;
    glVertex3f(_x     , _y     , 0.f) ;
      glVertex3f(_x + _w, _y     , 0.f) ;
    glVertex3f(_x + _w, _y     , 0.f) ;
      glVertex3f(_x + _w, _y + _h, 0.f) ;
    glVertex3f(_x + _w, _y + _h, 0.f) ;
      glVertex3f(_x     , _y + _h, 0.f) ;
    glVertex3f(_x     , _y + _h, 0.f) ;
      glVertex3f(_x     , _y     , 0.f) ;
  glEnd() ;

  GLERROR;
}

void GuiComponent::draw_cross()
{
  black.glColor() ;
  glBegin(GL_LINES) ;
    glVertex3f(_x     , _y     , 0.) ;
    glVertex3f(_x + _w, _y + _h, 0.) ;
    glVertex3f(_x     , _y + _h, 0.) ;
    glVertex3f(_x + _w, _y     , 0.) ;
  glEnd() ;

  GLERROR;
}

void GuiComponent::draw_backgournd()
{
  _background.glColor() ;
  glBegin(GL_QUADS) ;
    glVertex3f(_x     , _y     , 0.) ;
    glVertex3f(_x + _w, _y     , 0.) ;
    glVertex3f(_x + _w, _y + _h, 0.) ;
    glVertex3f(_x     , _y + _h, 0.) ;
  glEnd() ;

  GLERROR;
}

int GuiComponent::px_w()
{
  return static_cast<int>( root()->_px_w*(_w/2)) ;
}

int GuiComponent::px_h()
{
  return static_cast<int>( root()->_px_h*_h/2) ;
}

int GuiComponent::px_x()
{
  return static_cast<int>( root()->_px_w*(_x + 1.f)/2) ;
}

int GuiComponent::px_y()
{
  return static_cast<int>( root()->_px_h*(_y + 1.f)/2) ;
}

float GuiComponent::rel_w()
{
  return _w / _parent->w();
}

float GuiComponent::rel_h()
{
  return _h / _parent->h();
}

float GuiComponent::rel_y()
{
  return (_x - _parent->x()) / _parent->w();
}

float GuiComponent::rel_x()
{
  return (_y - _parent->y()) / _parent->h();
}

float GuiComponent::gl_x(const int& px)
{
  return static_cast<float>(px)/( root()->px_w()) ;
}
float GuiComponent::gl_y(const int& px)
{
  return static_cast<float>(px)/( root()->px_h()) ;
}

float GuiComponent::screen_x(const float& gl)
{
  return static_cast<int>(gl)*( root()->px_w()) ;
}
float GuiComponent::screen_y(const float& gl)
{
  return static_cast<int>(gl)/( root()->px_h()) ;
}

bool GuiComponent::inside(const float& x, const float& y)
{

  return  x >= _x    &&
          x <  _x+_w &&
          y >= _y    &&
          y <  _y+_h  ;
}

void GuiComponent::add(gui_p c)
{
  c->fitInto(this) ;
  c->_parent = this ;
  _children.push_back(c) ;
}

void GuiComponent::add(gui_p c, const int& param)
{
  add(c) ;
}


GuiComponent* GuiComponent::root()
{
  GuiComponent* p = _parent ;

  while(p->parent() != p)
  {
    p = p->parent() ;
  }

#ifdef GUI_DEBUG
    std::cerr << "ROOT FOUND " <<  p->_px_w << " " << _px_h << std::endl ;
#endif // GUI_DEBUG

  return p ;
}

void GuiComponent::OnTick()
{
#ifdef TICK_DEBUG
std::cerr << "Tick GuiComponent " << _children.size() << std::endl ;
#endif // TICK_DEBUG
  for(auto i : _children)
  {
  i->OnTick() ;
  }

}

void GuiComponent::OnKeyboard(unsigned char& key, int& x, int& y)
{
  for(auto i : _children)
    i->OnKeyboard(key, x, y) ;
}

void GuiComponent::OnKeyboardUp(unsigned char& key, int& x, int& y)
{
  for(auto i : _children)
    i->OnKeyboardUp(key, x, y) ;
}

void GuiComponent::OnMouse(int& button, int& state,const float& x,const float& y)
{
  //notify click for interested component
  //notify release for all

  if(!state)
  {
    for(auto i : _children)
    {
      if( i->inside(x,y) )
        i->OnMouse(button, state, x, y) ;
      else
        i->setFocus(false) ;

    }

    if(_children.size() == 0)
      setFocus(true) ;
  }


  else
    for(auto i : _children)
      i->OnMouse(button, state, x, y) ;
}

void GuiComponent::OnMotion( int& dx, int& dy)
{
  for(auto i : _children)
     i->OnMotion(dx, dy) ;
}

void GuiComponent::OnFocus()
{
  for(auto i : _children)
    i->OnFocus() ;
}

void GuiComponent::setFocus(const bool& n_f)
{
  _focus = n_f ;

  if(!n_f)
    for(auto i : _children)
      i->setFocus( n_f) ;
}

std::ostream& operator<<(std::ostream& out, GuiComponent& c)
{
  out << "x: " << c.x() << "\t"
      << "y: " << c.x() << "\t"
      << "w: " << c.w() << "\t"
      << "h: " << c.h() << std::endl;

  return out ;
}


