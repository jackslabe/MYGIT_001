/*
 * File: CoreAlgorithm.c
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

#include "CoreAlgorithm.h"
#include "CoreAlgorithm_private.h"

/* Named constants for Chart: '<Root>/SocCalcControl' */
#define CoreAlgorith_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define CoreAlgorithm_IN_CheckingData  ((uint8_T)1U)
#define CoreAlgorithm_IN_InitSoc       ((uint8_T)2U)
#define CoreAlgorithm_IN_NormalCalcSoc ((uint8_T)3U)
#define CoreAlgorithm_IN_SocCalcControl ((uint8_T)1U)

/* Forward declaration for local functions */
static boolean_T CoreAlgorithm_CheckData(real_T VC, real_T TC, real_T IBat);

/* Function for Chart: '<Root>/SocCalcControl' */
static boolean_T CoreAlgorithm_CheckData(real_T VC, real_T TC, real_T IBat)
{
  /* Graphical Function 'CheckData': '<S4>:19' */
  /* Transition: '<S4>:21' */
  return (VC > 2.0) && (VC < 5.0) && (TC > -50.0) && (TC < 100.0) && (IBat >
    -2.0) && (IBat < 2.0);
}

/* System initialize for referenced model: 'CoreAlgorithm' */
void CoreAlgorithm_Init(real32_T *rty_Soc, B_CoreAlgorithm_c_T *localB,
  DW_CoreAlgorithm_f_T *localDW)
{
  /* SystemInitialize for Chart: '<Root>/SocCalcControl' */
  localDW->is_SocCalcControl = CoreAlgorith_IN_NO_ACTIVE_CHILD;
  localDW->is_active_c3_CoreAlgorithm = 0U;
  localDW->is_c3_CoreAlgorithm = CoreAlgorith_IN_NO_ACTIVE_CHILD;
  localB->CheckingDataStatus = false;
  localB->InitSocStatus = false;
  localB->NormalCalcSocStatus = false;

  /* SystemInitialize for Merge: '<Root>/Merge1' */
  *rty_Soc = 0.0F;
}

/* Output and update for referenced model: 'CoreAlgorithm' */
void CoreAlgorithm(const real32_T *rtu_VC, const real32_T *rtu_TC, const
                   real32_T *rtu_IBat, const boolean_T *rtu_Trigger, real32_T
                   *rty_Soc, B_CoreAlgorithm_c_T *localB, DW_CoreAlgorithm_f_T
                   *localDW, ZCE_CoreAlgorithm_T *localZCE)
{
  if ((*rtu_Trigger) && (localZCE->TmpModelReferenceSubsystem_Trig != POS_ZCSIG))
  {
    /* Chart: '<Root>/SocCalcControl' */
    /* Gateway: SocCalcControl */
    /* During: SocCalcControl */
    if (localDW->is_active_c3_CoreAlgorithm == 0U) {
      /* Entry: SocCalcControl */
      localDW->is_active_c3_CoreAlgorithm = 1U;

      /* Entry Internal: SocCalcControl */
      /* Transition: '<S4>:2' */
      localDW->is_c3_CoreAlgorithm = CoreAlgorithm_IN_SocCalcControl;

      /* Entry Internal 'SocCalcControl': '<S4>:1' */
      /* Transition: '<S4>:4' */
      localDW->is_SocCalcControl = CoreAlgorithm_IN_CheckingData;

      /* Entry 'CheckingData': '<S4>:3' */
      localB->CheckingDataStatus = true;
    } else {
      /* During 'SocCalcControl': '<S4>:1' */
      switch (localDW->is_SocCalcControl) {
       case CoreAlgorithm_IN_CheckingData:
        /* During 'CheckingData': '<S4>:3' */
        if (CoreAlgorithm_CheckData((real_T)*rtu_VC, (real_T)*rtu_TC, (real_T)
             *rtu_IBat)) {
          /* Transition: '<S4>:7' */
          /* Exit 'CheckingData': '<S4>:3' */
          localB->CheckingDataStatus = false;
          localDW->is_SocCalcControl = CoreAlgorithm_IN_InitSoc;

          /* Entry 'InitSoc': '<S4>:5' */
          localB->InitSocStatus = true;
        }
        break;

       case CoreAlgorithm_IN_InitSoc:
        /* During 'InitSoc': '<S4>:5' */
        /* Transition: '<S4>:8' */
        /* Exit 'InitSoc': '<S4>:5' */
        localB->InitSocStatus = false;
        localDW->is_SocCalcControl = CoreAlgorithm_IN_NormalCalcSoc;

        /* Entry 'NormalCalcSoc': '<S4>:6' */
        localB->NormalCalcSocStatus = true;
        break;

       default:
        /* During 'NormalCalcSoc': '<S4>:6' */
        break;
      }
    }

    /* End of Chart: '<Root>/SocCalcControl' */

    /* Outputs for Enabled SubSystem: '<Root>/CheckingData' incorporates:
     *  EnablePort: '<S1>/Enable'
     */
    if (localB->CheckingDataStatus) {
      /* SignalConversion: '<S1>/OutportBufferForSoc' incorporates:
       *  Constant: '<S1>/Constant'
       */
      *rty_Soc = 0.01F;
    }

    /* End of Outputs for SubSystem: '<Root>/CheckingData' */

    /* Outputs for Enabled SubSystem: '<Root>/InitSoc' incorporates:
     *  EnablePort: '<S2>/Enable'
     */
    if (localB->InitSocStatus) {
      /* Lookup_n-D: '<S2>/1-D Lookup Table' */
      *rty_Soc = look1_iflf_binlxpw(*rtu_VC, rtCP_uDLookupTable_bp01Data,
        rtCP_uDLookupTable_tableData, 20U);
    }

    /* End of Outputs for SubSystem: '<Root>/InitSoc' */

    /* Outputs for Enabled SubSystem: '<Root>/NormalCalcSoc' incorporates:
     *  EnablePort: '<S3>/Enable'
     */
    if (localB->NormalCalcSocStatus) {
      /* Sum: '<S3>/Add' incorporates:
       *  Constant: '<S3>/Constant'
       *  Constant: '<S3>/Constant1'
       *  Memory: '<Root>/Memory1'
       *  Product: '<S3>/Divide'
       *  Product: '<S3>/Product'
       */
      *rty_Soc = *rtu_IBat * 0.1F / 3600.0F + localDW->Memory1_PreviousInput;
    }

    /* End of Outputs for SubSystem: '<Root>/NormalCalcSoc' */

    /* Update for Memory: '<Root>/Memory1' */
    localDW->Memory1_PreviousInput = *rty_Soc;
  }

  localZCE->TmpModelReferenceSubsystem_Trig = *rtu_Trigger;
}

/* Model initialize function */
void CoreAlgorithm_initialize(const char_T **rt_errorStatus,
  RT_MODEL_CoreAlgorithm_T *const CoreAlgorithm_M, B_CoreAlgorithm_c_T *localB,
  DW_CoreAlgorithm_f_T *localDW, ZCE_CoreAlgorithm_T *localZCE)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(CoreAlgorithm_M, rt_errorStatus);

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(B_CoreAlgorithm_c_T));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(DW_CoreAlgorithm_f_T));
  localZCE->TmpModelReferenceSubsystem_Trig = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
