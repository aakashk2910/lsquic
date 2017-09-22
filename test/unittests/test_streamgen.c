/* Copyright (c) 2017 LiteSpeed Technologies Inc.  See LICENSE. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/time.h>

#include "lsquic_types.h"
#include "lsquic_alarmset.h"
#include "lsquic_parse.h"
#include "lsquic_sfcw.h"
#include "lsquic_stream.h"
#include "lsquic.h"

struct test {
    int             lineno;
    const struct parse_funcs *
                    pf;
    /* Inputs.  These are returned by lsquic_stream_tosend_fin(),
     * lsquic_stream_tosend_read(), and lsquic_stream_tosend_offset().
     */
    int             fin[2];         /* There may be two calls to lsquic_stream_tosend_fin() */
    uint64_t        offset;
    uint32_t        stream_id;
    size_t          avail;          /* Space to write stream frame to */
    size_t          min_sz;         /* Minimum size needed to generate stream frame.  Any
                                     * sizes smaller than this should fail.
                                     */
    size_t          data_sz;
    char            data[0x100];

    /* Output.  This is how we expect the resulting frame to look.
     */
    int             len;            /* Return value of gen_stream_frame() */
    char            out[0x100];
};


static const struct test tests[] = {
    /*
     * Little-endian:
     */
    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 1, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x1C | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,   /* Offset */
          0x0A, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x1C | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,   /* Offset */
          0x0A, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 1, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x1C | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,   /* Offset */
          0x00, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2,
        .min_sz     = 1 + 2 + 8 + 2,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 1, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x21,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x1C | 0x00,
          0x21,                                             /* Stream ID */
          0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,   /* Offset */
          0x00, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 1 + 8 + 2,
        .min_sz     = 1 + 1 + 8 + 2,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 0, },
        .offset     = 0x77,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x04 | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0x77, 0x00,                                       /* Offset */
          0x0A, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 2 + 2 + 10,
        .min_sz     = 1 + 2 + 2 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 0, },
        .offset     = 0x0,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x00 | 0x01,
          0x10, 0x02,                                       /* Stream ID */
                                                            /* Offset */
          0x0A, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 0 + 2 + 10,
        .min_sz     = 1 + 2 + 0 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 1, },
        .offset     = 0x0,
        .stream_id  = 0x210,
        .data_sz    = 1,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x00 | 0x01,
          0x10, 0x02,                                       /* Stream ID */
                                                            /* Offset */
          0x01, 0x00,                                       /* Data length */
          '0',
        },
        .len        = 1 + 2 + 0 + 2 + 1,
        .min_sz     = 1 + 2 + 0 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x08 | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0xFF, 0xFF, 0xFF,                                 /* Offset */
          0x0A, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 3 + 2 + 10,
        .min_sz     = 1 + 2 + 3 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF + 1,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x0C | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0x00, 0x00, 0x00, 0x01,                           /* Offset */
          0x0A, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 4 + 2 + 10,
        .min_sz     = 1 + 2 + 4 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF + 1,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 10,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x00 | 0x0C | 0x01,
          0x10, 0x02,                                       /* Stream ID */
          0x00, 0x00, 0x00, 0x01,                           /* Offset */
          '0', '1', '2',
        },
        .len        = 1 + 2 + 4 + 0 + 3,
        .min_sz     = 1 + 2 + 4 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_037),
        .fin        = { 1, 0, },
        .offset     = 0xB4,
        .stream_id  = 0x01,
        .data_sz    = 0,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x04 | 0x00,
          0x01,                                             /* Stream ID */
          0xB4, 0x00,                                       /* Offset */
          0x00, 0x00,                                       /* Data length */
        },
        .len        = 6,
        .min_sz     = 6,
    },

    /*
     * Big-endian:
     */
    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 1, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x1C | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          0x00, 0x0A,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x1C | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          0x00, 0x0A,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 1, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x1C | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          0x00, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2,
        .min_sz     = 1 + 2 + 8 + 2,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 1, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x21,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x1C | 0x00,
          0x21,                                             /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          0x00, 0x00,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 1 + 8 + 2,
        .min_sz     = 1 + 1 + 8 + 2,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 0, },
        .offset     = 0x77,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x04 | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0x00, 0x77,                                       /* Offset */
          0x00, 0x0A,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 2 + 2 + 10,
        .min_sz     = 1 + 2 + 2 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 0, },
        .offset     = 0x0,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x00 | 0x01,
          0x02, 0x10,                                       /* Stream ID */
                                                            /* Offset */
          0x00, 0x0A,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 0 + 2 + 10,
        .min_sz     = 1 + 2 + 0 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 1, },
        .offset     = 0x0,
        .stream_id  = 0x210,
        .data_sz    = 1,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x00 | 0x01,
          0x02, 0x10,                                       /* Stream ID */
                                                            /* Offset */
          0x00, 0x01,                                       /* Data length */
          '0',
        },
        .len        = 1 + 2 + 0 + 2 + 1,
        .min_sz     = 1 + 2 + 0 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x08 | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0xFF, 0xFF, 0xFF,                                 /* Offset */
          0x00, 0x0A,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 3 + 2 + 10,
        .min_sz     = 1 + 2 + 3 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF + 1,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x20 | 0x0C | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0x01, 0x00, 0x00, 0x00,                           /* Offset */
          0x00, 0x0A,                                       /* Data length */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 4 + 2 + 10,
        .min_sz     = 1 + 2 + 4 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF + 1,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 10,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x00 | 0x00 | 0x0C | 0x01,
          0x02, 0x10,                                       /* Stream ID */
          0x01, 0x00, 0x00, 0x00,                           /* Offset */
          '0', '1', '2',
        },
        .len        = 1 + 2 + 4 + 0 + 3,
        .min_sz     = 1 + 2 + 4 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_039),
        .fin        = { 1, 0, },
        .offset     = 0xB4,
        .stream_id  = 0x01,
        .data_sz    = 0,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  1      f      d      ooo    ss            1fdoooss */
      /*  TYPE   FIN    DLEN   OLEN   SLEN  */
        { 0x80 | 0x40 | 0x20 | 0x04 | 0x00,
          0x01,                                             /* Stream ID */
          0x00, 0xB4,                                       /* Offset */
          0x00, 0x00,                                       /* Data length */
        },
        .len        = 6,
        .min_sz     = 6,
    },

    /*
     * IETF:
     */
    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 1, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 1 << 5 | 1 << 3 | 3 << 1 | 1,
          0x00, 0x0A,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 0 << 5 | 1 << 3 | 3 << 1 | 1,
          0x00, 0x0A,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 1, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 1 << 5 | 1 << 3 | 3 << 1 | 1,
          0x00, 0x00,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2,
        .min_sz     = 1 + 2 + 8 + 2,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 1, 0, },
        .offset     = 0x0807060504030201UL,
        .stream_id  = 0x21,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 1 << 5 | 0 << 3 | 3 << 1 | 1,
          0x00, 0x00,                                       /* Data length */
          0x21,                                             /* Stream ID */
          0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,   /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 1 + 8 + 2,
        .min_sz     = 1 + 1 + 8 + 2,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 0, },
        .offset     = 0x77,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 0 << 5 | 1 << 3 | 1 << 1 | 1,
          0x00, 0x0A,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
          0x00, 0x77,                                       /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 2 + 2 + 10,
        .min_sz     = 1 + 2 + 2 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 0, },
        .offset     = 0x0,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 0 << 5 | 1 << 3 | 0 << 1 | 1,
          0x00, 0x0A,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
                                                            /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 0 + 2 + 10,
        .min_sz     = 1 + 2 + 0 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 1, },
        .offset     = 0x0,
        .stream_id  = 0x210,
        .data_sz    = 1,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 1 << 5 | 1 << 3 | 0 << 1 | 1,
          0x00, 0x01,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
                                                            /* Offset */
          '0',
        },
        .len        = 1 + 2 + 0 + 2 + 1,
        .min_sz     = 1 + 2 + 0 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFF,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 0 << 5 | 1 << 3 | 2 << 1 | 1,
          0x00, 0x0A,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
          0x00, 0xFF, 0xFF, 0xFF,                           /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 4 + 2 + 10,
        .min_sz     = 1 + 2 + 4 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFFFFULL + 1,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 0 << 5 | 1 << 3 | 3 << 1 | 1,
          0x00, 0x0A,                                       /* Data length */
          0x02, 0x10,                                       /* Stream ID */
          0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,   /* Offset */
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        },
        .len        = 1 + 2 + 8 + 2 + 10,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 0, 0, },
        .offset     = 0xFFFFFFFFULL + 1,
        .stream_id  = 0x210,
        .data_sz    = 10,
        .data       = "0123456789",
        .avail      = 14,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 0 << 5 | 1 << 3 | 3 << 1 | 0,
          0x02, 0x10,                                       /* Stream ID */
          0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,   /* Offset */
          '0', '1', '2',
        },
        .len        = 1 + 2 + 8 + 0 + 3,
        .min_sz     = 1 + 2 + 8 + 0 + 1,
    },

    {   .lineno     = __LINE__,
        .pf         = select_pf_by_ver(LSQVER_040),
        .fin        = { 1, 0, },
        .offset     = 0xB4,
        .stream_id  = 0x01,
        .data_sz    = 0,
        .data       = "0123456789",
        .avail      = 0x100,
        .out        =
      /*  11     F        SS       OO       D            11FSSOOD */
      /*  TYPE   FIN      SLEN     OLEN     DLEN  */
        { 0xC0 | 1 << 5 | 0 << 3 | 1 << 1 | 1,
          0x00, 0x00,                                       /* Data length */
          0x01,                                             /* Stream ID */
          0x00, 0xB4,                                       /* Offset */
        },
        .len        = 6,
        .min_sz     = 6,
    },

};


static struct test_ctx {
    const struct test *test;
    int                next_fin;
    lsquic_stream_t    stream;
} test_ctx;


static int
stream_tosend_fin (void *stream)
{
    struct test_ctx *test_ctx = stream;
    return test_ctx->test->fin[ test_ctx->next_fin++ ];
}


static size_t
stream_tosend_read (void *stream, void *buf, size_t len, int *reached_fin)
{
    struct test_ctx *test_ctx = stream;
    if (test_ctx->test->data_sz < len)
        len = test_ctx->test->data_sz;
    memcpy(buf, test_ctx->test->data, len);
    *reached_fin = stream_tosend_fin(stream);
    return len;
}


static size_t
stream_tosend_size (void *stream)
{
    struct test_ctx *test_ctx = stream;
    return test_ctx->test->data_sz;
}


static void
reset_ctx (const struct test *test)
{
    test_ctx.test      = test;
    test_ctx.next_fin  = 0;
    test_ctx.stream.id = test->stream_id;
}


static void
run_test (int i)
{
    const struct test *const test = &tests[i];

    unsigned char out[0x100];
    int len;
    size_t min;

    if (test->len > 0)
    {
        /* Test that all sizes under specified min fail to produce a frame */
        for (min = 0; min < test->min_sz; ++min)
        {
            reset_ctx(test);
            len = test->pf->pf_gen_stream_frame(out, min, test->stream_id, test_ctx.test->offset,
                            stream_tosend_fin, stream_tosend_size, stream_tosend_read, &test_ctx);
            assert(-1 == len);
        }

        /* Test that it succeeds now: */
        reset_ctx(test);
        len = test->pf->pf_gen_stream_frame(out, min, test->stream_id, test_ctx.test->offset,
                        stream_tosend_fin, stream_tosend_size, stream_tosend_read, &test_ctx);
        assert(len == (int) min);
    }

    reset_ctx(test);
    len = test->pf->pf_gen_stream_frame(out, test->avail, test->stream_id, test_ctx.test->offset,
                    stream_tosend_fin, stream_tosend_size, stream_tosend_read, &test_ctx);

    if (test->len > 0) {
        /* Check parser operation */
        assert(test->len == len);
        assert(("Generated frame is correct", 0 == memcmp(test->out, out, test->len)));
    }
    else
    {
        assert(("This test should fail", len < 0));
    }
}


int
main (void)
{
    unsigned i;
    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i)
        run_test(i);
    return 0;
}