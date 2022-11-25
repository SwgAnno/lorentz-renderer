#ifndef SCENEOBSERVABLE_H
#define SCENEOBSERVABLE_H

#define DT 0.01666666f // approx 1/60s

#include <cmath>
#include <iostream>
#include <boost/thread/mutex.hpp>

#include <glt/gl.h>
#include <glt/error.h>
#include <glt/material.h>
#include <math/matrix4.h>
#include <math/vector3.h>
#include <math/vector2.h>


static const double DEG2RAD = M_PI / 180 ;
static const double RAD2DEG = 180  / M_PI ;

class SceneObservable
{
  public:
    SceneObservable( const char&  i_tag   = '\0' ,
                     const double& i_x     = 0.f  ,
                     const double& i_y     = 0.f  ,
                     const double& i_z     = 0.f  ,
                     const double& i_phi   = 0.f  ,
                     const double& i_theta = 0.f  ) ;

    char& tag()    { return _tag   ; } ;

    Vector3& pos() { return _pos   ; } ;
    Vector3& v()   { return _v     ; } ;
    Vector3& a()   { return _a     ; } ;
    double& phi()   { return _phi   ; } ;
    double& theta() { return _theta ; } ;

    virtual void move();

    //external sync
    void lock() ;
    void unlock() ;

  protected:

    char _tag ;
    //classify object

    Vector3 _pos, _v , _a;
    double _phi ,_theta ; // elevation , azimuth

    GltMaterial* _material ;

    boost::mutex _update_mtx ;


  private:
};

typedef std::shared_ptr<SceneObservable> observable_p ;

#endif // SCENEOBSERVABLE_H
