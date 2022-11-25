#include "scenecamera.h"

SceneCamera::SceneCamera( std::shared_ptr<SceneObservable> i_pov) :
  _pov(i_pov)
{
  //ctor
}

SceneCamera::~SceneCamera()
{
  //dtor
}

void SceneCamera::set()
{
  glEnable(GL_LIGHTING) ;

  const float l_pos[4] = {1.f, 1.f ,0 ,0} ;
  const float l_dir[3] = {1.f, 2.f ,0 } ;  //todo: create light manager

  glLightfv( GL_LIGHT0, GL_POSITION, l_pos ) ;
  glLightfv( GL_LIGHT0, GL_AMBIENT, l_pos ) ;
  glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, l_dir ) ;

  glEnable(GL_LIGHT0) ;
  glEnable(GL_DEPTH_TEST) ;

  glMatrixMode(GL_MODELVIEW) ;
  glPushMatrix() ;
  glLoadIdentity() ;

  glMatrixMode(GL_PROJECTION) ;
  glPushMatrix() ;
  _frustum.glMultMatrix() ;
}

void SceneCamera::reset()
{
  glDisable(GL_LIGHT0) ;
  glDisable(GL_LIGHTING) ;
  glDisable(GL_DEPTH_TEST) ;

  glMatrixMode(GL_MODELVIEW) ;
  glPopMatrix() ;

  glMatrixMode(GL_PROJECTION) ;
  glPopMatrix() ;
}

void SceneCamera::setAspectRatio(const float& w, const float& h)
{
  glMatrixMode(GL_MODELVIEW) ;
  glPushMatrix() ;
  glLoadIdentity() ;

  glFrustum(-.1 * w/h, .1 *w/h,  //left,right
            -.1,       .1     ,  //up,down
             .1,      100    ) ; //near,far
  _frustum = Matrix(GL_MODELVIEW_MATRIX) ;

  glPopMatrix() ;
}

Matrix SceneCamera::translate()
{
  return matrixTranslate( (_pov->pos())*-1) ;
}

Matrix SceneCamera::orient()
{
  return matrixRotate(-(_pov->phi())*RAD2DEG, -(_pov->theta())*RAD2DEG) ;
}

Matrix SceneCamera::v_orient( const Vector3& v )
{

// compute the versor ux,uz of the new frame, such as v is parallel to ux
  Vector3 ux ,uy, uz;

  if( v.length() != 0)
    ux = v ;
  else
    ux = Vector3(1,0,0) ;
  ux.normalize() ;

  if(abs(ux[1])!= 1)
    uz = xProduct(ux , Vector3(ux[2], 0, -ux[0])) ;
  else
    uz = Vector3(0,0,ux[1]) ;
  uz.normalize() ;

  uy = xProduct(ux,uz) * -1;

#ifdef AXIS_DEBUG
  std::cerr << uz.length() << " " << ux[1] <<"\n" ;
  std::cerr << "New Axis : " << ux << ", "
                             << uy << ", "
                             << uz << std::endl ;
#endif // DEBUG

  return matrixOrient(ux, uy, uz) ;
}

Matrix SceneCamera::v_orient()
{
  return v_orient( _pov->v() ) ;
}

void SceneCamera::initMat()
{
  glMatrixMode(GL_MODELVIEW) ;
  glPushMatrix() ;
  glLoadIdentity() ;


  glFrustum(-.1,.1,-.1,.1,.1,100) ;
  _frustum = Matrix(GL_MODELVIEW_MATRIX) ;

  glPopMatrix() ;
}
