/*
 * File: CoreAlgorithm_private.h
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

#ifndef RTW_HEADER_CoreAlgorithm_private_h_
#define RTW_HEADER_CoreAlgorithm_private_h_
#include "rtwtypes.h"
#include "model_reference_types.h"
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        (*((rtm)->errorStatus))
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   (*((rtm)->errorStatus) = (val))
#endif

#ifndef rtmGetErrorStatusPointer
# define rtmGetErrorStatusPointer(rtm) (rtm)->errorStatus
#endif

#ifndef rtmSetErrorStatusPointer
# define rtmSetErrorStatusPointer(rtm, val) ((rtm)->errorStatus = (val))
#endif

extern const real32_T rtCP_pooled_NcHUOfNrmMHw[21];
extern const real32_T rtCP_pooled_E3DbjmYX12l9[21];

#define rtCP_uDLookupTable_tableData   rtCP_pooled_NcHUOfNrmMHw  /* Computed Parameter: uDLookupTable_tableData
                                                                  * Referenced by: '<S2>/1-D Lookup Table'
                                                                  */
#define rtCP_uDLookupTable_bp01Data    rtCP_pooled_E3DbjmYX12l9  /* Computed Parameter: uDLookupTable_bp01Data
                                                                  * Referenced by: '<S2>/1-D Lookup Table'
                                                                  */
#endif                                 /* RTW_HEADER_CoreAlgorithm_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
