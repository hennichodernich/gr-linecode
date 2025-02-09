/* -*- c++ -*- */
/*
 * Copyright 2005,2006,2011,2012,2017 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INCLUDED_LINECODE_CORRELATE_ACCESS_CODE_TAG_FF_IMPL_H
#define INCLUDED_LINECODE_CORRELATE_ACCESS_CODE_TAG_FF_IMPL_H

#include <linecode/correlate_symbol_seq_tag_ff.h>
#include <gnuradio/filter/fft_filter.h>

namespace gr {
namespace linecode {

class correlate_symbol_seq_tag_ff_impl : public correlate_symbol_seq_tag_ff
{
private:
    std::vector<float> d_symbols;
    volk::vector<float> d_corr;
    volk::vector<float> d_corr_mag;
    float d_thresh, d_stashed_threshold;
    gr::filter::kernel::fft_filter_fff d_filter;

    pmt::pmt_t d_key, d_me; // d_key is the tag name, d_me is the block name + unique ID

    gr::thread::mutex d_mutex_symbol_seq;
    
    void _set_threshold(float threshold);
    
    static constexpr int s_nitems = 24 * 1024;

public:
    correlate_symbol_seq_tag_ff_impl(const std::vector<float>& symbols,
                                      float threshold,
                                      const std::string& tag_name);
    ~correlate_symbol_seq_tag_ff_impl() override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;

    void set_symbols(const std::vector<float>& symbols) override;    
    void set_threshold(float threshold) override;
    void set_tagname(const std::string& tag_name) override
    {
        d_key = pmt::string_to_symbol(tag_name);
    };
};

} /* namespace linecode */
} /* namespace gr */

#endif /* INCLUDED_LINECODE_CORRELATE_ACCESS_CODE_TAG_FF_IMPL_H */
