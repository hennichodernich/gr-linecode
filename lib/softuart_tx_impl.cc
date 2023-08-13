/* -*- c++ -*- */
/* 
 * Copyright 2020 hnch@gmx.net.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "softuart_tx_impl.h"

namespace gr {
  namespace linecode {

    softuart_tx::sptr
    softuart_tx::make(unsigned int samples_per_bit, unsigned int leading_idle_samples, unsigned int parity)
    {
      return gnuradio::get_initial_sptr
        (new softuart_tx_impl(samples_per_bit, leading_idle_samples, parity));
    }

    /*
     * The private constructor
     */
    softuart_tx_impl::softuart_tx_impl(unsigned int samples_per_bit, unsigned int leading_idle_samples, unsigned int parity)
      : gr::block("softuart_tx",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
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

