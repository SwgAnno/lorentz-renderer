#ifndef COMPONENT_H
#define COMPONENT_H

#define MEASURE std::cerr << "Component float measure: " \
                            << _x << " " << _y << " " \
                            << _w << " " << _h << "\n"

#define PX_MEASURE std::cerr << "Component pixel measure: " \
                            << px_x() << " " << px_y() << " " \
                            << px_w() << " " << px_h() << "\n"

///////////////////////

//////////////////////

#include <iostream>
#include <vector>
#include <memory>


#include <glt/gl.h>
#include <glt/viewport.h>
#include <glt/rgb.h>
#include <glt/error.h>

//  extern float px_ux , px_uy ;
//global variable for small graphics adjustment
//to be handled/updated by Window class


class GuiComponent
{
  public:

    typedef std::shared_ptr<GuiComponent> gui_p ;

    // pos&size relative to parent component
    GuiComponent(const float& i_x,
                 const float& i_y,
                 const float& i_w,
                 const float& i_h,
                 GuiComponent* i_parent) ;

    // pos relative to parent component / abs px size
    GuiComponent(const float& i_x,
                 const float& i_y,
                 const int& i_w,
                 const int& i_h,
                 GuiComponent* i_parent) ;

    // abs px pos& px size
    GuiComponent(const int& i_x,
                 const int& i_y,
                 const int& i_w,
                 const int& i_h,
                 GuiComponent* i_parent) ;

    //root component, fullscreen
    GuiComponent( const int& i_w,  const int& i_h) ;


    GuiComponent() {};
    virtual ~GuiComponent() {};

    //occupy new parent as you occupied your parent
    virtual void fitInto (const gui_p&) ;

    //occupy new component as you occupied your parent
    virtual void fitInto (GuiComponent* np) ;

    //resize to new parent measures
    virtual void resize ( const float& p_x,
                          const float& p_y,
                          const float& p_w,
                          const float& p_h) ;

    //root resize
    virtual void resize ( const int& w,
                          const int& h) ;

    virtual void draw() ;
    virtual void draw_backgournd() ;
    virtual void draw_children() ;
    virtual void draw_border() ;
    virtual void draw_cross() ;

    inline float& x() { return _x ;};
    inline float& y() { return _y ;};
    inline float& h() { return _h ;};
    inline float& w() { return _w ;};
    int px_h() ;
    int px_w() ;
    int px_x() ;
    int px_y() ;
    float rel_h() ;
    float rel_w() ;
    float rel_x() ;
    float rel_y() ;
    float gl_x(const int&) ;
    float gl_y(const int&) ;
    float screen_x(const float&) ;
    float screen_y(const float&) ;

    bool inside(const float& x, const float& y) ;

    GltColor& background() { return _background ;} ;

    virtual void add( gui_p c) ;
    virtual void add( gui_p c, const int& pos) ;
    GuiComponent*& parent() {return _parent ;}
    GuiComponent* root() ;

    virtual void OnTick() ;
    virtual void OnKeyboard(unsigned char& key, int& x, int& y) ;
    virtual void OnKeyboardUp(unsigned char& key, int& x, int& y) ;
    virtual void OnMouse(int& button, int& state,const float& x,const float& y) ;
    virtual void OnMotion( int& dx, int& dy) ;
    virtual void OnFocus() ;

    bool focus() { return _focus ;};
    void setFocus( const bool& ) ;

  protected:

  int _px_w , _px_h ;
  float _x , _y ; //absolute position in GL coordinate
  float _w , _h ; //size
  bool _focus ;
  GltColor _background = gray;

  GuiComponent* _parent ;
  std::vector<gui_p> _children ;

};

typedef std::shared_ptr<GuiComponent> gui_p ;

std::ostream& operator<<(std::ostream&, GuiComponent&) ;

#endif // COMPONENT_H
