#include <tr1/unordered_map>
#include <cstddef>
#include <vector>
#include <limits>

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
	arc<Value, Index> *next;
	arc(Value value, Index final);
};

template<class Value, class Index = Value>
struct node {
	std::vector<arc<Value, Index> > adjs;
};

template<class Value, class Index = Value>
struct graph : std::tr1::unordered_map<Index, node<Value, Index> > {
	// Index is assumed to be an integral value. If this were at least C++11,
	// we could use static asserts and type traits
	// Enabled only if the type passed for index is an integer type. Not modern, but C++98
	graph(size_t n,
	      typename
	      std_98::enable_if<std::numeric_limits<Index>::is_integer, void>::type* = nullptr) {
		this->reserve(n + 1);
	}

	/* insere arco (i, j) no grafo, bem como o seu peso; não evita repetições */
	void insert_new_arc(Index i, Index j, Value value) {
		/* insere arco (i,j) no grafo */
		arc<Value, Index> arc = new arc{value, j};
		arc.next = this->at(i).adjs;
		this->at(i).adjs = arc;
	}

	/* retira adjacente arco da lista de adjacentes de i */
	void remove_arc(Index i, Index j) {
		this->at(i).adjs.at(j);
	}
	void remove_arc(Index i, arc<Value, Index> *arc);

	/* retorna um apontador para o arco (i, j) ou NULL se não existir */
	arc<Value, Index> * find_arc(Index i, Index j);

	size_t index_count() { return this->size(); }
	std::vector<arc<Value, Index> > adjs_node(Index i) { return (*this)[i].adjs; }
};
