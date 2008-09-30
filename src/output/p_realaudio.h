/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   $Id$

   class definition for the RealAudio output module

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef __P_REALAUDIO_H
#define __P_REALAUDIO_H

#include "os.h"

#include "common.h"
#include "pr_generic.h"
#include "smart_pointers.h"

class ra_packetizer_c: public generic_packetizer_c {
private:
  int m_samples_per_sec, m_channels, m_bits_per_sample;
  uint32_t m_fourcc;
  memory_cptr m_private_data;

public:
  ra_packetizer_c(generic_reader_c *p_reader, track_info_c &p_ti, int samples_per_sec, int channels, int bits_per_sample, uint32_t fourcc,
                  unsigned char *private_data, int private_size) throw (error_c);
  virtual ~ra_packetizer_c();

  virtual int process(packet_cptr packet);
  virtual void set_headers();

  virtual const char *get_format_name() {
    return "RealAudio";
  }
  virtual connection_result_e can_connect_to(generic_packetizer_c *src, string &error_message);
};

#endif // __P_REALAUDIO_H
