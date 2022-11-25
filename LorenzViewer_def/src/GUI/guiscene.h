#ifndef GUISCENE_H
#define GUISCENE_H

#include <glt/viewport.h>

#include <GUI/component.h>
#include <scene/scenemaster.h>

class GuiScene : public GuiComponent
{
  public:
    GuiScene( SceneMaster*, std::shared_ptr<SceneCamera> );
    virtual ~GuiScene();

    virtual void draw() ;

    void resize ( const float& p_x,
                  const float& p_y,
                  const float& p_w,
                  const float& p_h) ;

    void setScene( SceneMaster* n_s, std::shared_ptr<SceneCamera> n_c = nullptr ) ;

  protected:

    GltViewport _screen ;

    SceneMaster* _scene ;
    std::shared_ptr<SceneCamera> _eye ;

  private:
};

#endif // GUISCENE_H
