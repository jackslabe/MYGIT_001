/*
 * File: Statistic.h
 *
 * Code generated for Simulink model 'Statistic'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Wed Apr 05 14:35:11 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Statistic_h_
#define RTW_HEADER_Statistic_h_
#ifndef Statistic_COMMON_INCLUDES_
# define Statistic_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#endif                                 /* Statistic_COMMON_INCLUDES_ */

#include "Statistic_types.h"

/* Shared type includes */
#include "model_reference_types.h"

/* Zero-crossing (trigger) state for model 'Statistic' */
typedef struct {
  ZCSigState TmpModelReferenceSubsystem_Trig;/* synthesized block */
} ZCE_Statistic_T;

/* Real-time Model Data Structure */
struct tag_RTM_Statistic_T {
  const char_T **errorStatus;
};

typedef struct {
  RT_MODEL_Statistic_T rtm;
  ZCE_Statistic_T rtzce;
} MdlrefDW_Statistic_T;

/* Model reference registration function */
extern void Statistic_initialize(const char_T **rt_errorStatus,
  RT_MODEL_Statistic_T *const Statistic_M, ZCE_Statistic_T *localZCE);
extern void Statistic(const real32_T rtu_VCell[96], const real32_T rtu_TCell[96],
                      const boolean_T *rtu_Trigger, real32_T *rty_VCellMax,
                      uint8_T *rty_VCellMaxNum, real32_T *rty_VCellMin, uint8_T *
                      rty_VCellMinNum, real32_T *rty_TCellMax, uint8_T
                      *rty_TCellMaxNum, real32_T *rty_TCellMin, uint8_T
                      *rty_TCellMinNum, ZCE_Statistic_T *localZCE);

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
 * '<Root>' : 'Statistic'
 */
#endif                                 /* RTW_HEADER_Statistic_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
