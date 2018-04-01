//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_PHN_string_H
#define __INC_PHN_string_H
#include <stdint.h>
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define Print_psd(s, p) sprintf(s, "%02d%02d%02d", p[0], p[1], p[2])


//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

//system.c
void Print_sys_param(void *p_data, char	*p_s, int len, int aux);
void Print_atmosphere_param(void *p_data, char	*p_s, int len, int aux);
void Print_able(char *p, int able);
void Print_select(char *p, int s);
void Str_set_sys_param(char	*p_s, int aux, int op, int val);
int Get_str_data(char *s_data, char* str_separator, int num, uint8_t	*err);

int Str_Password_match(char *p_s_psd);
void Password_modify(char	*p_s_psd, int idx, int op);
void Str_set_password(uint8_t *p_dst, char	*p_s_psd);
int Password_iteartor(char	*p_time_text, int idx, int director);

//model.c
void Print_unit( char unit, char *buf, int len);
void Print_float(int data, short int_len, short decimal_Places, char *str);
void Print_frefix_float(int data, int prec, char *prefix, char *str);

void Print_flow(char *buf, uint16_t f);
void Print_pressure(char *buf, uint16_t f);
void Print_temperature(char *buf, uint16_t f);
//mode_channel
void Print_singnaltype(int sgt, char *str);
void Print_touch_spot(int spot, char *str);

//ModelTime
extern int MdlTime_text_iteartor(char	*p_time_text, int idx, int director);
extern void MdlTime_text_modify(char	*p_time_text, int idx, int op);

//HMI_comm.c
void Str_Calculations(char *p_str, int len,  int op, int val, int rangel, int rangeh);


#endif
