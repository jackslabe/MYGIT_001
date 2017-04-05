/*
 * File: Schedule.h
 *
 * Code generated for Simulink model 'Schedule'.
 *
 * Model version                  : 1.13
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Tue Mar 28 21:13:31 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Schedule_h_
#define RTW_HEADER_Schedule_h_
#include <string.h>
#ifndef Schedule_COMMON_INCLUDES_
# define Schedule_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Schedule_COMMON_INCLUDES_ */

#include "Schedule_types.h"

/* Shared type includes */
#include "model_reference_types.h"

/* Block states (auto storage) for model 'Schedule' */
typedef struct {
  uint16_T Output_DSTATE;              /* '<S4>/Output' */
} DW_Schedule_f_T;

/* Real-time Model Data Structure */
struct tag_RTM_Schedule_T {
  const char_T **errorStatus;
};

typedef struct {
  DW_Schedule_f_T rtdw;
  RT_MODEL_Schedule_T rtm;
} MdlrefDW_Schedule_T;

/* Model reference registration function */
extern void Schedule_initialize(const char_T **rt_errorStatus,
  RT_MODEL_Schedule_T *const Schedule_M, DW_Schedule_f_T *localDW);
extern void Schedule(boolean_T *rty_u0ms_call, boolean_T *rty_u00ms_call,
                     boolean_T *rty_us_call, DW_Schedule_f_T *localDW);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S4>/Data Type Propagation' : Unused code path elimination
 * Block '<S5>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S6>/FixPt Data Type Duplicate1' : Unused code path elimination
 */

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
 * '<Root>' : 'Schedule'
 * '<S1>'   : 'Schedule/Compare To Constant'
 * '<S2>'   : 'Schedule/Compare To Constant1'
 * '<S3>'   : 'Schedule/Compare To Constant2'
 * '<S4>'   : 'Schedule/Counter Limited'
 * '<S5>'   : 'Schedule/Counter Limited/Increment Real World'
 * '<S6>'   : 'Schedule/Counter Limited/Wrap To Zero'
 */
#endif                                 /* RTW_HEADER_Schedule_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
