/* -*- c++ -*- */
/*
 * Copyright 2023 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "diff_manch_dec_impl.h"

namespace gr {
  namespace linecode {

    using input_type = unsigned char;
    using output_type = unsigned char;
    diff_manch_dec::sptr
    diff_manch_dec::make(unsigned int alignment)
    {
      return gnuradio::make_block_sptr<diff_manch_dec_impl>(
        alignment);
    }


    /*
     * The private constructor
     */
    diff_manch_dec_impl::diff_manch_dec_impl(unsigned int alignment)
      : gr::sync_decimator("diff_manch_dec",
              gr::io_signature::make(1, 1, sizeof(input_type)),
              gr::io_signature::make(1, 1, sizeof(output_type)),2),
	      d_alignment(alignment)
    {
    }

    /*
     * Our virtual destructor.
     */
    diff_manch_dec_impl::~diff_manch_dec_impl()
    {
    }

    int
    diff_manch_dec_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      unsigned int alignment = d_alignment;
      unsigned int new_alignment;
      unsigned int input_idx = 1;

      if (alignment > 1)
      {
	      alignment = 1;
      }

      new_alignment = alignment;


      // Do <+signal processing+>
      for (int i = 0; i < noutput_items; i++)
      {
	      if(alignment==0)	// assumption: samples belonging together are in[input_idx] and in[input_idx - 1]
	      {
	      		if(in[input_idx + 1] == in[input_idx]){	// look at other combination of samples, 1 into the future
				new_alignment = 1;		// if these are identical, our assumption must be wrong and the other alignment is more plausible
	      		}

	      }
	      else		// assumption: samples belonging together are in[input_idx + 1] and in[input_idx]
	      {
	      		if(in[input_idx] == in[input_idx - 1 ]){// look at current input sample combination
				new_alignment = 0;		// if these are identical, our assumption must be wrong, use the other alignment assumption
	      		}

	      }
	      alignment = new_alignment;
	

	      out[i] = (~(in[input_idx + alignment] ^ in[input_idx + alignment - 1])) & 0x01;
	      input_idx+=2;
      }


      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace linecode */
} /* namespace gr */
