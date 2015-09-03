
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


// Remember to use a different guard symbol in each header!
#ifndef _ONSETSUA_H_
#define _ONSETSUA_H_

#include <vamp-sdk/Plugin.h>
#include "info.h"
#include "bands.h"

using std::string;


class OnsetsUA : public Vamp::Plugin
{
public:
    OnsetsUA(float inputSampleRate);
    virtual ~OnsetsUA();

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

    // plugin-specific data and methods go here
    double *window;
    vectorbands spectralbands;

    // Plugin params    
    size_t m_stepSize;
    size_t m_blockSize;
    float sr;
  
    // Spectrum information    
    pinfo spectruminfo;
    
    // Energy for the bands of the previous frame
    vector<double> previousFrameBands;
    bool firstframe;
    
    // Vector storing the onset detection function for all the frames
    FeatureList odf;
    
    
    // Params
    double sensitivity;
    double mu; // Silence threshold
};

#endif
