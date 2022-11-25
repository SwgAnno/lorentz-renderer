#include "guipanel.h"

GuiPanel::GuiPanel(const float& i_x,
                   const float& i_y,
                   const float& i_w,
                   const float& i_h,
                   GuiComponent* i_parent) :
  GuiComponent(i_x, i_y, i_w, i_h, i_parent)
  {}

GuiPanel::GuiPanel(const float& i_x,
                   const float& i_y,
                   const int& i_w,
                   const int& i_h,
                   GuiComponent* i_parent) :
  GuiComponent(i_x, i_y, i_w, i_h, i_parent)
  {}

GuiPanel::GuiPanel(const int& i_x,
                   const int& i_y,
                   const int& i_w,
                   const int& i_h,
                   GuiComponent* i_parent) :
  GuiComponent(i_x, i_y, i_w, i_h, i_parent)
  {}

GuiPanel::GuiPanel(const bool& i_orient, const int& i_l, float* i_sec) :
  GuiComponent(1,1)
{
  setLayout(i_orient, i_l, i_sec) ;
}

GuiPanel::GuiPanel(const bool& i_orient, const int& i_l, int* i_sec) :
  GuiComponent(1,1)
{
  setLayout(i_orient, i_l, i_sec) ;
}

GuiPanel::GuiPanel(const bool& i_orient, const int& i_n_sec) :
  GuiComponent(1,1)
{
  setLayout(i_orient, i_n_sec) ;
}

GuiPanel::~GuiPanel()
{}

void GuiPanel::add(gui_p c)
{
  c->fitInto( _children[0].get() ) ;
  _children[0] = c ;
}

void GuiPanel::add(gui_p c,const int& pos)
{
  c->fitInto( _children[pos].get() ) ;
  _children[pos] = c ;
}

void GuiPanel::setLayout(const bool& orient, const int& l, float* dims)
{
  float pos = 0.f;

  if(orient) //x
  {
    for(int i=0; i<l ; ++i)
    {
      _children.push_back( gui_p( new GuiComponent(pos, 0.f, dims[i], 1.f,this))) ;
      pos+=dims[i];
    }
  }
  else
  {
    for(int i=0; i<l ; ++i)
    {
      _children.push_back( gui_p( new GuiComponent(0.f, pos, 1.f, dims[i], this))) ;
      pos+=dims[i];
    }
  }

}

void GuiPanel::setLayout(const bool& orient, const int& l, int* dims)
{
  int pos = 0;
  int dim ;

  if(orient) //x
  {
    dim = px_h();
    for(int i=0; i<l ; ++i)
    {
      _children.push_back( gui_p( new GuiComponent(pos, 0, dims[i], dim,this))) ;
      pos+=dims[i];
    }
  }
  else
  {
    dim = px_w();
    for(int i=0; i<l ; ++i)
    {
      _children.push_back( gui_p( new GuiComponent(0, pos, dim, dims[i], this))) ;
      pos+=dims[i];
    }
  }

}

void GuiPanel::setLayout(const bool& orient,const int& n_sec)
{
  float pos = 0;
  float l = static_cast<float>(1)/n_sec ;

  if(orient) //x
  {
    for(int i=0; i<n_sec ; ++i)
    {
      pos= static_cast<float>(i)/n_sec;
      _children.push_back( gui_p( new GuiComponent(pos, 0.f, l, 1.f, this))) ;
    }
  }
  else
  {
    for(int i=0; i<n_sec ; ++i)
    {
      pos= static_cast<float>(i)/n_sec;
      _children.push_back( gui_p( new GuiComponent(0.f, pos, 1.f, l, this))) ;
    }
  }

}

void GuiPanel::draw()
{
  #ifdef GUI_DEBUG
  std::cerr << "Calling GuiPanel draw\n" ;
  #endif // GUI_DEBUG
  GuiComponent::draw();
}
