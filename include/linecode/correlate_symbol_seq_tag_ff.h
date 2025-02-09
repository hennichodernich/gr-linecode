/* -*- c++ -*- */
/*
 * Copyright 2005,2006,2011,2012,2017 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INCLUDED_LINECODE_CORRELATE_SYMBOL_SEQ_TAG_FF_H
#define INCLUDED_LINECODE_CORRELATE_SYMBOL_SEQ_TAG_FF_H

#include <linecode/api.h>
#include <gnuradio/sync_block.h>
#include <string>

namespace gr {
namespace linecode {

/*!
 * \brief Examine input for specified access code, one bit at a time.
 * \ingroup packet_operators_blk
 *
 * \details
 * input:  stream of floats (generally, soft decisions)
 * output: unaltered stream of bits (plus tags)
 *
 * This block annotates the input stream with tags. The tags have
 * key name [tag_name], specified in the constructor. Used for
 * searching an input data stream for preambles, etc., by slicing
 * the soft decision symbol inputs.
 */
class LINECODE_API correlate_symbol_seq_tag_ff : virtual public sync_block
{
public:
    // gr::linecode::correlate_symbol_seq_tag_ff::sptr
    typedef std::shared_ptr<correlate_symbol_seq_tag_ff> sptr;

    /*!
     * \param symbols vector of soft symbols
     * \param threshold relative decision threshold, 1.0 is energy of symbol sequence
     * \param tag_name key of the tag inserted into the tag stream
     */
    static sptr
    make(const std::vector<float>& symbols, float threshold, const std::string& tag_name);

    virtual void set_symbols(const std::vector<float>& symbols) = 0;    
    virtual void set_threshold(float threshold) = 0;
    virtual void set_tagname(const std::string& tagname) = 0;
};

} /* namespace linecode */
} /* namespace gr */

#endif /* INCLUDED_LINECODE_CORRELATE_SYMBOL_SEQ_TAG_FF_H */
