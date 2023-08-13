/* -*- c++ -*- */
/*
 * Copyright 2023 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LINECODE_SOFTUART_RX_IMPL_H
#define INCLUDED_LINECODE_SOFTUART_RX_IMPL_H

#include <linecode/softuart_rx.h>

namespace gr {
  namespace linecode {

    class softuart_rx_impl : public softuart_rx
    {
     private:
        unsigned int d_spb;
        unsigned int d_idle;
        float d_threshold;
        bool found_startedge = false;
        bool in_byte = false;
        int polarity;
        unsigned int levelcounter = 0;
        unsigned char tmp_byte;
        unsigned int d_parity = 0;
        unsigned int num_parity = 0;
        
     public:
      softuart_rx_impl(unsigned int samples_per_bit, unsigned int idle_level, float threshold, unsigned int parity);
      ~softuart_rx_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace linecode
} // namespace gr

#endif /* INCLUDED_LINECODE_SOFTUART_RX_IMPL_H */
