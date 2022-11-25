
//general triangle-based moving graphic entity affected by Lorenz transform

//due to the linearity of "present to present" time lorenz tranform
//as long as the object does not rotate the history of absolute position only has to be stored

//loaded from .obj 3D object format file

 #ifndef LORENZRENDERABLE_H
#define LORENZRENDERABLE_H

#define MOV_CACHE_SIZE 256
//no clue on how to get reliably max number of thread for a machine
#define SETTER_POOL_SIZE 256

#include <fstream>
#include <regex>
#include <queue>

#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/regex.hpp>


#include <scene/scenerenderable.h>


//lorenz renderable timeset mode
const char P_ACTUAL = 0x00 ;
const char P_LAG    = 0x01 ;
const char P_T      = 0x02 ;


class LorenzRenderable : public SceneRenderable
{
  public:
    LorenzRenderable( const std::string&     src ,
                      const double& i_x     = 0.f ,
                      const double& i_y     = 0.f ,
                      const double& i_z     = 0.f ,
                      const double& i_phi   = 0.f ,
                      const double& i_theta = 0.f ) ;
    virtual ~LorenzRenderable();

    void move() ;
    void record() ;

    virtual int timeSet(const char& mode  ,
                        const Matrix& translate,
                        const Vector3& v_proj  ,
                        const double& v ,
                        const double& C ) ;

    void render() ;

    Matrix& model_mat() ;

  protected:

  //timeSet call for single vertex, this task can be executed in parallel

  //lorentz transform to t'=0
    int timeSet_actual( int ID                 ,
                        int n                  ,
                        const Matrix& trans    ,
                        const Vector3& v_proj  ,
                        const double& gamma    ,
                        const double& c        ,
                        const double& v        ,
                        const double& vcc      ) ;

  //lorenz transform considering light propoagation time
    int timeSet_lag( int ID               ,
                     int n                ,
                     const Matrix& trans  ,
                     const double& c       ) ;

  //display selected pos in history buffer
    int timeSet_t( int ID,
                   int n,
                   const int& t) ;

    Vector3 vertPosAtT( const Matrix& translate ,
                        const int& n_vert       ,
                        const int& t            );

// compute delay in vision due to limited light speed
    double   vertDelay( const double& c       ,
                        const double& gamma   ,
                        const Vector3& pos    ,
                        const Vector3& v_proj ) ;

// compute time distance between point in circular buffer
    double   vertT( const int& t_0,const int& t ) ;

    Vector3 _pos_hist[MOV_CACHE_SIZE] ,
            _v_hist[MOV_CACHE_SIZE]   ;
    int _now ; //history array pointer

    // .obj-like rendering infos
    float* _src_vert;
    float* _render_vert ;
    int _n_vert ;
    float* _norm ;
    int _n_norm ;
    unsigned short* _ele ;
    int _n_ele ;

    //multi-thread timeSet sync shared objs
    boost::thread setter[SETTER_POOL_SIZE] ;
    boost::interprocess::interprocess_semaphore active_thread;
    std::queue<int> done_pos;

  private:

    int load( std::ifstream& ) ;

    // .obj file regex
    const std::string float_s = "-?\\d+\\.?\\d*" ;
    const std::string  vex = "v *("+float_s+") *("+float_s+") *("+float_s+")" ;
    const std::string  vn = "vn *("+float_s+") *("+float_s+") *("+float_s+")" ;
    const std::string  face = "f *(\\d+)/(\\d+)?/(\\d+)? *"
                                "(\\d+)/(\\d+)?/(\\d+)? *"
                                "(\\d+)/(\\d+)?/(\\d+)?" ;

    boost::regex vex_regex, vn_regex, face_regex ;
};

#endif // LORENZRENDERABLE_H
