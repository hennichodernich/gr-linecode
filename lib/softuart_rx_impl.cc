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
#include "softuart_rx_impl.h"

namespace gr {
  namespace linecode {

    softuart_rx::sptr
    softuart_rx::make(unsigned int samples_per_bit, unsigned int idle_level, float threshold, unsigned int parity)
    {
      return gnuradio::get_initial_sptr
        (new softuart_rx_impl(samples_per_bit, idle_level, threshold, parity));
    }

    /*
     * The private constructor
     */
    softuart_rx_impl::softuart_rx_impl(unsigned int samples_per_bit, unsigned int idle_level, float threshold, unsigned int parity)
      : gr::block("softuart_rx",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
		d_spb(samples_per_bit), d_idle(idle_level), d_threshold(threshold), d_parity(parity)
    {
    	set_history(2); // need to look at two inputs
/*
	if (d_idle == 0){
		polarity = 1;
	}
	else if (d_idle == 1){
		polarity = -1;
	}
	else{
		d_idle = 0;
		polarity = 0;
	}
	*/
	polarity = -1;
	d_idle = 1;
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
    softuart_rx_impl::~softuart_rx_impl()
    {
    }

    void
    softuart_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = 2 * (1+8+num_parity+1) * d_spb * noutput_items;
    }

    int
    softuart_rx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      unsigned int in_idx = 1;
      unsigned int out_idx = 0;

      while (true){
	if((ninput_items[0] - (in_idx - 1)) < (d_spb * (1+8+num_parity+1))){
		break;
	}
      	// Do <+signal processing+>

      	if (in_byte == false){
      		if (found_startedge == false){
      			if ((in[in_idx] - in[in_idx - 1])>(polarity * d_threshold) && (round(in[in_idx]) == (1 - d_idle))){
	      			found_startedge = true;
	      			levelcounter = 0;
      			}
      		}
      		else{
			if (round(in[in_idx])==round(in[in_idx-1])){
				levelcounter++;
			}
			if (levelcounter==d_spb){
				in_byte = true;
			}
      		}
      		in_idx++;
      	}
      	else{
#ifdef DEBUG		
		std::cout << "new byte: ";
#endif		
		tmp_byte = 0;
		//in_idx points to first sample of first data bit
		for(int i=0;i<8;i++)
		{
			tmp_byte = tmp_byte >> 1;
			if (in[in_idx + d_spb/2] >= 0.5){
#ifdef DEBUG		
				std::cout << "1 ";
#endif		
				tmp_byte |= 0x80;
			}
			else{
#ifdef DEBUG		
				std::cout << "0 ";
#endif		
			}
			in_idx += d_spb;
		}
		if(num_parity == 1){
			//todo: check parity
			in_idx += d_spb;
		}
		if (in[in_idx + d_spb/2] >= 0.5){ // found stop bit
			out[out_idx] = tmp_byte;
			out_idx++;
			in_idx += d_spb;
		}
		in_byte = false;
		found_startedge = false;
#ifdef DEBUG		
		std::cout << "-->" << int(tmp_byte) << std::endl;
#endif		
      	}

     }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      
      consume_each (in_idx - 1);

      // Tell runtime system how many output items we produced.
      return out_idx;
    }

  } /* namespace linecode */
} /* namespace gr */

