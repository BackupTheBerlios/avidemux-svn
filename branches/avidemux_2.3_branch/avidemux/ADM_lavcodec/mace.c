/*
 * MACE decoder
 * Copyright (c) 2002 Laszlo Torok <torokl@alpha.dfmk.hu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file mace.c
 * MACE decoder.
 */

#include "avcodec.h"

/*
 * Adapted to ffmpeg by Francois Revol <revol@free.fr>
 * (removed 68k REG stuff, changed types, added some statics and consts,
 * libavcodec api, context stuff, interlaced stereo out).
 */

static const uint16_t MACEtab1[] = { 0xfff3, 0x0008, 0x004c, 0x00de, 0x00de, 0x004c, 0x0008, 0xfff3 };

static const uint16_t MACEtab3[] = { 0xffee, 0x008c, 0x008c, 0xffee };

static const uint16_t MACEtab2[][8] = {
    { 0x0025, 0x0074, 0x00CE, 0x014A, 0xFEB5, 0xFF31, 0xFF8B, 0xFFDA },
    { 0x0027, 0x0079, 0x00D8, 0x015A, 0xFEA5, 0xFF27, 0xFF86, 0xFFD8 },
    { 0x0029, 0x007F, 0x00E1, 0x0169, 0xFE96, 0xFF1E, 0xFF80, 0xFFD6 },
    { 0x002A, 0x0084, 0x00EB, 0x0179, 0xFE86, 0xFF14, 0xFF7B, 0xFFD5 },
    { 0x002C, 0x0089, 0x00F5, 0x0188, 0xFE77, 0xFF0A, 0xFF76, 0xFFD3 },
    { 0x002E, 0x0090, 0x0100, 0x019A, 0xFE65, 0xFEFF, 0xFF6F, 0xFFD1 },
    { 0x0030, 0x0096, 0x010B, 0x01AC, 0xFE53, 0xFEF4, 0xFF69, 0xFFCF },
    { 0x0033, 0x009D, 0x0118, 0x01C1, 0xFE3E, 0xFEE7, 0xFF62, 0xFFCC },
    { 0x0035, 0x00A5, 0x0125, 0x01D6, 0xFE29, 0xFEDA, 0xFF5A, 0xFFCA },
    { 0x0037, 0x00AC, 0x0132, 0x01EA, 0xFE15, 0xFECD, 0xFF53, 0xFFC8 },
    { 0x003A, 0x00B3, 0x013F, 0x01FF, 0xFE00, 0xFEC0, 0xFF4C, 0xFFC5 },
    { 0x003C, 0x00BB, 0x014D, 0x0216, 0xFDE9, 0xFEB2, 0xFF44, 0xFFC3 },
    { 0x003F, 0x00C3, 0x015C, 0x022D, 0xFDD2, 0xFEA3, 0xFF3C, 0xFFC0 },
    { 0x0042, 0x00CD, 0x016C, 0x0247, 0xFDB8, 0xFE93, 0xFF32, 0xFFBD },
    { 0x0045, 0x00D6, 0x017C, 0x0261, 0xFD9E, 0xFE83, 0xFF29, 0xFFBA },
    { 0x0048, 0x00DF, 0x018C, 0x027B, 0xFD84, 0xFE73, 0xFF20, 0xFFB7 },
    { 0x004B, 0x00E9, 0x019E, 0x0297, 0xFD68, 0xFE61, 0xFF16, 0xFFB4 },
    { 0x004F, 0x00F4, 0x01B1, 0x02B6, 0xFD49, 0xFE4E, 0xFF0B, 0xFFB0 },
    { 0x0052, 0x00FE, 0x01C5, 0x02D5, 0xFD2A, 0xFE3A, 0xFF01, 0xFFAD },
    { 0x0056, 0x0109, 0x01D8, 0x02F4, 0xFD0B, 0xFE27, 0xFEF6, 0xFFA9 },
    { 0x005A, 0x0116, 0x01EF, 0x0318, 0xFCE7, 0xFE10, 0xFEE9, 0xFFA5 },
    { 0x005E, 0x0122, 0x0204, 0x033A, 0xFCC5, 0xFDFB, 0xFEDD, 0xFFA1 },
    { 0x0062, 0x012F, 0x021A, 0x035E, 0xFCA1, 0xFDE5, 0xFED0, 0xFF9D },
    { 0x0066, 0x013C, 0x0232, 0x0385, 0xFC7A, 0xFDCD, 0xFEC3, 0xFF99 },
    { 0x006B, 0x014B, 0x024C, 0x03AE, 0xFC51, 0xFDB3, 0xFEB4, 0xFF94 },
    { 0x0070, 0x0159, 0x0266, 0x03D7, 0xFC28, 0xFD99, 0xFEA6, 0xFF8F },
    { 0x0075, 0x0169, 0x0281, 0x0403, 0xFBFC, 0xFD7E, 0xFE96, 0xFF8A },
    { 0x007A, 0x0179, 0x029E, 0x0432, 0xFBCD, 0xFD61, 0xFE86, 0xFF85 },
    { 0x007F, 0x018A, 0x02BD, 0x0463, 0xFB9C, 0xFD42, 0xFE75, 0xFF80 },
    { 0x0085, 0x019B, 0x02DC, 0x0494, 0xFB6B, 0xFD23, 0xFE64, 0xFF7A },
    { 0x008B, 0x01AE, 0x02FC, 0x04C8, 0xFB37, 0xFD03, 0xFE51, 0xFF74 },
    { 0x0091, 0x01C1, 0x031F, 0x0500, 0xFAFF, 0xFCE0, 0xFE3E, 0xFF6E },
    { 0x0098, 0x01D5, 0x0343, 0x0539, 0xFAC6, 0xFCBC, 0xFE2A, 0xFF67 },
    { 0x009F, 0x01EA, 0x0368, 0x0575, 0xFA8A, 0xFC97, 0xFE15, 0xFF60 },
    { 0x00A6, 0x0200, 0x038F, 0x05B3, 0xFA4C, 0xFC70, 0xFDFF, 0xFF59 },
    { 0x00AD, 0x0217, 0x03B7, 0x05F3, 0xFA0C, 0xFC48, 0xFDE8, 0xFF52 },
    { 0x00B5, 0x022E, 0x03E1, 0x0636, 0xF9C9, 0xFC1E, 0xFDD1, 0xFF4A },
    { 0x00BD, 0x0248, 0x040E, 0x067F, 0xF980, 0xFBF1, 0xFDB7, 0xFF42 },
    { 0x00C5, 0x0262, 0x043D, 0x06CA, 0xF935, 0xFBC2, 0xFD9D, 0xFF3A },
    { 0x00CE, 0x027D, 0x046D, 0x0717, 0xF8E8, 0xFB92, 0xFD82, 0xFF31 },
    { 0x00D7, 0x0299, 0x049F, 0x0767, 0xF898, 0xFB60, 0xFD66, 0xFF28 },
    { 0x00E1, 0x02B7, 0x04D5, 0x07BC, 0xF843, 0xFB2A, 0xFD48, 0xFF1E },
    { 0x00EB, 0x02D6, 0x050B, 0x0814, 0xF7EB, 0xFAF4, 0xFD29, 0xFF14 },
    { 0x00F6, 0x02F7, 0x0545, 0x0871, 0xF78E, 0xFABA, 0xFD08, 0xFF09 },
    { 0x0101, 0x0318, 0x0581, 0x08D1, 0xF72E, 0xFA7E, 0xFCE7, 0xFEFE },
    { 0x010C, 0x033C, 0x05C0, 0x0935, 0xF6CA, 0xFA3F, 0xFCC3, 0xFEF3 },
    { 0x0118, 0x0361, 0x0602, 0x099F, 0xF660, 0xF9FD, 0xFC9E, 0xFEE7 },
    { 0x0125, 0x0387, 0x0646, 0x0A0C, 0xF5F3, 0xF9B9, 0xFC78, 0xFEDA },
    { 0x0132, 0x03B0, 0x068E, 0x0A80, 0xF57F, 0xF971, 0xFC4F, 0xFECD },
    { 0x013F, 0x03DA, 0x06D9, 0x0AF7, 0xF508, 0xF926, 0xFC25, 0xFEC0 },
    { 0x014E, 0x0406, 0x0728, 0x0B75, 0xF48A, 0xF8D7, 0xFBF9, 0xFEB1 },
    { 0x015D, 0x0434, 0x077A, 0x0BF9, 0xF406, 0xF885, 0xFBCB, 0xFEA2 },
    { 0x016C, 0x0464, 0x07CF, 0x0C82, 0xF37D, 0xF830, 0xFB9B, 0xFE93 },
    { 0x017C, 0x0496, 0x0828, 0x0D10, 0xF2EF, 0xF7D7, 0xFB69, 0xFE83 },
    { 0x018E, 0x04CB, 0x0886, 0x0DA6, 0xF259, 0xF779, 0xFB34, 0xFE71 },
    { 0x019F, 0x0501, 0x08E6, 0x0E41, 0xF1BE, 0xF719, 0xFAFE, 0xFE60 },
    { 0x01B2, 0x053B, 0x094C, 0x0EE3, 0xF11C, 0xF6B3, 0xFAC4, 0xFE4D },
    { 0x01C5, 0x0576, 0x09B6, 0x0F8E, 0xF071, 0xF649, 0xFA89, 0xFE3A },
    { 0x01D9, 0x05B5, 0x0A26, 0x1040, 0xEFBF, 0xF5D9, 0xFA4A, 0xFE26 },
    { 0x01EF, 0x05F6, 0x0A9A, 0x10FA, 0xEF05, 0xF565, 0xFA09, 0xFE10 },
    { 0x0205, 0x063A, 0x0B13, 0x11BC, 0xEE43, 0xF4EC, 0xF9C5, 0xFDFA },
    { 0x021C, 0x0681, 0x0B91, 0x1285, 0xED7A, 0xF46E, 0xF97E, 0xFDE3 },
    { 0x0234, 0x06CC, 0x0C15, 0x1359, 0xECA6, 0xF3EA, 0xF933, 0xFDCB },
    { 0x024D, 0x071A, 0x0CA0, 0x1437, 0xEBC8, 0xF35F, 0xF8E5, 0xFDB2 },
    { 0x0267, 0x076A, 0x0D2F, 0x151D, 0xEAE2, 0xF2D0, 0xF895, 0xFD98 },
    { 0x0283, 0x07C0, 0x0DC7, 0x160F, 0xE9F0, 0xF238, 0xF83F, 0xFD7C },
    { 0x029F, 0x0818, 0x0E63, 0x170A, 0xE8F5, 0xF19C, 0xF7E7, 0xFD60 },
    { 0x02BD, 0x0874, 0x0F08, 0x1811, 0xE7EE, 0xF0F7, 0xF78B, 0xFD42 },
    { 0x02DD, 0x08D5, 0x0FB4, 0x1926, 0xE6D9, 0xF04B, 0xF72A, 0xFD22 },
    { 0x02FE, 0x093A, 0x1067, 0x1A44, 0xE5BB, 0xEF98, 0xF6C5, 0xFD01 },
    { 0x0320, 0x09A3, 0x1122, 0x1B70, 0xE48F, 0xEEDD, 0xF65C, 0xFCDF },
    { 0x0344, 0x0A12, 0x11E7, 0x1CAB, 0xE354, 0xEE18, 0xF5ED, 0xFCBB },
    { 0x0369, 0x0A84, 0x12B2, 0x1DF0, 0xE20F, 0xED4D, 0xF57B, 0xFC96 },
    { 0x0390, 0x0AFD, 0x1389, 0x1F48, 0xE0B7, 0xEC76, 0xF502, 0xFC6F },
    { 0x03B8, 0x0B7A, 0x1467, 0x20AC, 0xDF53, 0xEB98, 0xF485, 0xFC47 },
    { 0x03E3, 0x0BFE, 0x1551, 0x2223, 0xDDDC, 0xEAAE, 0xF401, 0xFC1C },
    { 0x040F, 0x0C87, 0x1645, 0x23A9, 0xDC56, 0xE9BA, 0xF378, 0xFBF0 },
    { 0x043E, 0x0D16, 0x1744, 0x2541, 0xDABE, 0xE8BB, 0xF2E9, 0xFBC1 },
    { 0x046E, 0x0DAB, 0x184C, 0x26E8, 0xD917, 0xE7B3, 0xF254, 0xFB91 },
    { 0x04A1, 0x0E47, 0x1961, 0x28A4, 0xD75B, 0xE69E, 0xF1B8, 0xFB5E },
    { 0x04D6, 0x0EEA, 0x1A84, 0x2A75, 0xD58A, 0xE57B, 0xF115, 0xFB29 },
    { 0x050D, 0x0F95, 0x1BB3, 0x2C5B, 0xD3A4, 0xE44C, 0xF06A, 0xFAF2 },
    { 0x0547, 0x1046, 0x1CEF, 0x2E55, 0xD1AA, 0xE310, 0xEFB9, 0xFAB8 },
    { 0x0583, 0x1100, 0x1E3A, 0x3066, 0xCF99, 0xE1C5, 0xEEFF, 0xFA7C },
    { 0x05C2, 0x11C3, 0x1F94, 0x3292, 0xCD6D, 0xE06B, 0xEE3C, 0xFA3D },
    { 0x0604, 0x128E, 0x20FC, 0x34D2, 0xCB2D, 0xDF03, 0xED71, 0xF9FB },
    { 0x0649, 0x1362, 0x2275, 0x372E, 0xC8D1, 0xDD8A, 0xEC9D, 0xF9B6 },
    { 0x0690, 0x143F, 0x23FF, 0x39A4, 0xC65B, 0xDC00, 0xEBC0, 0xF96F },
    { 0x06DC, 0x1527, 0x259A, 0x3C37, 0xC3C8, 0xDA65, 0xEAD8, 0xF923 },
    { 0x072A, 0x1619, 0x2749, 0x3EE8, 0xC117, 0xD8B6, 0xE9E6, 0xF8D5 },
    { 0x077C, 0x1715, 0x2909, 0x41B6, 0xBE49, 0xD6F6, 0xE8EA, 0xF883 },
    { 0x07D1, 0x181D, 0x2ADF, 0x44A6, 0xBB59, 0xD520, 0xE7E2, 0xF82E },
    { 0x082B, 0x1930, 0x2CC7, 0x47B4, 0xB84B, 0xD338, 0xE6CF, 0xF7D4 },
    { 0x0888, 0x1A50, 0x2EC6, 0x4AE7, 0xB518, 0xD139, 0xE5AF, 0xF777 },
    { 0x08EA, 0x1B7D, 0x30DE, 0x4E40, 0xB1BF, 0xCF21, 0xE482, 0xF715 },
    { 0x094F, 0x1CB7, 0x330C, 0x51BE, 0xAE41, 0xCCF3, 0xE348, 0xF6B0 },
    { 0x09BA, 0x1DFF, 0x3554, 0x5565, 0xAA9A, 0xCAAB, 0xE200, 0xF645 },
    { 0x0A29, 0x1F55, 0x37B4, 0x5932, 0xA6CD, 0xC84B, 0xE0AA, 0xF5D6 },
    { 0x0A9D, 0x20BC, 0x3A31, 0x5D2E, 0xA2D1, 0xC5CE, 0xDF43, 0xF562 },
    { 0x0B16, 0x2231, 0x3CC9, 0x6156, 0x9EA9, 0xC336, 0xDDCE, 0xF4E9 },
    { 0x0B95, 0x23B8, 0x3F80, 0x65AF, 0x9A50, 0xC07F, 0xDC47, 0xF46A },
    { 0x0C19, 0x2551, 0x4256, 0x6A39, 0x95C6, 0xBDA9, 0xDAAE, 0xF3E6 },
    { 0x0CA4, 0x26FB, 0x454C, 0x6EF7, 0x9108, 0xBAB3, 0xD904, 0xF35B },
    { 0x0D34, 0x28B8, 0x4864, 0x73EB, 0x8C14, 0xB79B, 0xD747, 0xF2CB },
    { 0x0DCB, 0x2A8A, 0x4B9F, 0x7918, 0x86E7, 0xB460, 0xD575, 0xF234 },
    { 0x0E68, 0x2C6F, 0x4EFE, 0x7E7E, 0x8181, 0xB101, 0xD390, 0xF197 },
    { 0x0F0D, 0x2E6B, 0x5285, 0x7FFF, 0x8000, 0xAD7A, 0xD194, 0xF0F2 },
    { 0x0FB9, 0x307E, 0x5635, 0x7FFF, 0x8000, 0xA9CA, 0xCF81, 0xF046 },
    { 0x106D, 0x32A7, 0x5A0D, 0x7FFF, 0x8000, 0xA5F2, 0xCD58, 0xEF92 },
    { 0x1128, 0x34EA, 0x5E12, 0x7FFF, 0x8000, 0xA1ED, 0xCB15, 0xEED7 },
    { 0x11ED, 0x3747, 0x6245, 0x7FFF, 0x8000, 0x9DBA, 0xC8B8, 0xEE12 },
    { 0x12B9, 0x39BF, 0x66A8, 0x7FFF, 0x8000, 0x9957, 0xC640, 0xED46 },
    { 0x138F, 0x3C52, 0x6B3C, 0x7FFF, 0x8000, 0x94C3, 0xC3AD, 0xEC70 },
    { 0x146F, 0x3F04, 0x7006, 0x7FFF, 0x8000, 0x8FF9, 0xC0FB, 0xEB90 },
    { 0x1558, 0x41D3, 0x7505, 0x7FFF, 0x8000, 0x8AFA, 0xBE2C, 0xEAA7 },
    { 0x164C, 0x44C3, 0x7A3E, 0x7FFF, 0x8000, 0x85C1, 0xBB3C, 0xE9B3 },
    { 0x174B, 0x47D5, 0x7FB3, 0x7FFF, 0x8000, 0x804C, 0xB82A, 0xE8B4 },
    { 0x1855, 0x4B0A, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0xB4F5, 0xE7AA },
    { 0x196B, 0x4E63, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0xB19C, 0xE694 },
    { 0x1A8D, 0x51E3, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0xAE1C, 0xE572 },
    { 0x1BBD, 0x558B, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0xAA74, 0xE442 },
    { 0x1CFA, 0x595C, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0xA6A3, 0xE305 },
    { 0x1E45, 0x5D59, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0xA2A6, 0xE1BA },
    { 0x1F9F, 0x6184, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0x9E7B, 0xE060 },
    { 0x2108, 0x65DE, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0x9A21, 0xDEF7 },
    { 0x2281, 0x6A6A, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0x9595, 0xDD7E },
    { 0x240C, 0x6F29, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0x90D6, 0xDBF3 },
    { 0x25A7, 0x741F, 0x7FFF, 0x7FFF, 0x8000, 0x8000, 0x8BE0, 0xDA58 },
};

static const uint16_t MACEtab4[][8] = {
    { 0x0040, 0x00D8, 0xFF27, 0xFFBF, 0, 0, 0, 0 },  { 0x0043, 0x00E2, 0xFF1D, 0xFFBC, 0, 0, 0, 0 },
    { 0x0046, 0x00EC, 0xFF13, 0xFFB9, 0, 0, 0, 0 },  { 0x004A, 0x00F6, 0xFF09, 0xFFB5, 0, 0, 0, 0 },
    { 0x004D, 0x0101, 0xFEFE, 0xFFB2, 0, 0, 0, 0 },  { 0x0050, 0x010C, 0xFEF3, 0xFFAF, 0, 0, 0, 0 },
    { 0x0054, 0x0118, 0xFEE7, 0xFFAB, 0, 0, 0, 0 },  { 0x0058, 0x0126, 0xFED9, 0xFFA7, 0, 0, 0, 0 },
    { 0x005C, 0x0133, 0xFECC, 0xFFA3, 0, 0, 0, 0 },  { 0x0060, 0x0141, 0xFEBE, 0xFF9F, 0, 0, 0, 0 },
    { 0x0064, 0x014E, 0xFEB1, 0xFF9B, 0, 0, 0, 0 },  { 0x0068, 0x015E, 0xFEA1, 0xFF97, 0, 0, 0, 0 },
    { 0x006D, 0x016D, 0xFE92, 0xFF92, 0, 0, 0, 0 },  { 0x0072, 0x017E, 0xFE81, 0xFF8D, 0, 0, 0, 0 },
    { 0x0077, 0x018F, 0xFE70, 0xFF88, 0, 0, 0, 0 },  { 0x007C, 0x01A0, 0xFE5F, 0xFF83, 0, 0, 0, 0 },
    { 0x0082, 0x01B2, 0xFE4D, 0xFF7D, 0, 0, 0, 0 },  { 0x0088, 0x01C6, 0xFE39, 0xFF77, 0, 0, 0, 0 },
    { 0x008E, 0x01DB, 0xFE24, 0xFF71, 0, 0, 0, 0 },  { 0x0094, 0x01EF, 0xFE10, 0xFF6B, 0, 0, 0, 0 },
    { 0x009B, 0x0207, 0xFDF8, 0xFF64, 0, 0, 0, 0 },  { 0x00A2, 0x021D, 0xFDE2, 0xFF5D, 0, 0, 0, 0 },
    { 0x00A9, 0x0234, 0xFDCB, 0xFF56, 0, 0, 0, 0 },  { 0x00B0, 0x024E, 0xFDB1, 0xFF4F, 0, 0, 0, 0 },
    { 0x00B9, 0x0269, 0xFD96, 0xFF46, 0, 0, 0, 0 },  { 0x00C1, 0x0284, 0xFD7B, 0xFF3E, 0, 0, 0, 0 },
    { 0x00C9, 0x02A1, 0xFD5E, 0xFF36, 0, 0, 0, 0 },  { 0x00D2, 0x02BF, 0xFD40, 0xFF2D, 0, 0, 0, 0 },
    { 0x00DC, 0x02DF, 0xFD20, 0xFF23, 0, 0, 0, 0 },  { 0x00E6, 0x02FF, 0xFD00, 0xFF19, 0, 0, 0, 0 },
    { 0x00F0, 0x0321, 0xFCDE, 0xFF0F, 0, 0, 0, 0 },  { 0x00FB, 0x0346, 0xFCB9, 0xFF04, 0, 0, 0, 0 },
    { 0x0106, 0x036C, 0xFC93, 0xFEF9, 0, 0, 0, 0 },  { 0x0112, 0x0392, 0xFC6D, 0xFEED, 0, 0, 0, 0 },
    { 0x011E, 0x03BB, 0xFC44, 0xFEE1, 0, 0, 0, 0 },  { 0x012B, 0x03E5, 0xFC1A, 0xFED4, 0, 0, 0, 0 },
    { 0x0138, 0x0411, 0xFBEE, 0xFEC7, 0, 0, 0, 0 },  { 0x0146, 0x0441, 0xFBBE, 0xFEB9, 0, 0, 0, 0 },
    { 0x0155, 0x0472, 0xFB8D, 0xFEAA, 0, 0, 0, 0 },  { 0x0164, 0x04A4, 0xFB5B, 0xFE9B, 0, 0, 0, 0 },
    { 0x0174, 0x04D9, 0xFB26, 0xFE8B, 0, 0, 0, 0 },  { 0x0184, 0x0511, 0xFAEE, 0xFE7B, 0, 0, 0, 0 },
    { 0x0196, 0x054A, 0xFAB5, 0xFE69, 0, 0, 0, 0 },  { 0x01A8, 0x0587, 0xFA78, 0xFE57, 0, 0, 0, 0 },
    { 0x01BB, 0x05C6, 0xFA39, 0xFE44, 0, 0, 0, 0 },  { 0x01CE, 0x0608, 0xF9F7, 0xFE31, 0, 0, 0, 0 },
    { 0x01E3, 0x064D, 0xF9B2, 0xFE1C, 0, 0, 0, 0 },  { 0x01F9, 0x0694, 0xF96B, 0xFE06, 0, 0, 0, 0 },
    { 0x020F, 0x06E0, 0xF91F, 0xFDF0, 0, 0, 0, 0 },  { 0x0227, 0x072E, 0xF8D1, 0xFDD8, 0, 0, 0, 0 },
    { 0x0240, 0x0781, 0xF87E, 0xFDBF, 0, 0, 0, 0 },  { 0x0259, 0x07D7, 0xF828, 0xFDA6, 0, 0, 0, 0 },
    { 0x0274, 0x0831, 0xF7CE, 0xFD8B, 0, 0, 0, 0 },  { 0x0290, 0x088E, 0xF771, 0xFD6F, 0, 0, 0, 0 },
    { 0x02AE, 0x08F0, 0xF70F, 0xFD51, 0, 0, 0, 0 },  { 0x02CC, 0x0955, 0xF6AA, 0xFD33, 0, 0, 0, 0 },
    { 0x02EC, 0x09C0, 0xF63F, 0xFD13, 0, 0, 0, 0 },  { 0x030D, 0x0A2F, 0xF5D0, 0xFCF2, 0, 0, 0, 0 },
    { 0x0330, 0x0AA4, 0xF55B, 0xFCCF, 0, 0, 0, 0 },  { 0x0355, 0x0B1E, 0xF4E1, 0xFCAA, 0, 0, 0, 0 },
    { 0x037B, 0x0B9D, 0xF462, 0xFC84, 0, 0, 0, 0 },  { 0x03A2, 0x0C20, 0xF3DF, 0xFC5D, 0, 0, 0, 0 },
    { 0x03CC, 0x0CAB, 0xF354, 0xFC33, 0, 0, 0, 0 },  { 0x03F8, 0x0D3D, 0xF2C2, 0xFC07, 0, 0, 0, 0 },
    { 0x0425, 0x0DD3, 0xF22C, 0xFBDA, 0, 0, 0, 0 },  { 0x0454, 0x0E72, 0xF18D, 0xFBAB, 0, 0, 0, 0 },
    { 0x0486, 0x0F16, 0xF0E9, 0xFB79, 0, 0, 0, 0 },  { 0x04B9, 0x0FC3, 0xF03C, 0xFB46, 0, 0, 0, 0 },
    { 0x04F0, 0x1078, 0xEF87, 0xFB0F, 0, 0, 0, 0 },  { 0x0528, 0x1133, 0xEECC, 0xFAD7, 0, 0, 0, 0 },
    { 0x0563, 0x11F7, 0xEE08, 0xFA9C, 0, 0, 0, 0 },  { 0x05A1, 0x12C6, 0xED39, 0xFA5E, 0, 0, 0, 0 },
    { 0x05E1, 0x139B, 0xEC64, 0xFA1E, 0, 0, 0, 0 },  { 0x0624, 0x147C, 0xEB83, 0xF9DB, 0, 0, 0, 0 },
    { 0x066A, 0x1565, 0xEA9A, 0xF995, 0, 0, 0, 0 },  { 0x06B3, 0x165A, 0xE9A5, 0xF94C, 0, 0, 0, 0 },
    { 0x0700, 0x175A, 0xE8A5, 0xF8FF, 0, 0, 0, 0 },  { 0x0750, 0x1865, 0xE79A, 0xF8AF, 0, 0, 0, 0 },
    { 0x07A3, 0x197A, 0xE685, 0xF85C, 0, 0, 0, 0 },  { 0x07FB, 0x1A9D, 0xE562, 0xF804, 0, 0, 0, 0 },
    { 0x0856, 0x1BCE, 0xE431, 0xF7A9, 0, 0, 0, 0 },  { 0x08B5, 0x1D0C, 0xE2F3, 0xF74A, 0, 0, 0, 0 },
    { 0x0919, 0x1E57, 0xE1A8, 0xF6E6, 0, 0, 0, 0 },  { 0x0980, 0x1FB2, 0xE04D, 0xF67F, 0, 0, 0, 0 },
    { 0x09ED, 0x211D, 0xDEE2, 0xF612, 0, 0, 0, 0 },  { 0x0A5F, 0x2296, 0xDD69, 0xF5A0, 0, 0, 0, 0 },
    { 0x0AD5, 0x2422, 0xDBDD, 0xF52A, 0, 0, 0, 0 },  { 0x0B51, 0x25BF, 0xDA40, 0xF4AE, 0, 0, 0, 0 },
    { 0x0BD2, 0x276E, 0xD891, 0xF42D, 0, 0, 0, 0 },  { 0x0C5A, 0x2932, 0xD6CD, 0xF3A5, 0, 0, 0, 0 },
    { 0x0CE7, 0x2B08, 0xD4F7, 0xF318, 0, 0, 0, 0 },  { 0x0D7A, 0x2CF4, 0xD30B, 0xF285, 0, 0, 0, 0 },
    { 0x0E14, 0x2EF4, 0xD10B, 0xF1EB, 0, 0, 0, 0 },  { 0x0EB5, 0x310C, 0xCEF3, 0xF14A, 0, 0, 0, 0 },
    { 0x0F5D, 0x333E, 0xCCC1, 0xF0A2, 0, 0, 0, 0 },  { 0x100C, 0x3587, 0xCA78, 0xEFF3, 0, 0, 0, 0 },
    { 0x10C4, 0x37EB, 0xC814, 0xEF3B, 0, 0, 0, 0 },  { 0x1183, 0x3A69, 0xC596, 0xEE7C, 0, 0, 0, 0 },
    { 0x124B, 0x3D05, 0xC2FA, 0xEDB4, 0, 0, 0, 0 },  { 0x131C, 0x3FBE, 0xC041, 0xECE3, 0, 0, 0, 0 },
    { 0x13F7, 0x4296, 0xBD69, 0xEC08, 0, 0, 0, 0 },  { 0x14DB, 0x458F, 0xBA70, 0xEB24, 0, 0, 0, 0 },
    { 0x15C9, 0x48AA, 0xB755, 0xEA36, 0, 0, 0, 0 },  { 0x16C2, 0x4BE9, 0xB416, 0xE93D, 0, 0, 0, 0 },
    { 0x17C6, 0x4F4C, 0xB0B3, 0xE839, 0, 0, 0, 0 },  { 0x18D6, 0x52D5, 0xAD2A, 0xE729, 0, 0, 0, 0 },
    { 0x19F2, 0x5688, 0xA977, 0xE60D, 0, 0, 0, 0 },  { 0x1B1A, 0x5A65, 0xA59A, 0xE4E5, 0, 0, 0, 0 },
    { 0x1C50, 0x5E6D, 0xA192, 0xE3AF, 0, 0, 0, 0 },  { 0x1D93, 0x62A4, 0x9D5B, 0xE26C, 0, 0, 0, 0 },
    { 0x1EE5, 0x670C, 0x98F3, 0xE11A, 0, 0, 0, 0 },  { 0x2046, 0x6BA5, 0x945A, 0xDFB9, 0, 0, 0, 0 },
    { 0x21B7, 0x7072, 0x8F8D, 0xDE48, 0, 0, 0, 0 },  { 0x2338, 0x7578, 0x8A87, 0xDCC7, 0, 0, 0, 0 },
    { 0x24CB, 0x7AB5, 0x854A, 0xDB34, 0, 0, 0, 0 },  { 0x266F, 0x7FFF, 0x8000, 0xD990, 0, 0, 0, 0 },
    { 0x2826, 0x7FFF, 0x8000, 0xD7D9, 0, 0, 0, 0 },  { 0x29F1, 0x7FFF, 0x8000, 0xD60E, 0, 0, 0, 0 },
    { 0x2BD0, 0x7FFF, 0x8000, 0xD42F, 0, 0, 0, 0 },  { 0x2DC5, 0x7FFF, 0x8000, 0xD23A, 0, 0, 0, 0 },
    { 0x2FD0, 0x7FFF, 0x8000, 0xD02F, 0, 0, 0, 0 },  { 0x31F2, 0x7FFF, 0x8000, 0xCE0D, 0, 0, 0, 0 },
    { 0x342C, 0x7FFF, 0x8000, 0xCBD3, 0, 0, 0, 0 },  { 0x3681, 0x7FFF, 0x8000, 0xC97E, 0, 0, 0, 0 },
    { 0x38F0, 0x7FFF, 0x8000, 0xC70F, 0, 0, 0, 0 },  { 0x3B7A, 0x7FFF, 0x8000, 0xC485, 0, 0, 0, 0 },
    { 0x3E22, 0x7FFF, 0x8000, 0xC1DD, 0, 0, 0, 0 },  { 0x40E7, 0x7FFF, 0x8000, 0xBF18, 0, 0, 0, 0 },
};
/* end of constants */

typedef struct MACEContext {
  short index, lev, factor, prev2, previous, level;
  short *outPtr;
} MACEContext;

/* /// "chomp3()" */
static void chomp3(MACEContext *ctx,
            uint8_t val,
            const uint16_t tab1[],
            const uint16_t tab2[][8],
            uint32_t numChannels)
{
  short current;

  current=(short)tab2[(ctx->index & 0x7f0) >> 4][val];
  if (current+ctx->lev > 32767) current=32767;
  else if (current+ctx->lev < -32768) current=-32767;
  else current+=ctx->lev;
  ctx->lev=current-(current >> 3);
//  *ctx->outPtr++=current >> 8;
  *ctx->outPtr=current;
  ctx->outPtr+=numChannels;
  if ( ( ctx->index += tab1[val]-(ctx->index>>5) ) < 0 ) ctx->index = 0;
}
/* \\\ */

/* /// "Exp1to3()" */
static void Exp1to3(MACEContext *ctx,
             uint8_t *inBuffer,
             void *outBuffer,
             uint32_t cnt,
             uint32_t numChannels,
             uint32_t whichChannel)
{
   uint8_t pkt;

/*
   if (inState) {
     ctx->index=inState[0];
     ctx->lev=inState[1];
   } else
*/
   ctx->index=ctx->lev=0;

   inBuffer+=(whichChannel-1)*2;

   ctx->outPtr=outBuffer;

   while (cnt>0) {
     pkt=inBuffer[0];
     chomp3(ctx, pkt       & 7, MACEtab1, MACEtab2, numChannels);
     chomp3(ctx,(pkt >> 3) & 3, MACEtab3, MACEtab4, numChannels);
     chomp3(ctx, pkt >> 5     , MACEtab1, MACEtab2, numChannels);
     pkt=inBuffer[1];
     chomp3(ctx, pkt       & 7, MACEtab1, MACEtab2, numChannels);
     chomp3(ctx,(pkt >> 3) & 3, MACEtab3, MACEtab4, numChannels);
     chomp3(ctx, pkt >> 5     , MACEtab1, MACEtab2, numChannels);

     inBuffer+=numChannels*2;
     --cnt;
   }

/*
   if (outState) {
     outState[0]=ctx->index;
     outState[1]=ctx->lev;
   }
*/
}
/* \\\ */

/* /// "chomp6()" */
static void chomp6(MACEContext *ctx,
            uint8_t val,
            const uint16_t tab1[],
            const uint16_t tab2[][8],
            uint32_t numChannels)
{
  short current;

  current=(short)tab2[(ctx->index & 0x7f0) >> 4][val];

  if ((ctx->previous^current)>=0) {
    if (ctx->factor+506>32767) ctx->factor=32767;
    else ctx->factor+=506;
  } else {
    if (ctx->factor-314<-32768) ctx->factor=-32767;
    else ctx->factor-=314;
  }

  if (current+ctx->level>32767) current=32767;
  else if (current+ctx->level<-32768) current=-32767;
  else current+=ctx->level;

  ctx->level=((current*ctx->factor) >> 15);
  current>>=1;

//  *ctx->outPtr++=(ctx->previous+ctx->prev2-((ctx->prev2-current) >> 2)) >> 8;
//  *ctx->outPtr++=(ctx->previous+current+((ctx->prev2-current) >> 2)) >> 8;
  *ctx->outPtr=(ctx->previous+ctx->prev2-((ctx->prev2-current) >> 2));
  ctx->outPtr+=numChannels;
  *ctx->outPtr=(ctx->previous+current+((ctx->prev2-current) >> 2));
  ctx->outPtr+=numChannels;
  ctx->prev2=ctx->previous;
  ctx->previous=current;

  if( ( ctx->index += tab1[val]-(ctx->index>>5) ) < 0 ) ctx->index = 0;
}
/* \\\ */

/* /// "Exp1to6()" */
static void Exp1to6(MACEContext *ctx,
             uint8_t *inBuffer,
             void *outBuffer,
             uint32_t cnt,
             uint32_t numChannels,
             uint32_t whichChannel)
{
   uint8_t pkt;

/*
   if (inState) {
     ctx->previous=inState[0];
     ctx->prev2=inState[1];
     ctx->index=inState[2];
     ctx->level=inState[3];
     ctx->factor=inState[4];
   } else
*/
   ctx->previous=ctx->prev2=ctx->index=ctx->level=ctx->factor=0;

   inBuffer+=(whichChannel-1);
   ctx->outPtr=outBuffer;

   while (cnt>0) {
     pkt=*inBuffer;

     chomp6(ctx, pkt >> 5     , MACEtab1, MACEtab2, numChannels);
     chomp6(ctx,(pkt >> 3) & 3, MACEtab3, MACEtab4, numChannels);
     chomp6(ctx, pkt       & 7, MACEtab1, MACEtab2, numChannels);

     inBuffer+=numChannels;
     --cnt;
   }

/*
   if (outState) {
     outState[0]=ctx->previous;
     outState[1]=ctx->prev2;
     outState[2]=ctx->index;
     outState[3]=ctx->level;
     outState[4]=ctx->factor;
   }
*/
}
/* \\\ */

static int mace_decode_init(AVCodecContext * avctx)
{
    if (avctx->channels > 2)
        return -1;
    return 0;
}

static int mace_decode_frame(AVCodecContext *avctx,
                            void *data, int *data_size,
                            uint8_t *buf, int buf_size)
{
    short *samples;
    MACEContext *c = avctx->priv_data;

    samples = (short *)data;
    switch (avctx->codec->id) {
    case CODEC_ID_MACE3:
#ifdef DEBUG
puts("mace_decode_frame[3]()");
#endif
        Exp1to3(c, buf, samples, buf_size / 2, avctx->channels, 1);
        if (avctx->channels == 2)
            Exp1to3(c, buf, samples+1, buf_size / 2, 2, 2);
        *data_size = 2 * 3 * buf_size;
        break;
    case CODEC_ID_MACE6:
#ifdef DEBUG
puts("mace_decode_frame[6]()");
#endif
        Exp1to6(c, buf, samples, buf_size, avctx->channels, 1);
        if (avctx->channels == 2)
            Exp1to6(c, buf, samples+1, buf_size, 2, 2);
        *data_size = 2 * 6 * buf_size;
        break;
    default:
        return -1;
    }
    return buf_size;
}

AVCodec mace3_decoder = {
    "mace3",
    CODEC_TYPE_AUDIO,
    CODEC_ID_MACE3,
    sizeof(MACEContext),
    mace_decode_init,
    NULL,
    NULL,
    mace_decode_frame,
};

AVCodec mace6_decoder = {
    "mace6",
    CODEC_TYPE_AUDIO,
    CODEC_ID_MACE6,
    sizeof(MACEContext),
    mace_decode_init,
    NULL,
    NULL,
    mace_decode_frame,
};

