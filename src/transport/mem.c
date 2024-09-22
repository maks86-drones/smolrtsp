#include <smolrtsp/transport.h>

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <sys/types.h>

#define MAX_RETRANSMITS 10

typedef struct {
    send_fn_t send;
    void *arg;
    size_t max_packet_size;
    uint8_t *packet_buffer;
} SmolRTSP_MemTransport;

declImpl(SmolRTSP_Transport, SmolRTSP_MemTransport);

SmolRTSP_Transport smolrtsp_transport_mem(size_t max_packet_size, send_fn_t send_fn, void *arg) {
    assert(send_fn != NULL);
    assert(max_packet_size > 0);

    SmolRTSP_MemTransport *self = malloc(sizeof *self);
    assert(self);
    self->send = send_fn;
    self->arg = arg;
    self->max_packet_size = max_packet_size;
    self->packet_buffer = (uint8_t*)malloc(max_packet_size);

    return DYN(SmolRTSP_MemTransport, SmolRTSP_Transport, self);
}

static void SmolRTSP_MemTransport_drop(VSelf) {
    VSELF(SmolRTSP_MemTransport);
    assert(self);

    free(self->packet_buffer);
    free(self);
}

impl(SmolRTSP_Droppable, SmolRTSP_MemTransport);

static int SmolRTSP_MemTransport_transmit(VSelf, SmolRTSP_IoVecSlice bufs, bool idr_slice) {
    VSELF(SmolRTSP_MemTransport);
    assert(self);
    assert(self->send);

    uint8_t *p = self->packet_buffer;
    assert(p);

    size_t offset = 0U;
    for (size_t i = 0U; i < bufs.len; ++i) {
        size_t const buff_size = bufs.ptr[i].iov_len;
        size_t next_offs = offset + buff_size;
        if (next_offs > self->max_packet_size) {
            // printf("========> RTP packet was fragmented: max packet: %d, next: %d, len: %d\n", self->max_packet_size, next_offs, bufs.ptr[i].iov_len);
            if (!self->send(p, offset, true, self->arg)) {
                return -1;
            }
            offset = 0;
            next_offs = buff_size;
        }

        memcpy(p + offset, bufs.ptr[i].iov_base, buff_size);
        offset = next_offs;
    }

    if (offset > 0) {
        return self->send(p, offset, idr_slice, self->arg);
    } else {
        return 0;
    }
}

static bool SmolRTSP_MemTransport_is_full(VSelf) {
    VSELF(SmolRTSP_MemTransport);
    (void)self;

    return false;
}

impl(SmolRTSP_Transport, SmolRTSP_MemTransport);
