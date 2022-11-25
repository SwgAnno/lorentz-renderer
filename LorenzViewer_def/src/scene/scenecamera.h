#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include <scene/sceneobservable.h>

class SceneCamera
{
  public:
    SceneCamera( std::shared_ptr<SceneObservable> );
    virtual ~SceneCamera();

    void set() ;
    void reset() ;

    //resize frustum to screen
    void setAspectRatio(const float& w, const float& h) ;

    //return useful transform info
    Matrix translate() ;
    Matrix orient() ;
    Matrix v_orient() ;
    Matrix v_orient( const Vector3& ) ;

    std::shared_ptr<SceneObservable>& pov() {return _pov ;} ;

  protected:

    void initMat() ;

    Matrix _frustum ;

    std::shared_ptr<SceneObservable> _pov ;

  private:
};

typedef std::shared_ptr<SceneCamera> camera_p ;

#endif // SCENECAMERA_H
