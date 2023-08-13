/* -*- c++ -*- */
/*
 * Copyright 2023 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LINECODE_SOFTUART_RX_H
#define INCLUDED_LINECODE_SOFTUART_RX_H

#include <linecode/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace linecode {

    /*!
     * \brief <+description of block+>
     * \ingroup linecode
     *
     */
    class LINECODE_API softuart_rx : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<softuart_rx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of linecode::softuart_rx.
       *
       * To avoid accidental use of raw pointers, linecode::softuart_rx's
       * constructor is in a private implementation
       * class. linecode::softuart_rx::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int samples_per_bit, unsigned int idle_level, float threshold, unsigned int parity);
    };

  } // namespace linecode
} // namespace gr

#endif /* INCLUDED_LINECODE_SOFTUART_RX_H */
