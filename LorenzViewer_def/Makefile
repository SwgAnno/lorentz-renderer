###########

DEBUG = -DDEBUG

lab.GLTROOT = $(LAB2)/glt
lab.GLT.inc = -I$(lab.GLTROOT)/src -I/usr/X11R6/include/
lab.GLT.lib = -L$(lab.GLTROOT)/lib -lglutm -lglt -lz -L/usr/X11R6/lib -lglut -lGLU \
							-lGL -lX11 -lXmu -ljpeg -lpng

win.GLTROOT = C:\Users\utente\Desktop\Pappa_01\Dev\C++\Libraries\glt
win.GLT.inc = -I$(win.GLTROOT)/src
win.GLT.lib = -L$(win.GLTROOT)/lib -lglutm -lglt -lz -lglut32 -lglut  \
							-lopengl32 -lglu32 -lgdi32 -lwinmm -ljpeg -lpng

GLT.inc = $(win.GLT.inc)
GLT.lib = $(win.GLT.lib)
lab : override GLT.inc = $(lab.GLT.inc)
lab : override GLT.lib = $(lab.GLT.lib)


###########################################################

lsys.boost = -lboost_system-mgw82-mt-d-x32-1_70
lthread.boost = -lboost_thread-mgw82-mt-d-x32-1_70
lregex.boost = -lboost_regex-mgw82-mt-d-x32-1_70

BOOSTROOT = C:\Users\utente\Desktop\Pappa_01\Dev\C++\Libraries\boost_1_70_0
BOOSTINCS = $(BOOSTROOT)
BOOSTLIBS = $(BOOSTROOT)/stage/lib
lab.BOOST.inc = -Iusr/includes
win.BOOST.inc = -I$(BOOSTINCS)
lab.BOOST.lib = -Lusr/lib -lboost_system -lboost_thread -lboost_regex -lpthread
win.BOOST.lib = -L$(BOOSTLIBS) $(lsys.boost) $(lthread.boost) $(lregex.boost)

BOOST.inc = $(win.BOOST.inc)
BOOST.lib = $(win.BOOST.lib)
lab : override BOOST.inc = $(lab.BOOST.inc)
lab : override BOOST.lib = $(lab.BOOST.lib)
###########################################################

my.cc = g++ -c -std=c++11 -o $@ $(GLT.inc) $(BOOST.inc) -Isrc $<
Debug : override my.cc += $(DEBUG)
my.link = g++ -pg -O3 -o $@ $+ $(GLT.lib) $(BOOST.lib)

OBJDIR = obj

GUIDIR = src/GUI
TARGET_GUI = component guiwindow guipanel guilabel guiscene guislider

component_REQ =
guiwindow_REQ = GUI/guipanel GUI/guilabel GUI/guiscene GUI/guislider
guipanel_REQ = GUI/component
guilabel_REQ = GUI/component
guiscene_REQ = GUI/component scene/scenemaster
guislider_REQ = GUI/component

TARGET_SCENE = scenemaster sceneobservable scenerenderable scenecamera scenecontroller

scenemaster_REQ = scene/scenecamera
sceneobservable_REQ =
scenerenderable_REQ = scene/sceneobservable
scenecamera_REQ = scene/sceneobservable
scenecontroller_REQ = scene/sceneobservable

TARGET_MAIN = main lorenzwindow lorenzscenemaster lorenzrenderable fixedlorenzrenderable

main_REQ = lorenzwindow
lorenzwindow_REQ = GUI/guiwindow GUI/guislider lorenzscenemaster scene/scenecontroller
lorenzscenemaster_REQ = scene/scenemaster lorenzrenderable
lorenzrenderable_REQ = scene/scenerenderable
fixedlorenzrenderable_REQ = lorenzrenderable

TARGET_ALL = $(patsubst %,obj/%.o , $(TARGET_MAIN))    \
             $(patsubst %,obj/GUI/%.o , $(TARGET_GUI)) \
             $(patsubst %,obj/scene/%.o , $(TARGET_SCENE))

.PHONY : Debug win lab run clean cleanall

LorenzViewer : $(TARGET_ALL) ; $(my.link)

obj : $(TARGET_ALL)
objgui : $(TARGET_GUI : % = $(OBJDIR)/%.o)
objmain: $(TARGET_MAIN: % = $(OBJDIR)/%.o)

Debug : LorenzViewer

win : LorenzViewer
lab : LorenzViewer

cleanall :
	-rm $(wildcard obj/*.o)
	-rm $(wildcard obj/GUI/*.o)
	-rm $(wildcard obj/scene/*.o)
	-rm LorenzViewer

clean :
	-rm $(wildcard obj/$(CLEANDIR)/*.o)
	-rm LorenzViewer

run : ; \
  ./LorenzViewer

obj/GUI/%.o : src/GUI/%.cpp src/GUI/%.h \
              $(patsubst %,obj/%.o , $(%_REQ)) ; \
  $(my.cc)

obj/scene/%.o : src/scene/%.cpp src/scene/%.h\
					      $(patsubst %,obj/%.o , $(%_REQ)) ; \
  $(my.cc)

obj/%.o : src/%.cpp src/%.h \
					$(patsubst %,obj/%.o , $(%_REQ)) ; \
  $(my.cc)

obj/%.o : src/%.cpp \
					$(patsubst %,obj/%.o , $(%_REQ)) ; \
  $(my.cc)
