#include "sceneobservable.h"

SceneObservable::SceneObservable( const  char& i_tag   ,
                                  const double& i_x     ,
                                  const double& i_y     ,
                                  const double& i_z     ,
                                  const double& i_phi   ,
                                  const double& i_theta ) :
  _tag(),
  _pos(i_x, i_y, i_z), _v(0,0,0),
  _phi( i_phi ), _theta( i_theta )
{

}


void SceneObservable::move()
{

  //exact solution to local linear differential equation
  //best approximation for discrete time sampled movement

  _pos += _a/2*DT*DT + _v*DT ;
  _v   += _a*DT ;

#ifdef INPUT_DEBUG
  std::cerr << _pos.x() << " "
            << _pos.y() << " "
            << _pos.z() << "\n" ;

#endif // INPUT_DEBUG
}

void SceneObservable::lock()
{
  _update_mtx.lock() ;
}

void SceneObservable::unlock()
{
  _update_mtx.unlock() ;
}
