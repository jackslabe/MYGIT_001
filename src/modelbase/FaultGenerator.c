/*
 * File: FaultGenerator.c
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

#include "FaultGenerator.h"
#include "FaultGenerator_private.h"

/* Named constants for Chart: '<Root>/Debounce and Undebounce' */
#define FaultGenerat_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define FaultGenerator_IN_debounce     ((uint8_T)1U)
#define FaultGenerator_IN_undebounce   ((uint8_T)2U)

/*
 * System initialize for atomic system:
 *    '<Root>/Debounce and Undebounce'
 *    '<Root>/Debounce and Undebounce1'
 */
void Faul_DebounceandUndebounce_Init(B_DebounceandUndebounce_Fault_T *localB,
  DW_DebounceandUndebounce_Faul_T *localDW)
{
  localDW->is_active_c1_ToolsLib = 0U;
  localDW->is_c1_ToolsLib = FaultGenerat_IN_NO_ACTIVE_CHILD;
  localDW->VfDIA_DbCounter_s = 0.0F;
  localDW->VfDIA_UdbCounter_s = 0.0F;
  localB->VbDIA_Malfunction_flg = false;
}

/*
 * Output and update for atomic system:
 *    '<Root>/Debounce and Undebounce'
 *    '<Root>/Debounce and Undebounce1'
 */
void FaultGene_DebounceandUndebounce(real32_T rtu_VfDIA_DbStepAddTick_s,
  real32_T rtu_VfDIA_DbCountAddThr_s, real32_T rtu_VfDIA_UdbStepAddTick_s,
  real32_T rtu_VfDIA_UdbCountAddThr_s, boolean_T rtu_VbDIA_DbReq_flg,
  B_DebounceandUndebounce_Fault_T *localB, DW_DebounceandUndebounce_Faul_T
  *localDW)
{
  /* Gateway: Debounce and Undebounce */
  /* During: Debounce and Undebounce */
  if (localDW->is_active_c1_ToolsLib == 0U) {
    /* Entry: Debounce and Undebounce */
    localDW->is_active_c1_ToolsLib = 1U;

    /* Entry Internal: Debounce and Undebounce */
    /* Transition: '<S5>:3' */
    localB->VbDIA_Malfunction_flg = false;
    localDW->is_c1_ToolsLib = FaultGenerator_IN_debounce;

    /* Entry 'debounce': '<S5>:1' */
    localDW->VfDIA_DbCounter_s = 0.0F;
  } else if (localDW->is_c1_ToolsLib == FaultGenerator_IN_debounce) {
    /* During 'debounce': '<S5>:1' */
    if (localB->VbDIA_Malfunction_flg) {
      /* Transition: '<S5>:4' */
      localDW->is_c1_ToolsLib = FaultGenerator_IN_undebounce;

      /* Entry 'undebounce': '<S5>:2' */
      localDW->VfDIA_UdbCounter_s = 0.0F;
    } else {
      /* Transition: '<S5>:18' */
      if (localDW->VfDIA_DbCounter_s >= rtu_VfDIA_DbCountAddThr_s) {
        /* Transition: '<S5>:28' */
        /* Transition: '<S5>:30' */
        localB->VbDIA_Malfunction_flg = true;
        localDW->VfDIA_DbCounter_s = rtu_VfDIA_DbCountAddThr_s;

        /* Transition: '<S5>:33' */
        /* Transition: '<S5>:89' */
        /* Transition: '<S5>:65' */
      } else {
        /* Transition: '<S5>:110' */
        if (rtu_VbDIA_DbReq_flg) {
          /* Transition: '<S5>:63' */
          /* Transition: '<S5>:66' */
          localDW->VfDIA_DbCounter_s += rtu_VfDIA_DbStepAddTick_s;

          /* Transition: '<S5>:89' */
          /* Transition: '<S5>:65' */
        } else {
          /* Transition: '<S5>:111' */
          if (localDW->VfDIA_DbCounter_s <= 0.0F) {
            /* Transition: '<S5>:87' */
            /* Transition: '<S5>:90' */
            localDW->VfDIA_DbCounter_s = 0.0F;

            /* Transition: '<S5>:65' */
          } else {
            /* Transition: '<S5>:107' */
            localDW->VfDIA_DbCounter_s -= rtu_VfDIA_DbStepAddTick_s;
          }
        }
      }

      /* Transition: '<S5>:69' */
    }
  } else {
    /* During 'undebounce': '<S5>:2' */
    if (!localB->VbDIA_Malfunction_flg) {
      /* Transition: '<S5>:5' */
      localDW->is_c1_ToolsLib = FaultGenerator_IN_debounce;

      /* Entry 'debounce': '<S5>:1' */
      localDW->VfDIA_DbCounter_s = 0.0F;
    } else {
      /* Transition: '<S5>:36' */
      if (localDW->VfDIA_UdbCounter_s >= rtu_VfDIA_UdbCountAddThr_s) {
        /* Transition: '<S5>:42' */
        /* Transition: '<S5>:76' */
        localB->VbDIA_Malfunction_flg = false;
        localDW->VfDIA_UdbCounter_s = rtu_VfDIA_UdbCountAddThr_s;

        /* Transition: '<S5>:41' */
        /* Transition: '<S5>:96' */
        /* Transition: '<S5>:44' */
      } else {
        /* Transition: '<S5>:73' */
        if (!rtu_VbDIA_DbReq_flg) {
          /* Transition: '<S5>:98' */
          /* Transition: '<S5>:99' */
          localDW->VfDIA_UdbCounter_s += rtu_VfDIA_UdbStepAddTick_s;

          /* Transition: '<S5>:96' */
          /* Transition: '<S5>:44' */
        } else {
          /* Transition: '<S5>:112' */
          if (localDW->VfDIA_UdbCounter_s <= 0.0F) {
            /* Transition: '<S5>:74' */
            /* Transition: '<S5>:75' */
            localDW->VfDIA_UdbCounter_s = 0.0F;

            /* Transition: '<S5>:44' */
          } else {
            /* Transition: '<S5>:43' */
            localDW->VfDIA_UdbCounter_s -= rtu_VfDIA_UdbStepAddTick_s;
          }
        }
      }

      /* Transition: '<S5>:46' */
    }
  }
}

/* System initialize for referenced model: 'FaultGenerator' */
void FaultGenerator_Init(B_FaultGenerator_c_T *localB, DW_FaultGenerator_f_T
  *localDW)
{
  /* SystemInitialize for Chart: '<Root>/Debounce and Undebounce' */
  Faul_DebounceandUndebounce_Init(&localB->sf_DebounceandUndebounce,
    &localDW->sf_DebounceandUndebounce);

  /* SystemInitialize for Chart: '<Root>/Debounce and Undebounce1' */
  Faul_DebounceandUndebounce_Init(&localB->sf_DebounceandUndebounce1,
    &localDW->sf_DebounceandUndebounce1);
}

/* Output and update for referenced model: 'FaultGenerator' */
void FaultGenerator(const real32_T *rtu_vcmax, const real32_T *rtu_vcmin, const
                    real32_T *rtu_Soc, const boolean_T *rtu_Trigger, uint8_T
                    *rty_VCellMaxAlarm, uint8_T *rty_VCellMinAlarm, uint8_T
                    *rty_SocMaxAlarm, uint8_T *rty_SocMinAlarm, uint8_T
                    *rty_FaultLevel, B_FaultGenerator_c_T *localB,
                    DW_FaultGenerator_f_T *localDW, ZCE_FaultGenerator_T
                    *localZCE)
{
  if ((*rtu_Trigger) && (localZCE->TmpModelReferenceSubsystem_Trig != POS_ZCSIG))
  {
    /* Chart: '<Root>/Debounce and Undebounce' incorporates:
     *  Constant: '<Root>/Constant'
     *  Constant: '<Root>/Constant1'
     *  Constant: '<Root>/Constant2'
     *  Constant: '<Root>/Constant3'
     *  Constant: '<S1>/Constant'
     *  RelationalOperator: '<S1>/Compare'
     */
    FaultGene_DebounceandUndebounce(0.1F, 3.0F, 0.1F, 3.0F, *rtu_vcmax >= 4.18F,
      &localB->sf_DebounceandUndebounce, &localDW->sf_DebounceandUndebounce);

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    *rty_VCellMaxAlarm = localB->sf_DebounceandUndebounce.VbDIA_Malfunction_flg;

    /* Chart: '<Root>/Debounce and Undebounce1' incorporates:
     *  Constant: '<Root>/Constant4'
     *  Constant: '<Root>/Constant5'
     *  Constant: '<Root>/Constant6'
     *  Constant: '<Root>/Constant7'
     *  Constant: '<S2>/Constant'
     *  RelationalOperator: '<S2>/Compare'
     */
    FaultGene_DebounceandUndebounce(0.1F, 3.0F, 0.1F, 3.0F, *rtu_vcmin <= 3.2F,
      &localB->sf_DebounceandUndebounce1, &localDW->sf_DebounceandUndebounce1);

    /* DataTypeConversion: '<Root>/Data Type Conversion2' */
    *rty_VCellMinAlarm = localB->sf_DebounceandUndebounce1.VbDIA_Malfunction_flg;

    /* RelationalOperator: '<S3>/Compare' incorporates:
     *  Constant: '<S3>/Constant'
     */
    *rty_SocMaxAlarm = (uint8_T)(*rtu_Soc > 100.0F);

    /* RelationalOperator: '<S4>/Compare' incorporates:
     *  Constant: '<S4>/Constant'
     */
    *rty_SocMinAlarm = (uint8_T)(*rtu_Soc < 5.0F);

    /* DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
     *  Logic: '<Root>/Logical Operator'
     */
    *rty_FaultLevel = (uint8_T)((*rty_SocMinAlarm != 0) && (*rty_SocMaxAlarm !=
      0));
  }

  localZCE->TmpModelReferenceSubsystem_Trig = *rtu_Trigger;
}

/* Model initialize function */
void FaultGenerator_initialize(const char_T **rt_errorStatus,
  RT_MODEL_FaultGenerator_T *const FaultGenerator_M, B_FaultGenerator_c_T
  *localB, DW_FaultGenerator_f_T *localDW, ZCE_FaultGenerator_T *localZCE)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(FaultGenerator_M, rt_errorStatus);

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(B_FaultGenerator_c_T));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(DW_FaultGenerator_f_T));
  localZCE->TmpModelReferenceSubsystem_Trig = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
