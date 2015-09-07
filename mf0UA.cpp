
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


#include "mf0UA.h"
#include "info.h"
#include "params.h"
#include "myfft.h"
#include "onsetdetection.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>


MF0UA::MF0UA(float inputSampleRate) : 
 Plugin(inputSampleRate),
 sr(inputSampleRate),
 algorithm(0),
 maxpolyphony(6),
 lowestnote(38),
 highestnote(2100),
 minnoteduration(0.023)
{
  window=NULL;
  mu=140.0/(44100.0/inputSampleRate);
  
  onsetsSensitivity=0.18;
}

MF0UA::~MF0UA()
{
     free(window);
}

string
MF0UA::getIdentifier() const
{
    return "mf0ua";
}

string
MF0UA::getName() const
{
    return "Polyphonic Transcription";
}

string
MF0UA::getDescription() const
{
    // Return something helpful here!
    return "Multiple fundamental frequency estimation for polyphonic music transcription";
}

string
MF0UA::getMaker() const
{
    // Your name here
    return "University of Alicante";
}

int
MF0UA::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 2;
}

string
MF0UA::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "Copyright 2012 Antonio Pertusa, Universidad de Alicante. Freely redistributable under GPL License";
}

MF0UA::InputDomain
MF0UA::getInputDomain() const
{
    return TimeDomain;  // Must be time domain for zero-padding
}

size_t
MF0UA::getPreferredBlockSize() const
{
    // Calculate window size for a 93ms window length
    return round((4096.0/44100.0)*sr);
}

size_t 
MF0UA::getPreferredStepSize() const
{
    // 87.5% overlap
    return getPreferredBlockSize()/8;
}

size_t
MF0UA::getMinChannelCount() const
{
    return 1;
}

size_t
MF0UA::getMaxChannelCount() const
{
    return 1;
}

MF0UA::ParameterList
MF0UA::getParameterDescriptors() const
{
    ParameterList list;

    // If the plugin has no adjustable parameters, return an empty
    // list here (and there's no need to provide implementations of
    // getParameter and setParameter in that case either).

    // Note that it is your responsibility to make sure the parameters
    // start off having their default values (e.g. in the constructor
    // above).  The host needs to know the default value so it can do
    // things like provide a "reset to default" function, but it will
    // not explicitly set your parameters to their defaults for you if
    // they have not changed in the mean time.

    // N merged frames (only for merged alg?)
    // kRES?
    // kMINPEAKVALUE?
    // MAX CANDIDATES
    // kLOUDNESS??
    // kSIL??

    ParameterDescriptor d;
    d.identifier = "algorithm";
    d.name = "Algorithm";
    d.description = "Algorithms corresponding to the extended method published in \"Efficient methods for joint estimation of multiple fundamental frequencies in music signals\", A. Pertusa and J. M. Iñesta, EURASIP Journal on Advances in Signal Processing (2012), 2012:27";
    d.unit = "";
    d.isQuantized = true;
    d.quantizeStep=1;
    d.minValue=0;
    d.maxValue=2;
    d.defaultValue = 0;
    d.valueNames.push_back("EURASIP12");
    d.valueNames.push_back("EURASIP12 + tracking");
    d.valueNames.push_back("Onset-based");
    list.push_back(d);
    
    d=ParameterDescriptor();
    d.identifier="maxpolyphony";
    d.name="Maximum polyphony";
    d.description="Maximum number of simultaneous sources";
    d.unit="";
    d.isQuantized=true;
    d.quantizeStep=1;
    d.minValue=1;
    d.maxValue=10;
    d.defaultValue=6;
    list.push_back(d);

    d=ParameterDescriptor();
    d.identifier="lowestnote";
    d.name="Minimum fundamental frequency";
    d.description="Lowest note frequency";
    d.isQuantized=false;
    d.minValue=1;
    d.maxValue=2500;
    d.defaultValue=38;    
    d.unit="Hz";
    list.push_back(d);

    d=ParameterDescriptor();
    d.identifier="highestnote";
    d.name="Maximum fundamental frequency";
    d.description="Highest note frequency";
    d.isQuantized=false;
    d.minValue=1;
    d.maxValue=2500;
    d.defaultValue=2100;
    d.unit="Hz";
    list.push_back(d);

   
    d=ParameterDescriptor();
    d.identifier="minnoteduration";
    d.name="Minimum note duration";
    d.description="Minimum note duration";
    d.isQuantized=false;
    d.minValue=0;
    d.maxValue=0.5;
    d.defaultValue=0.023;
    d.unit="s";
    list.push_back(d);
   
    return list;
}

float
MF0UA::getParameter(string identifier) const
{
    if (identifier == "algorithm")  return algorithm; 
    if (identifier == "maxpolyphony") return maxpolyphony;
    if (identifier == "lowestnote") return lowestnote;
    if (identifier == "highestnote") return highestnote;
    if (identifier == "minnoteduration") return minnoteduration;
     
    return 0;
}

void
MF0UA::setParameter(string identifier, float value) 
{
    if (identifier == "algorithm")  algorithm=value;
    else if (identifier=="maxpolyphony") maxpolyphony=value;
    else if (identifier=="lowestnote") lowestnote=value;
    else if (identifier=="highestnote") highestnote=value;
    else if (identifier=="minnoteduration") minnoteduration=value;

}

MF0UA::ProgramList
MF0UA::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
MF0UA::getCurrentProgram() const
{
    return ""; // no programs
}

void
MF0UA::selectProgram(string name)
{
}

MF0UA::OutputList
MF0UA::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "mf0ua";
    d.name = "UA Multiple f0 Estimation";
    d.description = "Estimated note pitch (MIDI note number)";
    d.unit = "MIDI units";
    d.hasFixedBinCount=true;
    d.binCount = 1;
    d.hasKnownExtents = true;
    d.minValue = 0;
    d.maxValue = 127;
    d.isQuantized = true;
    d.quantizeStep = 1;
    d.sampleType = OutputDescriptor::VariableSampleRate;
    d.sampleRate = (m_stepSize == 0) ? m_inputSampleRate/2048 : m_inputSampleRate/m_stepSize;
    d.hasDuration = true;
    list.push_back(d);

    return list;
}

bool
MF0UA::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_stepSize = stepSize;
    m_blockSize = blockSize;

    // Initialize spectruminfo
    initializeSpectrumInfo();

    // Memory allocation for window
    window = (double*) malloc(sizeof(double)*(spectruminfo.winsize));
              
    // Creation of the Hanning window
    Hanning(window,spectruminfo.N);

    // Bands generation (Only when the algorithm is onset-based)
    if (algorithm==2)
    {
      generatebands(spectruminfo.first_band_freq, spectruminfo.samplerate/2, spectralbands, spectruminfo.freq_resolution);
      spectruminfo.numbands=spectralbands.size();

      // Do not compute differences for the first frame
      resolutiondiff=(getPreferredBlockSize()/2)/getPreferredStepSize();
    }

    // This is the first frame
    firstframe=true;
    n_time=0;

    return true;
}

void
MF0UA::reset()
{
    // Clear buffers, reset stored values, etc
    if (!peaks.empty()) peaks.clear();
    bestmelodynotes.clear();
    previousFrameBands.clear();
    origin = Vamp::RealTime::zeroTime;
    firstframe=true;
    n_time=0;
}

MF0UA::FeatureSet
MF0UA::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    FeatureSet returnFeatureSet;
    double input[spectruminfo.winsize];
    double spectrum[spectruminfo.winsize+1];

    // Copy input data and complete with zeroes for zero padding
    for (int i=0; i<(int)m_blockSize;i++)
        input[i]=inputBuffers[0][i]; 
    for (int i=m_blockSize; i<spectruminfo.winsize; i++)
        input[i]=0.0;

    // First frame to set the origin time
    if (firstframe) {
     origin=timestamp;
    }

    // FFT computation
    fourier_spectrum(input, spectrum, spectruminfo.winsize, spectruminfo.winsize, window, spectruminfo.N);

    // Peak Picking
    mapa2 orderedSpectrumPeaks=spectralPeakPicking(spectrum);
    peaksatt currentpeaks(n_time,n_time,orderedSpectrumPeaks);
    peaks.push_back(currentpeaks); 
     
    
    // Bands computation (only for the onset detection algorithm)
    if (algorithm==2)
    {
    // XXX: HACER BIEN, MIRANDO LA RESOLUCION (DEPENDE DE LA VENTANA USADA SALDRA UNA COSA O LA OTRA)

      if (n_time%resolutiondiff==0)  // 1 of each 4 frames will be used for onset detection, as stepsize differs
      {
          // One-semitone bands computation
          vector<double> currentFrameBands(spectruminfo.numbands);
          OnsetDetection::computebands(spectrum, spectralbands, currentFrameBands, spectruminfo.winsize / 2, spectruminfo.maxbandsval);
    
          // Onset detection function. It is stored as a feature, and also in the vector odf for final postprocessing
          if (!firstframe)
          {
            float diff=OnsetDetection::onsetfunction(previousFrameBands, currentFrameBands, mu);

            Feature detectionFunction;
            detectionFunction.hasTimestamp=true;
            detectionFunction.timestamp = timestamp+ Vamp::RealTime::frame2RealTime(m_stepSize*resolutiondiff, int(m_inputSampleRate +0.5));
            detectionFunction.values.push_back(diff);
       
            odf.push_back(detectionFunction); // To get the onsets in getRemainingFeatures
          }
          previousFrameBands=currentFrameBands;          
      }
    }

    firstframe=false;

    n_time++;
     
    return returnFeatureSet;
}


MF0UA::FeatureSet
MF0UA::getRemainingFeatures()
{
    // Main process is done here
    MF0 mf0e(spectruminfo,maxpolyphony,lowestnote,highestnote);

    switch (algorithm)
    {
      case 0: // EURASIP12
      {
         mf0e.applyhypothesisfbf(peaks,bestmelodynotes, false);
         mf0e.cleannotes(bestmelodynotes);
      }
      break;
      
      case 1: // EURASIP12 + tracking
         mf0e.applyhypothesisfixsp(peaks, bestmelodynotes, false, false, NULL);
      break;
      
      case 2: // Onset detection (stored in odf). The scores of the combinations whithin each Inter Onset Interval are merged 
          {
          vector<double> bronsetvector=getOnsetTimes(); 
          vector<bool> quantizedonsets(n_time);

          // Convert onset times to a binary vector
          mf0e.quantizeonsets(bronsetvector, quantizedonsets, spectruminfo.time_resolution);
          
          // The function is the same as with beat option, but using the onset vector instead
          mf0e.applyhypothesisbeat(peaks,bestmelodynotes,quantizedonsets);          
          }
      break;      
    }
    // Get note features with durations
    FeatureSet remainingFeatures=getNoteFeatures(bestmelodynotes);
    
    return remainingFeatures;
}

vector<double>
MF0UA::getOnsetTimes()
{
    vector<double> onsetsvector;

    for (FeatureList::iterator i = odf.begin()+1; i != odf.end()-1; i++) 
    {
       Feature f=*i;
       Feature onset;
       
       onset.hasTimestamp=true;
       onset.timestamp=f.timestamp;
       
       float value=f.values[0];
       
       if (value>onsetsSensitivity && (i-1)->values[0] < value && value >= (i+1)->values[0])
       {
          float onsettime=atof(onset.timestamp.toString().c_str())-atof(origin.toString().c_str());
          onsetsvector.push_back(onsettime);
          onset.values.push_back(1);
          onset.label="Onset";
       }
    }
    return onsetsvector;
}

MF0UA::FeatureSet 
MF0UA::getNoteFeatures(const vectorchords &melodychords) 
{
    FeatureSet returnFeatureSet;

    map<int,double> activenotes;	// Detected pitches
    map<int,double> tmpactivenotes;

    double relative_time=0; //spectruminfo.time_resolution;

    double shift=0;
    if (algorithm==2) 
         shift=(spectruminfo.N/(float)spectruminfo.samplerate)/2.0; // times are centered in the window for onset detection option
    else shift=SHIFT*spectruminfo.time_resolution; // Slight shifting

    for (int i=0; i<(int)melodychords.size(); i++) 
    {
       tmpactivenotes=activenotes;
       relative_time=(melodychords[i].t_ini)*(spectruminfo.time_resolution);

       // Note offsets
       for (map<int,double>::iterator curr=activenotes.begin(); curr!=activenotes.end();curr++) 
       {
		int note=curr->first;
 		if (!(melodychords[i].isinchord(note)))
		{	
                   Feature n;
                   n.hasTimestamp=true;
                   n.hasDuration=true;
                   
                   // Add note to featureset
                   n.timestamp=Vamp::RealTime::fromSeconds(curr->second+shift) + origin;
                   n.values.push_back(curr->first);
                   float duration=relative_time-curr->second; 
                   n.duration=Vamp::RealTime::fromSeconds(duration);
                  // n.label="Note";
		
                   if (duration>=minnoteduration)
                     returnFeatureSet[0].push_back(n);
                   
                   tmpactivenotes.erase(note);
  		}
       }

       // Note onsets
       for (int j=0; j<(int)melodychords[i].numnotes(); j++) 
       {
          int note=melodychords[i].notes[j].first;
          if (tmpactivenotes.count(note)==0) 
			tmpactivenotes[note]=relative_time;
       }
       activenotes=tmpactivenotes;
    }
    return returnFeatureSet;
}


void 
MF0UA::initializeSpectrumInfo()
{
    spectruminfo.first_band_freq=kMINBANDFREQ;      // Minimum frequency for band processing
        
    spectruminfo.samplerate= sr;
    spectruminfo.N=getPreferredBlockSize();
    spectruminfo.res=kRES; // 4 by default, for zero padding
    spectruminfo.percentage=100-(getPreferredStepSize()/(float)(spectruminfo.N))*100;

    spectruminfo.freq_resolution=(double)(spectruminfo.samplerate)/((double)(spectruminfo.N)*spectruminfo.res);
    spectruminfo.winsize= spectruminfo.N*spectruminfo.res;
    spectruminfo.firstsample=(int)round(spectruminfo.W/spectruminfo.freq_resolution);       // Convert bandwidth (Hz->Samples)
    if (spectruminfo.percentage!=0) spectruminfo.time_resolution= (1.0-((double)spectruminfo.percentage/100.0))* (spectruminfo.N) / spectruminfo.samplerate;
    else spectruminfo.time_resolution=(double)(spectruminfo.N)/(double)(spectruminfo.samplerate);
    
    spectruminfo.min_sample=(int)floor(spectruminfo.first_band_freq/spectruminfo.freq_resolution);
}

mapa2
MF0UA::spectralPeakPicking(double spectrum[])
{
    mapa2 orderedspectrumpeaks;
    for (int i=1; i< (spectruminfo.winsize / 2) -1; i++) 
    {
        double spcurr= spectrum[i];
	double spprev= spectrum[i-1];
	double spnext= spectrum[i+1];  

        if (spcurr > kMINPEAKVALUE &&  spcurr > spprev && spcurr > spnext) 
        {
               orderedspectrumpeaks.insert(make_pair((i)*spectruminfo.freq_resolution, spcurr));
                    if (spcurr> spectruminfo.maxampval)
                          spectruminfo.maxampval=spcurr;
        }
    }
		
   return orderedspectrumpeaks; 
}
