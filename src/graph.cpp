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

#include "graph.h"

using namespace std;

// Commented for plugin compatibility with MacOS10.4 (unused function)
void MF0Graph::printGraph(const graph_t &g, const std::vector<float> &d, const vector<vertex_descriptor> &p, const string name[], const property_map<graph_t, edge_weight_t>::type &weightmap, const char filename[])
 {
/*   cout << "distances and parents:" << endl;
   graph_traits < graph_t >::vertex_iterator vi, vend;
   for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
     cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
     cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << 
          endl;
   }
   cout << endl;
 
   std::ofstream dot_file(filename);
   
   dot_file << "digraph D {\n"
          << "  rankdir=LR\n"
          << "  size=\"4,3\"\n"
          << "  ratio=\"fill\"\n"
          << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";
        
    graph_traits < graph_t >::edge_iterator ei, ei_end;
    
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
       graph_traits < graph_t >::edge_descriptor e = *ei;
       graph_traits < graph_t >::vertex_descriptor u = source(e, g), v = target(e, g);
       dot_file << name[u] << " -> " << name[v] << "[label=\"" << get(weightmap, e) << "\"";
       if (p[v] == u)
         dot_file << ", color=\"black\"";
       else
         dot_file << ", color=\"grey\"";
       dot_file << "]";
    }
    dot_file << "}";
*/
    }

std::vector<int> MF0Graph::buildgraph(std::string nodenames[], const int& num_nodes, Edge edge_array[], float weights[], int num_arcs, bool print, const char filename[])
{            
    graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<float> d(num_vertices(g));
    vertex_descriptor s = vertex(0, g);

    dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));

    graph_traits < graph_t >::vertex_iterator itinit, itend, it;
    tie(itinit,itend)=vertices(g);
   
    int lastnode=p[*(itend-1)];
    std::vector<int> shortestpath;
    while (lastnode!=0)
    {
      shortestpath.push_back(lastnode);
      lastnode=p[lastnode];
    }
    reverse(shortestpath.begin(), shortestpath.end());
    
    if (print) printGraph(g,d,p,nodenames,weightmap,filename);

    return shortestpath;
}
