#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc network_Init;
extern Gatefunc SWIG_this;
extern Gatefunc SWIG_ptr;
extern Gatefunc _wrap_TcpOpen;
extern Gatefunc _wrap_client;
extern Gatefunc _wrap_close;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,network_Init,"network_Init"},
  {(Myinterfun)sci_gateway,SWIG_this,"SWIG_this"},
  {(Myinterfun)sci_gateway,SWIG_ptr,"SWIG_ptr"},
  {(Myinterfun)sci_gateway,_wrap_TcpOpen,"TcpOpen"},
  {(Myinterfun)sci_gateway,_wrap_client,"client"},
  {(Myinterfun)sci_gateway,_wrap_close,"close"},
};
 
int C2F(libnetwork)()
{
  Rhs = Max(0, Rhs);
  if (*(Tab[Fin-1].f) != NULL) 
  {
     if(pvApiCtx == NULL)
     {
       pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
     }
     pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  }
  return 0;
}
#ifdef __cplusplus
}
#endif
