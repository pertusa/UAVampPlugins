
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


// Remember to use a different guard symbol in each header!
#ifndef _MF0UA_H_
#define _MF0UA_H_

#include <vamp-sdk/Plugin.h>
#include "info.h"
#include "bands.h"
#include "chord.h"
#include "defines.h"
#include "peaksatt.h"
#include "mf0.h"

using std::string;

class MF0UA : public Vamp::Plugin
{
public:
    MF0UA(float inputSampleRate);
    virtual ~MF0UA();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    OutputList getOutputDescriptors() const;

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();

protected:

    void initializeSpectrumInfo();
    mapa2 spectralPeakPicking(double spectrum[]);
    FeatureSet getNoteFeatures(const vectorchords &melodychords);
    vector<double> getOnsetTimes();


    double *window;

    // Plugin params    
    size_t m_stepSize;
    size_t m_blockSize;
    float sr;
  
    // Spectrum information    
    pinfo spectruminfo;

    // Local variables for mf0 estimation
    vectorchords bestmelodynotes; //, prevmelodynotes;
    peaksattvector peaks; //, previouspeaks;
    int n_time;
    
    // Params
    int algorithm;
    int maxpolyphony;
    float lowestnote;
    float highestnote;
    float minnoteduration;

    // For selection extents
    Vamp::RealTime origin;
    
    // For algorithm based on onset detection
    vectorbands spectralbands;    
    vector<double> previousFrameBands;
    bool firstframe;
    float onsetsSensitivity,mu;
    FeatureList odf;
    int resolutiondiff; 
    
};

#endif
