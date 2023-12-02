/*
 * File: Controller.h
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

#ifndef RTW_HEADER_Controller_h_
#define RTW_HEADER_Controller_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef Controller_COMMON_INCLUDES_
#define Controller_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Controller_COMMON_INCLUDES_ */

#include "Controller_types.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T aI;                           /* '<Root>/aI' */
  real_T at;                           /* '<Root>/at' */
} ExtU_Controller_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T u_sliding_mode;               /* '<Root>/u_sliding_mode' */
} ExtY_Controller_T;

/* Real-time Model Data Structure */
struct tag_RTM_Controller_T {
  const char_T * volatile errorStatus;
};

/* Model entry point functions */
extern void Controller_initialize(RT_MODEL_Controller_T *const M,
  ExtU_Controller_T *U, ExtY_Controller_T *Y);
extern void Controller_step(RT_MODEL_Controller_T *const M, ExtU_Controller_T *U,
  ExtY_Controller_T *Y);
extern void Controller_terminate(RT_MODEL_Controller_T *const M);

/* Exported data declaration */

/* Data with Exported storage */
extern real_T _r0[3];                  /* '<S1>/_r0' */
extern real_T _r0_DSTATE[3];           /* '<S1>/_r0' */
extern real_T _thetaa;                 /* '<S1>/_thetaa' */
extern real_T _thetaa_DSTATE;          /* '<S1>/_thetaa' */
extern real_T r[3];                    /* '<S1>/MATLAB Function3' */
extern real_T theta;                   /* '<S1>/MATLAB Function3' */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('janito_sliding_mode_acel_bacurau_2020a/Controller')    - opens subsystem janito_sliding_mode_acel_bacurau_2020a/Controller
 * hilite_system('janito_sliding_mode_acel_bacurau_2020a/Controller/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'janito_sliding_mode_acel_bacurau_2020a'
 * '<S1>'   : 'janito_sliding_mode_acel_bacurau_2020a/Controller'
 * '<S2>'   : 'janito_sliding_mode_acel_bacurau_2020a/Controller/MATLAB Function3'
 */
#endif                                 /* RTW_HEADER_Controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
