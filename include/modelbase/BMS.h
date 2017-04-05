/*
 * File: BMS.h
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

#ifndef RTW_HEADER_BMS_h_
#define RTW_HEADER_BMS_h_
#include <stddef.h>
#include <string.h>
#ifndef BMS_COMMON_INCLUDES_
# define BMS_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#endif                                 /* BMS_COMMON_INCLUDES_ */

#include "BMS_types.h"

/* Shared type includes */
#include "model_reference_types.h"

/* Child system includes */
#include "Statistic.h"
#include "Schedule.h"
#include "FaultGenerator.h"
#include "CoreLogic.h"
#include "CoreAlgorithm.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetErrorStatusPointer
# define rtmGetErrorStatusPointer(rtm) ((const char_T **)(&((rtm)->errorStatus)))
#endif

/* Block signals (auto storage) */
typedef struct {
  real32_T Model1_o5;                  /* '<Root>/Model1' */
  real32_T Model1_o7;                  /* '<Root>/Model1' */
  real32_T Soc;                        /* '<Root>/Model' */
  uint8_T VCellMaxNum;                 /* '<Root>/Model1' */
  uint8_T VCellMinNum;                 /* '<Root>/Model1' */
  uint8_T Model1_o6;                   /* '<Root>/Model1' */
  uint8_T Model1_o8;                   /* '<Root>/Model1' */
  uint8_T FaultLevel;                  /* '<Root>/Model2' */
  boolean_T schedule_o1;               /* '<Root>/schedule' */
  boolean_T schedule_o2;               /* '<Root>/schedule' */
  boolean_T schedule_o3;               /* '<Root>/schedule' */
} B_BMS_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  MdlrefDW_Schedule_T schedule_DWORK1; /* '<Root>/schedule' */
  MdlrefDW_Statistic_T Model1_DWORK1;  /* '<Root>/Model1' */
  MdlrefDW_CoreAlgorithm_T Model_DWORK1;/* '<Root>/Model' */
  MdlrefDW_FaultGenerator_T Model2_DWORK1;/* '<Root>/Model2' */
  MdlrefDW_CoreLogic_T Model3_DWORK1;  /* '<Root>/Model3' */
} DW_BMS_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T TCell;                      /* '<Root>/In2' */
  real32_T IBat;                       /* '<Root>/In3' */
  uint8_T Acc;                         /* '<Root>/In4' */
  real32_T CC;                         /* '<Root>/In5' */
  real_T VfIF_CC_V;                    /* '<Root>/VfIF_CC_V' */
  real_T VfIF_CP_V;                    /* '<Root>/VfIF_CP_V' */
  real_T VfIF_CC2_V;                   /* '<Root>/VfIF_CC2_V' */
  real_T VfIF_PumbStat_V;              /* '<Root>/VfIF_PumbStat_V' */
  real_T VfIF_AcChargeTemp_C;          /* '<Root>/VfIF_AcChargeTemp_C' */
  real_T VfIF_DcChargeTemp_C;          /* '<Root>/VfIF_DcChargeTemp_C' */
  real_T VfIF_IHI_A;                   /* '<Root>/VfIF_IHI_A' */
  real_T VfIF_ILO_A;                   /* '<Root>/VfIF_ILO_A' */
  real_T VfIF_TAMBIN_C;                /* '<Root>/VfIF_TAMBIN_C' */
  real_T VfIF_GNDFLT_O;                /* '<Root>/VfIF_GNDFLT_O' */
  real_T VfIF_VMain_V;                 /* '<Root>/VfIF_VMain_V' */
  real_T VfIF_VBat_V;                  /* '<Root>/VfIF_VBat_V' */
  real_T VfIF_PCTemp_C;                /* '<Root>/VfIF_PCTemp_C' */
  real_T VfIF_TOnBoarder_C;            /* '<Root>/VfIF_TOnBoarder_C' */
  real_T VbIF_HVILFB_Enum;             /* '<Root>/VbIF_HVILFB_Enum' */
  real_T VbIF_PosCntRB_Enum;           /* '<Root>/VbIF_PosCntRB_Enum' */
  real_T VbIF_NegCntRB_Enum;           /* '<Root>/VbIF_NegCntRB_Enum' */
  real_T VbIF_PCCntRB_Enum;            /* '<Root>/VbIF_PCCntRB_Enum' */
  real_T VbIF_PosCntCtrl_Enum;         /* '<Root>/VbIF_PosCntCtrl_Enum' */
  real_T VbIF_NegCntCtrl_Enum;         /* '<Root>/VbIF_NegCntCtrl_Enum' */
  real_T VbIF_PCCntCtrl_Enum;          /* '<Root>/VbIF_PCCntCtrl_Enum' */
  real_T VbIF_Acc_Enum;                /* '<Root>/VbIF_Acc_Enum' */
  real_T VbIF_LeadBatLow_Enum;         /* '<Root>/VbIF_LeadBatLow_Enum' */
  real_T VbIF_Collision_Enum;          /* '<Root>/VbIF_Collision_Enum' */
  real_T VbIF_LockStatus_Enum;         /* '<Root>/VbIF_LockStatus_Enum' */
  real_T VbIF_VcuHVCmd_Enum;           /* '<Root>/VbIF_VcuHVCmd_Enum' */
  real32_T VaIF_VCell_V[96];           /* '<Root>/VaIF_VCell_V' */
  real32_T VaIF_TCell_C[96];           /* '<Root>/VaIF_TCell_C' */
  real_T VaIF_BalStat_Enum;            /* '<Root>/VaIF_BalStat_Enum' */
} ExtU_BMS_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  uint8_T Out3;                        /* '<Root>/Out3' */
  uint8_T Out4;                        /* '<Root>/Out4' */
  uint8_T Out5;                        /* '<Root>/Out5' */
  uint8_T Out6;                        /* '<Root>/Out6' */
  real_T VbIF_FeedDog_Enum;            /* '<Root>/VbIF_FeedDog_Enum' */
  real_T VbIF_LedCmd_Enum;             /* '<Root>/VbIF_LedCmd_Enum' */
  real_T VbIF_NegCntCmd_Enum;          /* '<Root>/VbIF_NegCntCmd_Enum' */
  real_T VbIF_PCCntCmd_Enum;           /* '<Root>/VbIF_PCCntCmd_Enum' */
  real_T VbIF_POSCntCmd_Enum;          /* '<Root>/VbIF_POSCntCmd_Enum' */
  real_T VbIF_S2Cmd_Enum;              /* '<Root>/VbIF_S2Cmd_Enum' */
  real_T VbIF_ChargeWakeup_Enum;       /* '<Root>/VbIF_ChargeWakeup_Enum' */
  real_T VbIF_PumbPulse_Enum;          /* '<Root>/VbIF_PumbPulse_Enum' */
  real_T VbIF_LockCmd_Enum;            /* '<Root>/VbIF_LockCmd_Enum' */
  real_T VaIF_BalCmd_Enum;             /* '<Root>/VaIF_BalCmd_Enum' */
  real_T VuIF_InitFlag_Enum;           /* '<Root>/VuIF_InitFlag_Enum' */
  real_T VuIF_CarStat_Enum;            /* '<Root>/VuIF_CarStat_Enum' */
  real_T VuIF_HVStat_Enum;             /* '<Root>/VuIF_HVStat_Enum' */
  real_T VuIF_PreChStat_Enum;          /* '<Root>/VuIF_PreChStat_Enum' */
  real_T VuIF_PosRelayStat_Enum;       /* '<Root>/VuIF_PosRelayStat_Enum' */
  real_T VuIF_NegRelayStat_Enum;       /* '<Root>/VuIF_NegRelayStat_Enum' */
  real_T VuIF_PCRelayStat_Enum;        /* '<Root>/VuIF_PCRelayStat_Enum' */
  real_T VuIF_HVIL_Enum;               /* '<Root>/VuIF_HVIL_Enum' */
  real_T VuIF_FaultRank_Enum;          /* '<Root>/VuIF_FaultRank_Enum' */
  real_T VfOUT_VBat_V;                 /* '<Root>/VfOUT_VBat_V' */
  real_T VfOUT_VOut_V;                 /* '<Root>/VfOUT_VOut_V' */
  real_T VfOUT_VCellAvg_V;             /* '<Root>/VfOUT_VCellAvg_V' */
  real32_T VfOUT_VCellMax_V;           /* '<Root>/VfOUT_VCellMax_V' */
  real32_T VfOUT_VCellMin_V;           /* '<Root>/VfOUT_VCellMin_V' */
  real_T VuOUT_VCellMaxNum_n;          /* '<Root>/VuOUT_VCellMaxNum_n' */
  real_T VuOUT_VCellMinNum_n;          /* '<Root>/VuOUT_VCellMinNum_n' */
  real_T VfOUT_InletTemp_C;            /* '<Root>/VfOUT_InletTemp_C' */
  real_T VfOUT_OutletTemp_C;           /* '<Root>/VfOUT_OutletTemp_C' */
  real_T VfOUT_ACChTemp_C;             /* '<Root>/VfOUT_ACChTemp_C' */
  real_T VfOUT_DCChTemp_C;             /* '<Root>/VfOUT_DCChTemp_C' */
  real_T VfOUT_TCellAVG_C;             /* '<Root>/VfOUT_TCellAVG_C' */
  real_T VfOUT_TCellMax_C;             /* '<Root>/VfOUT_TCellMax_C' */
  real_T VfOUT_TCellMin_C;             /* '<Root>/VfOUT_TCellMin_C' */
  real_T VuOUT_TCellMaxNum_n;          /* '<Root>/VuOUT_TCellMaxNum_n' */
  real_T VuOUT_TCellMinNum_n;          /* '<Root>/VuOUT_TCellMinNum_n' */
  real_T VfOUT_SOC_h;                  /* '<Root>/VfOUT_SOC_h' */
  real_T VfOUT_Energy_Wh;              /* '<Root>/VfOUT_Energy_Wh' */
  real_T VfOUT_DischPowerLimit_W;      /* '<Root>/VfOUT_DischPowerLimit_W' */
  real_T VfOUT_ChargePowerLimit_W;     /* '<Root>/VfOUT_ChargePowerLimit_W' */
  real_T VfOUT_IBat_A;                 /* '<Root>/VfOUT_IBat_A' */
  real_T VuOUT_ChWakeup_Enum;          /* '<Root>/VuOUT_ChWakeup_Enum' */
  real_T VuOUT_CarChStat_Enum;         /* '<Root>/VuOUT_CarChStat_Enum' */
  real_T VuOUT_ChMode_Enum;            /* '<Root>/VuOUT_ChMode_Enum' */
  real_T VuOUT_ChGunConnStat_Enum;     /* '<Root>/VuOUT_ChGunConnStat_Enum' */
  real_T VuOUT_ChPowerSupplyStat_Enum; /* '<Root>/VuOUT_ChPowerSupplyStat_Enum' */
  real_T VuOUT_S2Stat_Enum;            /* '<Root>/VuOUT_S2Stat_Enum' */
  real_T VuOUT_ElecLockStat_Enum;      /* '<Root>/VuOUT_ElecLockStat_Enum' */
  real_T VuOUT_DCChargeConCmd_Enum;    /* '<Root>/VuOUT_DCChargeConCmd_Enum' */
  real_T VuOUT_HVOnRequest_Enum;       /* '<Root>/VuOUT_HVOnRequest_Enum' */
  real_T VfOUT_ChVSet_V;               /* '<Root>/VfOUT_ChVSet_V' */
  real_T VfOUT_ChISet_A;               /* '<Root>/VfOUT_ChISet_A' */
  real_T VuOUT_ChargerCtrl_Enum;       /* '<Root>/VuOUT_ChargerCtrl_Enum' */
  real_T ViOUT_ChRemainTime_V;         /* '<Root>/ViOUT_ChRemainTime_V' */
  real_T VuOUT_ChStat_Enum;            /* '<Root>/VuOUT_ChStat_Enum' */
  real_T VuOUT_ChCompFlag_Enum;        /* '<Root>/VuOUT_ChCompFlag_Enum' */
  real_T VuOUT_HeatRequest_Enum;       /* '<Root>/VuOUT_HeatRequest_Enum' */
  real_T VuOUT_HeatPreserve_Enum;      /* '<Root>/VuOUT_HeatPreserve_Enum' */
  real_T VuOUT_PumbStat_Enum;          /* '<Root>/VuOUT_PumbStat_Enum' */
  real_T VfOUT_CmpPwrReq_W;            /* '<Root>/VfOUT_CmpPwrReq_W' */
  real_T VbOUT_CmpPwrReqValid_Enum;    /* '<Root>/VbOUT_CmpPwrReqValid_Enum' */
  real_T VfOUT_PTCPwrReq_W;            /* '<Root>/VfOUT_PTCPwrReq_W' */
  real_T VbOUT_PTCPwrReqValid_Enum;    /* '<Root>/VbOUT_PTCPwrReqValid_Enum' */
  real_T VfOUT_PTCPwr_W;               /* '<Root>/VfOUT_PTCPwr_W' */
  real_T VbOUT_PTCPwrValid_Enum;       /* '<Root>/VbOUT_PTCPwrValid_Enum' */
} ExtY_BMS_T;

/* Real-time Model Data Structure */
struct tag_RTM_BMS_T {
  const char_T * volatile errorStatus;
};

/* Block signals (auto storage) */
extern B_BMS_T BMS_B;

/* Block states (auto storage) */
extern DW_BMS_T BMS_DW;

/* External inputs (root inport signals with auto storage) */
extern ExtU_BMS_T BMS_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_BMS_T BMS_Y;

/* Model entry point functions */
extern void BMS_initialize(void);
extern void BMS_step(void);
extern void BMS_terminate(void);

/* Real-time Model object */
extern RT_MODEL_BMS_T *const BMS_M;

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
 * '<Root>' : 'BMS'
 * '<S1>'   : 'BMS/BMUCmd'
 * '<S2>'   : 'BMS/Charging'
 * '<S3>'   : 'BMS/IOCmd'
 * '<S4>'   : 'BMS/SocPower'
 * '<S5>'   : 'BMS/Subsystem'
 * '<S6>'   : 'BMS/SysStat'
 * '<S7>'   : 'BMS/Temp'
 * '<S8>'   : 'BMS/ThermalManagement'
 * '<S9>'   : 'BMS/Voltage'
 */
#endif                                 /* RTW_HEADER_BMS_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
