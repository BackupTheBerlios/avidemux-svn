 typedef struct{  int line;   FLOAT bark;   FLOAT hear; }MaskingThreshold;static const int SecondFreqEntries[7] =  { 130, 126, 132, 0, 132, 132, 132 };static const MaskingThreshold SecondFreqSubband[7][132] = { /* 2th0, 130 entries */   {{1, 0.425, 45.05},    {2, 0.850, 25.87},    {3, 1.273, 18.70},    {4, 1.694, 14.85},    {5, 2.112, 12.41},    {6, 2.525, 10.72},    {7, 2.934, 9.47},    {8, 3.337, 8.50},    {9, 3.733, 7.73},    {10, 4.124, 7.10},    {11, 4.507, 6.56},    {12, 4.882, 6.11},    {13, 5.249, 5.72},    {14, 5.608, 5.37},    {15, 5.959, 5.07},    {16, 6.301, 4.79},    {17, 6.634, 4.55},    {18, 6.959, 4.32},    {19, 7.274, 4.11},    {20, 7.581, 3.92},    {21, 7.879, 3.74},    {22, 8.169, 3.57},    {23, 8.450, 3.40},    {24, 8.723, 3.25},    {25, 8.987, 3.10},    {26, 9.244, 2.95},    {27, 9.493, 2.81},    {28, 9.734, 2.67},    {29, 9.968, 2.53},    {30, 10.195, 2.39},    {31, 10.416, 2.25},    {32, 10.629, 2.11},    {33, 10.836, 1.97},    {34, 11.037, 1.83},    {35, 11.232, 1.68},    {36, 11.421, 1.53},    {37, 11.605, 1.38},    {38, 11.783, 1.23},    {39, 11.957, 1.07},    {40, 12.125, 0.90},    {41, 12.289, 0.74},    {42, 12.448, 0.56},    {43, 12.603, 0.39},    {44, 12.753, 0.21},    {45, 12.900, 0.02},    {46, 13.042, -0.17},    {47, 13.181, -0.36},    {48, 13.317, -0.56},    {50, 13.577, -0.96},    {52, 13.826, -1.38},    {54, 14.062, -1.79},    {56, 14.288, -2.21},    {58, 14.504, -2.63},    {60, 14.711, -3.03},    {62, 14.909, -3.41},    {64, 15.100, -3.77},    {66, 15.284, -4.09},    {68, 15.460, -4.37},    {70, 15.631, -4.60},    {72, 15.796, -4.78},    {74, 15.955, -4.91},    {76, 16.110, -4.97},    {78, 16.260, -4.98},    {80, 16.406, -4.92},    {82, 16.547, -4.81},    {84, 16.685, -4.65},    {86, 16.820, -4.43},    {88, 16.951, -4.17},    {90, 17.079, -3.87},    {92, 17.205, -3.54},    {94, 17.327, -3.19},    {96, 17.447, -2.82},    {100, 17.680, -2.06},    {104, 17.905, -1.32},    {108, 18.121, -0.64},    {112, 18.331, -0.04},    {116, 18.534, 0.47},    {120, 18.731, 0.89},    {124, 18.922, 1.23},    {128, 19.108, 1.51},    {132, 19.289, 1.74},    {136, 19.464, 1.93},    {140, 19.635, 2.11},    {144, 19.801, 2.28},    {148, 19.963, 2.46},    {152, 20.120, 2.63},    {156, 20.273, 2.82},    {160, 20.421, 3.03},    {164, 20.565, 3.25},    {168, 20.705, 3.49},    {172, 20.840, 3.74},    {176, 20.972, 4.02},    {180, 21.099, 4.32},    {184, 21.222, 4.64},    {188, 21.342, 4.98},    {192, 21.457, 5.35},    {200, 21.677, 6.15},    {208, 21.882, 7.07},    {216, 22.074, 8.10},    {224, 22.253, 9.25},    {232, 22.420, 10.54},    {240, 22.576, 11.97},    {248, 22.721, 13.56},    {256, 22.857, 15.31},    {264, 22.984, 17.23},    {272, 23.102, 19.34},    {280, 23.213, 21.64},    {288, 23.317, 24.15},    {296, 23.415, 26.88},    {304, 23.506, 29.84},    {312, 23.592, 33.05},    {320, 23.673, 36.52},    {328, 23.749, 40.25},    {336, 23.821, 44.27},    {344, 23.888, 48.59},    {352, 23.952, 53.22},    {360, 24.013, 58.18},    {368, 24.070, 63.49},    {376, 24.125, 68.00},    {384, 24.176, 68.00},    {392, 24.225, 68.00},    {400, 24.271, 68.00},    {408, 24.316, 68.00},    {416, 24.358, 68.00},    {424, 24.398, 68.00},    {432, 24.436, 68.00},    {440, 24.473, 68.00},    {448, 24.508, 68.00},    {456, 24.542, 68.00},    {464, 24.574, 68.00}, {0, 0.0, 0.0}, {0, 0.0, 0.0}}, /* 2th1, 126 entries */   {{1, 0.463, 42.10},    {2, 0.925, 24.17},    {3, 1.385, 17.47},    {4, 1.842, 13.87},    {5, 2.295, 11.60},    {6, 2.742, 10.01},    {7, 3.184, 8.84},    {8, 3.618, 7.94},    {9, 4.045, 7.22},    {10, 4.463, 6.62},    {11, 4.872, 6.12},    {12, 5.272, 5.70},    {13, 5.661, 5.33},    {14, 6.041, 5.00},    {15, 6.411, 4.71},    {16, 6.770, 4.45},    {17, 7.119, 4.21},    {18, 7.457, 4.00},    {19, 7.785, 3.79},    {20, 8.103, 3.61},    {21, 8.410, 3.43},    {22, 8.708, 3.26},    {23, 8.996, 3.09},    {24, 9.275, 2.93},    {25, 9.544, 2.78},    {26, 9.805, 2.63},    {27, 10.057, 2.47},    {28, 10.301, 2.32},    {29, 10.537, 2.17},    {30, 10.765, 2.02},    {31, 10.986, 1.86},    {32, 11.199, 1.71},    {33, 11.406, 1.55},    {34, 11.606, 1.38},    {35, 11.800, 1.21},    {36, 11.988, 1.04},    {37, 12.170, 0.86},    {38, 12.347, 0.67},    {39, 12.518, 0.49},    {40, 12.684, 0.29},    {41, 12.845, 0.09},    {42, 13.002, -0.11},    {43, 13.154, -0.32},    {44, 13.302, -0.54},    {45, 13.446, -0.75},    {46, 13.586, -0.97},    {47, 13.723, -1.20},    {48, 13.855, -1.43},    {50, 14.111, -1.88},    {52, 14.354, -2.34},    {54, 14.585, -2.79},    {56, 14.807, -3.22},    {58, 15.018, -3.62},    {60, 15.221, -3.98},    {62, 15.415, -4.30},    {64, 15.602, -4.57},    {66, 15.783, -4.77},    {68, 15.956, -4.91},    {70, 16.124, -4.98},    {72, 16.287, -4.97},    {74, 16.445, -4.90},    {76, 16.598, -4.76},    {78, 16.746, -4.55},    {80, 16.891, -4.29},    {82, 17.032, -3.99},    {84, 17.169, -3.64},    {86, 17.303, -3.26},    {88, 17.434, -2.86},    {90, 17.563, -2.45},    {92, 17.688, -2.04},    {94, 17.811, -1.63},    {96, 17.932, -1.24},    {100, 18.166, -0.51},    {104, 18.392, 0.12},    {108, 18.611, 0.64},    {112, 18.823, 1.06},    {116, 19.028, 1.39},    {120, 19.226, 1.66},    {124, 19.419, 1.88},    {128, 19.606, 2.08},    {132, 19.788, 2.27},    {136, 19.964, 2.46},    {140, 20.135, 2.65},    {144, 20.300, 2.86},    {148, 20.461, 3.09},    {152, 20.616, 3.33},    {156, 20.766, 3.60},    {160, 20.912, 3.89},    {164, 21.052, 4.20},    {168, 21.188, 4.54},    {172, 21.318, 4.91},    {176, 21.445, 5.31},    {180, 21.567, 5.73},    {184, 21.684, 6.18},    {188, 21.797, 6.67},    {192, 21.906, 7.19},    {200, 22.113, 8.33},    {208, 22.304, 9.63},    {216, 22.482, 11.08},    {224, 22.646, 12.71},    {232, 22.799, 14.53},    {240, 22.941, 16.54},    {248, 23.072, 18.77},    {256, 23.195, 21.23},    {264, 23.309, 23.94},    {272, 23.415, 26.90},    {280, 23.515, 30.14},    {288, 23.607, 33.67},    {296, 23.694, 37.51},    {304, 23.775, 41.67},    {312, 23.852, 46.17},    {320, 23.923, 51.04},    {328, 23.991, 56.29},    {336, 24.054, 61.94},    {344, 24.114, 68.00},    {352, 24.171, 68.00},    {360, 24.224, 68.00},    {368, 24.275, 68.00},    {376, 24.322, 68.00},    {384, 24.368, 68.00},    {392, 24.411, 68.00},    {400, 24.452, 68.00},    {408, 24.491, 68.00},    {416, 24.528, 68.00},    {424, 24.564, 68.00},    {432, 24.597, 68.00},    {0, 0.0, 0.0},    {0, 0.0, 0.0},    {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}},     /* 2th2, 132 entries */   {{1, 0.309, 58.23},    {2, 0.617, 33.44},    {3, 0.925, 24.17},    {4, 1.232, 19.20},    {5, 1.538, 16.05},    {6, 1.842, 13.87},    {7, 2.145, 12.26},    {8, 2.445, 11.01},    {9, 2.742, 10.01},    {10, 3.037, 9.20},    {11, 3.329, 8.52},    {12, 3.618, 7.94},    {13, 3.903, 7.44},    {14, 4.185, 7.00},    {15, 4.463, 6.62},    {16, 4.736, 6.28},    {17, 5.006, 5.97},    {18, 5.272, 5.70},    {19, 5.533, 5.44},    {20, 5.789, 5.21},    {21, 6.041, 5.00},    {22, 6.289, 4.80},    {23, 6.532, 4.62},    {24, 6.770, 4.45},    {25, 7.004, 4.29},    {26, 7.233, 4.14},    {27, 7.457, 4.00},    {28, 7.677, 3.86},    {29, 7.892, 3.73},    {30, 8.103, 3.61},    {31, 8.309, 3.49},    {32, 8.511, 3.37},    {33, 8.708, 3.26},    {34, 8.901, 3.15},    {35, 9.090, 3.04},    {36, 9.275, 2.93},    {37, 9.456, 2.83},    {38, 9.632, 2.73},    {39, 9.805, 2.63},    {40, 9.974, 2.53},    {41, 10.139, 2.42},    {42, 10.301, 2.32},    {43, 10.459, 2.22},    {44, 10.614, 2.12},    {45, 10.765, 2.02},    {46, 10.913, 1.92},    {47, 11.058, 1.81},    {48, 11.199, 1.71},    {50, 11.474, 1.49},    {52, 11.736, 1.27},    {54, 11.988, 1.04},    {56, 12.230, 0.80},    {58, 12.461, 0.55},    {60, 12.684, 0.29},    {62, 12.898, 0.02},    {64, 13.104, -0.25},    {66, 13.302, -0.54},    {68, 13.493, -0.83},    {70, 13.678, -1.12},    {72, 13.855, -1.43},    {74, 14.027, -1.73},    {76, 14.193, -2.04},    {78, 14.354, -2.34},    {80, 14.509, -2.64},    {82, 14.660, -2.93},    {84, 14.807, -3.22},    {86, 14.949, -3.49},    {88, 15.087, -3.74},    {90, 15.221, -3.98},    {92, 15.351, -4.20},    {94, 15.478, -4.40},    {96, 15.602, -4.57},    {100, 15.841, -4.82},    {104, 16.069, -4.96},    {108, 16.287, -4.97},    {112, 16.496, -4.86},    {116, 16.697, -4.63},    {120, 16.891, -4.29},    {124, 17.078, -3.87},    {128, 17.259, -3.39},    {132, 17.434, -2.86},    {136, 17.605, -2.31},    {140, 17.770, -1.77},    {144, 17.932, -1.24},    {148, 18.089, -0.74},    {152, 18.242, -0.29},    {156, 18.392, 0.12},    {160, 18.539, 0.48},    {164, 18.682, 0.79},    {168, 18.823, 1.06},    {172, 18.960, 1.29},    {176, 19.095, 1.49},    {180, 19.226, 1.66},    {184, 19.356, 1.81},    {188, 19.482, 1.95},    {192, 19.606, 2.08},    {200, 19.847, 2.33},    {208, 20.079, 2.59},    {216, 20.300, 2.86},    {224, 20.513, 3.17},    {232, 20.717, 3.51},    {240, 20.912, 3.89},    {248, 21.098, 4.31},    {256, 21.275, 4.79},    {264, 21.445, 5.31},    {272, 21.606, 5.88},    {280, 21.760, 6.50},    {288, 21.906, 7.19},    {296, 22.046, 7.93},    {304, 22.178, 8.75},    {312, 22.304, 9.63},    {320, 22.424, 10.58},    {328, 22.538, 11.60},    {336, 22.646, 12.71},    {344, 22.749, 13.90},    {352, 22.847, 15.18},    {352, 22.941, 16.54},    {368, 23.030, 18.01},    {376, 23.114, 19.57},    {384, 23.195, 21.23},    {382, 23.272, 23.01},    {400, 23.345, 24.90},    {408, 23.415, 26.90},    {416, 23.482, 29.03},    {424, 23.546, 31.28},    {432, 23.607, 33.67},    {440, 23.666, 36.19},    {448, 23.722, 38.86},    {456, 23.775, 41.67},    {464, 23.827, 44.63}, {472, 23.876, 47.76}, {480, 23.923, 51.04}}, /* 2th3, 0 entries (all dummies) */   {{0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									  0.0,									  0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									  0.0,									  0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									  0.0,									  0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}, {0,									 0.0,									 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0},   {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0, 0.0}, {0, 0.0,								  0.0}}, /* 2th4, 132 entries */   {{1, 0.213, 68.00},    {2, 0.425, 45.05},    {3, 0.638, 32.57},    {4, 0.850, 25.87},    {5, 1.062, 21.63},    {6, 1.273, 18.70},    {7, 1.484, 16.52},    {8, 1.694, 14.85},    {9, 1.903, 13.51},    {10, 2.112, 12.41},    {11, 2.319, 11.50},    {12, 2.525, 10.72},    {13, 2.730, 10.05},    {14, 2.934, 9.47},    {15, 3.136, 8.96},    {16, 3.337, 8.50},    {17, 3.536, 8.10},    {18, 3.733, 7.73},    {19, 3.929, 7.40},    {20, 4.124, 7.10},    {21, 4.316, 6.82},    {22, 4.507, 6.56},    {23, 4.695, 6.33},    {24, 4.882, 6.11},    {25, 5.067, 5.91},    {26, 5.249, 5.72},    {27, 5.430, 5.54},    {28, 5.608, 5.37},    {29, 5.785, 5.22},    {30, 5.959, 5.07},    {31, 6.131, 4.93},    {32, 6.301, 4.79},    {33, 6.469, 4.67},    {34, 6.634, 4.55},    {35, 6.798, 4.43},    {36, 6.959, 4.32},    {37, 7.118, 4.21},    {38, 7.274, 4.11},    {39, 7.429, 4.01},    {40, 7.581, 3.92},    {41, 7.731, 3.83},    {42, 7.879, 3.74},    {43, 8.025, 3.65},    {44, 8.169, 3.57},    {45, 8.310, 3.48},    {46, 8.450, 3.40},    {47, 8.587, 3.33},    {48, 8.723, 3.25},    {50, 8.987, 3.10},    {52, 9.244, 2.95},    {54, 9.493, 2.81},    {56, 9.734, 2.67},    {58, 9.968, 2.53},    {60, 10.195, 2.39},    {62, 10.416, 2.25},    {64, 10.629, 2.11},    {66, 10.836, 1.97},    {68, 11.037, 1.83},    {70, 11.232, 1.68},    {72, 11.421, 1.53},    {74, 11.605, 1.38},    {76, 11.783, 1.23},    {78, 11.957, 1.07},    {80, 12.125, 0.90},    {82, 12.289, 0.74},    {84, 12.448, 0.56},    {86, 12.603, 0.39},    {88, 12.753, 0.21},    {90, 12.900, 0.02},    {92, 13.042, -0.17},    {94, 13.181, -0.36},    {96, 13.317, -0.56},    {100, 13.578, -0.96},    {104, 13.826, -1.38},    {108, 14.062, -1.79},    {112, 14.288, -2.21},    {116, 14.504, -2.63},    {120, 14.711, -3.03},    {124, 14.909, -3.41},    {128, 15.100, -3.77},    {132, 15.284, -4.09},    {136, 15.460, -4.37},    {140, 15.631, -4.60},    {144, 15.796, -4.78},    {148, 15.955, -4.91},    {152, 16.110, -4.97},    {156, 16.260, -4.98},    {160, 16.406, -4.92},    {164, 16.547, -4.81},    {168, 16.685, -4.65},    {172, 16.820, -4.43},    {176, 16.951, -4.17},    {180, 17.079, -3.87},    {184, 17.205, -3.54},    {188, 17.327, -3.19},    {192, 17.447, -2.82},    {200, 17.680, -2.06},    {208, 17.905, -1.32},    {216, 18.121, -0.64},    {224, 18.331, -0.04},    {232, 18.534, 0.47},    {240, 18.731, 0.89},    {248, 18.922, 1.23},    {256, 19.108, 1.51},    {264, 19.289, 1.74},    {272, 19.464, 1.93},    {280, 19.635, 2.11},    {288, 19.801, 2.28},    {296, 19.963, 2.46},    {304, 20.120, 2.63},    {312, 20.273, 2.82},    {320, 20.421, 3.03},    {328, 20.565, 3.25},    {336, 20.705, 3.49},    {344, 20.840, 3.74},    {352, 20.972, 4.02},    {360, 21.099, 4.32},    {368, 21.222, 4.64},    {376, 21.342, 4.98},    {384, 21.457, 5.35},    {392, 21.569, 5.74},    {400, 21.677, 6.15},    {408, 21.781, 6.60},    {416, 21.882, 7.07},    {424, 21.980, 7.57},    {432, 22.074, 8.10},    {440, 22.165, 8.66},    {448, 22.253, 9.25},    {456, 22.338, 9.88},    {464, 22.420, 10.54}, {472, 22.499, 11.24}, {480, 22.576, 11.97}}, /* 2th5, 132 entries */   {{1, 0.232, 68.00},    {2, 0.463, 42.10},    {3, 0.694, 30.43},    {4, 0.925, 24.17},    {5, 1.156, 20.22},    {6, 1.385, 17.47},    {7, 1.614, 15.44},    {8, 1.842, 13.87},    {9, 2.069, 12.62},    {10, 2.295, 11.60},    {11, 2.519, 10.74},    {12, 2.742, 10.01},    {13, 2.964, 9.39},    {14, 3.184, 8.84},    {15, 3.402, 8.37},    {16, 3.618, 7.94},    {17, 3.832, 7.56},    {18, 4.045, 7.22},    {19, 4.255, 6.90},    {20, 4.463, 6.62},    {21, 4.668, 6.36},    {22, 4.872, 6.12},    {23, 5.073, 5.90},    {24, 5.272, 5.70},    {25, 5.468, 5.50},    {26, 5.661, 5.33},    {27, 5.853, 5.16},    {28, 6.041, 5.00},    {29, 6.227, 4.85},    {30, 6.411, 4.71},    {31, 6.592, 4.58},    {32, 6.770, 4.45},    {33, 6.946, 4.33},    {34, 7.119, 4.21},    {35, 7.289, 4.10},    {36, 7.457, 4.00},    {37, 7.622, 3.89},    {38, 7.785, 3.79},    {39, 7.945, 3.70},    {40, 8.103, 3.61},    {41, 8.258, 3.51},    {42, 8.410, 3.43},    {43, 8.560, 3.34},    {44, 8.708, 3.26},    {45, 8.853, 3.17},    {46, 8.996, 3.09},    {47, 9.137, 3.01},    {48, 9.275, 2.93},    {50, 9.544, 2.78},    {52, 9.805, 2.63},    {54, 10.057, 2.47},    {56, 10.301, 2.32},    {58, 10.537, 2.17},    {60, 10.765, 2.02},    {62, 10.986, 1.86},    {64, 11.199, 1.71},    {66, 11.406, 1.55},    {68, 11.606, 1.38},    {70, 11.800, 1.21},    {72, 11.988, 1.04},    {74, 12.170, 0.86},    {76, 12.347, 0.67},    {78, 12.518, 0.49},    {80, 12.684, 0.29},    {82, 12.845, 0.09},    {84, 13.002, -0.11},    {86, 13.154, -0.32},    {88, 13.302, -0.54},    {90, 13.446, -0.75},    {92, 13.586, -0.97},    {94, 13.723, -1.20},    {96, 13.855, -1.43},    {100, 14.111, -1.88},    {104, 14.354, -2.34},    {108, 14.585, -2.79},    {112, 14.807, -3.22},    {116, 15.018, -3.62},    {120, 15.221, -3.98},    {124, 15.415, -4.30},    {128, 15.602, -4.57},    {132, 15.783, -4.77},    {136, 15.956, -4.91},    {140, 16.124, -4.98},    {144, 16.287, -4.97},    {148, 16.445, -4.90},    {152, 16.598, -4.76},    {156, 16.746, -4.55},    {160, 16.891, -4.29},    {164, 17.032, -3.99},    {168, 17.169, -3.64},    {172, 17.303, -3.26},    {176, 17.434, -2.86},    {180, 17.563, -2.45},    {184, 17.688, -2.04},    {188, 17.811, -1.63},    {192, 17.932, -1.24},    {200, 18.166, -0.51},    {208, 18.392, 0.12},    {216, 18.611, 0.64},    {224, 18.823, 1.06},    {232, 19.028, 1.39},    {240, 19.226, 1.66},    {248, 19.419, 1.88},    {256, 19.606, 2.08},    {264, 19.788, 2.27},    {272, 19.964, 2.46},    {280, 20.135, 2.65},    {288, 20.300, 2.86},    {296, 20.461, 3.09},    {304, 20.616, 3.33},    {312, 20.766, 3.60},    {320, 20.912, 3.89},    {328, 21.052, 4.20},    {336, 21.188, 4.54},    {344, 21.318, 4.91},    {352, 21.445, 5.31},    {360, 21.567, 5.73},    {368, 21.684, 6.18},    {376, 21.797, 6.67},    {384, 21.906, 7.19},    {392, 22.012, 7.74},    {400, 22.113, 8.33},    {408, 22.210, 8.96},    {416, 22.304, 9.63},    {424, 22.395, 10.33},    {432, 22.482, 11.08},    {440, 22.566, 11.87},    {448, 22.646, 12.71},    {456, 22.724, 13.59},    {464, 22.799, 14.53}, {472, 22.871, 15.51}, {480, 22.941, 16.54}}, /* 2th6, 132 entries */   {{1, 0.154, 68.00},    {2, 0.309, 58.23},    {3, 0.463, 42.10},    {4, 0.617, 33.44},    {5, 0.771, 27.97},    {6, 0.925, 24.17},    {7, 1.079, 21.36},    {8, 1.232, 19.20},    {9, 1.385, 17.47},    {10, 1.538, 16.05},    {11, 1.690, 14.87},    {12, 1.842, 13.87},    {13, 1.994, 13.01},    {14, 2.145, 12.26},    {15, 2.295, 11.60},    {16, 2.445, 11.01},    {17, 2.594, 10.49},    {18, 2.742, 10.01},    {19, 2.890, 9.59},    {20, 3.037, 9.20},    {21, 3.184, 8.84},    {22, 3.329, 8.52},    {23, 3.474, 8.22},    {24, 3.618, 7.94},    {25, 3.761, 7.68},    {26, 3.903, 7.44},    {27, 4.045, 7.22},    {28, 4.185, 7.00},    {29, 4.324, 6.81},    {30, 4.463, 6.62},    {31, 4.600, 6.44},    {32, 4.736, 6.28},    {33, 4.872, 6.12},    {34, 5.006, 5.97},    {35, 5.139, 5.83},    {36, 5.272, 5.70},    {37, 5.403, 5.57},    {38, 5.533, 5.44},    {39, 5.661, 5.33},    {40, 5.789, 5.21},    {41, 5.916, 5.10},    {42, 6.041, 5.00},    {43, 6.166, 4.90},    {44, 6.289, 4.80},    {45, 6.411, 4.71},    {46, 6.532, 4.62},    {47, 6.651, 4.53},    {48, 6.770, 4.45},    {50, 7.004, 4.29},    {52, 7.233, 4.14},    {54, 7.457, 4.00},    {56, 7.677, 3.86},    {58, 7.892, 3.73},    {60, 8.103, 3.61},    {62, 8.309, 3.49},    {64, 8.511, 3.37},    {66, 8.708, 3.26},    {68, 8.901, 3.15},    {70, 9.090, 3.04},    {72, 9.275, 2.93},    {74, 9.456, 2.83},    {76, 9.632, 2.73},    {78, 9.805, 2.63},    {80, 9.974, 2.53},    {82, 10.139, 2.42},    {84, 10.301, 2.32},    {86, 10.459, 2.22},    {88, 10.614, 2.12},    {90, 10.765, 2.02},    {92, 10.913, 1.92},    {94, 11.058, 1.81},    {96, 11.199, 1.71},    {100, 11.474, 1.49},    {104, 11.736, 1.27},    {108, 11.988, 1.04},    {112, 12.230, 0.80},    {116, 12.461, 0.55},    {120, 12.684, 0.29},    {124, 12.898, 0.02},    {128, 13.104, -0.25},    {132, 13.302, -0.54},    {136, 13.493, -0.83},    {140, 13.678, -1.12},    {144, 13.855, -1.43},    {148, 14.027, -1.73},    {152, 14.193, -2.04},    {156, 14.354, -2.34},    {160, 14.509, -2.64},    {164, 14.660, -2.93},    {168, 14.807, -3.22},    {172, 14.949, -3.49},    {176, 15.087, -3.74},    {180, 15.221, -3.98},    {184, 15.351, -4.20},    {188, 15.478, -4.40},    {192, 15.602, -4.57},    {200, 15.841, -4.82},    {208, 16.069, -4.96},    {216, 16.287, -4.97},    {224, 16.496, -4.86},    {232, 16.697, -4.63},    {240, 16.891, -4.29},    {248, 17.078, -3.87},    {256, 17.259, -3.39},    {264, 17.434, -2.86},    {272, 17.605, -2.31},    {280, 17.770, -1.77},    {288, 17.932, -1.24},    {296, 18.089, -0.74},    {304, 18.242, -0.29},    {312, 18.392, 0.12},    {320, 18.539, 0.48},    {328, 18.682, 0.79},    {336, 18.823, 1.06},    {344, 18.960, 1.29},    {352, 19.095, 1.49},    {360, 19.226, 1.66},    {368, 19.356, 1.81},    {376, 19.482, 1.95},    {384, 19.606, 2.08},    {392, 19.728, 2.21},    {400, 19.847, 2.33},    {408, 19.964, 2.46},    {416, 20.079, 2.59},    {424, 20.191, 2.72},    {432, 20.300, 2.86},    {440, 20.408, 3.01},    {448, 20.513, 3.17},    {456, 20.616, 3.33},    {464, 20.717, 3.51}, {472, 20.815, 3.69}, {480, 20.912, 3.89}} };