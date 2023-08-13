/* -*- c++ -*- */

#define LINECODE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "linecode_swig_doc.i"

%{
#include "linecode/softuart_rx.h"
#include "linecode/softuart_tx.h"
%}


%include "linecode/softuart_rx.h"
GR_SWIG_BLOCK_MAGIC2(linecode, softuart_rx);
%include "linecode/softuart_tx.h"
GR_SWIG_BLOCK_MAGIC2(linecode, softuart_tx);
