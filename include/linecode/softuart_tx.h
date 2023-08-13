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


#ifndef INCLUDED_LINECODE_SOFTUART_TX_H
#define INCLUDED_LINECODE_SOFTUART_TX_H

#include <linecode/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace linecode {

    /*!
     * \brief <+description of block+>
     * \ingroup linecode
     *
     */
    class LINECODE_API softuart_tx : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<softuart_tx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of linecode::softuart_tx.
       *
       * To avoid accidental use of raw pointers, linecode::softuart_tx's
       * constructor is in a private implementation
       * class. linecode::softuart_tx::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int samples_per_bit, unsigned int leading_idle_samples, unsigned int parity);
    };

  } // namespace linecode
} // namespace gr

#endif /* INCLUDED_LINECODE_SOFTUART_TX_H */

