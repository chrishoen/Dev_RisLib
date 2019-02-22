#pragma once
/*==============================================================================
Filter constants used by the print facility.
==============================================================================*/

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

   static const int cFilterTableSize   = 400;

   static const int  FilterZero        =   0;
   static const int  PrintInit1        =   1;
   static const int  PrintInit2        =   2;
   static const int  PrintInit3        =   3;
   static const int  PrintInit4        =   4;
   static const int  PrintRun1         =   5;
   static const int  PrintRun2         =   6;
   static const int  PrintRun3         =   7;
   static const int  PrintRun4         =   8;

   static const int  SocketInitS1      =   9;
   static const int  SocketInitS2      =  10;
   static const int  SocketRxRunS1     =  11;
   static const int  SocketRxRunS2     =  12;
   static const int  SocketTxRunS1     =  13;
   static const int  SocketTxRunS2     =  14;
   static const int  SocketErrorS1     =  15;
   static const int  SocketErrorS2     =  16;
   static const int  SocketInitT1      =  17;
   static const int  SocketInitT2      =  18;
   static const int  SocketRxRunT1     =  19;
   static const int  SocketRxRunT2     =  20;
   static const int  SocketTxRunT1     =  21;
   static const int  SocketTxRunT2     =  22;
   static const int  SocketErrorT1     =  23;
   static const int  SocketErrorT2     =  24;

   static const int  SerialInitP1      =  25;
   static const int  SerialInitP2      =  26;
   static const int  SerialRxRunP1     =  27;
   static const int  SerialRxRunP2     =  28;
   static const int  SerialTxRunP1     =  29;
   static const int  SerialTxRunP2     =  30;
   static const int  SerialErrorP1     =  31;
   static const int  SerialErrorP2     =  32;
   static const int  SerialInitM1      =  33;
   static const int  SerialInitM2      =  34;
   static const int  SerialRxRunM1     =  35;
   static const int  SerialRxRunM2     =  36;
   static const int  SerialTxRunM1     =  37;
   static const int  SerialTxRunM2     =  38;
   static const int  SerialErrorM1     =  39;
   static const int  SerialErrorM2     =  40;
   static const int  ThreadInit1       =  41;
   static const int  ThreadInit2       =  42;
   static const int  ThreadInit3       =  43;
   static const int  ThreadInit4       =  44;
   static const int  ThreadRun1        =  45;
   static const int  ThreadRun2        =  46;
   static const int  ThreadRun3        =  47;
   static const int  ThreadRun4        =  48;
   static const int  ProcInit1         =  49;
   static const int  ProcInit2         =  50;
   static const int  ProcInit3         =  51;
   static const int  ProcInit4         =  52;
   static const int  ProcRun1          =  53;
   static const int  ProcRun2          =  54;
   static const int  ProcRun3          =  55;
   static const int  ProcRun4          =  56;
   static const int  PointInit1        =  57;
   static const int  PointInit2        =  58;
   static const int  PointInit3        =  59;
   static const int  PointInit4        =  60;
   static const int  PointRun1         =  61;
   static const int  PointRun2         =  62;
   static const int  PointRun3         =  63;
   static const int  PointRun4         =  64;
   static const int  QCallInit1        =  65;
   static const int  QCallInit2        =  66;
   static const int  QCallInit3        =  67;
   static const int  QCallInit4        =  68;
   static const int  QCallRun1         =  69;
   static const int  QCallRun2         =  70;
   static const int  QCallRun3         =  71;
   static const int  QCallRun4         =  72;
   static const int  FileInit1         =  73;
   static const int  FileInit2         =  74;
   static const int  FileInit3         =  75;
   static const int  FileInit4         =  76;
   static const int  FileRun1          =  77;
   static const int  FileRun2          =  78;
   static const int  FileRun3          =  79;
   static const int  FileRun4          =  80;
   static const int  FileError1        =  81;
   static const int  FileError2        =  82;
   static const int  FileError3        =  83;
   static const int  FileError4        =  84;

   static const int  View01            =  85;
   static const int  View02            =  86;
   static const int  View03            =  87;
   static const int  View04            =  88;
   static const int  View11            =  89;
   static const int  View12            =  90;
   static const int  View13            =  91;
   static const int  View14            =  92;
   static const int  View21            =  93;
   static const int  View22            =  94;
   static const int  View23            =  95;
   static const int  View24            =  96;
   static const int  View31            =  97;
   static const int  View32            =  98;
   static const int  View33            =  99;
   static const int  View34            = 100;
   static const int  View41            = 101;
   static const int  View42            = 102;
   static const int  View43            = 103;
   static const int  View44            = 104;

   static const int  Show1             = 105;
   static const int  Show2             = 106;
   static const int  Show3             = 107;
   static const int  Show4             = 108;
   static const int  Show5             = 109;
   static const int  Show6             = 110;

   static const int  State1            = 111;
   static const int  State2            = 112;
   static const int  State3            = 113;
   static const int  State4            = 114;

   static const int  CalcInit1         = 200;
   static const int  CalcInit2         = 201;
   static const int  CalcInit3         = 202;
   static const int  CalcInit4         = 203;
   static const int  CalcRun1          = 204;
   static const int  CalcRun2          = 205;
   static const int  CalcRun3          = 206;
   static const int  CalcRun4          = 207;
   static const int  CalcError1        = 208;
   static const int  CalcError2        = 209;
   static const int  CalcError3        = 210;
   static const int  CalcError4        = 211;
   static const int  MsgRx1            = 212;
   static const int  MsgRx2            = 213;
   static const int  MsgRx3            = 214;
   static const int  MsgRx4            = 215;
   static const int  ViewInit1         = 216; 
   static const int  ViewInit2         = 217;
   static const int  ViewInit3         = 218;
   static const int  ViewInit4         = 219;
   static const int  ViewRun1          = 220;
   static const int  ViewRun2          = 221;
   static const int  ViewRun3          = 222;
   static const int  ViewRun4          = 223;
   static const int  CamInit1          = 224;
   static const int  CamInit2          = 225;
   static const int  CamInit3          = 226;
   static const int  CamInit4          = 227;
   static const int  CamRun1           = 228;
   static const int  CamRun2           = 229;
   static const int  CamRun3           = 230;
   static const int  CamRun4           = 231;
   static const int  CamError1         = 232;
   static const int  CamError2         = 233;
   static const int  CamError3         = 234;
   static const int  CamError4         = 235;
   static const int  SensorInit1       = 236;
   static const int  SensorInit2       = 237;
   static const int  SensorInit3       = 238;
   static const int  SensorInit4       = 239;
   static const int  SensorRun1        = 240;
   static const int  SensorRun2        = 241;
   static const int  SensorRun3        = 242;
   static const int  SensorRun4        = 243;
   static const int  SensorError1      = 244;
   static const int  SensorError2      = 245;
   static const int  SensorError3      = 246;
   static const int  SensorError4      = 247;
   static const int  DspInit1          = 248;
   static const int  DspInit2          = 249;
   static const int  DspInit3          = 250;
   static const int  DspInit4          = 251;
   static const int  DspRun1           = 252;
   static const int  DspRun2           = 253;
   static const int  DspRun3           = 254;
   static const int  DspRun4           = 255;
   static const int  DspError1         = 256;
   static const int  DspError2         = 257;
   static const int  DspError3         = 258;
   static const int  DspError4         = 259;
   static const int  Result1           = 260;
   static const int  Result2           = 261;
   static const int  Result3           = 262;
   static const int  Result4           = 263;
   static const int  Result5           = 264;
   static const int  Result6           = 265;
   static const int  Result7           = 266;
   static const int  Result8           = 267;
   static const int  Cal1              = 268;
   static const int  Cal2              = 269;
   static const int  Cal3              = 270;
   static const int  Cal4              = 271;
   static const int  Cal5              = 272;
   static const int  Cal6              = 273;
   static const int  Cal7              = 274;
   static const int  Cal8              = 275;
   static const int  CalError1         = 276;
   static const int  CalError2         = 277;
   static const int  CalError3         = 278;
   static const int  CalError4         = 279;
   static const int  BackInit1         = 280;
   static const int  BackInit2         = 281;
   static const int  BackInit3         = 282;
   static const int  BackInit4         = 283;
   static const int  BackRun1          = 284;
   static const int  BackRun2          = 285;
   static const int  BackRun3          = 286;
   static const int  BackRun4          = 287;
   static const int  BackError1        = 288;
   static const int  BackError2        = 289;
   static const int  BackError3        = 290;
   static const int  BackError4        = 291;
   static const int  VidInit1          = 292;
   static const int  VidInit2          = 293;
   static const int  VidInit3          = 294;
   static const int  VidInit4          = 295;
   static const int  Vid1              = 296;
   static const int  Vid2              = 297;
   static const int  Vid3              = 298;
   static const int  Vid4              = 299;
   static const int  ZCalInit1         = 300;
   static const int  ZCalInit2         = 301;
   static const int  ZCalInit3         = 302;
   static const int  ZCalInit4         = 303;
   static const int  ZCalRun1          = 304;
   static const int  ZCalRun2          = 305;
   static const int  ZCalRun3          = 306;
   static const int  ZCalRun4          = 307;
   static const int  ZCalError1        = 308;
   static const int  ZCalError2        = 309;
   static const int  ZCalError3        = 310;
   static const int  ZCalError4        = 311;
   static const int  PEst1             = 312;
   static const int  PEst2             = 313;
   static const int  PClimb            = 314;
   static const int  PSearch1          = 315;
   static const int  PSearch2          = 316;
   static const int  PSearch3          = 317;
   static const int  PSearch4          = 318;
   static const int  PSlant            = 319;
   static const int  UStage1           = 320;
   static const int  UStage2           = 321;
   static const int  UStage3           = 322;
   static const int  UStage4           = 323;
   static const int  UMove1            = 324;
   static const int  UMove2            = 325;
   static const int  UError            = 326;
   static const int  PFind1            = 327;
   static const int  PFind2            = 328;
   static const int  UGrid1            = 329;
   static const int  UGrid2            = 330;
   static const int  UGrid3            = 331;
   static const int  UGrid4            = 332;
   static const int  UGrid5            = 333;
   static const int  UGrid6            = 334;
   static const int  UGrid7            = 335;
   static const int  UGrid8            = 336;
   static const int  RO1               = 337;
   static const int  RO2               = 338;
   static const int  RO3               = 339;
   static const int  RO4               = 340;

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

