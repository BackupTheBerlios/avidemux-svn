//
// C++ Interface: audiofilter_dolby
//
// Description: 
//
//
// Author: Mihail Zenkov <kreator@tut.by>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define NZEROS 500
#define GAIN 1.571116176e+00

static float xv_left[NZEROS+1];
static float xv_right[NZEROS+1];

static float xcoeffs[] =
{ +0.0000000000, +0.0003214310, +0.0000000000, +0.0003252099,
+0.0000000000, +0.0003302355, +0.0000000000, +0.0003365372,
+0.0000000000, +0.0003441444, +0.0000000000, +0.0003530868,
+0.0000000000, +0.0003633944, +0.0000000000, +0.0003750973,
+0.0000000000, +0.0003882260, +0.0000000000, +0.0004028113,
+0.0000000000, +0.0004188840, +0.0000000000, +0.0004364757,
+0.0000000000, +0.0004556178, +0.0000000000, +0.0004763424,
+0.0000000000, +0.0004986818, +0.0000000000, +0.0005226689,
+0.0000000000, +0.0005483367, +0.0000000000, +0.0005757189,
+0.0000000000, +0.0006048495, +0.0000000000, +0.0006357632,
+0.0000000000, +0.0006684951, +0.0000000000, +0.0007030809,
+0.0000000000, +0.0007395569, +0.0000000000, +0.0007779600,
+0.0000000000, +0.0008183281, +0.0000000000, +0.0008606994,
+0.0000000000, +0.0009051133, +0.0000000000, +0.0009516098,
+0.0000000000, +0.0010002299, +0.0000000000, +0.0010510155,
+0.0000000000, +0.0011040098, +0.0000000000, +0.0011592568,
+0.0000000000, +0.0012168018, +0.0000000000, +0.0012766915,
+0.0000000000, +0.0013389738, +0.0000000000, +0.0014036982,
+0.0000000000, +0.0014709156, +0.0000000000, +0.0015406788,
+0.0000000000, +0.0016130421, +0.0000000000, +0.0016880620,
+0.0000000000, +0.0017657969, +0.0000000000, +0.0018463073,
+0.0000000000, +0.0019296564, +0.0000000000, +0.0020159096,
+0.0000000000, +0.0021051351, +0.0000000000, +0.0021974040,
+0.0000000000, +0.0022927905, +0.0000000000, +0.0023913723,
+0.0000000000, +0.0024932304, +0.0000000000, +0.0025984499,
+0.0000000000, +0.0027071199, +0.0000000000, +0.0028193341,
+0.0000000000, +0.0029351907, +0.0000000000, +0.0030547934,
+0.0000000000, +0.0031782511, +0.0000000000, +0.0033056791,
+0.0000000000, +0.0034371988, +0.0000000000, +0.0035729386,
+0.0000000000, +0.0037130347, +0.0000000000, +0.0038576311,
+0.0000000000, +0.0040068807, +0.0000000000, +0.0041609461,
+0.0000000000, +0.0043200000, +0.0000000000, +0.0044842264,
+0.0000000000, +0.0046538214, +0.0000000000, +0.0048289944,
+0.0000000000, +0.0050099691, +0.0000000000, +0.0051969851,
+0.0000000000, +0.0053902989, +0.0000000000, +0.0055901856,
+0.0000000000, +0.0057969412, +0.0000000000, +0.0060108836,
+0.0000000000, +0.0062323557, +0.0000000000, +0.0064617271,
+0.0000000000, +0.0066993973, +0.0000000000, +0.0069457986,
+0.0000000000, +0.0072013994, +0.0000000000, +0.0074667084,
+0.0000000000, +0.0077422784, +0.0000000000, +0.0080287117,
+0.0000000000, +0.0083266658, +0.0000000000, +0.0086368590,
+0.0000000000, +0.0089600787, +0.0000000000, +0.0092971888,
+0.0000000000, +0.0096491399, +0.0000000000, +0.0100169797,
+0.0000000000, +0.0104018664, +0.0000000000, +0.0108050829,
+0.0000000000, +0.0112280539, +0.0000000000, +0.0116723666,
+0.0000000000, +0.0121397934, +0.0000000000, +0.0126323203,
+0.0000000000, +0.0131521795, +0.0000000000, +0.0137018887,
+0.0000000000, +0.0142842976, +0.0000000000, +0.0149026446,
+0.0000000000, +0.0155606252, +0.0000000000, +0.0162624744,
+0.0000000000, +0.0170130694, +0.0000000000, +0.0178180552,
+0.0000000000, +0.0186840011, +0.0000000000, +0.0196185982,
+0.0000000000, +0.0206309077, +0.0000000000, +0.0217316805,
+0.0000000000, +0.0229337690, +0.0000000000, +0.0242526657,
+0.0000000000, +0.0257072177, +0.0000000000, +0.0273205841,
+0.0000000000, +0.0291215436, +0.0000000000, +0.0311463069,
+0.0000000000, +0.0334410798, +0.0000000000, +0.0360657647,
+0.0000000000, +0.0390994399, +0.0000000000, +0.0426486966,
+0.0000000000, +0.0468607407, +0.0000000000, +0.0519447683,
+0.0000000000, +0.0582084316, +0.0000000000, +0.0661234757,
+0.0000000000, +0.0764519642, +0.0000000000, +0.0905102051,
+0.0000000000, +0.1107845781, +0.0000000000, +0.1426030657,
+0.0000000000, +0.1998184590, +0.0000000000, +0.3332243858,
+0.0000000000, +0.9999636803, +0.0000000000, -0.9999636803,
-0.0000000000, -0.3332243858, -0.0000000000, -0.1998184590,
-0.0000000000, -0.1426030657, -0.0000000000, -0.1107845781,
-0.0000000000, -0.0905102051, -0.0000000000, -0.0764519642,
-0.0000000000, -0.0661234757, -0.0000000000, -0.0582084316,
-0.0000000000, -0.0519447683, -0.0000000000, -0.0468607407,
-0.0000000000, -0.0426486966, -0.0000000000, -0.0390994399,
-0.0000000000, -0.0360657647, -0.0000000000, -0.0334410798,
-0.0000000000, -0.0311463069, -0.0000000000, -0.0291215436,
-0.0000000000, -0.0273205841, -0.0000000000, -0.0257072177,
-0.0000000000, -0.0242526657, -0.0000000000, -0.0229337690,
-0.0000000000, -0.0217316805, -0.0000000000, -0.0206309077,
-0.0000000000, -0.0196185982, -0.0000000000, -0.0186840011,
-0.0000000000, -0.0178180552, -0.0000000000, -0.0170130694,
-0.0000000000, -0.0162624744, -0.0000000000, -0.0155606252,
-0.0000000000, -0.0149026446, -0.0000000000, -0.0142842976,
-0.0000000000, -0.0137018887, -0.0000000000, -0.0131521795,
-0.0000000000, -0.0126323203, -0.0000000000, -0.0121397934,
-0.0000000000, -0.0116723666, -0.0000000000, -0.0112280539,
-0.0000000000, -0.0108050829, -0.0000000000, -0.0104018664,
-0.0000000000, -0.0100169797, -0.0000000000, -0.0096491399,
-0.0000000000, -0.0092971888, -0.0000000000, -0.0089600787,
-0.0000000000, -0.0086368590, -0.0000000000, -0.0083266658,
-0.0000000000, -0.0080287117, -0.0000000000, -0.0077422784,
-0.0000000000, -0.0074667084, -0.0000000000, -0.0072013994,
-0.0000000000, -0.0069457986, -0.0000000000, -0.0066993973,
-0.0000000000, -0.0064617271, -0.0000000000, -0.0062323557,
-0.0000000000, -0.0060108836, -0.0000000000, -0.0057969412,
-0.0000000000, -0.0055901856, -0.0000000000, -0.0053902989,
-0.0000000000, -0.0051969851, -0.0000000000, -0.0050099691,
-0.0000000000, -0.0048289944, -0.0000000000, -0.0046538214,
-0.0000000000, -0.0044842264, -0.0000000000, -0.0043200000,
-0.0000000000, -0.0041609461, -0.0000000000, -0.0040068807,
-0.0000000000, -0.0038576311, -0.0000000000, -0.0037130347,
-0.0000000000, -0.0035729386, -0.0000000000, -0.0034371988,
-0.0000000000, -0.0033056791, -0.0000000000, -0.0031782511,
-0.0000000000, -0.0030547934, -0.0000000000, -0.0029351907,
-0.0000000000, -0.0028193341, -0.0000000000, -0.0027071199,
-0.0000000000, -0.0025984499, -0.0000000000, -0.0024932304,
-0.0000000000, -0.0023913723, -0.0000000000, -0.0022927905,
-0.0000000000, -0.0021974040, -0.0000000000, -0.0021051351,
-0.0000000000, -0.0020159096, -0.0000000000, -0.0019296564,
-0.0000000000, -0.0018463073, -0.0000000000, -0.0017657969,
-0.0000000000, -0.0016880620, -0.0000000000, -0.0016130421,
-0.0000000000, -0.0015406788, -0.0000000000, -0.0014709156,
-0.0000000000, -0.0014036982, -0.0000000000, -0.0013389738,
-0.0000000000, -0.0012766915, -0.0000000000, -0.0012168018,
-0.0000000000, -0.0011592568, -0.0000000000, -0.0011040098,
-0.0000000000, -0.0010510155, -0.0000000000, -0.0010002299,
-0.0000000000, -0.0009516098, -0.0000000000, -0.0009051133,
-0.0000000000, -0.0008606994, -0.0000000000, -0.0008183281,
-0.0000000000, -0.0007779600, -0.0000000000, -0.0007395569,
-0.0000000000, -0.0007030809, -0.0000000000, -0.0006684951,
-0.0000000000, -0.0006357632, -0.0000000000, -0.0006048495,
-0.0000000000, -0.0005757189, -0.0000000000, -0.0005483367,
-0.0000000000, -0.0005226689, -0.0000000000, -0.0004986818,
-0.0000000000, -0.0004763424, -0.0000000000, -0.0004556178,
-0.0000000000, -0.0004364757, -0.0000000000, -0.0004188840,
-0.0000000000, -0.0004028113, -0.0000000000, -0.0003882260,
-0.0000000000, -0.0003750973, -0.0000000000, -0.0003633944,
-0.0000000000, -0.0003530868, -0.0000000000, -0.0003441444,
-0.0000000000, -0.0003365372, -0.0000000000, -0.0003302355,
-0.0000000000, -0.0003252099, -0.0000000000, -0.0003214310,
-0.0000000000,
};

float DolbyShiftLeft(float isamp)
{
	for (int i = 0; i < NZEROS; i++)
		xv_left[i] = xv_left[i+1];
	xv_left[NZEROS] = isamp / GAIN;

	float sum = 0;
	for (int i = 0; i <= NZEROS; i++)
		sum += (xcoeffs[i] * xv_left[i]);

	return sum;
}

float DolbyShiftRight(float isamp)
{
	for (int i = 0; i < NZEROS; i++)
		xv_right[i] = xv_right[i+1];
	xv_right[NZEROS] = isamp / GAIN;

	float sum = 0;
	for (int i = 0; i <= NZEROS; i++)
		sum += (xcoeffs[i] * xv_right[i]);

	return -sum;
}
