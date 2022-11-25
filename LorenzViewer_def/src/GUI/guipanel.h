#ifndef GUIPANEL_H
#define GUIPANEL_H

#include<vector>

#include <GUI/component.h>

class GuiPanel : public GuiComponent
{
  public:

    // pos&size relative to parent component
    GuiPanel(const float& i_x,
             const float& i_y,
             const float& i_w,
             const float& i_h,
                   GuiComponent* i_parent) ;

    // pos relative to parent component / abs px size
    GuiPanel(const float& i_x,
             const float& i_y,
             const int& i_w,
             const int& i_h,
                   GuiComponent* i_parent) ;

    // abs px pos& px size
    GuiPanel(const int& i_x,
             const int& i_y,
             const int& i_w,
             const int& i_h,
                   GuiComponent* i_parent) ;

    //layout initializers
    GuiPanel(const bool& orient, const int& l    , float* dims);
    GuiPanel(const bool& orient, const int& l    , int*   dims);
    GuiPanel(const bool& orient, const int& n_sec);

    GuiPanel() {};
    virtual ~GuiPanel();

    virtual void draw() ;

    virtual void add(gui_p c,const int& pos);
    virtual void add(gui_p c) ;

  protected:

  void setLayout(const bool& orient, const int& l, float* dims);
  void setLayout(const bool& orient, const int& l, int* dims);
  void setLayout(const bool& orient, const int& n_sec);
};

#endif // GUIPANEL_H
