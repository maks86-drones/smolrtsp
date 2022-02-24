/**
 * @file
 * @brief An RTSP request line.
 */

#pragma once

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/method.h>
#include <smolrtsp/types/request_uri.h>
#include <smolrtsp/types/rtsp_version.h>

#include <stdbool.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * An RTSP request line.
 */
typedef struct {
    /**
     * The method used.
     */
    SmolRTSP_Method method;

    /**
     * The request URI.
     */
    SmolRTSP_RequestUri uri;

    /**
     * The RTSP version used.
     */
    SmolRTSP_RtspVersion version;
} SmolRTSP_RequestLine;

/**
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_RequestLine_serialize(
    SmolRTSP_RequestLine self, SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_RequestLine_parse(
    SmolRTSP_RequestLine *restrict self,
    CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 */
bool SmolRTSP_RequestLine_eq(SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs)
    SMOLRTSP_PRIV_MUST_USE;
