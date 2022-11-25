#include "scenerenderable.h"

SceneRenderable::SceneRenderable( const double& i_x     ,
                                  const double& i_y     ,
                                  const double& i_z     ,
                                  const double& i_phi   ,
                                  const double& i_theta ) :
  SceneObservable( '/00', i_x, i_y, i_z, i_phi, i_theta),
  _scale(1)
{

}


SceneRenderable::~SceneRenderable()
{
  //dtor
}

void SceneRenderable::setScale( const double& n_scale)
{
  _scale = n_scale ;
}

Matrix& SceneRenderable::model_mat()
{
  _model_mat = matrixScale( _scale ) *
               matrixRotate( RAD2DEG*_phi, RAD2DEG*_theta) *
               matrixTranslate(_pos)  ;

  return _model_mat ;
}
