/*
 * File: Schedule.c
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

#include "Schedule.h"
#include "Schedule_private.h"

/* Output and update for referenced model: 'Schedule' */
void Schedule(boolean_T *rty_u0ms_call, boolean_T *rty_u00ms_call, boolean_T
              *rty_us_call, DW_Schedule_f_T *localDW)
{
  uint16_T rtb_FixPtSum1;

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   *  Math: '<Root>/Math Function'
   *  UnitDelay: '<S4>/Output'
   */
  *rty_u0ms_call = ((uint16_T)(localDW->Output_DSTATE % 2U) == 1);

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   *  Math: '<Root>/Math Function1'
   *  UnitDelay: '<S4>/Output'
   */
  *rty_u00ms_call = ((uint16_T)(localDW->Output_DSTATE % 20U) == 1);

  /* RelationalOperator: '<S3>/Compare' incorporates:
   *  Constant: '<S3>/Constant'
   *  Math: '<Root>/Math Function2'
   *  UnitDelay: '<S4>/Output'
   */
  *rty_us_call = ((uint16_T)(localDW->Output_DSTATE % 200U) == 1);

  /* Sum: '<S5>/FixPt Sum1' incorporates:
   *  Constant: '<S5>/FixPt Constant'
   *  UnitDelay: '<S4>/Output'
   */
  rtb_FixPtSum1 = (uint16_T)(localDW->Output_DSTATE + 1U);

  /* Switch: '<S6>/FixPt Switch' */
  if (rtb_FixPtSum1 > 1000) {
    /* Update for UnitDelay: '<S4>/Output' incorporates:
     *  Constant: '<S6>/Constant'
     */
    localDW->Output_DSTATE = 0U;
  } else {
    /* Update for UnitDelay: '<S4>/Output' */
    localDW->Output_DSTATE = rtb_FixPtSum1;
  }

  /* End of Switch: '<S6>/FixPt Switch' */
}

/* Model initialize function */
void Schedule_initialize(const char_T **rt_errorStatus, RT_MODEL_Schedule_T *
  const Schedule_M, DW_Schedule_f_T *localDW)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(Schedule_M, rt_errorStatus);

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(DW_Schedule_f_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
