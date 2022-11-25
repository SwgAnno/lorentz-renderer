#ifndef SCENEMASTER_H
#define SCENEMASTER_H

#include <scene/scenecamera.h>

class SceneMaster
{
  public:
    SceneMaster();
    virtual ~SceneMaster() ;

    virtual void render( std::shared_ptr<SceneCamera> ) = 0;
    virtual void update() {} ;  //move objs
    virtual void setup() {} ;   //setup collision & movement logic

    virtual void OnTick() {} ;
    virtual void OnKeyboard(unsigned char& key, int& x, int& y) {} ;
    virtual void OnKeyboardUp(unsigned char& key, int& x, int& y) {} ;
    virtual void OnMouse(int& button, int& state, int& x, int& y) {} ;
    virtual void OnMotion(const float& x,const float& y) {} ;

  protected:

  private:
};

#endif // SCENEMASTER_H
