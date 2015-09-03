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

#ifndef _MF0_
#define _MF0_

#include "info.h"
#include "params.h"
#include "bands.h"
#include "chord.h"
#include "spectralpattern.h"
#include "defines.h"
#include "peaksatt.h"
#include "complexnotes.h"
#include "partials.h"
#include "combination.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <deque>

using namespace std;

class MF0
{
  private:
    pinfo info;
    bool printlog;    
  
     double midi2freq(int midinote);
     int freq2midi(double freq);
     double centeredfreq(double f);
     bool isclosetoband(double freq);
     double selectpeakcloseto(mapa2 &spectrumpeaks,double centerfreq, int nh, double &maxval);
     mapa2 GetF0Candidates(mapa2 spectrumpeaks);
     double equilibrateloudness(Combination &comb); 
     void ComputeCombinationsSalience (mapa2 &spectrumpeaks, mapa2 &f0candidates, vcombinations &comb);
     void AddBestCombination(Combination &bestcombination, vectorchords &bestmelodynotes, int tini, int tend);
     void AddBestCombination(const vint &bestcombination, vectorchords &bestmelodynotes, int tini, int tend);
     mapa4 CleanFrameCombinations(vcombinations &vc);
     vsimplecombinations CleanFrameCombinationsLoudness(vcombinations &vc);
     double selectHighestSalience(mapa4 &m, itermapa4 &bestcomb);
     vint MergeFramesCombinations(const deque<mapa4> &qc);
     vsimplecombinations MergeFramesCombinationsLoudness(const deque<vsimplecombinations> &qc, double &maxenergy);
     void applyhypothesisonsets(peaksattvector &peaks, vectorchords &bestmelodynotes);
     void computeTempoChanges(vector<double> beatsvector, double tempochanges[], int size); // UNUSED
     void quantizebeats(vector<double> beatsvector, bool quantized[], int sizequantized); // UNUSED
     void printloudnessmatrix(vsimplecombinations &vbestcomb); // UNUSED
     double noteenergydiff(const SimpleCombination& sc1, const SimpleCombination &sc2);
     double getEdgeWeight(const SimpleCombination& sp1, const SimpleCombination& sp2); // UNUSED??
     void createNode(SimpleCombination &sc, unsigned long &numnodes, int nframes, vector<string> &nodenames, vector<map <int, SimpleCombination> > &allmergedcombinations);
     SimpleCombination predictCombination(const SimpleCombination& sc1, const SimpleCombination& sc2);
     void applyhypothesisfixsp3(peaksattvector &peaks, vectorchords &bestmelodynotes, bool plotintensity, bool printgraph, const char *graphfilename);
     void getpeaksvector (int t_ini, int t_end, peaksvector &allpeaks, peaksattvector &peaks, peaksattvector &previouspeaks);
     void printMirexFBF(const vectorchords &melodychords); // UNUSED
     void printMirexNTrack(const vectorchords &melodychords); // UNUSED
     void printeval(const vectorchords &bestmelodynotes); // UNUSED
     void processonsets(int t_end, double *onsetsvector, peaksvector &allpeaks, peaksattvector &peaks, peaksattvector &previouspeaks); // UNUSED??
     vector<double> LoadBeatsFile(string filename); // UNUSED
     void writeonsetsvector(vector<double> in, double out[], int n_frames, double timeres); // UNUSED

     int maxpolyphony;
     float lowestnote;
     float highestnote;
  
  public:
     void applyhypothesisfbf(peaksattvector &peaks, vectorchords &bestmelodynotes, bool eval);
     void applyhypothesisfixsp(peaksattvector &peaks, vectorchords &bestmelodynotes, bool plotintensity, bool printgraph, const char *graphfilename);
     void printbestnotes(const vectorchords &bestmelodynotes); // UNUSED
     void cleannotes(vectorchords& melodychords);
     void quantizeonsets(vector<double> onsetsvector, vector<bool> &quantized, float time_resolution); //, int resolutiondiff);
     void applyhypothesisbeat(peaksattvector &peaks, vectorchords &bestmelodynotes, vector<bool> quantized); // UNUSED
  
    MF0(const pinfo& info, int maxpolyphony, float lowestnote, float highestnote);
    ~MF0();  
};

#endif
