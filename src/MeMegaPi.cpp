#include "MeMegaPi.h"

MePort_Sig mePort[17] =
{
   { NC, NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC }, 
   { 16, 17 }, {  A8,  A9 }, { A10, A11 }, { A13, A12 }, {  NC,  NC }, 
   { NC, NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC },
   { NC, NC },{ NC, NC },
};

Encoder_port_type encoder_Port[6] =
{
  { NC,     NC,     NC,     NC,     NC},
  //NET2    NET1    PWM     DIR1    DIR2
  { 18,     31,     12,     34,     35},
  //ENB A   ENB B   PWMB    DIR B1  DIR B2
  { 19,     38,     8,      37,     36},
  { 3,      49,     9,      43,     42},
  { 2,      A1,     5,      A4,     A5},
  { NC,     NC,     NC,     NC,     NC},
};

megapi_dc_type megapi_dc_Port[14] =
{
  {NC,NC,NC}, {33,32,11}, {40,41, 7}, {47,48, 6}, {A3,A2, 4},
  {NC,NC,NC}, {NC,NC,NC}, {NC,NC,NC}, {NC,NC,NC}, {35,34,12},
  {36,37, 8}, {42,43, 9}, {A5,A4, 5},
};

megaPi_slot_type megaPi_slots[4] =
{
  {35,   34,   33,   32,   31,  18,  12,   11},
  {36,   37,   40,   41,   38,  19,   8,   7},
  {42,   43,   47,   48,   49,   3,   9,   6},
  {A5,   A4,   A3,   A2,   A1,   2,   5,   4},    // for megapi
};