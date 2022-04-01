#pragma once

/*==============================================================================
xray types and definitions.
==============================================================================*/

#include "type.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//

#define SENSOR_READ_ITEM_COUNT 27

typedef struct SensorScan_T
{
   uint32_t uiSgstnTime;
   uint16_t usReading[SENSOR_READ_ITEM_COUNT];

   //dbs: now using the two unused bytes to indicate whether or not a key is down or
   //     will be down in the next ADVANCE_KEY_DOWN_NOTIFICATION samples (set retroactively)
   uint16_t usKeyIsOrWillBeDown;
}SensorScan_s; //size is 4 + 27 * 2 + 2 = 60 bytes even

#if 0
typedef struct Sgstn_s
{
   uint32 uiSteno; //sgstn format steno
   uint32 uiTime; //sgstn format timestamp
} Sgstn_t;
#endif

enum EStenoKey
{
   ekey_NumberBar = 22,
   ekey_International = 23,
   ekey_UniversalBar = 24,
   ekey_StenoMark = 30, //this is the only item here which is not read from the sensor board.
   ekey_count
};

#define KEY_COUNT ekey_UniversalBar

typedef struct SKeyLevelData_T
{
   uint32_t uiPreciseMinimum;
   uint32_t uiPreciseMaximum;

   int iMinimumOffset;
}SKeyLevelData;

typedef struct SKeyLevel_T
{
   uint32_t uiPreciseAverageMinimums;

   SKeyLevelData SData[KEY_COUNT];
}SKeyLevel;

#define MAX_KEYBOARD_LEVELS 16
#define RUNNING_AVERAGE_COUNT_MAX 16
typedef struct SKeyboardCalibration_T
{
   SKeyLevel KeyboardLevels[MAX_KEYBOARD_LEVELS];
   int iKnownKeyboardLevels;
}SKeyboardCalibration;




//******************************************************************************
//******************************************************************************
//******************************************************************************
//

#define BIAS_KEY_COUNT  (7 * 4)

typedef struct SKeyboardConsts_T
{
   //these variables are all caps because they used to be #defines

   /* A general note about the following constants:
      Basically everything implicitly depends on the sample rate, currently 1 sample per 2 ms.
      If you start reading faster or slower, you definitely will have to change these values.
      For example, the slope is a difference in the key level *over time*. If more or less time
      passes, the slope delta will correspondingly increase or decrease. If this code were to
      be rewritten, the sample rate should be an explicit parameter, and these constants should
      be automatically adjusted accordingly.
   */

   struct SPerKeyConsts
   {
      /* B and C are values that determine a linear function that maps from the range of a key to
         the on threshold %. We needed shorter ranges to have more permissive on thresholds. Note
         that this linear function probably rapidly becomes less than ideal outside of the ranges
         it's been tested in. In other words, if you get writers with many more or fewer readings
         per key, these values will need to be updated.
      */
      int ON_THRESHOLD_B;
      int ON_THRESHOLD_C;
   } Key[ekey_count];

   /* If a key's slope goes below SLOPE_OFF, we will call the key unpressed.
   */
   int SLOPE_OFF;

   /* If a key's slope goes above SLOPE_ON and its reading goes above the on threshold (calculated
      from the B and C values above), we call the key on.
   */
   int SLOPE_ON;

   /* If a key's reading (as a percent of its range) goes below THRESHOLD_PERCENT_OFF, we turn
      the key off.
   */
   int THRESHOLD_PERCENT_OFF; //unit: 1/1024

   /* EXPECTED_NOISE_RANGE is taken to define an acceptable range of noisiness from the keys.
   */
   int EXPECTED_NOISE_RANGE;

   /* RUNNING_AVERAGE_COUNT determines how many we readings we average together in the running average.
   */
   int RUNNING_AVERAGE_COUNT;



   int flags;

   /* MIN_RANGE is the minimum amount of difference allowed between the minimum
      and maximum values for a key. During calibration, it's added on to the min
      to get the default value for the maximum. Keys with less than MIN_RANGE
      won't work at all.
   */
   int MIN_RANGE;

   /* PRECISE_RADIX is used to simulate floating point math by keeping the real
      part of a value left shifted by PRECISE_RADIX, and the fractional part of
      a value in the lower PRECISE_RADIX bits of the 32-bit value. I've tried to
      make variables stored in this manner have the word "Precise" in their name,
      but I might have missed some. Note that this changes a few operations,
      e.g., if you multiply two precise numbers, you need to right shift the result
      by PRECISE_RADIX. I need to do this because the 2nd derivative can be small.
   */
   int PRECISE_RADIX;


   /* These two do not appear to be used/important.
   */
   int DECAY_CONSTANT;
   int DEFAULT_MAX;

   /* If eSpikeDelay and eSpike2 are set, SPIKE2_PARAM sets how many readings a
      high reading is artificially held (this is to prevent the running average
      from completely hiding very brief spikes in the level of a key).
   */
   int SPIKE2_PARAM;

   /* INTERSTROKE_MIN_TIME is how many readings all the keys must remain unpressed
      before we call previously pressed keys a stroke.
   */
   uint32_t INTERSTROKE_MIN_TIME;

   /* bias is (for each key) a user-set adjustment, to make the key easier or harder
      to press.
   */
   char bias[BIAS_KEY_COUNT];

   int reserved[2];
}SKeyboardConsts;

//******************************************************************************
//******************************************************************************
//******************************************************************************
//

struct StenoTableStruct //used in Cat32FileHeaderStruct
{
   char cStenoTextTable[32];
   char cShiftedStenoTextTable[32];
   BYTE ucLeft;
   BYTE ucMiddle;
   BYTE ucRight;
   BYTE ucReserved;
#ifdef _WIN32
};
#else
} __attribute__((packed));
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


