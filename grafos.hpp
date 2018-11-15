#include <tr1/unordered_map>
#include <cstddef>
#include <list>
#include <limits>
#include <algorithm>

#define NUM_VERTICES(g) ( (g) ->vertex_count() )
// numero de vertices
#define NUM_ARCOS(g) ( (g) -> narcos )
// numero de arcos
#define ADJS_NO(i,g) ( (g) -> verts[i].adjs )
// primeiro arco da lista de adjacentes do nó i
#define PROX_ADJ(arco) ((arco) -> prox)
// proximo adjacente
#define ADJ_VALIDO(arco) (((arco) != NULL))
// se arco é válido
#define EXTREMO_FINAL(arco) ((arco) -> no_final)
// qual o extremo final de arco
#define VALOR_ARCO(arco) ((arco) -> valor)
// qual o valor do arco

// ---------------------------------------------------------
// More modern C++ features in C++98
namespace std_98 {
	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };
}
// ---------------------------------------------------------

template<class Value, class Index = Value>
struct arc {
	Index final;
	Value value;
	// arc<Value, Index> *next;
	arc(Value value, Index final);
};

template<class Value, class Index = Value>
struct node {
	std::list<arc<Value, Index> > adjs;
};

template<class Value, class Index = Value>
struct graph : std::tr1::unordered_map<Index, node<Value, Index> > {
	size_t n_arcs;

	// Index is assumed to be an integral value. If this were at least C++11,
	// we could use static asserts and type traits
	// Enabled only if the type passed for index is an integer type. Not modern, but C++98
	graph(size_t n,
	      typename // Unamed parameter used for SFINAE
	      std_98::enable_if<std::numeric_limits<Index>::is_integer, void>::type* = NULL) {
		this->reserve(n + 1);
		n_arcs = 0;
	}

	/* insere arco (i, j) no grafo, bem como o seu peso; não evita repetições */
	void insert_new_arc(Index i, Index j, Value value) {
		/* insere arco (i,j) no grafo */
		++n_arcs;
		this->at(i).adjs.push_front(new ::arc<Value, Index>(value, j));
	}

	/* retira arco adjacente da lista de adjacentes de i */
	bool remove_arc(Index i, Index j) {
		std::list<arc<Index, Value> >& adjs = this->at(i).adjs;
		for(typename std::list<arc<Index, Value> >::iterator &it = adjs.begin();
		    it != adjs.end(); ++it) {
			if(it->final == j) {
				--n_arcs;
				delete *it;
				adjs.erase(it);
				return true;
			}
		}
		return false;
	}
	bool remove_arc(Index i, arc<Value, Index> *arc) {
		// ::arc because the parameter has the same name
		std::list< ::arc<Value, Index> >& adjs = this->at(i).adjs;
		for(typename std::list< ::arc<Index, Value> >::iterator &it = adjs.begin();
		    it != adjs.end(); ++it) {
			if(*it == arc) {
				--n_arcs;
				delete *it;
				adjs.erase(it);
				return true;
			}
		}
		return true;
	}

	/* retorna um apontador para o arco (i, j) ou NULL se não existir */
	arc<Value, Index> * find_arc(Index i, Index j) {
		std::list< ::arc<Index, Value> >& adjs = this->at(i).adjs;
		for(typename std::list<arc<Index, Value> >::iterator &it = adjs.begin();
		    it != adjs.end(); ++it) {
			if(*it->final == j) {
				return *it;
			}
		}
		return NULL;
	}

	size_t vertex_count() { return this->size(); }
	std::list<arc<Value, Index> > adjs_node(Index i) { return this->at(i).adjs; }
};
