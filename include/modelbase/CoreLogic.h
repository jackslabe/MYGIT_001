/*
 * File: CoreLogic.h
 *
 * Code generated for Simulink model 'CoreLogic'.
 *
 * Model version                  : 1.34
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Tue Mar 28 22:23:15 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CoreLogic_h_
#define RTW_HEADER_CoreLogic_h_
#include <string.h>
#ifndef CoreLogic_COMMON_INCLUDES_
# define CoreLogic_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#endif                                 /* CoreLogic_COMMON_INCLUDES_ */

#include "CoreLogic_types.h"

/* Shared type includes */
#include "model_reference_types.h"

/* Block states (auto storage) for model 'CoreLogic' */
typedef struct {
  uint8_T is_active_c3_CoreLogic;      /* '<Root>/Chart' */
  uint8_T is_c3_CoreLogic;             /* '<Root>/Chart' */
  uint8_T InitFinished;                /* '<Root>/Chart' */
} DW_CoreLogic_f_T;

/* Zero-crossing (trigger) state for model 'CoreLogic' */
typedef struct {
  ZCSigState TmpModelReferenceSubsystem_Trig;/* synthesized block */
} ZCE_CoreLogic_T;

/* Real-time Model Data Structure */
struct tag_RTM_CoreLogic_T {
  const char_T **errorStatus;
};

typedef struct {
  DW_CoreLogic_f_T rtdw;
  RT_MODEL_CoreLogic_T rtm;
  ZCE_CoreLogic_T rtzce;
} MdlrefDW_CoreLogic_T;

/* Model reference registration function */
extern void CoreLogic_initialize(const char_T **rt_errorStatus,
  RT_MODEL_CoreLogic_T *const CoreLogic_M, DW_CoreLogic_f_T *localDW,
  ZCE_CoreLogic_T *localZCE);
extern void CoreLogic_Init(DW_CoreLogic_f_T *localDW);
extern void CoreLogic(const uint8_T *rtu_Acc, const real32_T *rtu_CC, const
                      uint8_T *rtu_FaultLevel, const real32_T *rtu_Soc, const
                      boolean_T *rtu_Trigger, DW_CoreLogic_f_T *localDW,
                      ZCE_CoreLogic_T *localZCE);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'CoreLogic'
 * '<S1>'   : 'CoreLogic/Chart'
 */
#endif                                 /* RTW_HEADER_CoreLogic_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
