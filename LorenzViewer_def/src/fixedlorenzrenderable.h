#ifndef FIXEDLORENZRENDERABLE_H
#define FIXEDLORENZRENDERABLE_H

#include <lorenzrenderable.h>


class FixedLorenzRenderable : public LorenzRenderable
{
  public:
    FixedLorenzRenderable( const std::string&     src ,
                           const double& i_x     = 0.f ,
                           const double& i_y     = 0.f ,
                           const double& i_z     = 0.f ,
                           const double& i_phi   = 0.f ,
                           const double& i_theta = 0.f ) ;
    virtual ~FixedLorenzRenderable();

    int timeSet(const char& mode  ,
                const Matrix& translate,
                const Vector3& v_proj  ,
                const double& v ,
                const double& C ) ;

  protected:

  private:
};

#endif // FIXEDLORENZRENDERABLE_H
