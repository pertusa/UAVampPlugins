
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


#include "onsetsUA.h"
#include "info.h"
#include "params.h"
#include "myfft.h"
#include "onsetdetection.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>



OnsetsUA::OnsetsUA(float inputSampleRate) : Plugin(inputSampleRate),
   sensitivity(0.18)
{
    sr=inputSampleRate;
    mu=140.0/(44100.0/inputSampleRate); // No onsets will be yielded when the frame energy is lower than this threshold
    window=NULL;
}

OnsetsUA::~OnsetsUA()
{
     free(window);
}

string
OnsetsUA::getIdentifier() const
{
    return "onsetsua";
}

string
OnsetsUA::getName() const
{
    return "Note Onset Detector";
}

string
OnsetsUA::getDescription() const
{
    // Return something helpful here!
    return "Note onset detection using a one-semitone filterbank";
}

string
OnsetsUA::getMaker() const
{
    // Your name here
    return "University of Alicante";
}

int
OnsetsUA::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
OnsetsUA::getCopyright() const
{
    return "Copyright 2012 Antonio Pertusa, Universidad de Alicante. Freely redistributable under GPL License";
}

OnsetsUA::InputDomain
OnsetsUA::getInputDomain() const
{
    return TimeDomain;  // Must be time domain for zero-padding
}

size_t
OnsetsUA::getPreferredBlockSize() const
{
    // Calculate window size for a 93ms window length
    return round((4096.0/44100.0)*sr);
}

size_t 
OnsetsUA::getPreferredStepSize() const
{
    // 50% overlap
    return getPreferredBlockSize()/2;
}

size_t
OnsetsUA::getMinChannelCount() const
{
    return 1;
}

size_t
OnsetsUA::getMaxChannelCount() const
{
    return 1;
}

OnsetsUA::ParameterList
OnsetsUA::getParameterDescriptors() const
{
    ParameterList list;

    ParameterDescriptor t;
    t.identifier="sensitivity";
    t.name="Sensitivity";
    t.description="The onset detection function peaks over this threshold will be considered as onsets";
    t.unit="";
    t.minValue=0;
    t.maxValue=1;
    t.defaultValue=0.18;
    t.isQuantized=false;
    list.push_back(t);

/*    ParameterDescriptor m;
    m.identifier = "mu";
    m.name = "Silence threshold";
    m.description = "No onsets will be detected for lower energy values";
    m.unit = "";
    m.minValue = 0;
    m.maxValue = 100;
    m.defaultValue = 70;
    m.isQuantized = false;
    list.push_back(m);
*/
    return list;
}

float
OnsetsUA::getParameter(string identifier) const
{
    if (identifier == "sensitivity")
       return sensitivity;

    return 0;
}

void
OnsetsUA::setParameter(string identifier, float value) 
{
    if (identifier=="sensitivity")
       sensitivity=value;
}

OnsetsUA::ProgramList
OnsetsUA::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
OnsetsUA::getCurrentProgram() const
{
    return ""; // no programs
}

void
OnsetsUA::selectProgram(string name)
{
}

OnsetsUA::OutputList
OnsetsUA::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "odf";
    d.name = "Onset Detection Function";
    d.description = "";
    d.unit = "";
    d.hasFixedBinCount = true;
    d.binCount = 1;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType=OutputDescriptor::VariableSampleRate;
    d.hasDuration = false;
    list.push_back(d);

    d=OutputDescriptor();
    d.identifier="onsets";
    d.name="Note Onsets";
    d.unit="";
    d.hasFixedBinCount=true;
    d.binCount=0;
    d.sampleType=OutputDescriptor::VariableSampleRate;
    list.push_back(d);
    
    return list;
}

bool
OnsetsUA::initialise(size_t channels, size_t stepSize, size_t blockSize)
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

    // Bands generation
    generatebands(spectruminfo.first_band_freq, spectruminfo.samplerate/2, spectralbands, spectruminfo.freq_resolution);
    spectruminfo.numbands=spectralbands.size();

    // Do not compute differences for the first frame
    firstframe=true;

    return true;
}

void
OnsetsUA::reset()
{
    // Clear buffers, reset stored values, etc
    firstframe=true;
}

OnsetsUA::FeatureSet
OnsetsUA::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    FeatureSet returnFeatureSet;
    double input[spectruminfo.winsize];
    double spectrum[spectruminfo.winsize+1];

    // Copy input data and complete with zeroes for zero padding
    for (int i=0; i<(int)m_blockSize;i++)
        input[i]=inputBuffers[0][i]; 
    for (int i=m_blockSize; i<spectruminfo.winsize; i++)
        input[i]=0.0;

    // FFT computation
    fourier_spectrum(input, spectrum, spectruminfo.winsize, spectruminfo.winsize, window, spectruminfo.N);
    
    // One-semitone bands computation
    vector<double> currentFrameBands(spectruminfo.numbands);
    OnsetDetection::computebands(spectrum, spectralbands, currentFrameBands, spectruminfo.winsize / 2, spectruminfo.maxbandsval);
    
    // Onset detection function. It is stored as a feature, and also in the vector odf for final postprocessing
    if (!firstframe)
    {
       float diff=OnsetDetection::onsetfunction(previousFrameBands, currentFrameBands, mu);

       Feature detectionFunction;
       detectionFunction.hasTimestamp=true;
       detectionFunction.timestamp = timestamp+ Vamp::RealTime::frame2RealTime(m_stepSize, int(m_inputSampleRate +0.5));
       detectionFunction.values.push_back(diff);
       
       odf.push_back(detectionFunction); // To get the onsets in getRemainingFeatures

       returnFeatureSet[0].push_back(detectionFunction);
    }
    else firstframe=false;

    previousFrameBands=currentFrameBands;

    return returnFeatureSet;
}

OnsetsUA::FeatureSet
OnsetsUA::getRemainingFeatures()
{
    FeatureSet returnFeatureSet;

    for (FeatureList::iterator i = odf.begin()+1; i != odf.end()-1; i++) 
    {
       Feature f=*i;
       Feature onset;
       
       onset.hasTimestamp=true;
       onset.timestamp=f.timestamp;
       
       float value=f.values[0];
       
       if (value>sensitivity && (i-1)->values[0] < value && value >= (i+1)->values[0])
       {
          onset.values.push_back(1);
          onset.label="Onset";
          returnFeatureSet[1].push_back(onset);
       }
    }
    
    return returnFeatureSet;
}

void 
OnsetsUA::initializeSpectrumInfo()
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
