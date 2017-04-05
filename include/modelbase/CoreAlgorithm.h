/*
 * File: CoreAlgorithm.h
 *
 * Code generated for Simulink model 'CoreAlgorithm'.
 *
 * Model version                  : 1.71
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Tue Mar 28 21:13:03 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CoreAlgorithm_h_
#define RTW_HEADER_CoreAlgorithm_h_
#include <string.h>
#ifndef CoreAlgorithm_COMMON_INCLUDES_
# define CoreAlgorithm_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#endif                                 /* CoreAlgorithm_COMMON_INCLUDES_ */

#include "CoreAlgorithm_types.h"

/* Shared type includes */
#include "model_reference_types.h"
#include "look1_iflf_binlxpw.h"

/* Block signals for model 'CoreAlgorithm' */
typedef struct {
  boolean_T CheckingDataStatus;        /* '<Root>/SocCalcControl' */
  boolean_T InitSocStatus;             /* '<Root>/SocCalcControl' */
  boolean_T NormalCalcSocStatus;       /* '<Root>/SocCalcControl' */
} B_CoreAlgorithm_c_T;

/* Block states (auto storage) for model 'CoreAlgorithm' */
typedef struct {
  real32_T Memory1_PreviousInput;      /* '<Root>/Memory1' */
  uint8_T is_active_c3_CoreAlgorithm;  /* '<Root>/SocCalcControl' */
  uint8_T is_c3_CoreAlgorithm;         /* '<Root>/SocCalcControl' */
  uint8_T is_SocCalcControl;           /* '<Root>/SocCalcControl' */
} DW_CoreAlgorithm_f_T;

/* Zero-crossing (trigger) state for model 'CoreAlgorithm' */
typedef struct {
  ZCSigState TmpModelReferenceSubsystem_Trig;/* synthesized block */
} ZCE_CoreAlgorithm_T;

/* Real-time Model Data Structure */
struct tag_RTM_CoreAlgorithm_T {
  const char_T **errorStatus;
};

typedef struct {
  B_CoreAlgorithm_c_T rtb;
  DW_CoreAlgorithm_f_T rtdw;
  RT_MODEL_CoreAlgorithm_T rtm;
  ZCE_CoreAlgorithm_T rtzce;
} MdlrefDW_CoreAlgorithm_T;

/* Model reference registration function */
extern void CoreAlgorithm_initialize(const char_T **rt_errorStatus,
  RT_MODEL_CoreAlgorithm_T *const CoreAlgorithm_M, B_CoreAlgorithm_c_T *localB,
  DW_CoreAlgorithm_f_T *localDW, ZCE_CoreAlgorithm_T *localZCE);
extern void CoreAlgorithm_Init(real32_T *rty_Soc, B_CoreAlgorithm_c_T *localB,
  DW_CoreAlgorithm_f_T *localDW);
extern void CoreAlgorithm(const real32_T *rtu_VC, const real32_T *rtu_TC, const
  real32_T *rtu_IBat, const boolean_T *rtu_Trigger, real32_T *rty_Soc,
  B_CoreAlgorithm_c_T *localB, DW_CoreAlgorithm_f_T *localDW,
  ZCE_CoreAlgorithm_T *localZCE);

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
 * '<Root>' : 'CoreAlgorithm'
 * '<S1>'   : 'CoreAlgorithm/CheckingData'
 * '<S2>'   : 'CoreAlgorithm/InitSoc'
 * '<S3>'   : 'CoreAlgorithm/NormalCalcSoc'
 * '<S4>'   : 'CoreAlgorithm/SocCalcControl'
 */
#endif                                 /* RTW_HEADER_CoreAlgorithm_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
