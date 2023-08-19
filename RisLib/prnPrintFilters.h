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
   static const int  QCallInit1        =  57;
   static const int  QCallInit2        =  58;
   static const int  QCallInit3        =  59;
   static const int  QCallInit4        =  60;
   static const int  QCallRun1         =  61;
   static const int  QCallRun2         =  62;
   static const int  QCallRun3         =  63;
   static const int  QCallRun4         =  64;
   static const int  FileInit1         =  65;
   static const int  FileInit2         =  66;
   static const int  FileInit3         =  67;
   static const int  FileInit4         =  68;
   static const int  FileRun1          =  69;
   static const int  FileRun2          =  70;
   static const int  FileRun3          =  71;
   static const int  FileRun4          =  72;
   static const int  FileError1        =  73;
   static const int  FileError2        =  74;
   static const int  FileError3        =  75;
   static const int  FileError4        =  76;

   static const int  ViewInit1         =  77;
   static const int  ViewInit2         =  78;
   static const int  ViewInit3         =  79;
   static const int  ViewInit4         =  80;
   static const int  ViewRun1          =  81;
   static const int  ViewRun2          =  82;
   static const int  ViewRun3          =  83;
   static const int  ViewRun4          =  84;

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

   static const int  CProc1            = 200;
   static const int  CProc2            = 201;
   static const int  CProc3            = 202;
   static const int  CProc4            = 203;

   static const int  Touch1            = 204;
   static const int  Touch2            = 205;
   static const int  Touch3            = 206;
   static const int  Touch4            = 207;

   static const int  TTA1              = 208;
   static const int  TTA2              = 209;
   static const int  TTA3              = 210;
   static const int  TTA4              = 211;
   static const int  TTA5              = 212;
   static const int  TTA6              = 213;
   static const int  TTA7              = 214;
   static const int  TTA8              = 215;

   static const int  DA1               = 216;
   static const int  DA2               = 217;
   static const int  DA3               = 218;
   static const int  DA4               = 219;
   static const int  DA5               = 220;
   static const int  DA6               = 221;
   static const int  DA7               = 222;
   static const int  DA8               = 223;

   static const int  Init1             = 224;
   static const int  Init2             = 225;
   static const int  Init3             = 226;
   static const int  Init4             = 227;

   static const int  HLC1              = 228;
   static const int  HLC2              = 229;
   static const int  HLC3              = 230;
   static const int  HLC4              = 231;

   static const int  EVT1              = 232;
   static const int  EVT2              = 233;
   static const int  EVT3              = 234;
   static const int  EVT4              = 235;

   static const int  Comm1             = 236;
   static const int  Comm2             = 237;
   static const int  Comm3             = 238;
   static const int  Comm4             = 239;

   static const int  Show0             = 240;
   static const int  Show1             = 241;
   static const int  Show2             = 242;
   static const int  Show3             = 243;
   static const int  Show4             = 244;
   static const int  Show5             = 245;
   static const int  Show6             = 246;

   static const int  Calc1             = 247;
   static const int  Calc2             = 248;
   static const int  Calc3             = 249;
   static const int  Calc4             = 250;

   static const int  DspRun1           = 251;
   static const int  DspRun2           = 252;
   static const int  DspRun3           = 253;
   static const int  DspRun4           = 254;

   static const int  Calc11            = 255;
   static const int  Calc12            = 256;
   static const int  Calc13            = 257;
   static const int  Calc14            = 258;
   static const int  Calc21            = 259;
   static const int  Calc22            = 260;
   static const int  Calc23            = 261;
   static const int  Calc24            = 262;

   static const int  ShowA             = 263;
   static const int  ShowB             = 264;
   static const int  ShowC             = 265;
   static const int  ShowD             = 266;
   static const int  ShowS             = 267;
   static const int  ShowT             = 268;

   static const int  ShowTx1           = 269;
   static const int  ShowTx2           = 270;
   static const int  ShowTx3           = 271;
   static const int  ShowTx4           = 272;

   static const int  ShowRx1           = 273;
   static const int  ShowRx2           = 274;
   static const int  ShowRx3           = 275;
   static const int  ShowRx4           = 276;

   static const int  ShowTxJ           = 277;
   static const int  ShowRxJ           = 278;

   static const int  FTMsg             = 279;

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

