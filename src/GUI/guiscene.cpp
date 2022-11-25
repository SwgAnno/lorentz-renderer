#include "guiscene.h"

GuiScene::GuiScene( SceneMaster* i_scene, std::shared_ptr<SceneCamera> i_eye) :
  GuiComponent(1,1), _screen(false), _scene(i_scene), _eye(i_eye)
{
#ifdef GUI_DEBUG
    std::cerr << "GuiScene constructor, Camera pointer: " << _eye << std::endl ;
#endif // GUI_DEBUG

  _eye->setAspectRatio( px_w()  ,
                        px_h()  ) ;
  _screen.resize(px_x(), px_y(), px_w(), px_h()) ;
}

GuiScene::~GuiScene()
{

}

void GuiScene::draw()
{

#ifdef GUI_DEBUG
  std::cerr << " Calling GuiScene draw\n" ;
#endif // GUI_DEBUG

  //get current viewport
  GltViewport old(true);
  _screen.set() ;

  _scene->render( _eye ) ;

  GLERROR;
  old.set();

  draw_border() ;
}

void GuiScene::resize ( const float& p_x,
                        const float& p_y,
                        const float& p_w,
                        const float& p_h)
{
  GuiComponent::resize(p_x, p_y, p_w, p_h) ;

  //parent has new pixel measure but old relative measure
  _eye->setAspectRatio( px_w()  ,
                        px_h()  ) ;
  _screen.resize( px_x() * (p_x/_parent->x() ) ,
                  px_y() * (p_y/_parent->y() ) ,
                  px_w() * (p_w/_parent->w() ) ,
                  px_h() * (p_h/_parent->h() ) ) ;

#ifdef GUI_DEBUG
  PX_MEASURE ;
  std::cerr << _screen.valid() << std::endl;
#endif // GUI_DEBUG

}

void GuiScene::setScene( SceneMaster* n_s,  std::shared_ptr<SceneCamera> n_c)
{
  _scene = n_s ;

  if(n_c)
    _eye = n_c ;
}

