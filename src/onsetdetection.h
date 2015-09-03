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

#ifndef _ONSETDETECTION_H_
#define _ONSETDETECTION_H_


class OnsetDetection
{
   public:
     static float onsetfunction(const vector<double>& previousbands, const vector<double>& currentbands, float mu);
     static void computebands(double *fft, vectorbands& spectralbands, vector<double>& bandvalues, int fftlen, double &maxbandval);     
};

#endif
