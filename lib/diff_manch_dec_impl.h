/* -*- c++ -*- */
/*
 * Copyright 2023 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LINECODE_DIFF_MANCH_DEC_IMPL_H
#define INCLUDED_LINECODE_DIFF_MANCH_DEC_IMPL_H

#include <linecode/diff_manch_dec.h>

namespace gr {
  namespace linecode {

    class diff_manch_dec_impl : public diff_manch_dec
    {
     private:
      unsigned int d_alignment = 0;

     public:
      diff_manch_dec_impl(unsigned int alignment);
      ~diff_manch_dec_impl();

     // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
          );
    };

  } // namespace linecode
} // namespace gr

#endif /* INCLUDED_LINECODE_DIFF_MANCH_DEC_IMPL_H */
