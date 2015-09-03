/****************
Copyright (c) 2005-2012 Antonio Pertusa Ibáñez <pertusa@dlsi.ua.es>
Copyright (c) 2012 Universidad de Alicante

This multiple fundamental frequency estimation method is free software: you
 can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation, either version
 3 of the License, or (at your option) any later version.

This multiple fundamental frequency estimation method is distributed in the
 hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code.  If not, see <http://www.gnu.org/licenses/>.

 Comments are welcomed
 
**************/

#ifndef PARAMS
#define PARAMS
#include <math.h>

/***************** Spectrum analysis parameters *****************/
#define kRES	 	4       // winsize=winsize*res (Zero padding) for 44kHz. If sr=22kHz, this value is multiplied by two

/****************** Processing parameters ************************/

#define kMINPEAKVALUE   0.1 	// 0.1 Minimum magnitude value for considering a peak
#define MAXHARMONICS   15 	// Max number of harmonics (for efficiency)
#define MAXCANDIDATES  10	// Max number of f0 candidates
#define MINHARMONICS   	0	// Minimum number of harmonics for a f0 candidate
#define STABLEFRAMES	3       // Peak picking is done n frames after onset (if possible) when -onset is used

#define kSMOOTHNESS	4.0	// Smoothness weight (kappa)
#define kLOUDNESS	0.15  	// Minimum note loudness relative to the maximum note in surrounding area
#define kMINLOUDNESS	5  	// Minimum intensity for a HPS
#define kDISCARDH	0.00	// If 0.1, harmonics lower to 10% of maximum spectral pattern value will be discarded

#define F0CANDIDATESCLOSETOPITCH   false	// To discard candidates that are not close to the western fundamental frequencies

#define PEAKLIMIT       11  	// If 0, the high and low limits to select a peak given a center frequency are relative to stepinterharm variable; otherwise, will be +-PEAKLIMIT Hz  
#define kLINEAR 	true	// If true, partials are assumed to be in phase when there is a partial overlap. If false, additivity of power spectrum is considered

/*************** Bands parameters **********************************/

#define kMINBANDFREQ	48.9994087219		// MidiNote= G0 (first pitch frequency)
#define MINBANDDISTANCE 15	// Only f0 candidates closer than MINBANDDISTANCE Hz to a center frequency band will be considered if F0CANDIDATESCLOSETOPITCH=true

const double stepinterbands=pow(2.0,1.0/24.0);	// Bands separation (Hz)
const double candrange=pow(2.0,1.0/24.0);	// Allows only f0 candidates in a range relative to freqrange from its closest band
const double stepinterharm=pow(2.0,1.0/24.0); 	// 1/96 = 1/4 semitone. Only used if PEAKLIMIT=0

/*********************** Gaussian smoothness **************/

const int tamgaussian=3;		      // Gaussian window size
static const double gaussian[]={0.21,0.58,0.21}; // Gaussian window values

/*********************** For -clean option **************/

#define kMINDUR		2	// Number of frames to clean non consecutive notes using cleannotes
#define kSIL		4

/*********************** Frame context params **************/

#define FRAMECONTEXT	4

/********************** Quantization grid for -beat option ***************/

#define QUANTIZE	8.0	// minimum note duration = beat/QUANTIZE (i.e., 4 means semiquaver)

/********************** For Ouput ************************************/

#define SHIFT		    2 //3 	// Shift n frames

#define SUBTRACTENERGY 	    false
#define EQUILIBRATELOUDNESS false	// if false, not used

/********************** For -fixsp -fixsp3 options ************************************/

#define kMAXMERGED		5 	// Max number of combinations (M) at each layer in the graph (if kMERGE=false, then it should be about 50)
#define kMERGE			true	// should be true for merging combination saliences when -fixsp or -fixsp3 options are used.

#endif
