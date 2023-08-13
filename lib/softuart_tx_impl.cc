/* -*- c++ -*- */
/*
 * Copyright 2023 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "softuart_tx_impl.h"

namespace gr {
  namespace linecode {

    using input_type = unsigned char;
    using output_type = unsigned char;
    softuart_tx::sptr
    softuart_tx::make(unsigned int samples_per_bit, unsigned int leading_idle_samples, unsigned int parity)
    {
      return gnuradio::make_block_sptr<softuart_tx_impl>(
        samples_per_bit, leading_idle_samples, parity);
    }


    /*
     * The private constructor
     */
    softuart_tx_impl::softuart_tx_impl(unsigned int samples_per_bit, unsigned int leading_idle_samples, unsigned int parity)
      : gr::block("softuart_tx",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
		d_spb(samples_per_bit), d_idle(leading_idle_samples), d_parity(parity)
    {
        if(d_parity==0){
            num_parity = 0;
        }
        else{
            num_parity = 1;
        }
    }

    /*
     * Our virtual destructor.
     */
    softuart_tx_impl::~softuart_tx_impl()
    {
    }

    void
    softuart_tx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
          ninput_items_required[0] = noutput_items;
    }

    int
    softuart_tx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];


      unsigned int in_idx = 0;
      unsigned int out_idx = 0;
      unsigned int bits[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      unsigned char tmp_byte, num_ones;

      // Do <+signal processing+>
     
      while(true){
        if((noutput_items-out_idx)<(d_spb * (1+8+num_parity+1) + d_idle)){
		      break;
	    }

	    tmp_byte = in[in_idx];

	    num_ones = 0;
	    bits[0]=0;
	    for(int i=0;i<8;i++){
            bits[i+1]=tmp_byte & 0x01;
            tmp_byte = tmp_byte >> 1;
            num_ones += bits[i+1];
        }
	    if(num_parity==1){
            if(d_parity==1){
                bits[9] = num_ones & 0x01;
            }
		    else if(d_parity==2){
                bits[9] = 1 - (num_ones & 0x01);
            }
        }
	    bits[9+num_parity]=1;
	      
        for(int k=0;k<d_idle;k++){
            out[out_idx++] = 1;
        }
        for(int i=0;i<10+num_parity;i++){
            for(int k=0;k<d_spb;k++){
                out[out_idx++] = bits[i];
            }
        }

	    in_idx++;
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (in_idx);

      // Tell runtime system how many output items we produced.
      return out_idx;
    }

  } /* namespace linecode */
} /* namespace gr */
