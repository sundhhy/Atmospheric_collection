//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_sys_cmd_H__
#define __INC_sys_cmd_H__
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------
#include <stdint.h>
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
typedef void (*cmd_recv)(void);


//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
int Init_Cmd_Thread (void);
int	Cmd_Rgt_recv(cmd_recv	crv);
int	Cmd_Rgt_idle_task(cmd_recv	crv);
int	Cmd_Rgt_time_task(cmd_recv	crv, int time_s);		//ִֻ��һ��
void Cmd_del_recv(int	cmd_fd);
#endif
