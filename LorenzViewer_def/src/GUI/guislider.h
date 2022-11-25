#ifndef GUISLIDER_H
#define GUISLIDER_H

#include <GUI/component.h>

template <typename T>
class GuiSlider : public GuiComponent
{
  public:
    GuiSlider(const T& max, const T& min);
    virtual ~GuiSlider();

    void resize ( const float& p_x,
                  const float& p_y,
                  const float& p_w,
                  const float& p_h) ;

    void draw() ;

    void set( T arg ) ;
    T value() ;

  protected:

    bool _active ;

    int _px_min , _px_max ;
    int _setVal ;
    T   _min    , _max ;
    T   _retVal ;

    void OnMouse(int& button, int& state,const float& x,const float& y) ;
    void OnMotion( int& dx, int& dy) ;

  private:
};

////////////////////////////////////////////////////
//Template function must be written entirely on a single file
// otherwise some function may not be compiled

template <typename T>
GuiSlider<T>::GuiSlider(const T& max, const T& min) :
  GuiComponent(1,1) ,
  _px_min(0), _px_max(2),
  _setVal(_px_min) ,
  _min(min) , _max(max)
{

}

template <typename T>
GuiSlider<T>::~GuiSlider()
{
  //dtor
}

//resize to new parent measures
template <typename T>
void GuiSlider<T>::resize( const float& p_x,  const float& p_y, const float& p_w, const float& p_h)
{
  GuiComponent::resize(p_x, p_y, p_w, p_h) ;

  float rateo = static_cast<float>( ( _setVal ) )/(_px_max) ;

  _px_max = std::max( 1, static_cast<int>( .8f*px_w() ) ) ;
  _setVal = rateo* _px_max ;

}

template <typename T>
void GuiSlider<T>::draw()
{
  GuiComponent::draw_backgournd() ;
  GuiComponent::draw_border() ;

  float p_x = static_cast<float>(_setVal)/_px_max *  _w*.8f ;

  black.glColor() ;
  glBegin( GL_LINES) ;
    glVertex2f( _x + .1*_w ,
                _y + .5*_h ) ;
    glVertex2f( _x + .9*_w ,
                _y + .5*_h ) ;
  glEnd() ;
  glBegin( GL_QUADS) ;
    glVertex2f( _x +  .1*_w + p_x ,
                _y + .3*_h  ) ;
    glVertex2f( _x + .12*_w + p_x ,
                _y + .5*_h  ) ;
    glVertex2f( _x + .1*_w  + p_x ,
                _y + .7*_h  ) ;
    glVertex2f( _x + .08*_w + p_x ,
                _y + .5*_h  ) ;
  glEnd() ;

}

template <typename T>
void GuiSlider<T>::set(T arg)
{
       if( arg > _max)
         _setVal = _px_max ;
  else if( arg < _min)
         _setVal = 0 ;
  else
        _setVal =  (arg - _min)/( _max - _min) * _px_max;
}

template <typename T>
T GuiSlider<T>::value()
{
  return _min +
        static_cast<float>( _setVal )/_px_max * (_max - _min) ;
}

template <typename T>
void GuiSlider<T>::OnMouse(int& button, int& state,const float& x,const float& y)
{
  switch( button )
  {
  case 0:
    if(!state)
      _active = true ;
    else
      _active = false ;

    break ;
  case 1:

    break ;
  case 2:

    break ;
  }
}

template <typename T>
void GuiSlider<T>::OnMotion( int& dx, int& dy)
{
  if( _active )
  {
    _setVal = (dx > 0 ? std::min( _px_max , _setVal + dx) :
                        std::max( 0 , _setVal + dx) ) ;
  }

}

template class GuiSlider<float> ;
template class GuiSlider<int> ;
template class GuiSlider<double> ;

#endif // GUISLIDER_H
