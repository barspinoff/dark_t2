
#include <fix.h>
// First, the sine table.
// The sine table is indexed by the top 8 bits of a fixang.
// Its units are fix >> 2 (so -1 = 0xc000, 0 = 0x0000, 1 = 0x4000)
// This means that the high bit is almost useless, but otherwise
//   we get results like sin(PI/2) = 0.fffe rather than 1.0000.
//
// cos[x] = sin[x + 64].

const ushort sintab[256+64+1] = { // indexed by fixang
	0x0000, 0x0192, 0x0324, 0x04b5, 0x0646, 0x07d6, 0x0964, 0x0af1, 
	0x0c7c, 0x0e06, 0x0f8d, 0x1112, 0x1294, 0x1413, 0x1590, 0x1709, 
	0x187e, 0x19ef, 0x1b5d, 0x1cc6, 0x1e2b, 0x1f8c, 0x20e7, 0x223d, 
	0x238e, 0x24da, 0x2620, 0x2760, 0x289a, 0x29ce, 0x2afb, 0x2c21, 
	0x2d41, 0x2e5a, 0x2f6c, 0x3076, 0x3179, 0x3274, 0x3368, 0x3453, 
	0x3537, 0x3612, 0x36e5, 0x37b0, 0x3871, 0x392b, 0x39db, 0x3a82, 
	0x3b21, 0x3bb6, 0x3c42, 0x3cc5, 0x3d3f, 0x3daf, 0x3e15, 0x3e72, 
	0x3ec5, 0x3f0f, 0x3f4f, 0x3f85, 0x3fb1, 0x3fd4, 0x3fec, 0x3ffb, 
	0x4000, 0x3ffb, 0x3fec, 0x3fd4, 0x3fb1, 0x3f85, 0x3f4f, 0x3f0f, 
	0x3ec5, 0x3e72, 0x3e15, 0x3daf, 0x3d3f, 0x3cc5, 0x3c42, 0x3bb6, 
	0x3b21, 0x3a82, 0x39db, 0x392b, 0x3871, 0x37b0, 0x36e5, 0x3612, 
	0x3537, 0x3453, 0x3368, 0x3274, 0x3179, 0x3076, 0x2f6c, 0x2e5a, 
	0x2d41, 0x2c21, 0x2afb, 0x29ce, 0x289a, 0x2760, 0x2620, 0x24da, 
	0x238e, 0x223d, 0x20e7, 0x1f8c, 0x1e2b, 0x1cc6, 0x1b5d, 0x19ef, 
	0x187e, 0x1709, 0x1590, 0x1413, 0x1294, 0x1112, 0x0f8d, 0x0e06, 
	0x0c7c, 0x0af1, 0x0964, 0x07d6, 0x0646, 0x04b5, 0x0324, 0x0192, 
	0x0000, 0xfe6e, 0xfcdc, 0xfb4b, 0xf9ba, 0xf82a, 0xf69c, 0xf50f, 
	0xf384, 0xf1fa, 0xf073, 0xeeee, 0xed6c, 0xebed, 0xea70, 0xe8f7, 
	0xe782, 0xe611, 0xe4a3, 0xe33a, 0xe1d5, 0xe074, 0xdf19, 0xddc3, 
	0xdc72, 0xdb26, 0xd9e0, 0xd8a0, 0xd766, 0xd632, 0xd505, 0xd3df, 
	0xd2bf, 0xd1a6, 0xd094, 0xcf8a, 0xce87, 0xcd8c, 0xcc98, 0xcbad, 
	0xcac9, 0xc9ee, 0xc91b, 0xc850, 0xc78f, 0xc6d5, 0xc625, 0xc57e, 
	0xc4df, 0xc44a, 0xc3be, 0xc33b, 0xc2c1, 0xc251, 0xc1eb, 0xc18e, 
	0xc13b, 0xc0f1, 0xc0b1, 0xc07b, 0xc04f, 0xc02c, 0xc014, 0xc005, 
	0xc000, 0xc005, 0xc014, 0xc02c, 0xc04f, 0xc07b, 0xc0b1, 0xc0f1, 
	0xc13b, 0xc18e, 0xc1eb, 0xc251, 0xc2c1, 0xc33b, 0xc3be, 0xc44a, 
	0xc4df, 0xc57e, 0xc625, 0xc6d5, 0xc78f, 0xc850, 0xc91b, 0xc9ee, 
	0xcac9, 0xcbad, 0xcc98, 0xcd8c, 0xce87, 0xcf8a, 0xd094, 0xd1a6, 
	0xd2bf, 0xd3df, 0xd505, 0xd632, 0xd766, 0xd8a0, 0xd9e0, 0xdb26, 
	0xdc72, 0xddc3, 0xdf19, 0xe074, 0xe1d5, 0xe33a, 0xe4a3, 0xe611, 
	0xe782, 0xe8f7, 0xea70, 0xebed, 0xed6c, 0xeeee, 0xf073, 0xf1fa, 
	0xf384, 0xf50f, 0xf69c, 0xf82a, 0xf9ba, 0xfb4b, 0xfcdc, 0xfe6e, 
	0x0000, 0x0192, 0x0324, 0x04b5, 0x0646, 0x07d6, 0x0964, 0x0af1, 
	0x0c7c, 0x0e06, 0x0f8d, 0x1112, 0x1294, 0x1413, 0x1590, 0x1709, 
	0x187e, 0x19ef, 0x1b5d, 0x1cc6, 0x1e2b, 0x1f8c, 0x20e7, 0x223d, 
	0x238e, 0x24da, 0x2620, 0x2760, 0x289a, 0x29ce, 0x2afb, 0x2c21, 
	0x2d41, 0x2e5a, 0x2f6c, 0x3076, 0x3179, 0x3274, 0x3368, 0x3453, 
	0x3537, 0x3612, 0x36e5, 0x37b0, 0x3871, 0x392b, 0x39db, 0x3a82, 
	0x3b21, 0x3bb6, 0x3c42, 0x3cc5, 0x3d3f, 0x3daf, 0x3e15, 0x3e72, 
	0x3ec5, 0x3f0f, 0x3f4f, 0x3f85, 0x3fb1, 0x3fd4, 0x3fec, 0x3ffb, 
	0x4000
};

// Now the arcsin table.
// The arcsin table is indexed by (((fix >> 2) + 0x4000) & 0xffff).
// That means -1 = 0xc000 + 0x4000 = 0x0000,
//             0 = 0x0000 + 0x4000 = 0x4000,
//             1 = 0x4000 + 0x4000 = 0x8000.
// So the high bit is almost useless, but otherwise we have problems
//   trying to differentiate between 1 and -1.
// Its units are fixangs.
//
// acos(x) = PI/2 - asin(x).   (PI/2 is fixang 0x4000)

// indexed by (high 8 bits of (fix >> 2 + 0x4000)
const fixang asintab[128+1] = { 
	0xc000, 0xc736, 0xca36, 0xcc86, 0xce7b, 0xd036, 0xd1c9, 0xd33c, 
	0xd497, 0xd5df, 0xd715, 0xd83f, 0xd95c, 0xda6e, 0xdb77, 0xdc79, 
	0xdd72, 0xde66, 0xdf53, 0xe03b, 0xe11d, 0xe1fc, 0xe2d6, 0xe3ac, 
	0xe47e, 0xe54d, 0xe619, 0xe6e2, 0xe7a9, 0xe86d, 0xe92e, 0xe9ed, 
	0xeaab, 0xeb66, 0xec1f, 0xecd7, 0xed8d, 0xee41, 0xeef4, 0xefa6, 
	0xf057, 0xf106, 0xf1b4, 0xf261, 0xf30d, 0xf3b8, 0xf462, 0xf50c, 
	0xf5b4, 0xf65c, 0xf704, 0xf7aa, 0xf851, 0xf8f6, 0xf99c, 0xfa40, 
	0xfae5, 0xfb89, 0xfc2d, 0xfcd0, 0xfd74, 0xfe17, 0xfeba, 0xff5d, 
	0x0000, 0x00a3, 0x0146, 0x01e9, 0x028c, 0x0330, 0x03d3, 0x0477, 
	0x051b, 0x05c0, 0x0664, 0x070a, 0x07af, 0x0856, 0x08fc, 0x09a4, 
	0x0a4c, 0x0af4, 0x0b9e, 0x0c48, 0x0cf3, 0x0d9f, 0x0e4c, 0x0efa, 
	0x0fa9, 0x105a, 0x110c, 0x11bf, 0x1273, 0x1329, 0x13e1, 0x149a, 
	0x1555, 0x1613, 0x16d2, 0x1793, 0x1857, 0x191e, 0x19e7, 0x1ab3, 
	0x1b82, 0x1c54, 0x1d2a, 0x1e04, 0x1ee3, 0x1fc5, 0x20ad, 0x219a, 
	0x228e, 0x2387, 0x2489, 0x2592, 0x26a4, 0x27c1, 0x28eb, 0x2a21, 
	0x2b69, 0x2cc4, 0x2e37, 0x2fca, 0x3185, 0x337a, 0x35ca, 0x38ca, 
	0x4000
};

// There are two exp tables.  The first is for integer exponents.
// The table only goes from -11 to 11 because that's all that will
// fit in a 16:16 fixed point number.  Add INTEGER_EXP_OFFSET to
// your exponent before looking it up in the table.

#define INTEGER_EXP_OFFSET 11

const ulong expinttab[INTEGER_EXP_OFFSET*2+1] = {
	0x00000001, 0x00000003, 0x00000008, 0x00000016, 
	0x0000003c, 0x000000a2, 0x000001ba, 0x000004b0, 
	0x00000cbf, 0x000022a5, 0x00005e2d, 0x00010000, 
	0x0002b7e1, 0x00076399, 0x001415e6, 0x00369920, 
	0x009469c5, 0x01936dc5, 0x0448a217, 0x0ba4f53f, 
	0x1fa7157c, 0x560a773e, 0xe9e22447, 
};

// Now for the fractional table, which currently has 16+1 values,
// which should be interpolated between.  We can crank up the
// accuracy later if we need it.  So this input to this table goes
// from 0 to 1 by sixteenths.

const ulong expfractab[16+1] = {
	0x00010000, 0x00011083, 0x00012216, 0x000134cc, 
	0x000148b6, 0x00015de9, 0x0001747a, 0x00018c80, 
	0x0001a613, 0x0001c14b, 0x0001de45, 0x0001fd1e, 
	0x00021df4, 0x000240e8, 0x0002661d, 0x00028db8, 
	0x0002b7e1, 
};

