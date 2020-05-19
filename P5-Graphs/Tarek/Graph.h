
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::map;

#define UNDISCOVERED 'u'
#define DISCOVERED   'd'
#define ACTIVE       'a'
#define FINISHED     'f'

template <typename T>
void pvec(const std::vector<T> & vec) {

  for(const T &x : vec) {
    std::cout << x << "\n";;
  }
}

class graph {

  private:

    // note:  this struct does not store both
    //   vertices in the edge -- just one.  This
    //   is because of the overall structure of
    //   the adjacency list organization:  an
    //   edge struct is stored in a vector associated
    //   with the other vertex.
    struct edge {
      int vertex_id;
      double weight;
      edge ( int vtx_id=0, double _weight=1.0) 
        : vertex_id { vtx_id}, weight { _weight} 
      { }
    };

    struct vertex {
      int id;
      vector<edge> outgoing;
      vector<edge> incoming;
      string name;

      vertex ( int _id=0, string _name="") 
        : id { _id }, name { _name } 
      { }
    };

    unordered_map<string, int> _name2id;
    unordered_set<string> edges;
    vector<vertex> vertices;

  public:

    // this struct is used for capturing the results of an operation.
    // typically a "report" will be a vector of vertex_labels indexed
    // by vertex-id.
    struct vertex_label {
      double dist;
      int pred;
      char state;
      int npaths;
      
      vertex_label( double _dist=0.0, int _pred=-1, char _state='?',
          int _npaths=0) 
        : dist { _dist }, pred { _pred }, state { _state}, npaths { 0 }
      { }

    };


    graph() {}

    ~graph() {}

  private:

    int add_vertex(const string &name) {
      int id = vertices.size();
        vertices.push_back(vertex(id, name));
        _name2id[name] = id;
        return id;
    }

    static
    string edge_string(const string &src, const string &dest) {
      return src + " " + dest;
    }

    void p_edge(edge &e) {
      std::cout << "(" << id2name(e.vertex_id) 
        << ", " << e.weight << ") ";
    }

  public:

    string  id2name(int id) {
      if(id<0 || id>=vertices.size())
        return "$NONE$";
      return vertices[id].name;
    }

    int name2id(const string &vtx_name) {
      if(_name2id.count(vtx_name)==0)
        return -1;
      return _name2id[vtx_name];
    }

    string name_vec2string(const vector<string> &vec) {
      string s = "";
      int i;

      if(vec.size()==0)
        return s;

      s = s + vec[0];
      for(i = 1; i<vec.size(); i++) {
        s = s + " " + vec[i];
      }
      return s;
    }

    string id_vec2string(const vector<int> &vec) {
      string s = "";
      int i;

      if(vec.size()==0)
        return s;

      s = s + id2name(vec[0]);
      for(i = 1; i<vec.size(); i++) {
        s = s + " " + id2name(vec[i]);
      }
      return s;
    }

    bool add_edge(const string &src, const string &dest, 
        double weight=1.0) {

      int s_id, d_id;

      string estring = edge_string(src, dest);

      if(edges.count(estring)==1) {
        std::cerr << "warning: duplicate edge '"
          << estring << "'\n";
        return false;
      }

      edges.insert(estring);

      // get id for source vertex
      if(_name2id.count(src)==0) 
        s_id = add_vertex(src);
      else
        s_id = _name2id[src];

      // get id for destination vertex
      if(_name2id.count(dest)==0) 
        d_id = add_vertex(dest);
      else
        d_id = _name2id[dest];

      vertices[s_id].outgoing.push_back(edge(d_id, weight));
      vertices[d_id].incoming.push_back(edge(s_id, weight));

      return true;
    }

    bool add_edge(const string &str) {
      std::stringstream ss(str);
      string src, dest, junk, weight_str;
      double weight;

      if(!(ss >> src))
        return false;
      if(!(ss >> dest))
        return false;
      if(!(ss >> weight_str)){
        // only two tokens: use default weight
        weight = 1.0;
      }
      else {
        if(!(std::stringstream(weight_str) >> weight)){
          // couldn't parse weight
          return false;
        }

        if(ss >> junk){
          // extra token?  format error
          return false;
        }
      }

      add_edge(src, dest, weight);

      return true;
    }

    void _add_edge(const string &str) {

      if(!add_edge(str))
        std::cout << "add_edge failed; str='" <<
          str << "'\n";
    }

    void display(){
      int u;

      for(u=0; u<vertices.size(); u++) {
        std::cout << vertices[u].name << " : ";

        for(edge &e : vertices[u].outgoing) 
          p_edge(e);
        std::cout << "\n";
      }
    }

    void ids2names(std::vector<int> &  ids, std::vector<string> & names) {
      names.clear();

      for(int &u : ids) {
        names.push_back(id2name(u));
      }
    }

    bool read_file(const string &fname) {
      std::ifstream file;
      string line;

      file.open(fname, std::ios::in);
      if(!file.is_open())
        return false;
      
      while(getline(file, line)) {
        // skip blank lines
        if(line.length() > 0) {
          if(!add_edge(line)) {
            std::cerr << "warning: skipped input line '" 
              << line << "' (ill-formatted)\n";
          }
        }
      }
      file.close();
      return true;
    }


    int num_nodes() {
      return vertices.size();
    }
    int num_edges() {
      return edges.size();
    }

  private:
    void init_report(std::vector<vertex_label> & report) {
      int u;

      report.clear();
      for(u=0; u<vertices.size(); u++) {
        report.push_back(vertex_label(-1, -1, UNDISCOVERED));
      }
    }


  public:
    //*******************************************************************************************

    bool bfs(int src, std::vector<vertex_label> &report) {
      int u, v;
      std::queue<int> q;
      std::map<int, int> id2size;

      if(src < 0 || src >= num_nodes())
        return false;

      init_report(report);

      report[src].dist = 0;
      report[src].pred = src;
      report[src].state = DISCOVERED;
      report[src].npaths = 1;
      q.push(src);

      while(!q.empty()) {

        u = q.front();
        q.pop();
        vector<int> path;

        for(edge &e : vertices[u].outgoing) {
          v = e.vertex_id;

	        if(report[v].state == UNDISCOVERED) {

		        report[v].dist = report[u].dist + 1;
            report[v].pred = u;
            report[v].state = DISCOVERED;

		        if (extract_path(report, v, path) == true) {

			      id2size[v] = path.size();
			      report[v].npaths = 1;
			      path.clear();
		        }

		        else {
			        report[v].npaths = 0;
		        }

		        q.push(v);
          }  

	        else {
		        if (extract_path(report, v, path) == true) {

			        if (id2size[v] == path.size()) {
				        report[v].npaths += 1;
				        path.clear();
			        }
			
			        else if (id2size[v] > path.size()) {
				        id2size[v] = path.size();
			          report[v].npaths = 1;
			          path.clear();
			        }
		        } 
		
		        else {
		        	report[v].npaths = 0;
		        }
	        }
	      }
      }

      return true;
    }

    bool bfs(const string src, std::vector<vertex_label> &report) {

      int u;

      if((u=name2id(src)) == -1)
          return false;
      bfs(u, report);
      return true;
    }

  private:

    void _dfs(int u, vector<vertex_label> & rpt, bool &cycle) {

      int v;

      rpt[u].state = ACTIVE;
      for(edge &e : vertices[u].outgoing) {
        v = e.vertex_id;
        if(rpt[v].state == UNDISCOVERED) {
          rpt[v].pred = u;
          rpt[v].dist = rpt[u].dist + 1;
          _dfs(v, rpt, cycle);
        }
        if(rpt[v].state == ACTIVE) 
          cycle = true;
      }
      rpt[u].state = FINISHED;
    }

  public:
    bool dfs(int u, vector<vertex_label> & rpt, bool &cycle) {

      if(u < 0 || u >= num_nodes()) 
        return false;

      cycle = false;

      init_report(rpt);
      rpt[u].pred = u;
      rpt[u].dist = 0;
      _dfs(u, rpt, cycle);
      return true;
    }

    bool dfs(const string &src, vector<vertex_label> & rpt, bool &cycle) {
      int u;

      if((u=name2id(src)) == -1)
          return false;
      dfs(u, rpt, cycle);
      return true;
    }

    bool has_cycle() {
      int u;
      bool cycle=false;
      vector<vertex_label> rpt;

      init_report(rpt);
      for(u=0; u<num_nodes(); u++) {
        if(rpt[u].state == UNDISCOVERED) {
          _dfs(u, rpt, cycle);
          if(cycle)
            return true;
        }
      }
      return false;
    }

    bool topo_sort(std::vector<int> &order) {
      std::queue<int> q;
      std::vector<int> indegrees;
      int u, v;
      int indeg;

      order.clear();
      if(has_cycle())
        return false;

      for(u=0; u<num_nodes(); u++) {
        indeg = vertices[u].incoming.size();

        indegrees.push_back(indeg);
        if(indeg==0)
          q.push(u);
      }

      while(!q.empty()){
        u = q.front();
        q.pop();
        order.push_back(u);
        for(edge &e : vertices[u].outgoing) {
          v = e.vertex_id;
          indegrees[v]--;
          if(indegrees[v]==0) 
            q.push(v);
        }
      }
      return true;
    }



    void disp_report(const vector<vertex_label> & rpt, 
        bool print_paths=false) {
      int u;
      vector<int> path;

       // THIS if STATEMENT IS NEW
        if(rpt.size() != num_nodes()) {
          std::cerr << "error - disp_report(): report vector has incorrect length\n";
          return;
        }

        for(u=0; u<num_nodes(); u++) {
          std::cout << id2name(u) << " : dist=" <<  rpt[u].dist
            << " ; pred=" <<  id2name(rpt[u].pred) << 
            " ; state='" << rpt[u].state << "'; npaths=" << 
            rpt[u].npaths << "\n";
          if(print_paths) {
            extract_path(rpt, u, path);
            std::cout << "     PATH: <" + id_vec2string(path) + ">\n";
          }
        }
    }

    //*********************************************************************************

    bool extract_path(const vector<vertex_label> & rpt, int dest, vector<int> & path) {

      path.clear();

      if(rpt.size() != num_nodes())
        return false;

	  if (rpt.at(dest).pred == dest) 
	  {
		  path.push_back(dest);
		  return true;
	  }

	  while (rpt.at(dest).pred != dest) 
	  {

		  if (rpt.at(dest).pred == -1) 
		  {
			  path.clear();
			  return false;
		  }

		  path.push_back(dest);
		  dest = rpt.at(dest).pred;
	  }

      return true;  
    }

    //*********************************************************************************************
    bool dag_critical_paths(vector<vertex_label> & rpt) {

      
	if(has_cycle())
        return false;

		if (rpt.size() == 0) 
		{
			init_report(rpt);
		}

		std::vector<int> sorted;
		
		if (topo_sort(sorted) == false)
	       	{
			return false;
		}

		for (int u = 0; u < sorted.size(); u++) 
		{
			for (int v = 0; v < vertices[u].outgoing.size(); v++) 
			{
				edge currEdge = vertices[u].outgoing[v];
				int V = rpt[currEdge.vertex_id].dist;
				int U = rpt[vertices[u].id].dist;
				int edgeWeight = currEdge.weight;
				
				if ( V < U + edgeWeight ) 
				{
					rpt[currEdge.vertex_id].dist = U + edgeWeight;
					rpt[currEdge.vertex_id].pred = vertices[u].id;
				}

			}
		}

      return true;
    }

        //***************************************************************************************
	
	bool dag_num_paths(vector<vertex_label> & rpt) {

                std::vector<int> sorted;
		std::map<int, int> inputPaths;
		std::map<int, int> outputPaths;

		if (has_cycle()) 
		{
			return false;
		}

		if (rpt.size() == 0) 
		{
			init_report(rpt);
		}


		

		if (topo_sort(sorted) == false) {
			return false;
		}

		for (int u = 0; u < sorted.size(); u++) 
		{

			int curr = sorted[u];
			int n = 0;

			if (vertices[curr].incoming.size() == 0)
		       	{
				inputPaths[curr] = 1;

			}

			else 
			{

			for (int v = 0; v < vertices[curr].incoming.size(); v++) {

				int in = vertices[curr].incoming[v].vertex_id;
				n = n + inputPaths[in];
			}

			inputPaths[curr] = n;
		}
	}

		for (int u = sorted.size() - 1; u >= 0; u--) 
		{

			int curr = sorted[u];
			if (vertices[curr].outgoing.size() == 0) 
			{
				outputPaths[curr] = 1;
			}

			else 
			{

				int n = 0;
				for (int v = 0; v < vertices[curr].outgoing.size(); v++) {

					int in = vertices[curr].outgoing[v].vertex_id;
					n += outputPaths[in];
				}

				outputPaths[curr] = n;
			}
		}

	  for (int i = 0; i < vertices.size(); i++) {
		  rpt[i].npaths = inputPaths[i] * outputPaths[i];
	  }

      return true;
    }

    //******************************************************************************************

    bool valid_topo_order(const vector<int> & order) {
      if(has_cycle())
        return false;
	  std::map<int, int> id2index;

	  for (int i = 0; i < order.size(); i++ ) 
	  {
		  id2index[order[i]] = i;
	  }

	  for (int u = 0; u < order.size(); u++)
	  {
		  for (int v = 0; v < vertices[order[u]].outgoing.size(); v++) 
		  {
			  int IDout = vertices[order[u]].outgoing[v].vertex_id;

			  if (id2index[IDout] < u) 
			  {
				  return false;
			  }
		  }
	  }
      return true;

    }

    //******************************************************************************************

    bool enum_paths(int target, vector<string> &paths) {

      paths.clear();
      if(has_cycle() || target < 0 || target >= num_nodes())
        return false;

	  vertex Targets = vertices[target];
	  vector<string> Names;
	  Names.push_back(Targets.name);

	  
	  for (int u = 0; u < Targets.incoming.size(); u++) 
	  {
		  trav(Targets.incoming[u], Names, paths);
		  
		  
	  }

	  Names.clear();

      return true;
    }

	void trav(edge parent, vector<string> &Names, vector<string> &paths) {

		vertex Targets = vertices[parent.vertex_id];
		Names.push_back(Targets.name);

		if (Targets.incoming.size() == 0) 
		{
			string names = Enum(Names);
			paths.push_back(names);
			Names.pop_back();
			return;

		}

		for (int u = 0; u < Targets.incoming.size(); u++) 
		{
			
			trav(Targets.incoming[u], Names, paths);
		}

		Names.pop_back();

	}

	string Enum(vector<string> &Names) {

		string Path;

		for (int i = Names.size()-1; i >= 0; i--) 
		{
			Path = Path + Names[i];
			if (i != 0) {
				Path += " ";
			}
		}
		return Path;
	}



    /*
     * (DONE)
     * func: enum_paths(string, vector<string> &)
     * desc: same as enum_paths(int, vector<string> &) above except
     *       target vertex is taken as its name (as a string).
     *
     *       Simply translates target vertex name to its integer id
     *       and calls enum_paths(int, vector<string> &) above.
     */
    bool enum_paths(const string &target,  vector<string> &paths) {

      int Target;

      if((Target=name2id(target)) == -1)
          return false;

      return enum_paths(Target, paths);
    }
    



};

