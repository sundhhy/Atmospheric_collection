#ifndef __ZikuDeclare
#define __ZikuDeclare

extern const unsigned short daytab[13];
extern const unsigned short daytab1[13];
extern const unsigned char date[13];
extern const unsigned char date1[13];

extern const unsigned char RecInt_Data[9];
extern const unsigned char DisTimes_Data[5];
extern const unsigned char Shibiao[5][9];

extern const unsigned char NUM5_7_0[6];
extern const unsigned char NUM5_7_1[6];
extern const unsigned char NUM5_7_2[6];
extern const unsigned char NUM5_7_3[6];
extern const unsigned char NUM5_7_4[6];
extern const unsigned char NUM5_7_5[6];
extern const unsigned char NUM5_7_6[6];
extern const unsigned char NUM5_7_7[6];
extern const unsigned char NUM5_7_8[6];
extern const unsigned char NUM5_7_9[6];
extern const unsigned char NUM5_7_dian[6];
extern const unsigned char NUM5_7_yigong[6];
extern const unsigned char NUM5_7_maohao[6];
extern const unsigned char NUM5_7_kongge[6];
extern const unsigned char NUM5_7_percent[6];
extern const unsigned char NUM5_7_ENT[6];

//extern const unsigned char CHAR_P[8];
extern const unsigned char CHAR_I[6];
//extern const unsigned char CHAR_D[8];
extern const unsigned char CHAR_S[6];
extern const unsigned char CHAR_V[6];
extern const unsigned char CHAR_K[6];
extern const unsigned char CHAR_A[6];
extern const unsigned char CHAR_M[6];
extern const unsigned char CHAR_H[6];
extern const unsigned char CHAR_L[6];
extern const unsigned char CHAR_O[6];
extern const unsigned char CHAR_U[6];
extern const unsigned char CHAR_T[6];
extern const unsigned char CHAR_N[6];
extern const unsigned char CHAR_A_[6];
extern const unsigned char CHAR_M_[6];
extern const unsigned char CHAR_XIEGONG[8];
extern const unsigned char CHAR_XINGHAO[8];

extern const unsigned char Curve_R[11];
extern const unsigned char Curve_curve[11];
extern const unsigned char Curve_H[11];


extern const unsigned char NUM4_5_0[3];
extern const unsigned char NUM4_5_1[3];
extern const unsigned char NUM4_5_2[3];
extern const unsigned char NUM4_5_3[3];
extern const unsigned char NUM4_5_4[3];
extern const unsigned char NUM4_5_5[3];
extern const unsigned char NUM4_5_6[3];
extern const unsigned char NUM4_5_7[3];
extern const unsigned char NUM4_5_8[3];
extern const unsigned char NUM4_5_9[3];
extern const unsigned char NUM4_5_percent[4];
extern const unsigned char SCHAR_m[4];
extern const unsigned char SCHAR_h[4];

extern const unsigned char RELAY_CLOSE[8];
extern const unsigned char RELAY_OPEN[8];

extern const unsigned char NUM6_11_0[16];
extern const unsigned char NUM6_11_1[16];
extern const unsigned char NUM6_11_2[16];
extern const unsigned char NUM6_11_3[16];
extern const unsigned char NUM6_11_4[16];
extern const unsigned char NUM6_11_5[16];
extern const unsigned char NUM6_11_6[16];
extern const unsigned char NUM6_11_7[16];
extern const unsigned char NUM6_11_8[16];
extern const unsigned char NUM6_11_9[16];
extern const unsigned char NUM6_11_dian[16];
extern const unsigned char NUM6_11_yigong[16];
extern const unsigned char NUM6_11_maohao[16];
extern const unsigned char NUM6_11_kongge[16];
extern const unsigned char NUM6_11_bar[16];
extern const unsigned char NUM6_11_lefKuoH[16];      //lulj 20180224 
extern const unsigned char NUM6_11_rhtKuoH[16];      //lulj 20180224 
extern const unsigned char NUM6_11_EquaL[16]; 
extern const unsigned char NUM6_11_FleftKH[16];
extern const unsigned char NUM6_11_FrightKH[16];
extern const unsigned char NUM6_11_FleftKH[16];
extern const unsigned char NUM6_11_FrightKH[16]; 
extern const unsigned char WORD_A[16];
extern const unsigned char WORD_B[16];
extern const unsigned char WORD_C[16];
extern const unsigned char WORD_D[16];
extern const unsigned char WORD_E[16];
extern const unsigned char WORD_F[16];
extern const unsigned char WORD_H[16];
extern const unsigned char WORD_I[16];
extern const unsigned char WORD_J[16];
extern const unsigned char WORD_K[16];
extern const unsigned char WORD_L[16];
extern const unsigned char WORD_M[16];
extern const unsigned char WORD_N[16];
extern const unsigned char WORD_O[16];
extern const unsigned char WORD_P[16];
extern const unsigned char WORD_R[16];
extern const unsigned char WORD_S[16];
extern const unsigned char WORD_T[16];
extern const unsigned char WORD_U[16];
extern const unsigned char WORD_V[16];
extern const unsigned char WORD_W[16];
extern const unsigned char WORD_Y[16];
extern const unsigned char WORD_Z[16];
extern const unsigned char WORD_G[16];
extern const unsigned char WORD_Q[16];
extern const unsigned char WORD_X[16];
extern const unsigned char WORD_lifang[16];
extern const unsigned char WORD_pingfang[16];
extern const unsigned char WORD_xiab2[16];
extern const unsigned char WORD_xiegong[16];
extern const unsigned char WORD_percent[16];
extern const unsigned char WORD_du[16];
//英文字库
#if SOFT==English


extern const unsigned char WORD_b[16];

extern const unsigned char WORD_e[16];
extern const unsigned char WORD_j[16];
extern const unsigned char WORD_l[16];
extern const unsigned char WORD_o[16];
extern const unsigned char WORD_q[16];
extern const unsigned char WORD_v[16]; 
extern const unsigned char WORD_w[16];

extern const unsigned char WORD_y[16];
   	    
#endif
extern const unsigned char WORD_a[16];
extern const unsigned char WORD_d[16];
extern const unsigned char WORD_c[16];
extern const unsigned char WORD_f[16];
extern const unsigned char WORD_g[16];
extern const unsigned char WORD_h[16];
extern const unsigned char WORD_i[16];
extern const unsigned char WORD_k[16];
extern const unsigned char WORD_m[16];
extern const unsigned char WORD_n[16];
extern const unsigned char WORD_p[16];
extern const unsigned char WORD_r[16];
extern const unsigned char WORD_s[16];
extern const unsigned char WORD_t[16];
extern const unsigned char WORD_x[16];
extern const unsigned char WORD_u[16];
extern const unsigned char WORD_z[16];
extern const unsigned char unittab[58][7];
extern const unsigned char typetab[14][7];

#if SOFT==Chinese
extern const unsigned char SHU[32];
extern const unsigned char RU[32];
extern const unsigned char SHE[32];
extern const unsigned char ZHI3[32];
//extern const unsigned char ZU[32];
extern const unsigned char TAI[32];
extern const unsigned char MI[32];
extern const unsigned char MA[32];
extern const unsigned char JIN[32];
extern const unsigned char TUI[32];
extern const unsigned char CHU[32];
extern const unsigned char XI[32];
extern const unsigned char TONG[32];
/*extern const unsigned char KONG[32];
extern const unsigned char ZHI[32];*/
extern const unsigned char RI[32];
extern const unsigned char QI[32];
extern const unsigned char SHI[32];
extern const unsigned char JIAN[32];
extern const unsigned char DUI[32];
extern const unsigned char BI[32];
extern const unsigned char DU[32];
extern const unsigned char DI[32];
extern const unsigned char ZHI1[32];
extern const unsigned char BO[32];
extern const unsigned char TE[32];
extern const unsigned char LV[32];
extern const unsigned char TONG1[32];
extern const unsigned char DAO[32];
extern const unsigned char XIN[32];
extern const unsigned char HAO[32];
extern const unsigned char LIANG[32];
extern const unsigned char CHENG[32];
extern const unsigned char JI[32];
extern const unsigned char LU[32];
extern const unsigned char GE[32];
extern const unsigned char LV1[32];
extern const unsigned char XIAO[32];
extern const unsigned char QIE[32];
extern const unsigned char CHU1[32];
extern const unsigned char DUAN[32];
extern const unsigned char ZU1[32];
extern const unsigned char OU[32];
extern const unsigned char CHU2[32];
extern const unsigned char LI[32];
extern const unsigned char BAO[32];
extern const unsigned char CHI[32];
extern const unsigned char QI1[32];
extern const unsigned char ZHONG[32];
extern const unsigned char DIAN[32];
extern const unsigned char DAN[32];
extern const unsigned char WEI[32];
extern const unsigned char WEI_W[32];    //维
extern const unsigned char HU[32];       //护
extern const unsigned char ZHU[32];      //主
extern const unsigned char CAI_1[32];    //菜
extern const unsigned char CHA_1[32];    //查
extern const unsigned char XUN_1[32];    //询
extern const unsigned char CI_1[32];     //次
extern const unsigned char QI_1[32];     //启
extern const unsigned char DANG[32];     //当
extern const unsigned char QIAN_1[32];   //前
extern const unsigned char YAN_2[32];    //延
extern const unsigned char TIAO[32];     //调
extern const unsigned char SHI_1[32];    //始
extern const unsigned char WEN_1[32];    //温
extern const unsigned char YA[32];       //压
extern const unsigned char LI_1[32];     //力
extern const unsigned char DA[32];       //大
extern const unsigned char QI_2[32];     //气
extern const unsigned char GONG[32];     //工
extern const unsigned char KUANG[32];    //况
//extern const unsigned char BIAO[32];     //标
extern const unsigned char LIU[32];
extern const unsigned char BAO1[32];
extern const unsigned char JING[32];
extern const unsigned char XUN[32];
extern const unsigned char FAN[32];
extern const unsigned char HUI[32];
extern const unsigned char WEI1[32];
extern const unsigned char KAI[32];
extern const unsigned char FANG[32];
extern const unsigned char SHI1[32];
extern const unsigned char FOU[32];
//extern const unsigned char CI[32];
//extern const unsigned char GONG[32];
//extern const unsigned char NENG[32];
//extern const unsigned char SUAN[32];
//extern const unsigned char FA[32];
//extern const unsigned char CHUAN[32];
//extern const unsigned char JI1[32];
/*extern const unsigned char TING[32];
extern const unsigned char ZHI2[32];*/
extern const unsigned char QI2[32];
extern const unsigned char CAI[32];
extern const unsigned char YANG[32];
extern const unsigned char ZHI4[32];
//extern const unsigned char XIE[32];
//extern const unsigned char YI[32];
extern const unsigned char ZHOU[32];
extern const unsigned char SHU1[32];
extern const unsigned char FEN[32];
extern const unsigned char JU[32];
extern const unsigned char BEI[32];
/*extern const unsigned char QU[32];*/
extern const unsigned char ZUO[32];
extern const unsigned char YONG[32];
extern const unsigned char ZHENG[32];
/*extern const unsigned char FAN1[32];
extern const unsigned char GEI[32];*/
extern const unsigned char DING[32];
extern const unsigned char CHA[32];
extern const unsigned char WU[32];
extern const unsigned char BIAN[32];
extern const unsigned char HUA[32];
extern const unsigned char LING[32];
extern const unsigned char QIAN[32];
extern const unsigned char YI1[32];
extern const unsigned char BEN[32];
extern const unsigned char JI3[32];

extern const unsigned char BIAO[32];

extern const unsigned char SHANG[32];
extern const unsigned char XIA[32];
extern const unsigned char XIAN[32];
/*extern const unsigned char SONG[32];
extern const unsigned char FAN2[32];
extern const unsigned char WEI2[32];*/
extern const unsigned char SHENG[32];
extern const unsigned char CHAN[32];
extern const unsigned char CE[32];
extern const unsigned char SHI2[32];
extern const unsigned char CHU3[32];
extern const unsigned char ZHONG1[32];
extern const unsigned char DDD[32];
extern const unsigned char CHENG1[32];
extern const unsigned char GONG[32];
extern const unsigned char SHI7[32];
extern const unsigned char BAI[32];
extern const unsigned char CAN[32];
extern const unsigned char KA[32];

/*
extern const unsigned char WAN[32];
extern const unsigned char CHENG1[32];
*/
extern const unsigned char ZHUANG[32];
extern const unsigned char CAO[32];
extern const unsigned char TI[32];
extern const unsigned char SHI5[32];
extern const unsigned char KE[32];
extern const unsigned char RONG[32];
extern const unsigned char ZONG[32];
extern const unsigned char LI1[32];
extern const unsigned char SHI3[32];
extern const unsigned char GE1[32];
extern const unsigned char SHI4[32];
extern const unsigned char CUN[32];
extern const unsigned char QUAN[32];
extern const unsigned char BU[32];
extern const unsigned char FEN1[32];
extern const unsigned char ZI[32];
extern const unsigned char DONG[32];
extern const unsigned char JIU[32];
extern const unsigned char XU[32];
extern const unsigned char BU1[32];
extern const unsigned char YI2[32];
extern const unsigned char JIE[32];
extern const unsigned char SU[32];
extern const unsigned char QUE[32];
extern const unsigned char WEN[32];
extern const unsigned char JIAN1[32];
extern const unsigned char MING[32];
extern const unsigned char ZAI[32];
extern const unsigned char QING[32];
extern const unsigned char SHAO[32];
extern const unsigned char DENG[32];
extern const unsigned char SHI6[32];
extern const unsigned char XUAN[32];
extern const unsigned char ZE[32];
extern const unsigned char DAFANGGE[32];
extern const unsigned char XUANZHONG[32];
extern const unsigned char WANG[32];
extern const unsigned char LUO[32];
extern const unsigned char DUAN1[32];
extern const unsigned char KOU[32];
extern const unsigned char YAN[32];
extern const unsigned char GUAN[32];
extern const unsigned char JI4[32];
extern const unsigned char JIAO[32];
extern const unsigned char TING[32];
extern const unsigned char ZHI[32];
extern const unsigned char YAN1[32];

extern const unsigned char QU1[32];
extern const unsigned char XIAO1[32];

extern const unsigned char WORD_v[16];
extern const unsigned char WORD_w[16];
extern const unsigned char WORD_j[16];
#endif
extern const unsigned char NUM24_32_0[96];
extern const unsigned char NUM24_32_1[96];
extern const unsigned char NUM24_32_2[96];
extern const unsigned char NUM24_32_3[96];
extern const unsigned char NUM24_32_4[96];
extern const unsigned char NUM24_32_5[96];
extern const unsigned char NUM24_32_6[96];
extern const unsigned char NUM24_32_7[96];
extern const unsigned char NUM24_32_8[96];
extern const unsigned char NUM24_32_9[96];
extern const unsigned char NUM24_32_dian[96];
extern const unsigned char NUM24_32_yigong[96];
extern const unsigned char NUM24_32_kongge[96];
extern const unsigned char Huan_24_32[96];
extern const unsigned char Ying_24_32[96];
extern const unsigned char Shi_24_32[96];
extern const unsigned char Yong_24_32[96];

extern const unsigned char auchCRCHi[256];
extern const char auchCRCLo[256];
extern const unsigned char StopREC[];
#endif
