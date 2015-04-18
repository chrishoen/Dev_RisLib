#ifndef DAS_COMMAND_H
#define DAS_COMMAND_H
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

   FILE NAME       : das_command.h
   ID Number       :
   Initial Release : 


   *** REVISION HISTORY ***

   Revision     :
   Release date :
   PPRs         :
   Change description:

****************************************************************************/
#include "message.h"

#include <iostream>
#include <fstream>

struct DAS_command_data
{
  //! DAS Command Input
  Uint8 command_input;
  //! DAS Command Sequence Number
  Uint8 command_sequence_num;
  //! DAS Event Time Mark
  Float64 event_time_mark;
  //! DAS Description Delay
  Float32 event_time_delay;
  //! DAS Event Description
  Uint8 event_description[60];
};

class DAS_command : public Message
{
public:

  //---------------------------------------------------------------------
  //! Constructor, no additional processing.
  //---------------------------------------------------------------------
  DAS_command(void);

  //---------------------------------------------------------------------
  //! Copy Constructor initializes the object based on the passed in 
  //! object.
  //---------------------------------------------------------------------
  DAS_command(const DAS_command& right);

  //---------------------------------------------------------------------
  //! Destructor, no additional processing.
  //---------------------------------------------------------------------
  virtual ~DAS_command(void);

  //---------------------------------------------------------------------
  //! Assignment Operator
  //---------------------------------------------------------------------
  const DAS_command& operator=(const DAS_command& right);

  //---------------------------------------------------------------------
  //! Equality Operator
  //---------------------------------------------------------------------
  Boolean8 operator==(const DAS_command& right) const;

  //---------------------------------------------------------------------
  //! Inequality operator
  //---------------------------------------------------------------------
  Boolean8 operator!=(const DAS_command& right) const;

  //---------------------------------------------------------------------
  //! Reinitializes all member data to their default values.
  //---------------------------------------------------------------------
  virtual void set_to_defaults(void);

  //---------------------------------------------------------------------
  //! Calculates the size of the byte formatted message based on
  //! the current state of all member data.
  //! @return the combined size of all member data.
  //---------------------------------------------------------------------
  virtual Uint16 get_encoded_size(void) const;

  //---------------------------------------------------------------------
  //! Gets the id of the message.
  //! @return ID of the message.
  //---------------------------------------------------------------------
  virtual Uint16 get_id(void) const;

  //---------------------------------------------------------------------
  //! Encodes all member data into a byte buffer
  //! @param[in] buffer, the byte buffer where the encoded data
  //!                    will be stored
  //! @return Returns the number of bytes encoded
  //---------------------------------------------------------------------
  virtual void encode(Uint8 *buffer) const;

  //---------------------------------------------------------------------
  //! Extracts all member data from a byte buffer
  //! @param[in] buffer, the byte buffer where the encoded data
  //!                    is stored
  //! @return Returns the number of bytes dencoded
  //---------------------------------------------------------------------
  virtual void decode(const Uint8 *buffer);


  //---------------------------------------------------------------------
  //! This method will be used to call the correct visit() method in 
  //! order to retrieve the lost type information for the message 
  //! object.  
  //! @param[in] msg_visitor, A Message Visitor object, that contains 
  //!                         the correct visit() method.
  //---------------------------------------------------------------------
  virtual void accept(Message_observer* msg_observer);

  //---------------------------------------------------------------------
  //! @return a reference to the internal message data
  //---------------------------------------------------------------------
  inline DAS_command_data& get_data(void)
  {
    return data;
  }
  
  //---------------------------------------------------------------------
  //! @return a reference to the internal message data
  //---------------------------------------------------------------------
  inline const DAS_command_data& get_data(void) const
  {
    return data;
  }
  
  //---------------------------------------------------------------------
  //! @return a pointer to this message's internal buffer
  //---------------------------------------------------------------------
  virtual Uint8* get_internal_buffer_ptr(void);
  
private:
  //! Not a variable length message, so this is the actual size
  static const Int32 INTERNAL_BUFFER_SIZE = 74;

  //! All of the internal message data
  DAS_command_data data;

  //! Buffer to hold all internal variables
  Uint8 internal_buffer[INTERNAL_BUFFER_SIZE];
};

#endif // DAS_COMMAND_H

