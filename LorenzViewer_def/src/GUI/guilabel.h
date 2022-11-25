#ifndef GUILABEL_H
#define GUILABEL_H

#include <string>

#include <fonts/fonts.h>
#include <glt/fontasci.h>
#include <node/text.h>

#include <GUI/component.h>

class GuiLabel : public GuiComponent
{
  public:
    GuiLabel(const std::string& i_msg,
             const GltFontAscii& i_font );

    virtual ~GuiLabel();

    std::string& msg() {return _msg ;} ;

    void draw();

  protected:

    std::string _msg ;
    const GltFontAscii&  _font;

  private:
};

#endif // GUILABEL_H
