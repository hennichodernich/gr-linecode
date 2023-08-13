/* -*- c++ -*- */
/*
 * Copyright 2023 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LINECODE_DIFF_MANCH_DEC_H
#define INCLUDED_LINECODE_DIFF_MANCH_DEC_H

#include <linecode/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace linecode {

    /*!
     * \brief <+description of block+>
     * \ingroup linecode
     *
     */
    class LINECODE_API diff_manch_dec : virtual public gr::sync_decimator
    {
     public:
      typedef std::shared_ptr<diff_manch_dec> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of linecode::diff_manch_dec.
       *
       * To avoid accidental use of raw pointers, linecode::diff_manch_dec's
       * constructor is in a private implementation
       * class. linecode::diff_manch_dec::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int alignment);
    };

  } // namespace linecode
} // namespace gr

#endif /* INCLUDED_LINECODE_DIFF_MANCH_DEC_H */
