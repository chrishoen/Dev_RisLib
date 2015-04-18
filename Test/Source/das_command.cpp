/*
****************************************************************************

                 LOCKHEED MARTIN PROPRIETARY INFORMATION

                      EXPORT CONTROLLED INFORMATION

             IMPORTANT NOTICE - EXPORT CONTROLLED INFORMATION

     The Technical Data of this document and/or drawing contains data
     directly related to an article or a component of an article included
     on the United States Munitions List and as such is subject to the
     International Traffic in Arms Regulation (ITAR).  This data may not
     be disclosed, exported, transferred or diverted in any manner to any
     national foreign to the United States or to any country or
     destination outside the United States without the written approval
     of the United States Department of State.

         COPYRIGHT - LOCKHEED MARTIN, NIAGARA FALLS, NEW YORK, 2009 

****************************************************************************

   LOCKHEED MARTIN GRAVITY / RADAR SYSTEMS
   MS2 - NIAGARA OPERATIONS
   2221 Niagara Falls Boulevard
   Niagara Falls, New York  14304 - 1696

   FILE NAME       : das_command.cpp
   ID Number       :
   Initial Release : 


   *** REVISION HISTORY ***

   Revision     :
   Release date :
   PPRs         :
   Change description:

****************************************************************************/
#include "das_command.h"

#include "message_id.h"

#include <sstream>
#include <string>


//---------------------------------------------------------------------
//! Constructor, no additional processing.
//---------------------------------------------------------------------
DAS_command::DAS_command(void)
{
  set_to_defaults();
}

//---------------------------------------------------------------------
//! Copy Constructor initializes the object based on the passed in 
//! object.
//---------------------------------------------------------------------
DAS_command::DAS_command(const DAS_command& right)
{
  data.command_input = right.data.command_input;
  data.command_sequence_num = right.data.command_sequence_num;
  data.event_time_mark = right.data.event_time_mark;
  data.event_time_delay = right.data.event_time_delay;
  std::memcpy(data.event_description, right.data.event_description, 60);
}

//---------------------------------------------------------------------
//! Destructor, no additional processing.
//---------------------------------------------------------------------
DAS_command::~DAS_command(void)
{
    // No processing done in destructor
}

//---------------------------------------------------------------------
//! Assignment Operator
//---------------------------------------------------------------------
const DAS_command &DAS_command::operator=( const DAS_command& right )
{
  // make sure not same object
  if (this != &right) 
  {
    data.command_input = right.data.command_input;
    data.command_sequence_num = right.data.command_sequence_num;
    data.event_time_mark = right.data.event_time_mark;
    data.event_time_delay = right.data.event_time_delay;
    std::memcpy(data.event_description, right.data.event_description, 60);
  }
  return *this;
}

//---------------------------------------------------------------------
//! Equality Operator
//---------------------------------------------------------------------
Boolean8 DAS_command::operator==( const DAS_command& right ) const
{
  Boolean8 equal = true;

  if( data.command_input != right.data.command_input )
  {
    equal = false;
  }
  if( data.command_sequence_num != right.data.command_sequence_num )
  {
    equal = false;
  }
  if( data.event_time_mark != right.data.event_time_mark )
  {
    equal = false;
  }
  if( data.event_time_delay != right.data.event_time_delay )
  {
    equal = false;
  }
  if( std::memcmp(data.event_description, right.data.event_description, 
                                                                 60) != 0 )
  {
    equal = false;
  }

  return( equal );
}

//---------------------------------------------------------------------
//! Inequality operator
//---------------------------------------------------------------------
Boolean8 DAS_command::operator!=( const DAS_command& right ) const
{
   return ! ( *this == right );
}

//---------------------------------------------------------------------
//! Reinitializes all member data to their default values.
//---------------------------------------------------------------------
void DAS_command::set_to_defaults(void)
{
  data.command_input = 0;
  data.command_sequence_num = 0;
  data.event_time_mark = 0;
  data.event_time_delay = 0;
  std::memset(data.event_description, 0, 60);
}

//---------------------------------------------------------------------
  //! @return a pointer to this message's internal buffer
//---------------------------------------------------------------------
Uint8* DAS_command::get_internal_buffer_ptr(void)
{
  encode( &internal_buffer[0] );
  return( &internal_buffer[0] );
}

//---------------------------------------------------------------------
//! Calculates the size of the byte formatted message based on
//! the current state of all member data.
//! @return the combined size of all member data.
//---------------------------------------------------------------------
Uint16 DAS_command::get_encoded_size(void) const
{
  return INTERNAL_BUFFER_SIZE;
}

//---------------------------------------------------------------------
//! Gets the id of the message.
//! @return ID of the message.
//---------------------------------------------------------------------
Uint16 DAS_command::get_id(void) const
{
  return DAS_COMMAND_ID;
}

//---------------------------------------------------------------------
//! Encodes all member data into a byte buffer
//! @param[in] buffer, the byte buffer where the encoded data
//!                    will be stored
//! @return Returns the number of bytes encoded
//---------------------------------------------------------------------
void DAS_command::encode(Uint8 *buffer) const
{
  Int32 num_bytes = 0;

  num_bytes += to_buf_be(data.command_input, &buffer[num_bytes]);
  num_bytes += to_buf_be(data.command_sequence_num, &buffer[num_bytes]);
  num_bytes += to_buf_be(data.event_time_mark, &buffer[num_bytes]);
  num_bytes += to_buf_be(data.event_time_delay, &buffer[num_bytes]);
  num_bytes += to_buf_be(data.event_description, &buffer[num_bytes], 60);
}

//---------------------------------------------------------------------
//! Extracts all member data from a byte buffer
//! @param[in] buffer, the byte buffer where the encoded data
//!                    is stored
//! @return Returns the number of bytes dencoded
//---------------------------------------------------------------------
void DAS_command::decode(const Uint8 *buffer)
{
  Int32 num_bytes = 0;

  num_bytes += to_value_be(data.command_input, &buffer[num_bytes]);
  num_bytes += to_value_be(data.command_sequence_num, &buffer[num_bytes]);
  num_bytes += to_value_be(data.event_time_mark, &buffer[num_bytes]);
  num_bytes += to_value_be(data.event_time_delay, &buffer[num_bytes]);
  num_bytes += to_value_be(data.event_description, &buffer[num_bytes], 60);
}

//---------------------------------------------------------------------
//! This method will be used to call the correct visit() method in 
//! order to retrieve the lost type information for the message 
//! object.  
//! @param[in] msg_visitor, A Message Visitor object, that contains 
//!                         the correct visit() method.
//---------------------------------------------------------------------
void DAS_command::accept(Message_observer* msg_observer)
{
  msg_observer->on_das_command_msg_rcv(this);
}

