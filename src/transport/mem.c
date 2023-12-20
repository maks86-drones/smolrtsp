#include <smolrtsp/transport.h>

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include <sys/types.h>

#define MAX_RETRANSMITS 10

typedef int (*send_fn_t)(uint8_t const* data, size_t size, bool last);

typedef struct {
    send_fn_t send;
} SmolRTSP_MemTransport;

declImpl(SmolRTSP_Transport, SmolRTSP_MemTransport);

SmolRTSP_Transport smolrtsp_transport_mem(int (*send_fn)(uint8_t const* data, size_t size, bool last)) {
    assert(fn != NULL);

    SmolRTSP_MemTransport *self = malloc(sizeof *self);
    assert(self);
    self->send = fn;

    return DYN(SmolRTSP_MemTransport, SmolRTSP_Transport, self);
}

static void SmolRTSP_MemTransport_drop(VSelf) {
    VSELF(SmolRTSP_UdpTransport);
    assert(self);

    free(self);
}

impl(SmolRTSP_Droppable, SmolRTSP_MemTransport);

static int SmolRTSP_MemTransport_transmit(VSelf, SmolRTSP_IoVecSlice bufs) {
    VSELF(SmolRTSP_MemTransport);
    assert(self);

    for (size_t i = 0U; i < bufs.len; ++i) {
        int const ret = self->send(bufs.ptr->iov_base, bufs.ptr->iov_len, i == (bufs.len - 1U));
        if (ret != 0) {
            return ret;
        }
    }

    return 0;
}

static bool SmolRTSP_MemTransport_is_full(VSelf) {
    VSELF(SmolRTSP_UdpTransport);
    (void)self;

    return false;
}

impl(SmolRTSP_Transport, SmolRTSP_MemTransport);
