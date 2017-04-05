/*
 * File: BMS.c
 *
 * Code generated for Simulink model 'BMS'.
 *
 * Model version                  : 1.63
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Wed Apr 05 14:38:43 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "BMS.h"
#include "BMS_private.h"

/* Block signals (auto storage) */
B_BMS_T BMS_B;

/* Block states (auto storage) */
DW_BMS_T BMS_DW;

/* External inputs (root inport signals with auto storage) */
ExtU_BMS_T BMS_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_BMS_T BMS_Y;

/* Real-time model */
RT_MODEL_BMS_T BMS_M_;
RT_MODEL_BMS_T *const BMS_M = &BMS_M_;

/* Model step function */
void BMS_step(void)
{
  /* ModelReference: '<Root>/schedule' */
  Schedule(&BMS_B.schedule_o1, &BMS_B.schedule_o2, &BMS_B.schedule_o3,
           &(BMS_DW.schedule_DWORK1.rtdw));

  /* ModelReference: '<Root>/Model1' */
  Statistic(&BMS_U.VaIF_VCell_V[0], &BMS_U.VaIF_TCell_C[0], &BMS_B.schedule_o1,
            &BMS_Y.VfOUT_VCellMax_V, &BMS_B.VCellMaxNum, &BMS_Y.VfOUT_VCellMin_V,
            &BMS_B.VCellMinNum, &BMS_B.Model1_o5, &BMS_B.Model1_o6,
            &BMS_B.Model1_o7, &BMS_B.Model1_o8, &(BMS_DW.Model1_DWORK1.rtzce));

  /* ModelReference: '<Root>/Model' */
  CoreAlgorithm(&BMS_Y.VfOUT_VCellMin_V, &BMS_U.TCell, &BMS_U.IBat,
                &BMS_B.schedule_o2, &BMS_B.Soc, &(BMS_DW.Model_DWORK1.rtb),
                &(BMS_DW.Model_DWORK1.rtdw), &(BMS_DW.Model_DWORK1.rtzce));

  /* ModelReference: '<Root>/Model2' */
  FaultGenerator(&BMS_Y.VfOUT_VCellMax_V, &BMS_Y.VfOUT_VCellMin_V, &BMS_B.Soc,
                 &BMS_B.schedule_o3, &BMS_Y.Out3, &BMS_Y.Out4, &BMS_Y.Out5,
                 &BMS_Y.Out6, &BMS_B.FaultLevel, &(BMS_DW.Model2_DWORK1.rtb),
                 &(BMS_DW.Model2_DWORK1.rtdw), &(BMS_DW.Model2_DWORK1.rtzce));

  /* ModelReference: '<Root>/Model3' */
  CoreLogic(&BMS_U.Acc, &BMS_U.CC, &BMS_B.FaultLevel, &BMS_B.Soc,
            &BMS_B.schedule_o3, &(BMS_DW.Model3_DWORK1.rtdw),
            &(BMS_DW.Model3_DWORK1.rtzce));
}

/* Model initialize function */
void BMS_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(BMS_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &BMS_B), 0,
                sizeof(B_BMS_T));

  /* states (dwork) */
  (void) memset((void *)&BMS_DW, 0,
                sizeof(DW_BMS_T));

  /* external inputs */
  (void) memset((void *)&BMS_U, 0,
                sizeof(ExtU_BMS_T));

  /* external outputs */
  (void) memset((void *)&BMS_Y, 0,
                sizeof(ExtY_BMS_T));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Model' */
  CoreAlgorithm_initialize(rtmGetErrorStatusPointer(BMS_M),
    &(BMS_DW.Model_DWORK1.rtm), &(BMS_DW.Model_DWORK1.rtb),
    &(BMS_DW.Model_DWORK1.rtdw), &(BMS_DW.Model_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Model1' */
  Statistic_initialize(rtmGetErrorStatusPointer(BMS_M),
                       &(BMS_DW.Model1_DWORK1.rtm), &(BMS_DW.Model1_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Model2' */
  FaultGenerator_initialize(rtmGetErrorStatusPointer(BMS_M),
    &(BMS_DW.Model2_DWORK1.rtm), &(BMS_DW.Model2_DWORK1.rtb),
    &(BMS_DW.Model2_DWORK1.rtdw), &(BMS_DW.Model2_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Model3' */
  CoreLogic_initialize(rtmGetErrorStatusPointer(BMS_M),
                       &(BMS_DW.Model3_DWORK1.rtm), &(BMS_DW.Model3_DWORK1.rtdw),
                       &(BMS_DW.Model3_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<Root>/schedule' */
  Schedule_initialize(rtmGetErrorStatusPointer(BMS_M),
                      &(BMS_DW.schedule_DWORK1.rtm),
                      &(BMS_DW.schedule_DWORK1.rtdw));

  /* ConstCode for Outport: '<Root>/VfOUT_VBat_V' */
  BMS_Y.VfOUT_VBat_V = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_VOut_V' */
  BMS_Y.VfOUT_VOut_V = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_VCellAvg_V' */
  BMS_Y.VfOUT_VCellAvg_V = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_VCellMaxNum_n' */
  BMS_Y.VuOUT_VCellMaxNum_n = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_VCellMinNum_n' */
  BMS_Y.VuOUT_VCellMinNum_n = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_FeedDog_Enum' */
  BMS_Y.VbIF_FeedDog_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_LedCmd_Enum' */
  BMS_Y.VbIF_LedCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_NegCntCmd_Enum' */
  BMS_Y.VbIF_NegCntCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_PCCntCmd_Enum' */
  BMS_Y.VbIF_PCCntCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_POSCntCmd_Enum' */
  BMS_Y.VbIF_POSCntCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_S2Cmd_Enum' */
  BMS_Y.VbIF_S2Cmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_ChargeWakeup_Enum' */
  BMS_Y.VbIF_ChargeWakeup_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_PumbPulse_Enum' */
  BMS_Y.VbIF_PumbPulse_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VbIF_LockCmd_Enum' */
  BMS_Y.VbIF_LockCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VaIF_BalCmd_Enum' */
  BMS_Y.VaIF_BalCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_InitFlag_Enum' */
  BMS_Y.VuIF_InitFlag_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_CarStat_Enum' */
  BMS_Y.VuIF_CarStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_HVStat_Enum' */
  BMS_Y.VuIF_HVStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_PreChStat_Enum' */
  BMS_Y.VuIF_PreChStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_PosRelayStat_Enum' */
  BMS_Y.VuIF_PosRelayStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_NegRelayStat_Enum' */
  BMS_Y.VuIF_NegRelayStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_PCRelayStat_Enum' */
  BMS_Y.VuIF_PCRelayStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_HVIL_Enum' */
  BMS_Y.VuIF_HVIL_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuIF_FaultRank_Enum' */
  BMS_Y.VuIF_FaultRank_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_InletTemp_C' */
  BMS_Y.VfOUT_InletTemp_C = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_OutletTemp_C' */
  BMS_Y.VfOUT_OutletTemp_C = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_ACChTemp_C' */
  BMS_Y.VfOUT_ACChTemp_C = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_DCChTemp_C' */
  BMS_Y.VfOUT_DCChTemp_C = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_TCellAVG_C' */
  BMS_Y.VfOUT_TCellAVG_C = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_TCellMax_C' */
  BMS_Y.VfOUT_TCellMax_C = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_TCellMin_C' */
  BMS_Y.VfOUT_TCellMin_C = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_TCellMaxNum_n' */
  BMS_Y.VuOUT_TCellMaxNum_n = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_TCellMinNum_n' */
  BMS_Y.VuOUT_TCellMinNum_n = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_SOC_h' */
  BMS_Y.VfOUT_SOC_h = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_Energy_Wh' */
  BMS_Y.VfOUT_Energy_Wh = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_DischPowerLimit_W' */
  BMS_Y.VfOUT_DischPowerLimit_W = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_ChargePowerLimit_W' */
  BMS_Y.VfOUT_ChargePowerLimit_W = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_IBat_A' */
  BMS_Y.VfOUT_IBat_A = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChWakeup_Enum' */
  BMS_Y.VuOUT_ChWakeup_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_CarChStat_Enum' */
  BMS_Y.VuOUT_CarChStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChMode_Enum' */
  BMS_Y.VuOUT_ChMode_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChGunConnStat_Enum' */
  BMS_Y.VuOUT_ChGunConnStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChPowerSupplyStat_Enum' */
  BMS_Y.VuOUT_ChPowerSupplyStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_S2Stat_Enum' */
  BMS_Y.VuOUT_S2Stat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ElecLockStat_Enum' */
  BMS_Y.VuOUT_ElecLockStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_DCChargeConCmd_Enum' */
  BMS_Y.VuOUT_DCChargeConCmd_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_HVOnRequest_Enum' */
  BMS_Y.VuOUT_HVOnRequest_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_ChVSet_V' */
  BMS_Y.VfOUT_ChVSet_V = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_ChISet_A' */
  BMS_Y.VfOUT_ChISet_A = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChargerCtrl_Enum' */
  BMS_Y.VuOUT_ChargerCtrl_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/ViOUT_ChRemainTime_V' */
  BMS_Y.ViOUT_ChRemainTime_V = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChStat_Enum' */
  BMS_Y.VuOUT_ChStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_ChCompFlag_Enum' */
  BMS_Y.VuOUT_ChCompFlag_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_HeatRequest_Enum' */
  BMS_Y.VuOUT_HeatRequest_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_HeatPreserve_Enum' */
  BMS_Y.VuOUT_HeatPreserve_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VuOUT_PumbStat_Enum' */
  BMS_Y.VuOUT_PumbStat_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_CmpPwrReq_W' */
  BMS_Y.VfOUT_CmpPwrReq_W = 0.0;

  /* ConstCode for Outport: '<Root>/VbOUT_CmpPwrReqValid_Enum' */
  BMS_Y.VbOUT_CmpPwrReqValid_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_PTCPwrReq_W' */
  BMS_Y.VfOUT_PTCPwrReq_W = 0.0;

  /* ConstCode for Outport: '<Root>/VbOUT_PTCPwrReqValid_Enum' */
  BMS_Y.VbOUT_PTCPwrReqValid_Enum = 0.0;

  /* ConstCode for Outport: '<Root>/VfOUT_PTCPwr_W' */
  BMS_Y.VfOUT_PTCPwr_W = 0.0;

  /* ConstCode for Outport: '<Root>/VbOUT_PTCPwrValid_Enum' */
  BMS_Y.VbOUT_PTCPwrValid_Enum = 0.0;

  /* SystemInitialize for ModelReference: '<Root>/Model' */
  CoreAlgorithm_Init(&BMS_B.Soc, &(BMS_DW.Model_DWORK1.rtb),
                     &(BMS_DW.Model_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<Root>/Model2' */
  FaultGenerator_Init(&(BMS_DW.Model2_DWORK1.rtb), &(BMS_DW.Model2_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<Root>/Model3' */
  CoreLogic_Init(&(BMS_DW.Model3_DWORK1.rtdw));
}

/* Model terminate function */
void BMS_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
