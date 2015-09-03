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

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/config.hpp>

using namespace boost;

typedef property<edge_weight_t, float> VertexNameProperty;
typedef adjacency_list < listS, vecS, directedS, no_property, VertexNameProperty > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;

class MF0Graph
{
 public:
    static void printGraph(const graph_t &g, const std::vector<float> &d, const std::vector<vertex_descriptor> &p, const std::string name[], const property_map<graph_t, edge_weight_t>::type &weightmap, const char filename[]);
    static std::vector<int> buildgraph(std::string nodenames[], const int& num_nodes, Edge edge_array[], float weights[], int num_arcs, bool print, const char filename[]);
};

#endif
