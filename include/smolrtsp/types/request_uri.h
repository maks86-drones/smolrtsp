/**
 * @file
 * An RTSP request URI.
 */

#pragma once

#include <smolrtsp/types/error.h>

#include <slice99.h>

/**
 * An RTSP request URI.
 */
typedef CharSlice99 SmolRTSP_RequestURI;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult
SmolRTSP_RequestURI_parse(SmolRTSP_RequestURI *restrict self, CharSlice99 input);
