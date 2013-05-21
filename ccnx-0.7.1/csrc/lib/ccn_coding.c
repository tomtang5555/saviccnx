/**
 * @file ccn_coding.c
 * @brief Support for scanning and parsing ccnb-encoded data.
 * 
 * Part of the CCNx C Library.
 *
 * Copyright (C) 2008, 2009 Palo Alto Research Center, Inc.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details. You should have received
 * a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <ccn/coding.h>
//TANG
#include <stdio.h>

/**
 * This macro documents what's happening in the state machine by
 * hinting at the XML syntax would be emitted in a re-encoder.
 * But it actually does nothing.
 */
#define XML(goop) ((void)0)

/**
 * This macro defines whether or not use optimization code written by TT.
 */
#define OPTI_TT

/**
 * Decodes ccnb decoded data
 *
 * @param d holds the current state of the decoder.
 * @param p points to a new block of ccnb data to feed to the decoder.
 * @param n is the size of the input, in bytes.
 * @returns the number of bytes consumed.
 *
 * The client should ensure that the decoder is initialized to all zero
 * before the first call.  In the default mode, the decoder will return
 * only when it runs out of data, encounters an error, or reaches the end
 * of the element that it started at.  This is a good way to pull
 * ccnb-encoded objects from a byte stream.
 *
 * By setting the CCN_DSTATE_PAUSE bit is set in the decoder state, the
 * decoder will additionally return just after recognizing each token.
 * In this instance, use CCN_GET_TT_FROM_DSTATE() to extract
 * the token type from the decoder state;
 * CCN_CLOSE will be reported as CCN_NO_TOKEN.
 *
 * The pause bit persists, so the end test should take that into account
 * by using the CCN_FINAL_DSTATE() macro instead of testing for state 0.
 *
 * Once an error state is entered, no addition input is processed.
 *
 * @see ccn_buf_decoder_start(), ccn_buf_advance(), ccn_buf_check_close()
 */
ssize_t
ccn_skeleton_decode(struct ccn_skeleton_decoder *d,
                    const unsigned char *p, size_t n)
{
    // TANG
    //int j;
    //printf("%p,%zu,%d,%d,%zu,%zu,%zu,%zu,%p,",
    //        d, d->index, d->state, d->nest, d->numval, d->token_index, d->element_index, n, p);
    //for (j = 0; j < n; j++)
    //    printf("%02x", p[j]);
    //printf(",|,");
#ifdef OPTI_TT
    if (n == 0)
        return 0;
    switch (d->state) {
//    case 0: //1,585
//        break;
    case 32768: //70,678
        switch (p[0]) {
        case 0xf2:
            d->index += 1;
            d->state = 164097;
            d->nest += 1;
            d->numval = 14;
            return (1);
            break;
        case 0x01:
            if (p[1] == 0xd2){
                d->index += 2;
                d->state = 164097;
                d->nest += 1;
                d->numval = 26;
                return (2);
            }
            break;
        case 0x04:
            if (p[1] == 0x82){
                d->index += 2;
                d->state = 164097;
                d->nest += 1;
                d->numval = 64;
                return (2);
            }
            break;
        case 0xfa:
            d->index += 1;
            d->state = 164097;
            d->nest += 1;
            d->numval = 15;
            return (1);
            break;
        default:
            break;
        }
        break;

    case 164097: //234,787
        switch (p[0]) {
        case 0xfa: //1, 66817
            d->token_index = d->index;
            d->element_index = d->index;
            d->index += 1;
            //d->state = 164097;
            d->nest += 1;
            d->numval = 15;
            return (1);
            break;
        case 0x9d: //1, 65933
            d->token_index = d->index;
            //d->element_index = 0 //d->index;
            d->index += 1;
            d->state = 360454;
            //d->nest += 0;
            d->numval = 3;
            return (1);
            break;
        case 0x95: //1, 59688
            d->token_index = d->index;
            //d->element_index = 0 //d->index;
            d->index += 1;
            d->state = 360454;
            //d->nest += 0;
            d->numval = 2;
            return (1);
            break;
        case 0x8d: //1, 20362
            d->token_index = d->index;
            //d->element_index = 0 //d->index;
            d->index += 1;
            d->state = 360454;
            //d->nest += 0;
            d->numval = 1;
            return (1);
            break;
        case 0xf2: //1, 2367
            d->token_index = d->index;
            d->element_index = d->index;
            d->index += 1;
            //d->state = 164097;
            d->nest += 1;
            d->numval = 14;
            return (1);
            break;
        case 0x8e: //1, 2352
            d->token_index = d->index;
            //d->element_index = d->index;
            d->index += 1;
            d->state = 425987;
            //d->nest += 1;
            d->numval = 1;
            return (1);
            break;
        case 0xb5: //1, 1507
            d->token_index = d->index;
            //d->element_index = d->index;
            d->index += 1;
            d->state = 360454;
            //d->nest += ;
            d->numval = 6;
            return (1);
            break;
        //case 0xa5: //1, 899
        //    break;
        default:
            break;
        }
        break;

    case 360454: //157,193
        break;
    case 491521: //107,110
        break;
    case 491520: //3,921
        break;
    case 425987: //2,363
        break;
    default:
        break;
    }
#endif

    enum ccn_decoder_state state = d->state;
    int tagstate = 0;
    size_t numval = d->numval;
    ssize_t i = 0;
    unsigned char c;
    size_t chunk;
    int pause = 0;
    if (d->state >= 0) {
        pause = d->state & CCN_DSTATE_PAUSE;
        tagstate = (d->state >> 8) & 3;
        state = d->state & 0xFF;
    }
    while (i < n) {
        switch (state) {
            case CCN_DSTATE_INITIAL:
            case CCN_DSTATE_NEWTOKEN: /* start new thing */
                d->token_index = i + d->index;
                if (tagstate > 1 && tagstate-- == 2) {
                    XML("\""); /* close off the attribute value */
                } 
                if (p[i] == CCN_CLOSE) {
                    i++;
                    if (d->nest <= 0 || tagstate > 1) {
                        state = CCN_DSTATE_ERR_NEST;
                        break;
                    }
                    if (tagstate == 1) {
                        tagstate = 0;
                        XML("/>");
                    }
                    else {
                        XML("</%s>");
                    }
                    d->nest -= 1;
                    if (d->nest == 0) {
                        state = CCN_DSTATE_INITIAL;
                        n = i;
                    }
                    if (pause) {
                        state |= (((int)CCN_NO_TOKEN) << 16);
                        n = i;
                    }
                    break;
                }
                numval = 0;
                state = CCN_DSTATE_NUMVAL;
                /* FALLTHRU */
            case CCN_DSTATE_NUMVAL: /* parsing numval */
                c = p[i++];
                if ((c & CCN_TT_HBIT) == CCN_CLOSE) {
                    if (numval > ((~(size_t)0U) >> (7 + CCN_TT_BITS)))
                        state = CCN_DSTATE_ERR_OVERFLOW;
                    numval = (numval << 7) + (c & 127);
                }
                else {
                    numval = (numval << (7-CCN_TT_BITS)) +
                             ((c >> CCN_TT_BITS) & CCN_MAX_TINY);
                    c &= CCN_TT_MASK;
                    switch (c) {
                        case CCN_EXT:
                            if (tagstate == 1) {
                                tagstate = 0;
                                XML(">");
                            }
                            d->nest += 1;
                            d->element_index = d->token_index;
                            state = CCN_DSTATE_NEWTOKEN;
                            break;
                        case CCN_DTAG:
                            if (tagstate == 1) {
                                tagstate = 0;
                                XML(">");
                            }
                            d->nest += 1;
                            d->element_index = d->token_index;
                            XML("<%s");
                            tagstate = 1;
                            state = CCN_DSTATE_NEWTOKEN;
                            break;
                        case CCN_BLOB:
                            if (tagstate == 1) {
                                tagstate = 0;
                                XML(" ccnbencoding=\"base64Binary\">");
                            }
                            state = CCN_DSTATE_BLOB;
                            if (numval == 0)
                                state = CCN_DSTATE_NEWTOKEN;
                            break;
                        case CCN_UDATA:
                            if (tagstate == 1) {
                                tagstate = 0;
                                XML(">");
                            }
                            state = CCN_DSTATE_UDATA;
                            if (numval == 0)
                                state = CCN_DSTATE_NEWTOKEN;
                            break;
                        case CCN_DATTR:
                            if (tagstate != 1) {
                                state = CCN_DSTATE_ERR_ATTR;
                                break;
                            }
                            tagstate = 3;
                            state = CCN_DSTATE_NEWTOKEN;
                            break;
                        case CCN_ATTR:
                            if (tagstate != 1) {
                                state = CCN_DSTATE_ERR_ATTR;
                                break;
                            }
                            numval += 1; /* encoded as length-1 */
                            state = CCN_DSTATE_ATTRNAME;
                            break;
                        case CCN_TAG:
                            if (tagstate == 1) {
                                tagstate = 0;
                                XML(">");
                            }
                            numval += 1; /* encoded as length-1 */
                            d->nest += 1;
                            d->element_index = d->token_index;
                            state = CCN_DSTATE_TAGNAME;
                            break;
                        default:
                            state = CCN_DSTATE_ERR_CODING;
                    }
                    if (pause) {
                        state |= (c << 16);
                        n = i;
                    }
                }
                break;
            case CCN_DSTATE_TAGNAME: /* parsing tag name */
                chunk = n - i;
                if (chunk > numval)
                    chunk = numval;
                if (chunk == 0) {
                    state = CCN_DSTATE_ERR_BUG;
                    break;
                }
                numval -= chunk;
                i += chunk;
                if (numval == 0) {
                    if (d->nest == 0) {
                        state = CCN_DSTATE_ERR_NEST;
                        break;
                    }
                    XML("<%s");
                    tagstate = 1;
                    state = CCN_DSTATE_NEWTOKEN;
                }
                break;                
            case CCN_DSTATE_ATTRNAME: /* parsing attribute name */
                chunk = n - i;
                if (chunk > numval)
                    chunk = numval;
                if (chunk == 0) {
                    state = CCN_DSTATE_ERR_BUG;
                    break;
                }
                numval -= chunk;
                i += chunk;
                if (numval == 0) {
                    if (d->nest == 0) {
                        state = CCN_DSTATE_ERR_ATTR;
                        break;
                    }
                    XML(" %s=\"");
                    tagstate = 3;
                    state = CCN_DSTATE_NEWTOKEN;
                }
                break;
            case CCN_DSTATE_UDATA: /* utf-8 data */
            case CCN_DSTATE_BLOB: /* BLOB */
                chunk = n - i;
                if (chunk > numval)
                    chunk = numval;
                if (chunk == 0) {
                    state = CCN_DSTATE_ERR_BUG;
                    break;
                }
                numval -= chunk;
                i += chunk;
                if (numval == 0)
                    state = CCN_DSTATE_NEWTOKEN;
                break;
            default:
                n = i;
        }
    }
    if (state < 0)
        tagstate = pause = 0;
    d->state = state | pause | (tagstate << 8); 
    d->numval = numval;
    d->index += i;

    // TANG
    //printf("%p,%zu,%d,%d,%zu,%zu,%zu,%zu,\n",
    //        d, d->index, d->state, d->nest, d->numval, d->token_index, d->element_index, i);

    return(i);
}
