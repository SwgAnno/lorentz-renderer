#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

//keyboard and mouse event driven Observable Controller

#include<vector>

#include<scene/sceneobservable.h>

class SceneController
{
  public:

    static const int DIR_FWD = 0x01 ;
    static const int DIR_SIDE = 0x04 ;
    static const int DIR_UP = 0x10 ;

    SceneController( );
    SceneController( observable_p );
    virtual ~SceneController();

    observable_p& obj() { return _obj; } ;

    void move() ;

    void OnTick() ;
    void OnMotion( const float& d_theta, const float& d_phi) ;
    void OnKeyboard(   unsigned char& key, int& x, int& y) ;
    void OnKeyboardUp( unsigned char& key, int& x, int& y) ;

  protected:

    void updateV() ;

    observable_p _obj ;

    bool _pause ;
    char _joystick ;

  private:
};

#endif // SCENECONTROLLER_H
