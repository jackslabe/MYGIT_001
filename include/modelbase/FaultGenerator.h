/*
 * File: FaultGenerator.h
 *
 * Code generated for Simulink model 'FaultGenerator'.
 *
 * Model version                  : 1.39
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Sat Apr 01 15:39:27 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_FaultGenerator_h_
#define RTW_HEADER_FaultGenerator_h_
#include <string.h>
#ifndef FaultGenerator_COMMON_INCLUDES_
# define FaultGenerator_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#endif                                 /* FaultGenerator_COMMON_INCLUDES_ */

#include "FaultGenerator_types.h"

/* Shared type includes */
#include "model_reference_types.h"

/* Block signals for system '<Root>/Debounce and Undebounce' */
typedef struct {
  boolean_T VbDIA_Malfunction_flg;     /* '<Root>/Debounce and Undebounce' */
} B_DebounceandUndebounce_Fault_T;

/* Block states (auto storage) for system '<Root>/Debounce and Undebounce' */
typedef struct {
  real32_T VfDIA_DbCounter_s;          /* '<Root>/Debounce and Undebounce' */
  real32_T VfDIA_UdbCounter_s;         /* '<Root>/Debounce and Undebounce' */
  uint8_T is_active_c1_ToolsLib;       /* '<Root>/Debounce and Undebounce' */
  uint8_T is_c1_ToolsLib;              /* '<Root>/Debounce and Undebounce' */
} DW_DebounceandUndebounce_Faul_T;

/* Block signals for model 'FaultGenerator' */
typedef struct {
  B_DebounceandUndebounce_Fault_T sf_DebounceandUndebounce1;/* '<Root>/Debounce and Undebounce1' */
  B_DebounceandUndebounce_Fault_T sf_DebounceandUndebounce;/* '<Root>/Debounce and Undebounce' */
} B_FaultGenerator_c_T;

/* Block states (auto storage) for model 'FaultGenerator' */
typedef struct {
  DW_DebounceandUndebounce_Faul_T sf_DebounceandUndebounce1;/* '<Root>/Debounce and Undebounce1' */
  DW_DebounceandUndebounce_Faul_T sf_DebounceandUndebounce;/* '<Root>/Debounce and Undebounce' */
} DW_FaultGenerator_f_T;

/* Zero-crossing (trigger) state for model 'FaultGenerator' */
typedef struct {
  ZCSigState TmpModelReferenceSubsystem_Trig;/* synthesized block */
} ZCE_FaultGenerator_T;

/* Real-time Model Data Structure */
struct tag_RTM_FaultGenerator_T {
  const char_T **errorStatus;
};

typedef struct {
  B_FaultGenerator_c_T rtb;
  DW_FaultGenerator_f_T rtdw;
  RT_MODEL_FaultGenerator_T rtm;
  ZCE_FaultGenerator_T rtzce;
} MdlrefDW_FaultGenerator_T;

/* Model reference registration function */
extern void FaultGenerator_initialize(const char_T **rt_errorStatus,
  RT_MODEL_FaultGenerator_T *const FaultGenerator_M, B_FaultGenerator_c_T
  *localB, DW_FaultGenerator_f_T *localDW, ZCE_FaultGenerator_T *localZCE);
extern void Faul_DebounceandUndebounce_Init(B_DebounceandUndebounce_Fault_T
  *localB, DW_DebounceandUndebounce_Faul_T *localDW);
extern void FaultGene_DebounceandUndebounce(real32_T rtu_VfDIA_DbStepAddTick_s,
  real32_T rtu_VfDIA_DbCountAddThr_s, real32_T rtu_VfDIA_UdbStepAddTick_s,
  real32_T rtu_VfDIA_UdbCountAddThr_s, boolean_T rtu_VbDIA_DbReq_flg,
  B_DebounceandUndebounce_Fault_T *localB, DW_DebounceandUndebounce_Faul_T
  *localDW);
extern void FaultGenerator_Init(B_FaultGenerator_c_T *localB,
  DW_FaultGenerator_f_T *localDW);
extern void FaultGenerator(const real32_T *rtu_vcmax, const real32_T *rtu_vcmin,
  const real32_T *rtu_Soc, const boolean_T *rtu_Trigger, uint8_T
  *rty_VCellMaxAlarm, uint8_T *rty_VCellMinAlarm, uint8_T *rty_SocMaxAlarm,
  uint8_T *rty_SocMinAlarm, uint8_T *rty_FaultLevel, B_FaultGenerator_c_T
  *localB, DW_FaultGenerator_f_T *localDW, ZCE_FaultGenerator_T *localZCE);

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
 * '<Root>' : 'FaultGenerator'
 * '<S1>'   : 'FaultGenerator/Compare To Constant'
 * '<S2>'   : 'FaultGenerator/Compare To Constant1'
 * '<S3>'   : 'FaultGenerator/Compare To Constant2'
 * '<S4>'   : 'FaultGenerator/Compare To Constant3'
 * '<S5>'   : 'FaultGenerator/Debounce and Undebounce'
 * '<S6>'   : 'FaultGenerator/Debounce and Undebounce1'
 */
#endif                                 /* RTW_HEADER_FaultGenerator_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
