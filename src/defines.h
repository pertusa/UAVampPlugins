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

#ifndef DEFINES
#define DEFINES

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

#define vdouble vector<double>
#define mapa2 map<double, double, ltdoub>
#define itermapa2 map<double, double, ltdoub>::iterator

#define mapa3 map<double, double, gtdoub>
#define itermapa3 map<double, double, gtdoub>::iterator

#define peaksvector vector<mapa2>
#define iterpeaksvector vector<mapa2>::iterator

#define myvector vector<spectralpattern>
#define itermyvector vector<spectralpattern>::iterator

#define peaksattvector vector<peaksatt>
#define iterpeaksattvector vector<peaksatt>::iterator

#define notepair pair<int,double>
#define notesvector vector<notepair>

#define vectorchords vector<chord>

#define hashset set<int>
typedef std::vector< std::vector<double> > Tmatrix;

#define vectormapa2 vector<mapa2>

#define vint vector<int>

struct ltdoub
{
  bool operator()(const double n1, const double n2) const
  {
    return n1<n2;
  }
};

struct gtdoub
{
  bool operator()(const double n1, const double n2) const
  {
    return n1>n2;
  }
};

#endif
