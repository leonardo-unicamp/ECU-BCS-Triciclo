/*
 * IIRFilter.c
 *
 *  Created on: Apr 19, 2023
 *      Author: rodri
 */

// Como instalar a biblioteca DSP CMSIS: https://www.youtube.com/watch?v=lwsWaMeP1co&ab_channel=AbhayKant e https://www.youtube.com/watch?v=Su_UtzwF-40&ab_channel=AbhayKant
// Git com o programa para gerar os coeficientes para o filtro no MATLAB: https://github.com/mattgaidica/iir-designer-cmsis-dsp e https://github.com/matteoscordino/iir-designer-cmsis-dsp?ref=hackernoon.com
// Como gerar os coeficientes par ao filtro: https://www.youtube.com/watch?v=pCEDs9oZioI&ab_channel=WRKits

#include "main.h"

#ifdef USE_FILTER

#include "arm_math.h"

#if FILTER == LP_IIR_46_00_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.276222572493110,
		   0.552445144986220,
		   0.276222572493110,
		  -1.552848223665588,
		  -0.603216208979000,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.564778232621796,
		  -0.615533178171835,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.588720383470928,
		  -0.640251913547288,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.624812548896307,
		  -0.677514759809580,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.673191581059460,
		  -0.727463008039278,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.733898611548899,
		  -0.790139123963805,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.806736457066063,
		  -0.865339528472479,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.891069846550162,
		  -0.952408344934071
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_20_00_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000005282596986,
		   0.000010565193972,
		   0.000005282596986,
		   0.317514162458231,
		  -0.027497413533412,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.323560328699773,
		  -0.047063218495103,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.336115104066049,
		  -0.087691325022073,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.356179295225357,
		  -0.152620411527152,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.385465651846880,
		  -0.247393052367809,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.426723294199009,
		  -0.380905587610602,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.484323378532003,
		  -0.567303376021949,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.565333674093522,
		  -0.829458199336335
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_13_00_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000019957074,
		   0.000000039914148,
		   0.000000019957074,
		   0.793466928417235,
		  -0.159112238654322,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.806497855044461,
		  -0.178148074923504,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.833343539244408,
		  -0.217364783266220,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.875659666510940,
		  -0.279181021915225,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.936163006667645,
		  -0.367565502154306,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.018949200750709,
		  -0.488501217704150,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.129980764545710,
		  -0.650698329975006,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.277794014718995,
		  -0.866626859791453
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};



#elif FILTER == LP_IIR_4_00_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000001,
		   0.000000000000002,
		   0.000000000000001,
		   1.552848223664983,
		  -0.603216208978533,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.564778232622862,
		  -0.615533178172665,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.588720383470407,
		  -0.640251913546843,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.624812548896299,
		  -0.677514759809620,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.673191581059539,
		  -0.727463008039338,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.733898611548889,
		  -0.790139123963788,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.806736457066056,
		  -0.865339528472474,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.891069846550164,
		  -0.952408344934073
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_2_00_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.764183312416838,
		  -0.778204989886220,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.771733962296317,
		  -0.785815652110416,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.786734421775395,
		  -0.800935334803656,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.808969393363800,
		  -0.823347029297187,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.838082435789931,
		  -0.852691461334655,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.873537655427669,
		  -0.888428478023130,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.914572807345349,
		  -0.929789776130405,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.960149349816784,
		  -0.975728559630990
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_1_00_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.878659471890570,
		  -0.882373906808250,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.882915147075980,
		  -0.886637996199976,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.891319207660280,
		  -0.895058673066645,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.903654397562344,
		  -0.907418251773962,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.919588780251131,
		  -0.923384139495106,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.938670278355744,
		  -0.942503365023932,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.960322413814840,
		  -0.964198310502190,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.983843796643946,
		  -0.987766199176023
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_50_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.938419094154701,
		  -0.939376059168081,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.940679737690847,
		  -0.941637818745968,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.945129500510943,
		  -0.946089778339422,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.951625992289244,
		  -0.952589477326818,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.959957489591898,
		  -0.960925087750026,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.969845753861759,
		  -0.970818233690200,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.980950689405064,
		  -0.981928651554033,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.992877463983096,
		  -0.993861314180747
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_10_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.987532700531277,
		  -0.987571933499607,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.988007440311216,
		  -0.988046682650693,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.988939334952178,
		  -0.988978595686821,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.990293792211640,
		  -0.990333079682633,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.992020359762798,
		  -0.992059681315430,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.994054424511429,
		  -0.994093786215551,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.996319433862042,
		  -0.996358840276392,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.998729592492542,
		  -0.998769046482299
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_01_HZ_16_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.998749801028347,
		  -0.998750195565808,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.998797801441305,
		  -0.998798195988242,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.998891964335566,
		  -0.998892358901090,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999028683460513,
		  -0.999029078053023,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999202720952136,
		  -0.999203115579000,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999407406163460,
		  -0.999407800830728,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999634889349000,
		  -0.999635284061171,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999876440862259,
		  -0.999876835622111
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_46_00_HZ_8_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.523959406756823,
		   1.047918813513646,
		   0.523959406756823,
		  -1.557318593822620,
		  -0.607831579487683,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.605238235461430,
		  -0.657305536461054,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.702008790735731,
		  -0.757214929023218,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.847529740023345,
		  -0.907455977110453
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_20_00_HZ_8_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.002271840012451,
		   0.004543680024903,
		   0.002271840012451,
		   0.319763902271565,
		  -0.034777724501378,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.345121039357245,
		  -0.116835143825428,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.404372288519084,
		  -0.308576213864914,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.521309265637001,
		  -0.686992220901832
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_13_00_HZ_8_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000140011900002,
		   0.000280023800004,
		   0.000140011900002,
		   0.798323491502031,
		  -0.166206802406954,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.852425877662784,
		  -0.245240641995475,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.974448946408126,
		  -0.423494362869510,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.198631205477071,
		  -0.750984256738550
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_4_00_HZ_8_TAPS

#define IIR_ORDER     8
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000033988541,
		   0.000000067977081,
		   0.000000033988541,
		   1.557318593822618,
		  -0.607831579487681,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.605238235461429,
		  -0.657305536461054,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.702008790735729,
		  -0.757214929023216,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.847529740023346,
		  -0.907455977110455
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_2_00_HZ_8_TAPS

#define IIR_ORDER     8
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000177838,
		   0.000000000355676,
		   0.000000000177838,
		   1.767018651463266,
		  -0.781062864124579,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.796966165642348,
		  -0.811248400272168,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.855059020399815,
		  -0.869802975343477,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.936870401643022,
		  -0.952264591056673
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_1_00_HZ_8_TAPS

#define IIR_ORDER     8
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000810,
		   0.000000000001620,
		   0.000000000000810,
		   1.880259521923831,
		  -0.883977120417337,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.897013405250830,
		  -0.900764129071286,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.928769222871884,
		  -0.932582733439794,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.971898055346295,
		  -0.975796839080363
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_50_HZ_8_TAPS

#define IIR_ORDER     8
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000003,
		   0.000000000000007,
		   0.000000000000003,
		   1.939269633591658,
		  -0.940227018502065,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.948133538515166,
		  -0.949095299386850,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.964726901948732,
		  -0.965696854685791,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.986837906976686,
		  -0.987818775546283
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_10_HZ_8_TAPS

#define IIR_ORDER     8
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.987711418818557,
		  -0.987750655314706,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.989566542080496,
		  -0.989605815195913,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.993003489112948,
		  -0.993042830072095,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.997512010282722,
		  -0.997551440237975
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_01_HZ_8_TAPS

#define IIR_ORDER     8
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000000,
		   0.000000000000000,
		   0.000000000000000,
		   1.998767873456739,
		  -0.998768267997768,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.998955295536889,
		  -0.998955690114913,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999301698876235,
		  -0.999302093522637,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999754477598851,
		  -0.999754872334628
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_46_00_HZ_4_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.719359552810643,
		   1.438719105621287,
		   0.719359552810643,
		  -1.575239977788152,
		  -0.626334259159141,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		  -1.768827859923722,
		  -0.826201332947616
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_20_00_HZ_4_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.046582906636444,
		   0.093165813272887,
		   0.046582906636444,
		   0.328975677370952,
		  -0.064587654916443,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   0.453119520652385,
		  -0.466325570763237
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_13_00_HZ_4_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.011623323621400,
		   0.023246647242799,
		   0.011623323621400,
		   0.818112478588957,
		  -0.195114947537567,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.070471084547619,
		  -0.563765408218865
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_4_00_HZ_4_TAPS

#define IIR_ORDER     4
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000183216023370,
		   0.000366432046739,
		   0.000183216023370,
		   1.575239977788153,
		  -0.626334259159143,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.768827859923723,
		  -0.826201332947617
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_2_00_HZ_4_TAPS

#define IIR_ORDER     4
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000013293728899,
		   0.000026587457798,
		   0.000013293728899,
		   1.778313488139435,
		  -0.792447471832947,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.893415601022500,
		  -0.908464412949294
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_1_00_HZ_4_TAPS

#define IIR_ORDER     4
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000898486146,
		   0.000001796972293,
		   0.000000898486146,
		   1.886609582621507,
		  -0.890339736284025,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.949215958025841,
		  -0.953069895327891
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_50_HZ_4_TAPS

#define IIR_ORDER     4
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000058451424,
		   0.000000116902849,
		   0.000000058451424,
		   1.942638230540116,
		  -0.943597278470369,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.975269634851874,
		  -0.976244792359440
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_10_HZ_4_TAPS

#define IIR_ORDER     4
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000096614,
		   0.000000000193228,
		   0.000000000096614,
		   1.988418017374659,
		  -0.988457267818734,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.995163241283863,
		  -0.995202624875511
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_01_HZ_4_TAPS

#define IIR_ORDER     4
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000000000010,
		   0.000000000000019,
		   0.000000000000010,
		   1.998839297808074,
		  -0.998839692363201,
		   1.000000000000000,
		   2.000000000000000,
		   1.000000000000000,
		   1.999518826760270,
		  -0.999519221449531
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_46_00_HZ_2_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.837089190566345,
		   1.674178381132690,
		   0.837089190566345,
		  -1.647459981076977,
		  -0.700896781188403
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_20_00_HZ_2_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.206572083826148,
		   0.413144167652296,
		   0.206572083826148,
		   0.369527377351241,
		  -0.195815712655833
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_13_00_HZ_2_TAPS

#define IIR_ORDER     16
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.104078356756097,
		   0.208156713512194,
		   0.104078356756097,
		   0.903419042858346,
		  -0.319732469882734
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_4_00_HZ_2_TAPS

#define IIR_ORDER     2
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.013359200027856,
		   0.026718400055713,
		   0.013359200027856,
		   1.647459981076977,
		  -0.700896781188403
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_2_00_HZ_2_TAPS

#define IIR_ORDER     2
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.003621681514929,
		   0.007243363029857,
		   0.003621681514929,
		   1.822694925196308,
		  -0.837181651256023
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_1_00_HZ_2_TAPS

#define IIR_ORDER     2
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000944691843840,
		   0.001889383687680,
		   0.000944691843840,
		   1.911197067426073,
		  -0.914975834801433
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_50_HZ_2_TAPS

#define IIR_ORDER     2
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000241359049042,
		   0.000482718098084,
		   0.000241359049042,
		   1.955578240315035,
		  -0.956543676511203
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_10_HZ_2_TAPS

#define IIR_ORDER     2
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000009825916820,
		   0.000019651833641,
		   0.000009825916820,
		   1.991114292201654,
		  -0.991153595868935
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};

#elif FILTER == LP_IIR_0_01_HZ_2_TAPS

#define IIR_ORDER     2
#define IIR_NUMSTAGES (IIR_ORDER/2)

static float32_t m_biquad_state[IIR_ORDER];
static float32_t m_biquad_coeffs[5*IIR_NUMSTAGES] =
{
		   0.000000098652211,
		   0.000000197304421,
		   0.000000098652211,
		   1.999111423470796,
		  -0.999111818079639
};

arm_biquad_cascade_df2T_instance_f32 const iir_inst =
{
  IIR_ORDER/2,
  m_biquad_state,
  m_biquad_coeffs
};


#else
	#undef USE_FILTER
#endif

#endif //USE_FILTER





float filter(float fData)
{
#ifndef USE_FILTER
	return fData;
#endif

#ifdef USE_FILTER
	float fSrc = fData;
	float fReturn;

	arm_biquad_cascade_df2T_f32(&iir_inst, &fSrc, &fReturn, 1);

	return fReturn;
#endif
}

