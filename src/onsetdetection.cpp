/****************
Copyright (c) 2005-2012 Antonio Pertusa Ibáñez <pertusa@dlsi.ua.es>
Copyright (c) 2012 Universidad de Alicante

This onset detection system is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or (at your
 option) any later version.

This onset detection system is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code.  If not, see <http://www.gnu.org/licenses/>.

 Comments are welcomed
 
**************/


#include "params.h"
#include "bands.h"
#include "onsetdetection.h"

#include <math.h>

/*----------------------------------
 Onset detection function
 ----------------------------------*/
float 
OnsetDetection::onsetfunction(const vector<double>& previousbands, const vector<double>& currentbands, float mu)
{
     double sum=0;
     double diff=0;

     vector<double>::const_iterator itcurrent;
     vector<double>::const_iterator itprevious=previousbands.begin();

     for (itcurrent=currentbands.begin(); itcurrent!=currentbands.end(); itcurrent++) 
     {
         float banddiff=(*itcurrent)-(*itprevious);
         if (banddiff>0)
           diff+=banddiff;
        
         sum+=fabs(*itcurrent);
         itprevious++;
     }
     
     if (sum>mu)  // Silence threshold
          return diff/sum;
     else return 0;
}

void
OnsetDetection::computebands(double *fft, vectorbands& spectralbands, vector<double>& bandvalues, int fftlen, double& maxbandval) 
{
	if (!bandvalues.empty()) bandvalues.clear();

	for (int i=0; i<(int)spectralbands.size(); i++) {
		double val;
		if (spectralbands[i].size!=0) 
		{
                    val=spectralbands[i].applybandRMS(fft,fftlen);
		}
		else val=0.0;
		if (val>maxbandval) maxbandval=val;
           
		bandvalues.push_back(val);
	}
}


