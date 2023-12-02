/*
 * File: Controller.c
 *
 * Code generated for Simulink model 'Controller'.
 *
 * Model version                  : 13.1
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Thu Nov 23 15:18:56 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Controller.h"
#include "Controller_private.h"

float acccc;
float udis;
float thetasl;
float dthetasl;

/* Exported data definition */

/* Data with Exported storage */
real_T last;
real_T _r0[3];                         /* '<S1>/_r0' */
real_T _r0_DSTATE[3];                  /* '<S1>/_r0' */
real_T _thetaa;                        /* '<S1>/_thetaa' */
real_T _thetaa_DSTATE;                 /* '<S1>/_thetaa' */
real_T r[3];                           /* '<S1>/MATLAB Function3' */
real_T theta;                          /* '<S1>/MATLAB Function3' */
real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T tmp;
  int32_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u1 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u0 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp_0, tmp);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model step function */
void Controller_step(RT_MODEL_Controller_T *const M, ExtU_Controller_T *U,
                     ExtY_Controller_T *Y)
{
  real_T ac;
  real_T theta_0;
  real_T u_0;
  real_T x0_idx_3;
  _r0[0] = _r0_DSTATE[0];
  _r0[1] = _r0_DSTATE[1];
  _r0[2] = _r0_DSTATE[2];
  _thetaa = _thetaa_DSTATE;

  /*  Massa suspensa = massa total 253 kg - 2*mr */
  /*  Massa das rodas + 1/2 suspensão + 1/2 braço suspensão, kg */
  /*  Momento de inercia- */
  /*  Momento de inercia da barra */
  /*  Aceleraçao da gravidade */
  /*  Bitola traseira, em mm  ????? */
  /*  Rigidez molas suspensao traseira  ???? */
  /*  Amortecimento suspensao traseira  ????? */
  /*  Raio roda traseira */
  /*  Comprimento livre da mola   ????? A calcular. Carregada e 109 + 29 mm */
  /*  Altura do CG sobre o pino de controle     ?????? */
  /*  Meio bra�o de controle */
  /*  Massa suspensa */
  /*  Momento de inercia- */
  /*  Momento de inercia da barra */
  /*  Acelera�ao da gravidade */
  /*  Rigidez molas suspensao traseira  ???? */
  /*  Amortecimento suspensao traseira  ????? */
  /*  Raio roda traseira */
  /*  Comprimento livre da mola   ????? A calcular. Carregada e 109 + 29 mm */
  /*  Altura do CG sobre o pino de controle     ?????? */
  /*  Meio bra�o de controle   0.240 */
  if ((U->at == 0.0) && (U->aI == 0.0)) {
    ac = 0.0;
  } else if (U->at * U->at + U->aI * U->aI < 96.236100000000008) {
    ac = 0.0;
  } else {
    ac = sqrt((U->at * U->at + U->aI * U->aI) - 96.236100000000008);
  }
  acccc = ac;

  theta_0 = rt_atan2d_snf(ac, 9.81) - rt_atan2d_snf(U->at, U->aI);
  x0_idx_3 = ac / 9.81;
  r[0] = atan(x0_idx_3);
  r[1] = (atan(x0_idx_3) - _r0[0]) / 0.01;
  r[2] = (r[1] - _r0[1]) / 0.01;
  x0_idx_3 = (theta_0 - _thetaa) / 0.01;
  dthetasl = x0_idx_3;

  /*  x0(1)=y; x0(2)=yponto; x0(3)=theta; x0(4)=thetaponto; */
  /*  u=-( ddx3d + udis + f4  + f3*lamb2)/(g4  + g3*lamb2); */
  u_0 = (r[1] - x0_idx_3) - (theta_0 - r[0]) * 5.0;
  if (u_0 < 0.0) {
    u_0 = -1.0;
  } else if (u_0 > 0.0) {
    u_0 = 1.0;
  } else if (u_0 == 0.0) {
    u_0 = 0.0;
  } else {
    u_0 = (rtNaN);
  }
  udis = u_0;

  Y->u_sliding_mode = -((((((3531.6000000000004 * sin(theta_0) - -360.0 * ac *
    cos(theta_0)) - 0.0 * x0_idx_3) * 0.535 / 150.24099999999999 - r[2]) + -u_0)
    - r[1] * 5.0) + x0_idx_3 * 5.0) / 0.0066559727371356689;

  if(Y->u_sliding_mode >= 200 && Y->u_sliding_mode <= -200)
  {
	  Y->u_sliding_mode = last;
  }
  else
  {
	  last = Y->u_sliding_mode;
  }

  theta = theta_0;
  thetasl = theta;
  _r0_DSTATE[0] = r[0];
  _r0_DSTATE[1] = r[1];
  _r0_DSTATE[2] = r[2];
  _thetaa_DSTATE = theta;
  UNUSED_PARAMETER(M);
}

/* Model initialize function */
void Controller_initialize(RT_MODEL_Controller_T *const M, ExtU_Controller_T *U,
  ExtY_Controller_T *Y)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(M, (NULL));

  /* Storage classes */
  _r0[0] = 0.0;
  _r0[1] = 0.0;
  _r0[2] = 0.0;
  r[0] = 0.0;
  r[1] = 0.0;
  r[2] = 0.0;
  _r0_DSTATE[0] = 0.0;
  _r0_DSTATE[1] = 0.0;
  _r0_DSTATE[2] = 0.0;
  _thetaa = 0.0;
  theta = 0.0;
  _thetaa_DSTATE = 0.0;

  /* external inputs */
  (void)memset(U, 0, sizeof(ExtU_Controller_T));

  /* external outputs */
  Y->u_sliding_mode = 0.0;
  UNUSED_PARAMETER(M);
}

/* Model terminate function */
void Controller_terminate(RT_MODEL_Controller_T *const M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
