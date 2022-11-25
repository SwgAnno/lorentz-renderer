#include "scenecontroller.h"

SceneController::SceneController( ) :
    _pause(false), _joystick('\00')
{
}

SceneController::SceneController( observable_p i_obj ) :
    _pause(false), _joystick('\00'), _obj( i_obj)
{
}

SceneController::~SceneController()
{
  //dtor
}


void SceneController::move()
{
  if( !_obj)
    return ;

  if( !_pause )
  {
  _obj->lock() ;
  _obj->move() ;
  _obj->unlock() ;
  }
  else
  {
  _obj->lock() ;
  _obj->v() += _obj->a()*DT ;
  _obj->unlock() ;
  }


#ifdef INPUT_DEBUG
  std::cerr << "Controller: " <<static_cast<int>(_joystick) << std::endl ;
#endif // INPUT_DEBUG
}

void SceneController::OnTick()
{
  updateV() ;
  move();
}

void SceneController::OnMotion(const float& d_theta, const float& d_phi)
{
  if( _obj )
  {
  _obj->lock() ;
  _obj->theta() += d_theta ;
  _obj->phi()   -= d_phi ;  //screen movement has opposite direction to view movement
  _obj->unlock() ;
  }

}

void SceneController::OnKeyboard(unsigned char& key, int& x, int& y)
{
  switch(key)
  {
  case 'w' :
    _joystick |= DIR_FWD*3 ;
    break ;
  case 's' :
    _joystick |= DIR_FWD ;
    break ;
  case 'd' :
    _joystick |= DIR_SIDE*3 ;
    break ;
  case 'a' :
    _joystick |= DIR_SIDE ;
    break ;
  case ' ' :
    _joystick |= DIR_UP*3 ;
    break ;
  case 'q' :
    _joystick |= DIR_UP ;
    break ;
  }

}


void SceneController::OnKeyboardUp( unsigned char& key, int& x, int& y)
{
#ifdef INPUT_DEBUG
  std::cerr << (int)key << " "
            << (int)(_joystick & ~(DIR_FWD*3))  << " "
            << (int)(_joystick & ~(DIR_SIDE*3)) << " "
            << (int)(_joystick & ~(DIR_UP*3))   << " "
            << std::endl ;
#endif // INPUT_DEBUG

    switch(key)
  {
  case 'p' :
    _pause = !_pause ;
    break ;
  case 'w' :
  case 's' :
    _joystick &= ~(DIR_FWD*3)  ;
    break ;
  case 'd' :
  case 'a' :
    _joystick &= ~(DIR_SIDE*3)  ;
    break ;
  case ' ' :
  case 'q' :
    _joystick &= ~(DIR_UP*3)  ;
    break ;
  case 'z' :
    Vector3 zero(0,0,0) ;
    if( _obj )
    {
    _obj->lock() ;
    _obj->v() = zero ;
    _obj->a() = zero ;
    _obj->unlock() ;
    }
    break ;

  }

}

void SceneController::updateV()
{
  //OpneGL has strange xyz axis
  //here we do a conversion between WASD and opengl movement
  //taking into consideration its effect on ViewMatrix

  if( _obj )
  {
    _obj->lock() ;
    _obj->a() +=
        Vector3( ( _joystick&DIR_FWD ? sin(_obj->theta()) : 0)
                  * (_joystick&(DIR_FWD*2) ? 1 : -1) ,
                  ( _joystick&DIR_UP   ? 1 : 0)
                  * (_joystick&(DIR_UP*2)   ? 1 : -1) ,
                  ( _joystick&DIR_FWD  ? -cos(_obj->theta()) : 0)
                  * (_joystick&(DIR_FWD*2)  ? 1 : -1) )
        +
        Vector3( ( _joystick&DIR_SIDE ? cos(_obj->theta()) : 0)
                  * (_joystick&(DIR_SIDE*2) ? 1 : -1) ,
                  0                                  ,
                  ( _joystick&DIR_SIDE  ? sin(_obj->theta()) : 0)
                  * (_joystick&(DIR_SIDE*2)  ? 1 : -1) ) ;
    _obj->a().normalize() ;
    _obj->a()*= 8 ;

    _obj->unlock() ;
  }
}
