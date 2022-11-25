#ifndef SCENERENEDERABLE_H
#define SCENERENEDERABLE_H

#include <glutm/shape.h>
#include <glt/rgb.h>
#include <glt/material.h>

#include<scene/sceneobservable.h>

class SceneRenderable : public SceneObservable
{
  public:
    SceneRenderable( const double& i_x     = 0.f ,
                     const double& i_y     = 0.f ,
                     const double& i_z     = 0.f ,
                     const double& i_phi   = 0.f ,
                     const double& i_theta = 0.f ) ;
    virtual ~SceneRenderable();

    void setScale( const double& ) ;

    virtual void render() = 0;

    virtual Matrix& model_mat() ;

  protected:

    double _scale ;

    Matrix _model_mat ;

  private:
};

#endif // SCENERENEDERABLE_H
