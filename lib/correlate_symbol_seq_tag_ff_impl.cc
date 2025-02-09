/* -*- c++ -*- */
/*
 * Copyright 2004,2006,2010-2012,2017 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "correlate_symbol_seq_tag_ff_impl.h"
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/pfb_arb_resampler.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <volk/volk.h>
#include <boost/format.hpp>
#include <cstdio>
#include <stdexcept>

namespace gr {
namespace linecode {

correlate_symbol_seq_tag_ff::sptr correlate_symbol_seq_tag_ff::make(
    const std::vector<float>& symbols, float threshold, const std::string& tag_name)
{
    return gnuradio::make_block_sptr<correlate_symbol_seq_tag_ff_impl>(
        symbols, threshold, tag_name);
}


correlate_symbol_seq_tag_ff_impl::correlate_symbol_seq_tag_ff_impl(
    const std::vector<float>& symbols, float threshold, const std::string& tag_name)
    : sync_block("correlate_symbol_seq_tag_ff",
                 gr::io_signature::make(1, 1, sizeof(float)),
                 gr::io_signature::make(1, 1, sizeof(float))),
      d_symbols(symbols),
      d_filter(1, symbols), // taps will be re-set later in the constructor.
      d_corr(s_nitems),
      d_corr_mag(s_nitems)
{

    std::stringstream str;
    str << name() << unique_id();
    d_me = pmt::string_to_symbol(str.str());
    d_key = pmt::string_to_symbol(tag_name);
    
    // Create time-reversed conjugate of symbols
    std::reverse(d_symbols.begin(), d_symbols.end());
    
    set_threshold(threshold);
    
    // We'll (ab)use the history for our own purposes of tagging back in time.
    // Keep a history of the length of the sync word to delay for tagging.
    set_history(d_symbols.size() + 1);
    
    const int nsamples = d_filter.set_taps(d_symbols);
    set_output_multiple(nsamples);

}

correlate_symbol_seq_tag_ff_impl::~correlate_symbol_seq_tag_ff_impl() {}

void correlate_symbol_seq_tag_ff_impl::set_symbols(const std::vector<float>& symbols)
{
    gr::thread::scoped_lock lock(d_setlock);

    d_symbols = symbols;

    // Per comments in gr-filter/include/gnuradio/filter/fft_filter.h,
    // set the block output multiple to the FFT filter kernel's internal,
    // assumed "nsamples", to ensure the scheduler always passes a
    // proper number of samples.
    const int nsamples = d_filter.set_taps(d_symbols);
    set_output_multiple(nsamples);

    // It looks like the kernel::fft_filter_ccc stashes a tail between
    // calls, so that contains our filtering history (I think).  The
    // fft_filter_ccc block (which calls the kernel::fft_filter_ccc) sets
    // the history to 1 (0 history items), so let's follow its lead.
    // set_history(1);

    // We'll (ab)use the history for our own purposes of tagging back in time.
    // Keep a history of the length of the sync word to delay for tagging.
    set_history(d_symbols.size() + 1);

    _set_threshold(d_stashed_threshold);
}

void correlate_symbol_seq_tag_ff_impl::_set_threshold(float threshold)
{
    d_stashed_threshold = threshold;

    // Compute a correlation threshold.
    // Compute the value of the discrete autocorrelation of the matched
    // filter with offset 0 (aka the autocorrelation peak).
    float corr = 0;
    for (size_t i = 0; i < d_symbols.size(); i++)
        corr += abs(d_symbols[i] * d_symbols[i]);
    d_thresh = threshold * corr * corr;
    
}

void correlate_symbol_seq_tag_ff_impl::set_threshold(float threshold)
{
    gr::thread::scoped_lock lock(d_setlock);
    _set_threshold(threshold);
}

int correlate_symbol_seq_tag_ff_impl::work(int noutput_items,
                                            gr_vector_const_void_star& input_items,
                                            gr_vector_void_star& output_items)
{
    gr::thread::scoped_lock l(d_mutex_symbol_seq);

    const float* in = (const float*)input_items[0];
    float* out = (float*)output_items[0];
    
    uint64_t abs_out_sample_cnt = nitems_written(0);

    float* corr = d_corr.data();    
    // Our correlation filter length
    unsigned int hist_len = history() - 1;

    // Calculate the correlation of the non-delayed input with the
    // known symbols.
    d_filter.filter(noutput_items, &in[hist_len], corr);
    // Find the magnitude squared of the correlation
    //volk_32fc_magnitude_squared_32f(&d_corr_mag[0], corr, noutput_items);
    volk_32f_x2_multiply_32f(&d_corr_mag[0], corr, corr, noutput_items);
    
    for (int i = 0; i < noutput_items; i++) {
        out[i] = in[i];

        if (d_corr_mag[i] > d_thresh) {
            GR_LOG_DEBUG(d_logger,
                         boost::format("writing tag at sample %llu") %
                             (abs_out_sample_cnt + i));
            add_item_tag(0,                      // stream ID
                         abs_out_sample_cnt + i, // sample
                         d_key,                  // frame info
                         pmt::from_float(d_corr_mag[i]), // data
                         d_me                    // block src id
            );
        }
    }

    return noutput_items;
}

} /* namespace linecode */
} /* namespace gr */
