#include "lorenzscenemaster.h"

LorenzSceneMaster::LorenzSceneMaster( const char& i_mode, float limit ) :
   C(100) , box_l(limit), _mode(i_mode)
{

}

LorenzSceneMaster::~LorenzSceneMaster()
{

}

void LorenzSceneMaster::render( camera_p eye)
{
  std::shared_ptr<SceneObservable> pov = eye->pov() ;
  vCheck( pov ) ;

  eye->set() ; //push level onto opengl matrix stack
               // matrix stack now points to GL_PROJECTION
               // and contains only frustum

  Vector3 v_project ;

  _gamma_i = sqrt( 1-(( pov->v().length() / C )*( pov->v().length() / C )) ) ;
  _lorenz = matrixScale( 1/_gamma_i , 1, 1) ;

  _v_orient   = eye->v_orient() ;
  _v_orient_i = _v_orient.inverse() ;
  //v_project = Vector3( _v_orient [0], _v_orient[1], _v_orient[2] ) ;
  v_project = pov->v() ;
  v_project.normalize() ;

  switch( _mode)
  {
  case P_LAG :
  case P_T   :
    _view_mat = eye->translate() *
                eye->orient()    ;

    _lorenz_fix = _view_mat                      *
                  Matrix( GL_PROJECTION_MATRIX ) ;
    break ;
  case P_ACTUAL :
    _view_mat = eye->translate() *
                _v_orient_i      *
                _lorenz          *
                _v_orient        *
                eye->orient()    ;

    _lorenz_fix = eye->translate()             *
                _v_orient_i                    *
                matrixScale( _gamma_i , 1, 1)  *
                _v_orient                      *
                eye->orient()                  *
                Matrix( GL_PROJECTION_MATRIX ) ;
    break ;
  }

  _view_mat.glMultMatrix() ;

  int n = 0 ;

  for(auto i: _objs)
  {
    if( i==pov )
      continue ;
    if( i->tag() & SCENE_RENDER )
    {
      if( i->tag() & SCENE_FIX )
      {
        glPushMatrix() ;
        _lorenz_fix.glLoadMatrix() ;
      }

      _mats[ n++ % N_MATS].set() ;
      GLERROR;

      i->timeSet( _mode             ,
                  eye->translate()  ,
                  v_project         ,
                  pov->v().length() ,
                  C                 ) ;
      i->render() ;

      if( i->tag() & SCENE_FIX )
        glPopMatrix() ;
    }
  }




#ifdef SCENE_DEBUG

  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity() ;

  goldMaterial.set() ;
  glBegin(GL_QUADS) ;
    glNormal3f( 1 , 0 , 0) ;

    glVertex3f( -2.f , -2.f , -2.f) ;
    glVertex3f( -2.f , -2.f ,  2.f) ;
    glVertex3f( -2.f ,  2.f ,  2.f) ;
    glVertex3f( -2.f ,  2.f , -2.f) ;

    glVertex3f(  2.f , -2.f , -2.f) ;
    glVertex3f(  2.f , -2.f ,  2.f) ;
    glVertex3f(  2.f ,  2.f ,  2.f) ;
    glVertex3f(  2.f ,  2.f , -2.f) ;
  glEnd() ;
  pearlMaterial.set() ;
  glBegin(GL_QUADS) ;
    glNormal3f( 0 , 1 , 0) ;

    glVertex3f( -box_l , -box_l , -box_l) ;
    glVertex3f(  box_l , -box_l , -box_l) ;
    glVertex3f(  box_l , -box_l ,  box_l) ;
    glVertex3f( -box_l , -box_l ,  box_l) ;

    glNormal3f( 0 , -1 , 0) ;

    glVertex3f( -box_l ,  box_l , -box_l) ;
    glVertex3f(  box_l ,  box_l , -box_l) ;
    glVertex3f(  box_l ,  box_l ,  box_l) ;
    glVertex3f( -box_l ,  box_l ,  box_l) ;
  glEnd() ;

#endif // DEBUG

  eye->reset() ;
}


// have to fill objs position history to do calculation
void LorenzSceneMaster::init()
{
  for(int i=0 ; i<MOV_CACHE_SIZE ; i++)
  {
    setup() ;
    update() ;
  }

  std::cerr << "LorenzSceneMaster initialized" << std::endl ;
}

void LorenzSceneMaster::setup()
{
  for( auto i: _objs)
  {
    i->a() = Vector3(0,0,0) ;
  }
}

void LorenzSceneMaster::update()
{
  for( auto i : _objs)
  {
    if( i->tag() & SCENE_MOVE )
    {

#ifdef GRAVITY
 //     i->a() += Vector3( 0, -9.8,0) ;
#endif // GRAVITY
      i->move() ;
      vCheck( i ) ;
      collCheck( i ) ;
    }

    i->record() ;
  }
}

void LorenzSceneMaster::add( render_p obj , const char& n_tag)
{
  obj->tag() = n_tag ;

  _objs.push_back( obj ) ;
}

void LorenzSceneMaster::OnTick()
{

#ifdef TICK_DEBUG
  std::cerr << "LorenzSceneMaster Tick" << std::endl ;
#endif // TICK_DEBUG
  setup() ;
  update() ;
}

void LorenzSceneMaster::vCheck(std::shared_ptr<SceneObservable> obj)
{
  Vector3 obj_v = obj->v() ;

  // we have a cosmic speed limit
  if( obj_v.length() > C)
  {
    obj_v.normalize() ;
    obj_v *= C *.999 ;
    obj->v() = obj_v ;
  }
}

void LorenzSceneMaster::collCheck( std::shared_ptr<SceneObservable> obj)
{
  if( fabs(obj->pos()[0]) > box_l )
  {
    //obj->pos()[0] *= box_l/fabs (obj->pos()[0]) ;
    obj->v()[0] *= -1 ;
  }
  if( fabs(obj->pos()[1]) > box_l )
  {
    //obj->pos()[1] *= box_l/fabs(obj->pos()[1]) ;
    obj->v()[1] *= -1 ;
  }
  if( fabs(obj->pos()[2]) > box_l )
  {
    //obj->pos()[2] *= box_l/fabs(obj->pos()[2]) ;
    obj->v()[2] *= -1 ;
  }

}

void LorenzSceneMaster::collCheck( std::shared_ptr<LorenzRenderable> obj)
{
  collCheck( std::dynamic_pointer_cast<SceneObservable,LorenzRenderable>(obj) ) ;
  // https://stackoverflow.com/questions/13403490/passing-shared-ptrderived-as-shared-ptrbase

  obj->record() ;
}
