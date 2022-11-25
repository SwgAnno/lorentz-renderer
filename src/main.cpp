#include <iostream>
#include<vector>
#include<string>

#include<boost/thread/thread.hpp>
#include<boost/chrono.hpp>

#include<glt/gl.h>
#include<glutm/window.h>
#include<glutm/main.h>

#include<lorenzwindow.h>

bool GlutMain(const std::vector<std::string> &args)
{
  int w = 1200,
      h = 700 ;

  if(args.size()>2)
  {
    w = atoi( args[1].c_str() ) ;
    h = atoi( args[2].c_str() ) ;

    if( !w )
      w = 1200 ;
    if( !h )
      h = 700 ;

  }

  LorenzWindow* window = new LorenzWindow("Lorenz Viewer", w, h);
  window->open();

  return true;
}
