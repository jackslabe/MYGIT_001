/*
 * File: CoreLogic.c
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

#include "CoreLogic.h"
#include "CoreLogic_private.h"

/* Named constants for Chart: '<Root>/Chart' */
#define CoreLogic_IN_ChargeMode        ((uint8_T)1U)
#define CoreLogic_IN_Init              ((uint8_T)2U)
#define CoreLogic_IN_NO_ACTIVE_CHILD   ((uint8_T)0U)
#define CoreLogic_IN_RunMode           ((uint8_T)3U)
#define CoreLogic_IN_WatiForSleep      ((uint8_T)4U)

/* System initialize for referenced model: 'CoreLogic' */
void CoreLogic_Init(DW_CoreLogic_f_T *localDW)
{
  /* SystemInitialize for Chart: '<Root>/Chart' */
  localDW->is_active_c3_CoreLogic = 0U;
  localDW->is_c3_CoreLogic = CoreLogic_IN_NO_ACTIVE_CHILD;
  localDW->InitFinished = 0U;
}

/* Output and update for referenced model: 'CoreLogic' */
void CoreLogic(const uint8_T *rtu_Acc, const real32_T *rtu_CC, const uint8_T
               *rtu_FaultLevel, const real32_T *rtu_Soc, const boolean_T
               *rtu_Trigger, DW_CoreLogic_f_T *localDW, ZCE_CoreLogic_T
               *localZCE)
{
  if ((*rtu_Trigger) && (localZCE->TmpModelReferenceSubsystem_Trig != POS_ZCSIG))
  {
    /* Chart: '<Root>/Chart' */
    /* Gateway: Chart */
    /* During: Chart */
    if (localDW->is_active_c3_CoreLogic == 0U) {
      /* Entry: Chart */
      localDW->is_active_c3_CoreLogic = 1U;

      /* Entry Internal: Chart */
      /* Transition: '<S1>:2' */
      localDW->is_c3_CoreLogic = CoreLogic_IN_Init;

      /* Entry 'Init': '<S1>:1' */
      localDW->InitFinished = 1U;
    } else {
      switch (localDW->is_c3_CoreLogic) {
       case CoreLogic_IN_ChargeMode:
        /* During 'ChargeMode': '<S1>:4' */
        if ((*rtu_FaultLevel < 3) || (*rtu_Soc == 100.0F)) {
          /* Transition: '<S1>:9' */
          localDW->is_c3_CoreLogic = CoreLogic_IN_WatiForSleep;
        }
        break;

       case CoreLogic_IN_Init:
        /* During 'Init': '<S1>:1' */
        if ((localDW->InitFinished == 1) && (*rtu_Acc == 1) && ((*rtu_CC >=
              11000.0F) || (*rtu_CC <= 9000.0F))) {
          /* Transition: '<S1>:6' */
          localDW->is_c3_CoreLogic = CoreLogic_IN_RunMode;
        } else if ((localDW->InitFinished == 1) && (*rtu_CC < 11000.0F) &&
                   (*rtu_CC > 9000.0F)) {
          /* Transition: '<S1>:7' */
          localDW->is_c3_CoreLogic = CoreLogic_IN_ChargeMode;
        } else {
          if (localDW->InitFinished == 2) {
            /* Transition: '<S1>:14' */
            localDW->is_c3_CoreLogic = CoreLogic_IN_WatiForSleep;
          }
        }
        break;

       case CoreLogic_IN_RunMode:
        /* During 'RunMode': '<S1>:3' */
        if ((*rtu_Acc == 0) && ((*rtu_CC >= 11000.0F) || (*rtu_CC <= 9000.0F)))
        {
          /* Transition: '<S1>:8' */
          localDW->is_c3_CoreLogic = CoreLogic_IN_WatiForSleep;
        } else {
          if ((*rtu_CC < 11000.0F) && (*rtu_CC > 9000.0F)) {
            /* Transition: '<S1>:13' */
            localDW->is_c3_CoreLogic = CoreLogic_IN_ChargeMode;
          }
        }
        break;

       default:
        /* During 'WatiForSleep': '<S1>:5' */
        if ((*rtu_Acc == 1) && ((*rtu_CC >= 11000.0F) || (*rtu_CC <= 9000.0F)))
        {
          /* Transition: '<S1>:11' */
          localDW->is_c3_CoreLogic = CoreLogic_IN_RunMode;
        }
        break;
      }
    }

    /* End of Chart: '<Root>/Chart' */
  }

  localZCE->TmpModelReferenceSubsystem_Trig = *rtu_Trigger;
}

/* Model initialize function */
void CoreLogic_initialize(const char_T **rt_errorStatus, RT_MODEL_CoreLogic_T *
  const CoreLogic_M, DW_CoreLogic_f_T *localDW, ZCE_CoreLogic_T *localZCE)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(CoreLogic_M, rt_errorStatus);

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(DW_CoreLogic_f_T));
  localZCE->TmpModelReferenceSubsystem_Trig = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
