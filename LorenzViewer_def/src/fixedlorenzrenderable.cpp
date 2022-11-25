#include "fixedlorenzrenderable.h"

FixedLorenzRenderable::FixedLorenzRenderable( const std::string& src ,
                                              const double& i_x     ,
                                              const double& i_y     ,
                                              const double& i_z     ,
                                              const double& i_phi   ,
                                              const double& i_theta ) :
  LorenzRenderable( src, i_x, i_y, i_z, i_phi, i_theta)
{
  _render_vert = _src_vert ;

  Matrix model = SceneRenderable::model_mat()                ;
  Matrix rot    = matrixRotate( RAD2DEG*_phi, RAD2DEG*_theta) ;

  for(int i=0; i<_n_vert; i++)
  {
    Vector3 v0( _render_vert[3*i   ] ,
                _render_vert[3*i +1] ,
                _render_vert[3*i +2] ) ;

    Vector3 n0( _norm[3*i   ] ,
                _norm[3*i +1] ,
                _norm[3*i +2] ) ;

    v0 = model * v0 ;
    n0 = rot   * n0 ;

    _render_vert[3*i   ] = v0[0] ;
    _render_vert[3*i +1] = v0[1] ;
    _render_vert[3*i +2] = v0[2] ;

    _norm[3*i   ] = n0[0] ;
    _norm[3*i +1] = n0[1] ;
    _norm[3*i +2] = n0[2] ;
  }

}

FixedLorenzRenderable::~FixedLorenzRenderable()
{
  //dtor
}

int FixedLorenzRenderable::timeSet(const char& mode  ,
                                   const Matrix& translate,
                                   const Vector3& v_proj  ,
                                   const double& v ,
                                   const double& C )
{

}
