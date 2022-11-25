#include "lorenzrenderable.h"

LorenzRenderable::LorenzRenderable( const std::string& src ,
                                    const double& i_x     ,
                                    const double& i_y     ,
                                    const double& i_z     ,
                                    const double& i_phi   ,
                                    const double& i_theta ) :
  SceneRenderable( i_x, i_y, i_z, i_phi, i_theta),
  _now(0) ,
  _n_vert(0), _n_ele(0), _n_norm(0),
  vex_regex(vex, boost::regex_constants::icase),
  vn_regex(vn, boost::regex_constants::icase),
  face_regex(face, boost::regex_constants::icase),
  active_thread(SETTER_POOL_SIZE)
{
  std::ifstream in(src.c_str()) ;

  if( !in )
  {
    _n_vert = 0 ;
    _n_ele  = 0 ;
    std::cerr << "LorenzRenderable input file not found/valid"
              << std::endl ;
    return ;
  }

  load(in) ;
  std::cerr << "Done loading " << src
            << ", loaded " << _n_vert << " vertices"
            << std::endl ;

  for(int i=0; i<MOV_CACHE_SIZE; i++)
  {
    _pos_hist[i] = _pos ;
  }

}

LorenzRenderable::~LorenzRenderable()
{
  delete _src_vert ;
  delete _render_vert ;
  delete _norm ;
  delete _ele ;
}

void LorenzRenderable::move()
{
  SceneObservable::move() ;

  _now = (_now+1) % MOV_CACHE_SIZE ;
  record() ;

}

void LorenzRenderable::record()
{
  int prev = (_now + MOV_CACHE_SIZE -1) % MOV_CACHE_SIZE ;

  //record movement end position and mean velocity to get there
  _pos_hist[_now] = _pos ;
  _v_hist[_now]   = Vector3( _pos[0]-_pos_hist[prev][0] ,
                             _pos[1]-_pos_hist[prev][1] ,
                             _pos[2]-_pos_hist[prev][2] ) / DT  ;

}

int LorenzRenderable::timeSet(const char& mode  ,
                              const Matrix& translate,
                              const Vector3& v_proj  ,
                              const double& v ,
                              const double& C )
{
  _update_mtx.lock() ;

  double gamma = sqrt(1- (v/C)*(v/C) ) ,
        vcc   = v/C/C                 ;

  int next_thread ;

  //initialize queue
  while(!done_pos.empty())
    done_pos.pop() ;
  for(int i=0; i< SETTER_POOL_SIZE; i++)
    done_pos.push(i) ;

  switch( mode )
  {
  case P_LAG :
    for(int i=0; i<_n_vert ; i++)
    {
      active_thread.wait() ;
      next_thread = done_pos.front() ;
      done_pos.pop() ;
      #ifdef TIME_DEBUG
      std::cerr << "timeSetThread " << i << " starting" << std::endl ;
      #endif // TIME_DEBUG

      setter[next_thread] = boost::thread(
                              boost::bind( &LorenzRenderable::timeSet_lag, this,
                                           next_thread, i, translate, C
                                         )
                                         );
//      setter[next_thread].join() ;
    }
    break ;

  case P_T   :
    for(int i=0; i<_n_vert ; i++)
    {
      active_thread.wait() ;
      next_thread = done_pos.front() ;
      done_pos.pop() ;
      #ifdef TIME_DEBUG
      std::cerr << "timeSetThread " << next_thread << " starting" << std::endl ;
      #endif // DEBUG
      setter[next_thread] = boost::thread(
                              boost::bind( &LorenzRenderable::timeSet_t, this,
                                           next_thread, i, _now
                                         )
                                         );
//      setter[next_thread].join() ;
    }
    break ;

  case P_ACTUAL :
    for(int i=0; i<_n_vert ; i++)
    {
      active_thread.wait() ;
      next_thread = done_pos.front() ;
      done_pos.pop() ;
      #ifdef TIME_DEBUG
      std::cerr << "timeSetThread " << i << " starting" << std::endl ;
      #endif // DEBUG
      setter[next_thread] = boost::thread(
                              boost::bind( &LorenzRenderable::timeSet_actual, this,
                                           next_thread, i, translate, v_proj,gamma, C, v , vcc
                                         )
                                         );
//      setter[next_thread].join() ;
    }
    break ;
  }



  for(int i=0; i<SETTER_POOL_SIZE ; i++)
    if(setter[i].joinable())
      setter[i].join() ;

  _update_mtx.unlock() ;
  return 0 ;
}

void LorenzRenderable::render()
{
  glPushAttrib(GL_TRANSFORM_BIT) ; // call to matrix mode shall be transparent

  glEnableClientState(GL_VERTEX_ARRAY) ;
  glEnableClientState(GL_NORMAL_ARRAY) ;

    glVertexPointer( 3, GL_FLOAT, 0, _render_vert) ;
    glNormalPointer(    GL_FLOAT, 0, _norm) ;

    glDrawElements( GL_TRIANGLES, _n_ele, GL_UNSIGNED_SHORT, _ele) ;

  glDisableClientState(GL_VERTEX_ARRAY) ;
  glDisableClientState(GL_NORMAL_ARRAY) ;

  glPopAttrib() ;

  GLERROR;
}

Matrix& LorenzRenderable::model_mat()
{
  // model translation is given by src_vert modification
  // we have to rotate and scale according to new position

  _model_mat = matrixTranslate( _pos*-1 )                  *
 //              matrixScale( _scale )                       *
 //              matrixRotate( RAD2DEG*_phi, RAD2DEG*_theta) *
               matrixTranslate( _pos )    ;


  return _model_mat ;
}

int LorenzRenderable::timeSet_actual( int ID                ,
                                      int n                 ,
                                      const Matrix& trans   ,
                                      const Vector3& v_proj ,
                                      const double& gamma    ,
                                      const double& c        ,
                                      const double& v        ,
                                      const double& vcc      )
{

  int start = (_now+1) % MOV_CACHE_SIZE ,
      end = _now ,
      time_0 =  (end + MOV_CACHE_SIZE - MOV_CACHE_SIZE/2) % MOV_CACHE_SIZE ,
      buf        ;

  double start_t = gamma*( vertT( time_0, start) -( v_proj*vertPosAtT(trans, n, start ) )*vcc ) ,
        end_t   = gamma*( vertT( time_0, end  ) -( v_proj*vertPosAtT(trans, n, end   ) )*vcc ) ,
        buf_t   ;


  // relative time position is preserved in Lorenz transform
  // so we skip vertex if it can't be present
  if( start_t > 0 ||
        end_t < 0  )
  {
#ifdef TIME_DEBUG
    std::cerr << start_t << " "
              << end_t << " "
              << end << " "
              << "TIME DISPLACEMENT: object never occur in \"present\""
              << std::endl ;
#endif // TIME_DEBUG

    _render_vert[3*n]   = 0.f ;
    _render_vert[3*n+1] = 0.f ;
    _render_vert[3*n+2] = 0.f ;

    done_pos.push(ID) ;
    active_thread.post() ;
    return 1 ;
  }

  //find "present occurrence" with bisection
  while( (start+1) % MOV_CACHE_SIZE != end)
  {
    #ifdef TIME_DEBUG
       std::cerr << n << " present between:\t"
                 << start_t << "\t"
                 << end_t   << std::endl ;
    #endif // TIME_DEBUG

    buf   = ( ( start+end + ( end<start ? MOV_CACHE_SIZE : 0 ) )/2 ) % MOV_CACHE_SIZE ;
    buf_t = gamma*( vertT( time_0, buf) -( v_proj*vertPosAtT(trans, n, buf ) )*vcc ) ;

    if( buf_t > 0   )
      end = buf ;
    else
      start = buf ;
  }

  //compute exact solution to t'=0
  double rel_v = v_proj*_v_hist[end] ;

  //reusing variable
  end_t  = v*( ( v_proj*vertPosAtT(trans, n, end ) - vertT(time_0 , end)*rel_v ) ) /
           ( c*c -rel_v*v ) ;
  buf_t  = ( end_t - vertT(time_0 , end) )  ;

#ifdef TIME_DEBUG
  std::cerr << n << " computed present time: "
            << end_t << "\t" << buf_t << "\n        " ;
#endif // TIME_DEBUG

  _render_vert[3*n]   = _src_vert[3*n]   + _pos_hist[end][0]
                        + _v_hist[end][0]*buf_t - v*v_proj[0]*end_t  ;
  _render_vert[3*n+1] = _src_vert[3*n+1] + _pos_hist[end][1]
                        + _v_hist[end][1]*buf_t - v*v_proj[1]*end_t  ;
  _render_vert[3*n+2] = _src_vert[3*n+2] + _pos_hist[end][2]
                        + _v_hist[end][2]*buf_t - v*v_proj[2]*end_t  ;


  done_pos.push(ID) ;
  active_thread.post() ;
  return 0;
}

int LorenzRenderable::timeSet_lag( int ID               ,
                                   int n                ,
                                   const Matrix& trans  ,
                                   const double& c       )
{

  int start = (_now+1) % MOV_CACHE_SIZE ,
      end = _now ,
      buf        ;

  double start_t = vertPosAtT(trans, n, start ).length() + c * vertT( _now,start ) ,
        end_t   = vertPosAtT(trans, n, end ).length()  /*c * vertT( _now,end ) = 0*/ ,
        buf_t   ;


  // relative time position is preserved in Lorenz transform
  // so we skip vertex if it can't be present
  if( start_t > 0 )
  {
#ifdef TIME_DEBUG
    std::cerr << start_t << " "
              << end_t << " "
              << end << " "
              << "TIME DISPLACEMENT: object still not visible \"present\""
              << std::endl ;
#endif // TIME_DEBUG

    _render_vert[3*n]   = 0.f ;
    _render_vert[3*n+1] = 0.f ;
    _render_vert[3*n+2] = 0.f ;

    done_pos.push(ID) ;
    active_thread.post() ;
    return 1 ;
  }

  //find "present occurrence" with bisection
  while( (start+1) % MOV_CACHE_SIZE != end)
  {
#ifdef TIME_DEBUG
       std::cerr << n << " present between:\t"
                 << buf_t << "\t"
                 << vertT( _now,buf ) <<std::endl ;
#endif // TIME_DEBUG

    buf   = ( ( start+end + ( end<start ? MOV_CACHE_SIZE : 0 ) )/2 ) % MOV_CACHE_SIZE ;

    buf_t =  vertPosAtT(trans, n, buf  ).length() + c * vertT( _now,buf ) ;

    if( buf_t > 0   )
      end = buf ;
    else
      start = buf ;
  }

  // compute intersetion with light cone
  double t0 = vertT( _now,end ) ;
  double dv = c*c - _v_hist[end].norm() ;
  double x2 = ( vertPosAtT(trans, n, end ) - _v_hist[end]*t0 ).norm() ;
  double s = _v_hist[end]*vertPosAtT(trans, n, end )
          - _v_hist[end].norm()*t0 ;
  buf_t  = (s - sqrt( s*s + x2*dv)) / dv  - t0;

#ifdef TIME_DEBUG
  std::cerr << n << " " << buf_t << "\n" ;
#endif // TIME_DEBUG

  _render_vert[3*n]   = _src_vert[3*n]   + _pos_hist[end][0]
                        +  _v_hist[end][0]*buf_t ;
  _render_vert[3*n+1] = _src_vert[3*n+1] + _pos_hist[end][1]
                        + _v_hist[end][1]*buf_t  ;
  _render_vert[3*n+2] = _src_vert[3*n+2] + _pos_hist[end][2]
                        + _v_hist[end][2]*buf_t  ;


  done_pos.push(ID) ;
  active_thread.post() ;
  return 0;
}

int LorenzRenderable::timeSet_t( int ID                ,
                                 int n,
                                 const int&  t )
{

  _render_vert[3*n]   = _src_vert[3*n]   + _pos_hist[t][0] ;
  _render_vert[3*n+1] = _src_vert[3*n+1] + _pos_hist[t][1] ;
  _render_vert[3*n+2] = _src_vert[3*n+2] + _pos_hist[t][2] ;

  done_pos.push(ID) ;
  active_thread.post() ;
  return 0 ;
}

Vector3 LorenzRenderable::vertPosAtT( const Matrix& translate ,
                                      const int& n_vert       ,
                                      const int& t            )
{
  return translate *
         Vector3( _pos_hist[t][0] + _src_vert[ 3*n_vert   ] ,
                  _pos_hist[t][1] + _src_vert[ 3*n_vert+1 ] ,
                  _pos_hist[t][2] + _src_vert[ 3*n_vert+2 ] ) ;
}

double  LorenzRenderable::vertDelay( const double& c       ,
                                     const double& gamma   ,
                                     const Vector3& pos    ,
                                     const Vector3& v_proj )
{
  return ( pos - (v_proj* (pos*v_proj)*(1-gamma)) ).length() / c ;
}

double LorenzRenderable::vertT( const int& t_0,const int& t )
{
  return  ( (   t   - ( t  >_now ? MOV_CACHE_SIZE : 0 )
              - t_0 + ( t_0>_now ? MOV_CACHE_SIZE : 0 ) )
          %MOV_CACHE_SIZE )
          * DT ;
}

//.obj file loader
int LorenzRenderable::load( std::ifstream& in)
{
  std::string  line;
  boost::smatch  result ;

  while( in )
  {
    std::getline( in, line) ;
    if( in.bad() )
    {
      std::cerr << "Error 0 parsing .obj input file" ;
      break ;
    }

    switch( line[0])
    {
    case 'f':
      _n_ele += 3 ;
      break ;
    case 'v':
      if     ( line[1] == ' ')
        _n_vert++ ;
      else if( line[1] == 'n');
        _n_norm++ ;

      break ;
    }
  }

#ifdef OBJ_DEBUG
  std::cerr << _n_vert <<" "<< _n_ele<< std::endl ;
#endif // OBJ_DEBUG

  _src_vert    = new float[_n_vert*3] ;
  _render_vert = new float[_n_vert*3] ;
         _norm        = new float[_n_vert*3] ;  //per-vertex normal
  float* norm_temp    = new float[_n_norm*3] ;  //normal data container
  _ele         = new unsigned short[_n_ele] ;

  int temp_n[3], temp_v[3] ;

  in.clear() ;
  in.seekg( in.beg ) ;

  int ele_pos = 0, vert_pos = 0, norm_pos = 0 ;

  while( in  )
  {
    std::getline( in, line) ;
    if( in.bad() )
    {
      std::cerr << "Error 1 parsing .obj input file" ;
      break ;
    }

#ifdef OBJ_DEBUG
    std::cerr << line<< std::endl ;
#endif // OBJ_DEBUG
    switch( line[0])
    {
    case 'v':
      if     ( line[1] == ' ')
      {
        boost::regex_match(line, result, vex_regex) ;
        #ifdef OBJ_DEBUG
          std::cerr << result.str(1) << " "
                    << result.str(2) << " "
                    << result.str(3) << " "
                    << std::endl ;
        #endif // OBJ_DEBUG
        _src_vert[vert_pos++] = atof( result.str(1).c_str()) ;
        _src_vert[vert_pos++] = atof( result.str(2).c_str()) ;
        _src_vert[vert_pos++] = atof( result.str(3).c_str()) ;
      }
      else if( line[1] == 'n')
      {
        boost::regex_match(line, result, vn_regex) ;
        #ifdef OBJ_DEBUG
          std::cerr << result.str(1) << " "
                    << result.str(2) << " "
                    << result.str(3) << " "
                    << std::endl ;
        #endif // OBJ_DEBUG
        norm_temp[norm_pos++] = atof( result.str(1).c_str()) ;
        norm_temp[norm_pos++] = atof( result.str(2).c_str()) ;
        norm_temp[norm_pos++] = atof( result.str(3).c_str()) ;
      }
      break ;

    case 'f':
      boost::regex_match(line, result, face_regex) ;
      #ifdef OBJ_DEBUG
    std::cerr << result.str( 1) << " "
              << result.str( 4) << " "
              << result.str( 7) << " "
              << std::endl ;
      #endif // OBJ_DEBUG
      temp_v[0] = atoi( result.str(1).c_str()) -1 ;
      temp_v[1] = atoi( result.str(4).c_str()) -1 ;
      temp_v[2] = atoi( result.str(7).c_str()) -1 ;
      temp_n[0] = atoi( result.str(3).c_str()) -1 ;
      temp_n[1] = atoi( result.str(6).c_str()) -1 ;
      temp_n[2] = atoi( result.str(9).c_str()) -1 ;

      for(int i=0; i<3; i++)
      {
        _ele[ele_pos++] = temp_v[i] ;

        for(int m=0; m<3; m++)
          _norm[ temp_v[i]*3 + m ] = norm_temp[ temp_n[i]*3 + m ] ;
      }
      break ;
    }

  }

  in.close() ;

  return 0 ;

}
