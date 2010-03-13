/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   class definition for the MP3 output module

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef __P_MP3_H
#define __P_MP3_H

#include "common/common.h"

#include "common/byte_buffer.h"
#include "common/mp3.h"
#include "common/samples_timecode_conv.h"
#include "merge/pr_generic.h"

class mp3_packetizer_c: public generic_packetizer_c {
private:
  int64_t m_bytes_output, m_packetno, m_bytes_skipped;
  int m_samples_per_sec, m_channels, m_samples_per_frame;
  byte_buffer_c m_byte_buffer;
  bool m_codec_id_set, m_valid_headers_found;
  int64_t m_previous_timecode, m_num_packets_with_same_timecode;
  samples_to_timecode_converter_c m_s2tc;
  int64_t m_single_packet_duration;

public:
  mp3_packetizer_c(generic_reader_c *p_reader, track_info_c &p_ti, int samples_per_sec, int channels, bool source_is_good) throw (error_c);
  virtual ~mp3_packetizer_c();

  virtual int process(packet_cptr packet);
  virtual void set_headers();

  virtual const char *get_format_name() {
    return "MP3";
  }
  virtual connection_result_e can_connect_to(generic_packetizer_c *src, std::string &error_message);

private:
  virtual unsigned char *get_mp3_packet(mp3_header_t *mp3header);

  virtual void handle_garbage(int64_t bytes);
};

#endif // __P_MP3_H
