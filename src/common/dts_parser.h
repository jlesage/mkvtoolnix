/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   DTS decoder & parser

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef MTX_COMMON_DTS_PARSER_H
#define MTX_COMMON_DTS_PARSER_H

#include "common/common_pch.h"

class byte_buffer_c;

namespace mtx { namespace dts {

struct header_t;

class parser_c {
protected:
  struct impl_t;
  std::unique_ptr<impl_t> m;

public:
  parser_c();
  virtual ~parser_c();

  bool detect(unsigned char const *buffer, std::size_t size, std::size_t num_required_headers);
  bool detect(memory_c &buffer, std::size_t num_required_headers);
  header_t get_first_header() const;

  memory_cptr decode(unsigned char const *buffer, std::size_t size);
  memory_cptr decode(memory_c &buffer);

  void reset();

protected:
  void decode_buffer();
  void decode_step(byte_buffer_c &remainder_buffer, std::size_t multiples_of, std::function<void()> const &worker);
};

}}

#endif // MTX_COMMON_DTS_PARSER_H
