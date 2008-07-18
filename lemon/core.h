/* -*- mode: C++; indent-tabs-mode: nil; -*-
 *
 * This file is a part of LEMON, a generic C++ optimization library.
 *
 * Copyright (C) 2003-2008
 * Egervary Jeno Kombinatorikus Optimalizalasi Kutatocsoport
 * (Egervary Research Group on Combinatorial Optimization, EGRES).
 *
 * Permission to use, modify and distribute this software is granted
 * provided that this copyright notice appears in all copies. For
 * precise terms see the accompanying LICENSE file.
 *
 * This software is provided "AS IS" with no warranty of any kind,
 * express or implied, and with no claim as to its suitability for any
 * purpose.
 *
 */

#ifndef LEMON_CORE_H
#define LEMON_CORE_H

#include <vector>
#include <algorithm>

#include <lemon/bits/enable_if.h>
#include <lemon/bits/traits.h>

///\file
///\brief LEMON core utilities.

namespace lemon {

  /// \brief Dummy type to make it easier to create invalid iterators.
  ///
  /// Dummy type to make it easier to create invalid iterators.
  /// See \ref INVALID for the usage.
  struct Invalid {
  public:
    bool operator==(Invalid) { return true;  }
    bool operator!=(Invalid) { return false; }
    bool operator< (Invalid) { return false; }
  };

  /// \brief Invalid iterators.
  ///
  /// \ref Invalid is a global type that converts to each iterator
  /// in such a way that the value of the target iterator will be invalid.
#ifdef LEMON_ONLY_TEMPLATES
  const Invalid INVALID = Invalid();
#else
  extern const Invalid INVALID;
#endif

  /// \addtogroup gutils
  /// @{

  ///Creates convenience typedefs for the digraph types and iterators

  ///This \c \#define creates convenience typedefs for the following types
  ///of \c Digraph: \c Node,  \c NodeIt, \c Arc, \c ArcIt, \c InArcIt,
  ///\c OutArcIt, \c BoolNodeMap, \c IntNodeMap, \c DoubleNodeMap,
  ///\c BoolArcMap, \c IntArcMap, \c DoubleArcMap.
  ///
  ///\note If the graph type is a dependent type, ie. the graph type depend
  ///on a template parameter, then use \c TEMPLATE_DIGRAPH_TYPEDEFS()
  ///macro.
#define DIGRAPH_TYPEDEFS(Digraph)                                       \
  typedef Digraph::Node Node;                                           \
  typedef Digraph::NodeIt NodeIt;                                       \
  typedef Digraph::Arc Arc;                                             \
  typedef Digraph::ArcIt ArcIt;                                         \
  typedef Digraph::InArcIt InArcIt;                                     \
  typedef Digraph::OutArcIt OutArcIt;                                   \
  typedef Digraph::NodeMap<bool> BoolNodeMap;                           \
  typedef Digraph::NodeMap<int> IntNodeMap;                             \
  typedef Digraph::NodeMap<double> DoubleNodeMap;                       \
  typedef Digraph::ArcMap<bool> BoolArcMap;                             \
  typedef Digraph::ArcMap<int> IntArcMap;                               \
  typedef Digraph::ArcMap<double> DoubleArcMap

  ///Creates convenience typedefs for the digraph types and iterators

  ///\see DIGRAPH_TYPEDEFS
  ///
  ///\note Use this macro, if the graph type is a dependent type,
  ///ie. the graph type depend on a template parameter.
#define TEMPLATE_DIGRAPH_TYPEDEFS(Digraph)                              \
  typedef typename Digraph::Node Node;                                  \
  typedef typename Digraph::NodeIt NodeIt;                              \
  typedef typename Digraph::Arc Arc;                                    \
  typedef typename Digraph::ArcIt ArcIt;                                \
  typedef typename Digraph::InArcIt InArcIt;                            \
  typedef typename Digraph::OutArcIt OutArcIt;                          \
  typedef typename Digraph::template NodeMap<bool> BoolNodeMap;         \
  typedef typename Digraph::template NodeMap<int> IntNodeMap;           \
  typedef typename Digraph::template NodeMap<double> DoubleNodeMap;     \
  typedef typename Digraph::template ArcMap<bool> BoolArcMap;           \
  typedef typename Digraph::template ArcMap<int> IntArcMap;             \
  typedef typename Digraph::template ArcMap<double> DoubleArcMap

  ///Creates convenience typedefs for the graph types and iterators

  ///This \c \#define creates the same convenience typedefs as defined
  ///by \ref DIGRAPH_TYPEDEFS(Graph) and six more, namely it creates
  ///\c Edge, \c EdgeIt, \c IncEdgeIt, \c BoolEdgeMap, \c IntEdgeMap,
  ///\c DoubleEdgeMap.
  ///
  ///\note If the graph type is a dependent type, ie. the graph type depend
  ///on a template parameter, then use \c TEMPLATE_DIGRAPH_TYPEDEFS()
  ///macro.
#define GRAPH_TYPEDEFS(Graph)                                           \
  DIGRAPH_TYPEDEFS(Graph);                                              \
  typedef Graph::Edge Edge;                                             \
  typedef Graph::EdgeIt EdgeIt;                                         \
  typedef Graph::IncEdgeIt IncEdgeIt;                                   \
  typedef Graph::EdgeMap<bool> BoolEdgeMap;                             \
  typedef Graph::EdgeMap<int> IntEdgeMap;                               \
  typedef Graph::EdgeMap<double> DoubleEdgeMap

  ///Creates convenience typedefs for the graph types and iterators

  ///\see GRAPH_TYPEDEFS
  ///
  ///\note Use this macro, if the graph type is a dependent type,
  ///ie. the graph type depend on a template parameter.
#define TEMPLATE_GRAPH_TYPEDEFS(Graph)                                  \
  TEMPLATE_DIGRAPH_TYPEDEFS(Graph);                                     \
  typedef typename Graph::Edge Edge;                                    \
  typedef typename Graph::EdgeIt EdgeIt;                                \
  typedef typename Graph::IncEdgeIt IncEdgeIt;                          \
  typedef typename Graph::template EdgeMap<bool> BoolEdgeMap;           \
  typedef typename Graph::template EdgeMap<int> IntEdgeMap;             \
  typedef typename Graph::template EdgeMap<double> DoubleEdgeMap

  /// \brief Function to count the items in the graph.
  ///
  /// This function counts the items (nodes, arcs etc) in the graph.
  /// The complexity of the function is O(n) because
  /// it iterates on all of the items.
  template <typename Graph, typename Item>
  inline int countItems(const Graph& g) {
    typedef typename ItemSetTraits<Graph, Item>::ItemIt ItemIt;
    int num = 0;
    for (ItemIt it(g); it != INVALID; ++it) {
      ++num;
    }
    return num;
  }

  // Node counting:

  namespace _core_bits {

    template <typename Graph, typename Enable = void>
    struct CountNodesSelector {
      static int count(const Graph &g) {
        return countItems<Graph, typename Graph::Node>(g);
      }
    };

    template <typename Graph>
    struct CountNodesSelector<
      Graph, typename
      enable_if<typename Graph::NodeNumTag, void>::type>
    {
      static int count(const Graph &g) {
        return g.nodeNum();
      }
    };
  }

  /// \brief Function to count the nodes in the graph.
  ///
  /// This function counts the nodes in the graph.
  /// The complexity of the function is O(n) but for some
  /// graph structures it is specialized to run in O(1).
  ///
  /// If the graph contains a \e nodeNum() member function and a
  /// \e NodeNumTag tag then this function calls directly the member
  /// function to query the cardinality of the node set.
  template <typename Graph>
  inline int countNodes(const Graph& g) {
    return _core_bits::CountNodesSelector<Graph>::count(g);
  }

  // Arc counting:

  namespace _core_bits {

    template <typename Graph, typename Enable = void>
    struct CountArcsSelector {
      static int count(const Graph &g) {
        return countItems<Graph, typename Graph::Arc>(g);
      }
    };

    template <typename Graph>
    struct CountArcsSelector<
      Graph,
      typename enable_if<typename Graph::ArcNumTag, void>::type>
    {
      static int count(const Graph &g) {
        return g.arcNum();
      }
    };
  }

  /// \brief Function to count the arcs in the graph.
  ///
  /// This function counts the arcs in the graph.
  /// The complexity of the function is O(e) but for some
  /// graph structures it is specialized to run in O(1).
  ///
  /// If the graph contains a \e arcNum() member function and a
  /// \e EdgeNumTag tag then this function calls directly the member
  /// function to query the cardinality of the arc set.
  template <typename Graph>
  inline int countArcs(const Graph& g) {
    return _core_bits::CountArcsSelector<Graph>::count(g);
  }

  // Edge counting:
  namespace _core_bits {

    template <typename Graph, typename Enable = void>
    struct CountEdgesSelector {
      static int count(const Graph &g) {
        return countItems<Graph, typename Graph::Edge>(g);
      }
    };

    template <typename Graph>
    struct CountEdgesSelector<
      Graph,
      typename enable_if<typename Graph::EdgeNumTag, void>::type>
    {
      static int count(const Graph &g) {
        return g.edgeNum();
      }
    };
  }

  /// \brief Function to count the edges in the graph.
  ///
  /// This function counts the edges in the graph.
  /// The complexity of the function is O(m) but for some
  /// graph structures it is specialized to run in O(1).
  ///
  /// If the graph contains a \e edgeNum() member function and a
  /// \e EdgeNumTag tag then this function calls directly the member
  /// function to query the cardinality of the edge set.
  template <typename Graph>
  inline int countEdges(const Graph& g) {
    return _core_bits::CountEdgesSelector<Graph>::count(g);

  }


  template <typename Graph, typename DegIt>
  inline int countNodeDegree(const Graph& _g, const typename Graph::Node& _n) {
    int num = 0;
    for (DegIt it(_g, _n); it != INVALID; ++it) {
      ++num;
    }
    return num;
  }

  /// \brief Function to count the number of the out-arcs from node \c n.
  ///
  /// This function counts the number of the out-arcs from node \c n
  /// in the graph.
  template <typename Graph>
  inline int countOutArcs(const Graph& _g,  const typename Graph::Node& _n) {
    return countNodeDegree<Graph, typename Graph::OutArcIt>(_g, _n);
  }

  /// \brief Function to count the number of the in-arcs to node \c n.
  ///
  /// This function counts the number of the in-arcs to node \c n
  /// in the graph.
  template <typename Graph>
  inline int countInArcs(const Graph& _g,  const typename Graph::Node& _n) {
    return countNodeDegree<Graph, typename Graph::InArcIt>(_g, _n);
  }

  /// \brief Function to count the number of the inc-edges to node \c n.
  ///
  /// This function counts the number of the inc-edges to node \c n
  /// in the graph.
  template <typename Graph>
  inline int countIncEdges(const Graph& _g,  const typename Graph::Node& _n) {
    return countNodeDegree<Graph, typename Graph::IncEdgeIt>(_g, _n);
  }

  namespace _core_bits {

    template <typename Digraph, typename Item, typename RefMap>
    class MapCopyBase {
    public:
      virtual void copy(const Digraph& from, const RefMap& refMap) = 0;

      virtual ~MapCopyBase() {}
    };

    template <typename Digraph, typename Item, typename RefMap,
              typename ToMap, typename FromMap>
    class MapCopy : public MapCopyBase<Digraph, Item, RefMap> {
    public:

      MapCopy(ToMap& tmap, const FromMap& map)
        : _tmap(tmap), _map(map) {}

      virtual void copy(const Digraph& digraph, const RefMap& refMap) {
        typedef typename ItemSetTraits<Digraph, Item>::ItemIt ItemIt;
        for (ItemIt it(digraph); it != INVALID; ++it) {
          _tmap.set(refMap[it], _map[it]);
        }
      }

    private:
      ToMap& _tmap;
      const FromMap& _map;
    };

    template <typename Digraph, typename Item, typename RefMap, typename It>
    class ItemCopy : public MapCopyBase<Digraph, Item, RefMap> {
    public:

      ItemCopy(It& it, const Item& item) : _it(it), _item(item) {}

      virtual void copy(const Digraph&, const RefMap& refMap) {
        _it = refMap[_item];
      }

    private:
      It& _it;
      Item _item;
    };

    template <typename Digraph, typename Item, typename RefMap, typename Ref>
    class RefCopy : public MapCopyBase<Digraph, Item, RefMap> {
    public:

      RefCopy(Ref& map) : _map(map) {}

      virtual void copy(const Digraph& digraph, const RefMap& refMap) {
        typedef typename ItemSetTraits<Digraph, Item>::ItemIt ItemIt;
        for (ItemIt it(digraph); it != INVALID; ++it) {
          _map.set(it, refMap[it]);
        }
      }

    private:
      Ref& _map;
    };

    template <typename Digraph, typename Item, typename RefMap,
              typename CrossRef>
    class CrossRefCopy : public MapCopyBase<Digraph, Item, RefMap> {
    public:

      CrossRefCopy(CrossRef& cmap) : _cmap(cmap) {}

      virtual void copy(const Digraph& digraph, const RefMap& refMap) {
        typedef typename ItemSetTraits<Digraph, Item>::ItemIt ItemIt;
        for (ItemIt it(digraph); it != INVALID; ++it) {
          _cmap.set(refMap[it], it);
        }
      }

    private:
      CrossRef& _cmap;
    };

    template <typename Digraph, typename Enable = void>
    struct DigraphCopySelector {
      template <typename From, typename NodeRefMap, typename ArcRefMap>
      static void copy(Digraph &to, const From& from,
                       NodeRefMap& nodeRefMap, ArcRefMap& arcRefMap) {
        for (typename From::NodeIt it(from); it != INVALID; ++it) {
          nodeRefMap[it] = to.addNode();
        }
        for (typename From::ArcIt it(from); it != INVALID; ++it) {
          arcRefMap[it] = to.addArc(nodeRefMap[from.source(it)],
                                    nodeRefMap[from.target(it)]);
        }
      }
    };

    template <typename Digraph>
    struct DigraphCopySelector<
      Digraph,
      typename enable_if<typename Digraph::BuildTag, void>::type>
    {
      template <typename From, typename NodeRefMap, typename ArcRefMap>
      static void copy(Digraph &to, const From& from,
                       NodeRefMap& nodeRefMap, ArcRefMap& arcRefMap) {
        to.build(from, nodeRefMap, arcRefMap);
      }
    };

    template <typename Graph, typename Enable = void>
    struct GraphCopySelector {
      template <typename From, typename NodeRefMap, typename EdgeRefMap>
      static void copy(Graph &to, const From& from,
                       NodeRefMap& nodeRefMap, EdgeRefMap& edgeRefMap) {
        for (typename From::NodeIt it(from); it != INVALID; ++it) {
          nodeRefMap[it] = to.addNode();
        }
        for (typename From::EdgeIt it(from); it != INVALID; ++it) {
          edgeRefMap[it] = to.addEdge(nodeRefMap[from.u(it)],
                                      nodeRefMap[from.v(it)]);
        }
      }
    };

    template <typename Graph>
    struct GraphCopySelector<
      Graph,
      typename enable_if<typename Graph::BuildTag, void>::type>
    {
      template <typename From, typename NodeRefMap, typename EdgeRefMap>
      static void copy(Graph &to, const From& from,
                       NodeRefMap& nodeRefMap, EdgeRefMap& edgeRefMap) {
        to.build(from, nodeRefMap, edgeRefMap);
      }
    };

  }

  /// \brief Class to copy a digraph.
  ///
  /// Class to copy a digraph to another digraph (duplicate a digraph). The
  /// simplest way of using it is through the \c copyDigraph() function.
  ///
  /// This class not just make a copy of a graph, but it can create
  /// references and cross references between the nodes and arcs of
  /// the two graphs, it can copy maps for use with the newly created
  /// graph and copy nodes and arcs.
  ///
  /// To make a copy from a graph, first an instance of DigraphCopy
  /// should be created, then the data belongs to the graph should
  /// assigned to copy. In the end, the \c run() member should be
  /// called.
  ///
  /// The next code copies a graph with several data:
  ///\code
  ///  DigraphCopy<NewGraph, OrigGraph> dc(new_graph, orig_graph);
  ///  // create a reference for the nodes
  ///  OrigGraph::NodeMap<NewGraph::Node> nr(orig_graph);
  ///  dc.nodeRef(nr);
  ///  // create a cross reference (inverse) for the arcs
  ///  NewGraph::ArcMap<OrigGraph::Arc> acr(new_graph);
  ///  dc.arcCrossRef(acr);
  ///  // copy an arc map
  ///  OrigGraph::ArcMap<double> oamap(orig_graph);
  ///  NewGraph::ArcMap<double> namap(new_graph);
  ///  dc.arcMap(namap, oamap);
  ///  // copy a node
  ///  OrigGraph::Node on;
  ///  NewGraph::Node nn;
  ///  dc.node(nn, on);
  ///  // Executions of copy
  ///  dc.run();
  ///\endcode
  template <typename To, typename From>
  class DigraphCopy {
  private:

    typedef typename From::Node Node;
    typedef typename From::NodeIt NodeIt;
    typedef typename From::Arc Arc;
    typedef typename From::ArcIt ArcIt;

    typedef typename To::Node TNode;
    typedef typename To::Arc TArc;

    typedef typename From::template NodeMap<TNode> NodeRefMap;
    typedef typename From::template ArcMap<TArc> ArcRefMap;


  public:


    /// \brief Constructor for the DigraphCopy.
    ///
    /// It copies the content of the \c _from digraph into the
    /// \c _to digraph.
    DigraphCopy(To& to, const From& from)
      : _from(from), _to(to) {}

    /// \brief Destructor of the DigraphCopy
    ///
    /// Destructor of the DigraphCopy
    ~DigraphCopy() {
      for (int i = 0; i < int(_node_maps.size()); ++i) {
        delete _node_maps[i];
      }
      for (int i = 0; i < int(_arc_maps.size()); ++i) {
        delete _arc_maps[i];
      }

    }

    /// \brief Copies the node references into the given map.
    ///
    /// Copies the node references into the given map. The parameter
    /// should be a map, which key type is the Node type of the source
    /// graph, while the value type is the Node type of the
    /// destination graph.
    template <typename NodeRef>
    DigraphCopy& nodeRef(NodeRef& map) {
      _node_maps.push_back(new _core_bits::RefCopy<From, Node,
                           NodeRefMap, NodeRef>(map));
      return *this;
    }

    /// \brief Copies the node cross references into the given map.
    ///
    ///  Copies the node cross references (reverse references) into
    ///  the given map. The parameter should be a map, which key type
    ///  is the Node type of the destination graph, while the value type is
    ///  the Node type of the source graph.
    template <typename NodeCrossRef>
    DigraphCopy& nodeCrossRef(NodeCrossRef& map) {
      _node_maps.push_back(new _core_bits::CrossRefCopy<From, Node,
                           NodeRefMap, NodeCrossRef>(map));
      return *this;
    }

    /// \brief Make copy of the given map.
    ///
    /// Makes copy of the given map for the newly created digraph.
    /// The new map's key type is the destination graph's node type,
    /// and the copied map's key type is the source graph's node type.
    template <typename ToMap, typename FromMap>
    DigraphCopy& nodeMap(ToMap& tmap, const FromMap& map) {
      _node_maps.push_back(new _core_bits::MapCopy<From, Node,
                           NodeRefMap, ToMap, FromMap>(tmap, map));
      return *this;
    }

    /// \brief Make a copy of the given node.
    ///
    /// Make a copy of the given node.
    DigraphCopy& node(TNode& tnode, const Node& snode) {
      _node_maps.push_back(new _core_bits::ItemCopy<From, Node,
                           NodeRefMap, TNode>(tnode, snode));
      return *this;
    }

    /// \brief Copies the arc references into the given map.
    ///
    /// Copies the arc references into the given map.
    template <typename ArcRef>
    DigraphCopy& arcRef(ArcRef& map) {
      _arc_maps.push_back(new _core_bits::RefCopy<From, Arc,
                          ArcRefMap, ArcRef>(map));
      return *this;
    }

    /// \brief Copies the arc cross references into the given map.
    ///
    ///  Copies the arc cross references (reverse references) into
    ///  the given map.
    template <typename ArcCrossRef>
    DigraphCopy& arcCrossRef(ArcCrossRef& map) {
      _arc_maps.push_back(new _core_bits::CrossRefCopy<From, Arc,
                          ArcRefMap, ArcCrossRef>(map));
      return *this;
    }

    /// \brief Make copy of the given map.
    ///
    /// Makes copy of the given map for the newly created digraph.
    /// The new map's key type is the to digraph's arc type,
    /// and the copied map's key type is the from digraph's arc
    /// type.
    template <typename ToMap, typename FromMap>
    DigraphCopy& arcMap(ToMap& tmap, const FromMap& map) {
      _arc_maps.push_back(new _core_bits::MapCopy<From, Arc,
                          ArcRefMap, ToMap, FromMap>(tmap, map));
      return *this;
    }

    /// \brief Make a copy of the given arc.
    ///
    /// Make a copy of the given arc.
    DigraphCopy& arc(TArc& tarc, const Arc& sarc) {
      _arc_maps.push_back(new _core_bits::ItemCopy<From, Arc,
                          ArcRefMap, TArc>(tarc, sarc));
      return *this;
    }

    /// \brief Executes the copies.
    ///
    /// Executes the copies.
    void run() {
      NodeRefMap nodeRefMap(_from);
      ArcRefMap arcRefMap(_from);
      _core_bits::DigraphCopySelector<To>::
        copy(_to, _from, nodeRefMap, arcRefMap);
      for (int i = 0; i < int(_node_maps.size()); ++i) {
        _node_maps[i]->copy(_from, nodeRefMap);
      }
      for (int i = 0; i < int(_arc_maps.size()); ++i) {
        _arc_maps[i]->copy(_from, arcRefMap);
      }
    }

  protected:


    const From& _from;
    To& _to;

    std::vector<_core_bits::MapCopyBase<From, Node, NodeRefMap>* >
    _node_maps;

    std::vector<_core_bits::MapCopyBase<From, Arc, ArcRefMap>* >
    _arc_maps;

  };

  /// \brief Copy a digraph to another digraph.
  ///
  /// Copy a digraph to another digraph. The complete usage of the
  /// function is detailed in the DigraphCopy class, but a short
  /// example shows a basic work:
  ///\code
  /// copyDigraph(trg, src).nodeRef(nr).arcCrossRef(ecr).run();
  ///\endcode
  ///
  /// After the copy the \c nr map will contain the mapping from the
  /// nodes of the \c from digraph to the nodes of the \c to digraph and
  /// \c ecr will contain the mapping from the arcs of the \c to digraph
  /// to the arcs of the \c from digraph.
  ///
  /// \see DigraphCopy
  template <typename To, typename From>
  DigraphCopy<To, From> copyDigraph(To& to, const From& from) {
    return DigraphCopy<To, From>(to, from);
  }

  /// \brief Class to copy a graph.
  ///
  /// Class to copy a graph to another graph (duplicate a graph). The
  /// simplest way of using it is through the \c copyGraph() function.
  ///
  /// This class not just make a copy of a graph, but it can create
  /// references and cross references between the nodes, edges and arcs of
  /// the two graphs, it can copy maps for use with the newly created
  /// graph and copy nodes, edges and arcs.
  ///
  /// To make a copy from a graph, first an instance of GraphCopy
  /// should be created, then the data belongs to the graph should
  /// assigned to copy. In the end, the \c run() member should be
  /// called.
  ///
  /// The next code copies a graph with several data:
  ///\code
  ///  GraphCopy<NewGraph, OrigGraph> dc(new_graph, orig_graph);
  ///  // create a reference for the nodes
  ///  OrigGraph::NodeMap<NewGraph::Node> nr(orig_graph);
  ///  dc.nodeRef(nr);
  ///  // create a cross reference (inverse) for the edges
  ///  NewGraph::EdgeMap<OrigGraph::Arc> ecr(new_graph);
  ///  dc.edgeCrossRef(ecr);
  ///  // copy an arc map
  ///  OrigGraph::ArcMap<double> oamap(orig_graph);
  ///  NewGraph::ArcMap<double> namap(new_graph);
  ///  dc.arcMap(namap, oamap);
  ///  // copy a node
  ///  OrigGraph::Node on;
  ///  NewGraph::Node nn;
  ///  dc.node(nn, on);
  ///  // Executions of copy
  ///  dc.run();
  ///\endcode
  template <typename To, typename From>
  class GraphCopy {
  private:

    typedef typename From::Node Node;
    typedef typename From::NodeIt NodeIt;
    typedef typename From::Arc Arc;
    typedef typename From::ArcIt ArcIt;
    typedef typename From::Edge Edge;
    typedef typename From::EdgeIt EdgeIt;

    typedef typename To::Node TNode;
    typedef typename To::Arc TArc;
    typedef typename To::Edge TEdge;

    typedef typename From::template NodeMap<TNode> NodeRefMap;
    typedef typename From::template EdgeMap<TEdge> EdgeRefMap;

    struct ArcRefMap {
      ArcRefMap(const To& to, const From& from,
                const EdgeRefMap& edge_ref, const NodeRefMap& node_ref)
        : _to(to), _from(from),
          _edge_ref(edge_ref), _node_ref(node_ref) {}

      typedef typename From::Arc Key;
      typedef typename To::Arc Value;

      Value operator[](const Key& key) const {
        bool forward = _from.u(key) != _from.v(key) ?
          _node_ref[_from.source(key)] ==
          _to.source(_to.direct(_edge_ref[key], true)) :
          _from.direction(key);
        return _to.direct(_edge_ref[key], forward);
      }

      const To& _to;
      const From& _from;
      const EdgeRefMap& _edge_ref;
      const NodeRefMap& _node_ref;
    };


  public:


    /// \brief Constructor for the GraphCopy.
    ///
    /// It copies the content of the \c _from graph into the
    /// \c _to graph.
    GraphCopy(To& to, const From& from)
      : _from(from), _to(to) {}

    /// \brief Destructor of the GraphCopy
    ///
    /// Destructor of the GraphCopy
    ~GraphCopy() {
      for (int i = 0; i < int(_node_maps.size()); ++i) {
        delete _node_maps[i];
      }
      for (int i = 0; i < int(_arc_maps.size()); ++i) {
        delete _arc_maps[i];
      }
      for (int i = 0; i < int(_edge_maps.size()); ++i) {
        delete _edge_maps[i];
      }

    }

    /// \brief Copies the node references into the given map.
    ///
    /// Copies the node references into the given map.
    template <typename NodeRef>
    GraphCopy& nodeRef(NodeRef& map) {
      _node_maps.push_back(new _core_bits::RefCopy<From, Node,
                           NodeRefMap, NodeRef>(map));
      return *this;
    }

    /// \brief Copies the node cross references into the given map.
    ///
    ///  Copies the node cross references (reverse references) into
    ///  the given map.
    template <typename NodeCrossRef>
    GraphCopy& nodeCrossRef(NodeCrossRef& map) {
      _node_maps.push_back(new _core_bits::CrossRefCopy<From, Node,
                           NodeRefMap, NodeCrossRef>(map));
      return *this;
    }

    /// \brief Make copy of the given map.
    ///
    /// Makes copy of the given map for the newly created graph.
    /// The new map's key type is the to graph's node type,
    /// and the copied map's key type is the from graph's node
    /// type.
    template <typename ToMap, typename FromMap>
    GraphCopy& nodeMap(ToMap& tmap, const FromMap& map) {
      _node_maps.push_back(new _core_bits::MapCopy<From, Node,
                           NodeRefMap, ToMap, FromMap>(tmap, map));
      return *this;
    }

    /// \brief Make a copy of the given node.
    ///
    /// Make a copy of the given node.
    GraphCopy& node(TNode& tnode, const Node& snode) {
      _node_maps.push_back(new _core_bits::ItemCopy<From, Node,
                           NodeRefMap, TNode>(tnode, snode));
      return *this;
    }

    /// \brief Copies the arc references into the given map.
    ///
    /// Copies the arc references into the given map.
    template <typename ArcRef>
    GraphCopy& arcRef(ArcRef& map) {
      _arc_maps.push_back(new _core_bits::RefCopy<From, Arc,
                          ArcRefMap, ArcRef>(map));
      return *this;
    }

    /// \brief Copies the arc cross references into the given map.
    ///
    ///  Copies the arc cross references (reverse references) into
    ///  the given map.
    template <typename ArcCrossRef>
    GraphCopy& arcCrossRef(ArcCrossRef& map) {
      _arc_maps.push_back(new _core_bits::CrossRefCopy<From, Arc,
                          ArcRefMap, ArcCrossRef>(map));
      return *this;
    }

    /// \brief Make copy of the given map.
    ///
    /// Makes copy of the given map for the newly created graph.
    /// The new map's key type is the to graph's arc type,
    /// and the copied map's key type is the from graph's arc
    /// type.
    template <typename ToMap, typename FromMap>
    GraphCopy& arcMap(ToMap& tmap, const FromMap& map) {
      _arc_maps.push_back(new _core_bits::MapCopy<From, Arc,
                          ArcRefMap, ToMap, FromMap>(tmap, map));
      return *this;
    }

    /// \brief Make a copy of the given arc.
    ///
    /// Make a copy of the given arc.
    GraphCopy& arc(TArc& tarc, const Arc& sarc) {
      _arc_maps.push_back(new _core_bits::ItemCopy<From, Arc,
                          ArcRefMap, TArc>(tarc, sarc));
      return *this;
    }

    /// \brief Copies the edge references into the given map.
    ///
    /// Copies the edge references into the given map.
    template <typename EdgeRef>
    GraphCopy& edgeRef(EdgeRef& map) {
      _edge_maps.push_back(new _core_bits::RefCopy<From, Edge,
                           EdgeRefMap, EdgeRef>(map));
      return *this;
    }

    /// \brief Copies the edge cross references into the given map.
    ///
    /// Copies the edge cross references (reverse
    /// references) into the given map.
    template <typename EdgeCrossRef>
    GraphCopy& edgeCrossRef(EdgeCrossRef& map) {
      _edge_maps.push_back(new _core_bits::CrossRefCopy<From,
                           Edge, EdgeRefMap, EdgeCrossRef>(map));
      return *this;
    }

    /// \brief Make copy of the given map.
    ///
    /// Makes copy of the given map for the newly created graph.
    /// The new map's key type is the to graph's edge type,
    /// and the copied map's key type is the from graph's edge
    /// type.
    template <typename ToMap, typename FromMap>
    GraphCopy& edgeMap(ToMap& tmap, const FromMap& map) {
      _edge_maps.push_back(new _core_bits::MapCopy<From, Edge,
                           EdgeRefMap, ToMap, FromMap>(tmap, map));
      return *this;
    }

    /// \brief Make a copy of the given edge.
    ///
    /// Make a copy of the given edge.
    GraphCopy& edge(TEdge& tedge, const Edge& sedge) {
      _edge_maps.push_back(new _core_bits::ItemCopy<From, Edge,
                           EdgeRefMap, TEdge>(tedge, sedge));
      return *this;
    }

    /// \brief Executes the copies.
    ///
    /// Executes the copies.
    void run() {
      NodeRefMap nodeRefMap(_from);
      EdgeRefMap edgeRefMap(_from);
      ArcRefMap arcRefMap(_to, _from, edgeRefMap, nodeRefMap);
      _core_bits::GraphCopySelector<To>::
        copy(_to, _from, nodeRefMap, edgeRefMap);
      for (int i = 0; i < int(_node_maps.size()); ++i) {
        _node_maps[i]->copy(_from, nodeRefMap);
      }
      for (int i = 0; i < int(_edge_maps.size()); ++i) {
        _edge_maps[i]->copy(_from, edgeRefMap);
      }
      for (int i = 0; i < int(_arc_maps.size()); ++i) {
        _arc_maps[i]->copy(_from, arcRefMap);
      }
    }

  private:

    const From& _from;
    To& _to;

    std::vector<_core_bits::MapCopyBase<From, Node, NodeRefMap>* >
    _node_maps;

    std::vector<_core_bits::MapCopyBase<From, Arc, ArcRefMap>* >
    _arc_maps;

    std::vector<_core_bits::MapCopyBase<From, Edge, EdgeRefMap>* >
    _edge_maps;

  };

  /// \brief Copy a graph to another graph.
  ///
  /// Copy a graph to another graph. The complete usage of the
  /// function is detailed in the GraphCopy class, but a short
  /// example shows a basic work:
  ///\code
  /// copyGraph(trg, src).nodeRef(nr).arcCrossRef(ecr).run();
  ///\endcode
  ///
  /// After the copy the \c nr map will contain the mapping from the
  /// nodes of the \c from graph to the nodes of the \c to graph and
  /// \c ecr will contain the mapping from the arcs of the \c to graph
  /// to the arcs of the \c from graph.
  ///
  /// \see GraphCopy
  template <typename To, typename From>
  GraphCopy<To, From>
  copyGraph(To& to, const From& from) {
    return GraphCopy<To, From>(to, from);
  }

  namespace _core_bits {

    template <typename Graph, typename Enable = void>
    struct FindArcSelector {
      typedef typename Graph::Node Node;
      typedef typename Graph::Arc Arc;
      static Arc find(const Graph &g, Node u, Node v, Arc e) {
        if (e == INVALID) {
          g.firstOut(e, u);
        } else {
          g.nextOut(e);
        }
        while (e != INVALID && g.target(e) != v) {
          g.nextOut(e);
        }
        return e;
      }
    };

    template <typename Graph>
    struct FindArcSelector<
      Graph,
      typename enable_if<typename Graph::FindEdgeTag, void>::type>
    {
      typedef typename Graph::Node Node;
      typedef typename Graph::Arc Arc;
      static Arc find(const Graph &g, Node u, Node v, Arc prev) {
        return g.findArc(u, v, prev);
      }
    };
  }

  /// \brief Finds an arc between two nodes of a graph.
  ///
  /// Finds an arc from node \c u to node \c v in graph \c g.
  ///
  /// If \c prev is \ref INVALID (this is the default value), then
  /// it finds the first arc from \c u to \c v. Otherwise it looks for
  /// the next arc from \c u to \c v after \c prev.
  /// \return The found arc or \ref INVALID if there is no such an arc.
  ///
  /// Thus you can iterate through each arc from \c u to \c v as it follows.
  ///\code
  /// for(Arc e=findArc(g,u,v);e!=INVALID;e=findArc(g,u,v,e)) {
  ///   ...
  /// }
  ///\endcode
  ///
  ///\sa ArcLookUp
  ///\sa AllArcLookUp
  ///\sa DynArcLookUp
  ///\sa ConArcIt
  template <typename Graph>
  inline typename Graph::Arc
  findArc(const Graph &g, typename Graph::Node u, typename Graph::Node v,
          typename Graph::Arc prev = INVALID) {
    return _core_bits::FindArcSelector<Graph>::find(g, u, v, prev);
  }

  /// \brief Iterator for iterating on arcs connected the same nodes.
  ///
  /// Iterator for iterating on arcs connected the same nodes. It is
  /// higher level interface for the findArc() function. You can
  /// use it the following way:
  ///\code
  /// for (ConArcIt<Graph> it(g, src, trg); it != INVALID; ++it) {
  ///   ...
  /// }
  ///\endcode
  ///
  ///\sa findArc()
  ///\sa ArcLookUp
  ///\sa AllArcLookUp
  ///\sa DynArcLookUp
  template <typename _Graph>
  class ConArcIt : public _Graph::Arc {
  public:

    typedef _Graph Graph;
    typedef typename Graph::Arc Parent;

    typedef typename Graph::Arc Arc;
    typedef typename Graph::Node Node;

    /// \brief Constructor.
    ///
    /// Construct a new ConArcIt iterating on the arcs which
    /// connects the \c u and \c v node.
    ConArcIt(const Graph& g, Node u, Node v) : _graph(g) {
      Parent::operator=(findArc(_graph, u, v));
    }

    /// \brief Constructor.
    ///
    /// Construct a new ConArcIt which continues the iterating from
    /// the \c e arc.
    ConArcIt(const Graph& g, Arc a) : Parent(a), _graph(g) {}

    /// \brief Increment operator.
    ///
    /// It increments the iterator and gives back the next arc.
    ConArcIt& operator++() {
      Parent::operator=(findArc(_graph, _graph.source(*this),
                                _graph.target(*this), *this));
      return *this;
    }
  private:
    const Graph& _graph;
  };

  namespace _core_bits {

    template <typename Graph, typename Enable = void>
    struct FindEdgeSelector {
      typedef typename Graph::Node Node;
      typedef typename Graph::Edge Edge;
      static Edge find(const Graph &g, Node u, Node v, Edge e) {
        bool b;
        if (u != v) {
          if (e == INVALID) {
            g.firstInc(e, b, u);
          } else {
            b = g.u(e) == u;
            g.nextInc(e, b);
          }
          while (e != INVALID && (b ? g.v(e) : g.u(e)) != v) {
            g.nextInc(e, b);
          }
        } else {
          if (e == INVALID) {
            g.firstInc(e, b, u);
          } else {
            b = true;
            g.nextInc(e, b);
          }
          while (e != INVALID && (!b || g.v(e) != v)) {
            g.nextInc(e, b);
          }
        }
        return e;
      }
    };

    template <typename Graph>
    struct FindEdgeSelector<
      Graph,
      typename enable_if<typename Graph::FindEdgeTag, void>::type>
    {
      typedef typename Graph::Node Node;
      typedef typename Graph::Edge Edge;
      static Edge find(const Graph &g, Node u, Node v, Edge prev) {
        return g.findEdge(u, v, prev);
      }
    };
  }

  /// \brief Finds an edge between two nodes of a graph.
  ///
  /// Finds an edge from node \c u to node \c v in graph \c g.
  /// If the node \c u and node \c v is equal then each loop edge
  /// will be enumerated once.
  ///
  /// If \c prev is \ref INVALID (this is the default value), then
  /// it finds the first arc from \c u to \c v. Otherwise it looks for
  /// the next arc from \c u to \c v after \c prev.
  /// \return The found arc or \ref INVALID if there is no such an arc.
  ///
  /// Thus you can iterate through each arc from \c u to \c v as it follows.
  ///\code
  /// for(Edge e = findEdge(g,u,v); e != INVALID;
  ///     e = findEdge(g,u,v,e)) {
  ///   ...
  /// }
  ///\endcode
  ///
  ///\sa ConEdgeIt

  template <typename Graph>
  inline typename Graph::Edge
  findEdge(const Graph &g, typename Graph::Node u, typename Graph::Node v,
            typename Graph::Edge p = INVALID) {
    return _core_bits::FindEdgeSelector<Graph>::find(g, u, v, p);
  }

  /// \brief Iterator for iterating on edges connected the same nodes.
  ///
  /// Iterator for iterating on edges connected the same nodes. It is
  /// higher level interface for the findEdge() function. You can
  /// use it the following way:
  ///\code
  /// for (ConEdgeIt<Graph> it(g, src, trg); it != INVALID; ++it) {
  ///   ...
  /// }
  ///\endcode
  ///
  ///\sa findEdge()
  template <typename _Graph>
  class ConEdgeIt : public _Graph::Edge {
  public:

    typedef _Graph Graph;
    typedef typename Graph::Edge Parent;

    typedef typename Graph::Edge Edge;
    typedef typename Graph::Node Node;

    /// \brief Constructor.
    ///
    /// Construct a new ConEdgeIt iterating on the edges which
    /// connects the \c u and \c v node.
    ConEdgeIt(const Graph& g, Node u, Node v) : _graph(g) {
      Parent::operator=(findEdge(_graph, u, v));
    }

    /// \brief Constructor.
    ///
    /// Construct a new ConEdgeIt which continues the iterating from
    /// the \c e edge.
    ConEdgeIt(const Graph& g, Edge e) : Parent(e), _graph(g) {}

    /// \brief Increment operator.
    ///
    /// It increments the iterator and gives back the next edge.
    ConEdgeIt& operator++() {
      Parent::operator=(findEdge(_graph, _graph.u(*this),
                                 _graph.v(*this), *this));
      return *this;
    }
  private:
    const Graph& _graph;
  };


  ///Dynamic arc look up between given endpoints.

  ///Using this class, you can find an arc in a digraph from a given
  ///source to a given target in amortized time <em>O(log d)</em>,
  ///where <em>d</em> is the out-degree of the source node.
  ///
  ///It is possible to find \e all parallel arcs between two nodes with
  ///the \c findFirst() and \c findNext() members.
  ///
  ///See the \ref ArcLookUp and \ref AllArcLookUp classes if your
  ///digraph is not changed so frequently.
  ///
  ///This class uses a self-adjusting binary search tree, Sleator's
  ///and Tarjan's Splay tree for guarantee the logarithmic amortized
  ///time bound for arc lookups. This class also guarantees the
  ///optimal time bound in a constant factor for any distribution of
  ///queries.
  ///
  ///\tparam G The type of the underlying digraph.
  ///
  ///\sa ArcLookUp
  ///\sa AllArcLookUp
  template<class G>
  class DynArcLookUp
    : protected ItemSetTraits<G, typename G::Arc>::ItemNotifier::ObserverBase
  {
  public:
    typedef typename ItemSetTraits<G, typename G::Arc>
    ::ItemNotifier::ObserverBase Parent;

    TEMPLATE_DIGRAPH_TYPEDEFS(G);
    typedef G Digraph;

  protected:

    class AutoNodeMap : public ItemSetTraits<G, Node>::template Map<Arc>::Type {
    public:

      typedef typename ItemSetTraits<G, Node>::template Map<Arc>::Type Parent;

      AutoNodeMap(const G& digraph) : Parent(digraph, INVALID) {}

      virtual void add(const Node& node) {
        Parent::add(node);
        Parent::set(node, INVALID);
      }

      virtual void add(const std::vector<Node>& nodes) {
        Parent::add(nodes);
        for (int i = 0; i < int(nodes.size()); ++i) {
          Parent::set(nodes[i], INVALID);
        }
      }

      virtual void build() {
        Parent::build();
        Node it;
        typename Parent::Notifier* nf = Parent::notifier();
        for (nf->first(it); it != INVALID; nf->next(it)) {
          Parent::set(it, INVALID);
        }
      }
    };

    const Digraph &_g;
    AutoNodeMap _head;
    typename Digraph::template ArcMap<Arc> _parent;
    typename Digraph::template ArcMap<Arc> _left;
    typename Digraph::template ArcMap<Arc> _right;

    class ArcLess {
      const Digraph &g;
    public:
      ArcLess(const Digraph &_g) : g(_g) {}
      bool operator()(Arc a,Arc b) const
      {
        return g.target(a)<g.target(b);
      }
    };

  public:

    ///Constructor

    ///Constructor.
    ///
    ///It builds up the search database.
    DynArcLookUp(const Digraph &g)
      : _g(g),_head(g),_parent(g),_left(g),_right(g)
    {
      Parent::attach(_g.notifier(typename Digraph::Arc()));
      refresh();
    }

  protected:

    virtual void add(const Arc& arc) {
      insert(arc);
    }

    virtual void add(const std::vector<Arc>& arcs) {
      for (int i = 0; i < int(arcs.size()); ++i) {
        insert(arcs[i]);
      }
    }

    virtual void erase(const Arc& arc) {
      remove(arc);
    }

    virtual void erase(const std::vector<Arc>& arcs) {
      for (int i = 0; i < int(arcs.size()); ++i) {
        remove(arcs[i]);
      }
    }

    virtual void build() {
      refresh();
    }

    virtual void clear() {
      for(NodeIt n(_g);n!=INVALID;++n) {
        _head.set(n, INVALID);
      }
    }

    void insert(Arc arc) {
      Node s = _g.source(arc);
      Node t = _g.target(arc);
      _left.set(arc, INVALID);
      _right.set(arc, INVALID);

      Arc e = _head[s];
      if (e == INVALID) {
        _head.set(s, arc);
        _parent.set(arc, INVALID);
        return;
      }
      while (true) {
        if (t < _g.target(e)) {
          if (_left[e] == INVALID) {
            _left.set(e, arc);
            _parent.set(arc, e);
            splay(arc);
            return;
          } else {
            e = _left[e];
          }
        } else {
          if (_right[e] == INVALID) {
            _right.set(e, arc);
            _parent.set(arc, e);
            splay(arc);
            return;
          } else {
            e = _right[e];
          }
        }
      }
    }

    void remove(Arc arc) {
      if (_left[arc] == INVALID) {
        if (_right[arc] != INVALID) {
          _parent.set(_right[arc], _parent[arc]);
        }
        if (_parent[arc] != INVALID) {
          if (_left[_parent[arc]] == arc) {
            _left.set(_parent[arc], _right[arc]);
          } else {
            _right.set(_parent[arc], _right[arc]);
          }
        } else {
          _head.set(_g.source(arc), _right[arc]);
        }
      } else if (_right[arc] == INVALID) {
        _parent.set(_left[arc], _parent[arc]);
        if (_parent[arc] != INVALID) {
          if (_left[_parent[arc]] == arc) {
            _left.set(_parent[arc], _left[arc]);
          } else {
            _right.set(_parent[arc], _left[arc]);
          }
        } else {
          _head.set(_g.source(arc), _left[arc]);
        }
      } else {
        Arc e = _left[arc];
        if (_right[e] != INVALID) {
          e = _right[e];
          while (_right[e] != INVALID) {
            e = _right[e];
          }
          Arc s = _parent[e];
          _right.set(_parent[e], _left[e]);
          if (_left[e] != INVALID) {
            _parent.set(_left[e], _parent[e]);
          }

          _left.set(e, _left[arc]);
          _parent.set(_left[arc], e);
          _right.set(e, _right[arc]);
          _parent.set(_right[arc], e);

          _parent.set(e, _parent[arc]);
          if (_parent[arc] != INVALID) {
            if (_left[_parent[arc]] == arc) {
              _left.set(_parent[arc], e);
            } else {
              _right.set(_parent[arc], e);
            }
          }
          splay(s);
        } else {
          _right.set(e, _right[arc]);
          _parent.set(_right[arc], e);

          if (_parent[arc] != INVALID) {
            if (_left[_parent[arc]] == arc) {
              _left.set(_parent[arc], e);
            } else {
              _right.set(_parent[arc], e);
            }
          } else {
            _head.set(_g.source(arc), e);
          }
        }
      }
    }

    Arc refreshRec(std::vector<Arc> &v,int a,int b)
    {
      int m=(a+b)/2;
      Arc me=v[m];
      if (a < m) {
        Arc left = refreshRec(v,a,m-1);
        _left.set(me, left);
        _parent.set(left, me);
      } else {
        _left.set(me, INVALID);
      }
      if (m < b) {
        Arc right = refreshRec(v,m+1,b);
        _right.set(me, right);
        _parent.set(right, me);
      } else {
        _right.set(me, INVALID);
      }
      return me;
    }

    void refresh() {
      for(NodeIt n(_g);n!=INVALID;++n) {
        std::vector<Arc> v;
        for(OutArcIt e(_g,n);e!=INVALID;++e) v.push_back(e);
        if(v.size()) {
          std::sort(v.begin(),v.end(),ArcLess(_g));
          Arc head = refreshRec(v,0,v.size()-1);
          _head.set(n, head);
          _parent.set(head, INVALID);
        }
        else _head.set(n, INVALID);
      }
    }

    void zig(Arc v) {
      Arc w = _parent[v];
      _parent.set(v, _parent[w]);
      _parent.set(w, v);
      _left.set(w, _right[v]);
      _right.set(v, w);
      if (_parent[v] != INVALID) {
        if (_right[_parent[v]] == w) {
          _right.set(_parent[v], v);
        } else {
          _left.set(_parent[v], v);
        }
      }
      if (_left[w] != INVALID){
        _parent.set(_left[w], w);
      }
    }

    void zag(Arc v) {
      Arc w = _parent[v];
      _parent.set(v, _parent[w]);
      _parent.set(w, v);
      _right.set(w, _left[v]);
      _left.set(v, w);
      if (_parent[v] != INVALID){
        if (_left[_parent[v]] == w) {
          _left.set(_parent[v], v);
        } else {
          _right.set(_parent[v], v);
        }
      }
      if (_right[w] != INVALID){
        _parent.set(_right[w], w);
      }
    }

    void splay(Arc v) {
      while (_parent[v] != INVALID) {
        if (v == _left[_parent[v]]) {
          if (_parent[_parent[v]] == INVALID) {
            zig(v);
          } else {
            if (_parent[v] == _left[_parent[_parent[v]]]) {
              zig(_parent[v]);
              zig(v);
            } else {
              zig(v);
              zag(v);
            }
          }
        } else {
          if (_parent[_parent[v]] == INVALID) {
            zag(v);
          } else {
            if (_parent[v] == _left[_parent[_parent[v]]]) {
              zag(v);
              zig(v);
            } else {
              zag(_parent[v]);
              zag(v);
            }
          }
        }
      }
      _head[_g.source(v)] = v;
    }


  public:

    ///Find an arc between two nodes.

    ///Find an arc between two nodes in time <em>O(</em>log<em>d)</em>, where
    /// <em>d</em> is the number of outgoing arcs of \c s.
    ///\param s The source node
    ///\param t The target node
    ///\return An arc from \c s to \c t if there exists,
    ///\ref INVALID otherwise.
    Arc operator()(Node s, Node t) const
    {
      Arc a = _head[s];
      while (true) {
        if (_g.target(a) == t) {
          const_cast<DynArcLookUp&>(*this).splay(a);
          return a;
        } else if (t < _g.target(a)) {
          if (_left[a] == INVALID) {
            const_cast<DynArcLookUp&>(*this).splay(a);
            return INVALID;
          } else {
            a = _left[a];
          }
        } else  {
          if (_right[a] == INVALID) {
            const_cast<DynArcLookUp&>(*this).splay(a);
            return INVALID;
          } else {
            a = _right[a];
          }
        }
      }
    }

    ///Find the first arc between two nodes.

    ///Find the first arc between two nodes in time
    /// <em>O(</em>log<em>d)</em>, where <em>d</em> is the number of
    /// outgoing arcs of \c s.
    ///\param s The source node
    ///\param t The target node
    ///\return An arc from \c s to \c t if there exists, \ref INVALID
    /// otherwise.
    Arc findFirst(Node s, Node t) const
    {
      Arc a = _head[s];
      Arc r = INVALID;
      while (true) {
        if (_g.target(a) < t) {
          if (_right[a] == INVALID) {
            const_cast<DynArcLookUp&>(*this).splay(a);
            return r;
          } else {
            a = _right[a];
          }
        } else {
          if (_g.target(a) == t) {
            r = a;
          }
          if (_left[a] == INVALID) {
            const_cast<DynArcLookUp&>(*this).splay(a);
            return r;
          } else {
            a = _left[a];
          }
        }
      }
    }

    ///Find the next arc between two nodes.

    ///Find the next arc between two nodes in time
    /// <em>O(</em>log<em>d)</em>, where <em>d</em> is the number of
    /// outgoing arcs of \c s.
    ///\param s The source node
    ///\param t The target node
    ///\return An arc from \c s to \c t if there exists, \ref INVALID
    /// otherwise.

    ///\note If \c e is not the result of the previous \c findFirst()
    ///operation then the amorized time bound can not be guaranteed.
#ifdef DOXYGEN
    Arc findNext(Node s, Node t, Arc a) const
#else
    Arc findNext(Node, Node t, Arc a) const
#endif
    {
      if (_right[a] != INVALID) {
        a = _right[a];
        while (_left[a] != INVALID) {
          a = _left[a];
        }
        const_cast<DynArcLookUp&>(*this).splay(a);
      } else {
        while (_parent[a] != INVALID && _right[_parent[a]] ==  a) {
          a = _parent[a];
        }
        if (_parent[a] == INVALID) {
          return INVALID;
        } else {
          a = _parent[a];
          const_cast<DynArcLookUp&>(*this).splay(a);
        }
      }
      if (_g.target(a) == t) return a;
      else return INVALID;
    }

  };

  ///Fast arc look up between given endpoints.

  ///Using this class, you can find an arc in a digraph from a given
  ///source to a given target in time <em>O(log d)</em>,
  ///where <em>d</em> is the out-degree of the source node.
  ///
  ///It is not possible to find \e all parallel arcs between two nodes.
  ///Use \ref AllArcLookUp for this purpose.
  ///
  ///\warning This class is static, so you should refresh() (or at least
  ///refresh(Node)) this data structure
  ///whenever the digraph changes. This is a time consuming (superlinearly
  ///proportional (<em>O(m</em>log<em>m)</em>) to the number of arcs).
  ///
  ///\tparam G The type of the underlying digraph.
  ///
  ///\sa DynArcLookUp
  ///\sa AllArcLookUp
  template<class G>
  class ArcLookUp
  {
  public:
    TEMPLATE_DIGRAPH_TYPEDEFS(G);
    typedef G Digraph;

  protected:
    const Digraph &_g;
    typename Digraph::template NodeMap<Arc> _head;
    typename Digraph::template ArcMap<Arc> _left;
    typename Digraph::template ArcMap<Arc> _right;

    class ArcLess {
      const Digraph &g;
    public:
      ArcLess(const Digraph &_g) : g(_g) {}
      bool operator()(Arc a,Arc b) const
      {
        return g.target(a)<g.target(b);
      }
    };

  public:

    ///Constructor

    ///Constructor.
    ///
    ///It builds up the search database, which remains valid until the digraph
    ///changes.
    ArcLookUp(const Digraph &g) :_g(g),_head(g),_left(g),_right(g) {refresh();}

  private:
    Arc refreshRec(std::vector<Arc> &v,int a,int b)
    {
      int m=(a+b)/2;
      Arc me=v[m];
      _left[me] = a<m?refreshRec(v,a,m-1):INVALID;
      _right[me] = m<b?refreshRec(v,m+1,b):INVALID;
      return me;
    }
  public:
    ///Refresh the data structure at a node.

    ///Build up the search database of node \c n.
    ///
    ///It runs in time <em>O(d</em>log<em>d)</em>, where <em>d</em> is
    ///the number of the outgoing arcs of \c n.
    void refresh(Node n)
    {
      std::vector<Arc> v;
      for(OutArcIt e(_g,n);e!=INVALID;++e) v.push_back(e);
      if(v.size()) {
        std::sort(v.begin(),v.end(),ArcLess(_g));
        _head[n]=refreshRec(v,0,v.size()-1);
      }
      else _head[n]=INVALID;
    }
    ///Refresh the full data structure.

    ///Build up the full search database. In fact, it simply calls
    ///\ref refresh(Node) "refresh(n)" for each node \c n.
    ///
    ///It runs in time <em>O(m</em>log<em>D)</em>, where <em>m</em> is
    ///the number of the arcs of \c n and <em>D</em> is the maximum
    ///out-degree of the digraph.

    void refresh()
    {
      for(NodeIt n(_g);n!=INVALID;++n) refresh(n);
    }

    ///Find an arc between two nodes.

    ///Find an arc between two nodes in time <em>O(</em>log<em>d)</em>, where
    /// <em>d</em> is the number of outgoing arcs of \c s.
    ///\param s The source node
    ///\param t The target node
    ///\return An arc from \c s to \c t if there exists,
    ///\ref INVALID otherwise.
    ///
    ///\warning If you change the digraph, refresh() must be called before using
    ///this operator. If you change the outgoing arcs of
    ///a single node \c n, then
    ///\ref refresh(Node) "refresh(n)" is enough.
    ///
    Arc operator()(Node s, Node t) const
    {
      Arc e;
      for(e=_head[s];
          e!=INVALID&&_g.target(e)!=t;
          e = t < _g.target(e)?_left[e]:_right[e]) ;
      return e;
    }

  };

  ///Fast look up of all arcs between given endpoints.

  ///This class is the same as \ref ArcLookUp, with the addition
  ///that it makes it possible to find all arcs between given endpoints.
  ///
  ///\warning This class is static, so you should refresh() (or at least
  ///refresh(Node)) this data structure
  ///whenever the digraph changes. This is a time consuming (superlinearly
  ///proportional (<em>O(m</em>log<em>m)</em>) to the number of arcs).
  ///
  ///\tparam G The type of the underlying digraph.
  ///
  ///\sa DynArcLookUp
  ///\sa ArcLookUp
  template<class G>
  class AllArcLookUp : public ArcLookUp<G>
  {
    using ArcLookUp<G>::_g;
    using ArcLookUp<G>::_right;
    using ArcLookUp<G>::_left;
    using ArcLookUp<G>::_head;

    TEMPLATE_DIGRAPH_TYPEDEFS(G);
    typedef G Digraph;

    typename Digraph::template ArcMap<Arc> _next;

    Arc refreshNext(Arc head,Arc next=INVALID)
    {
      if(head==INVALID) return next;
      else {
        next=refreshNext(_right[head],next);
//         _next[head]=next;
        _next[head]=( next!=INVALID && _g.target(next)==_g.target(head))
          ? next : INVALID;
        return refreshNext(_left[head],head);
      }
    }

    void refreshNext()
    {
      for(NodeIt n(_g);n!=INVALID;++n) refreshNext(_head[n]);
    }

  public:
    ///Constructor

    ///Constructor.
    ///
    ///It builds up the search database, which remains valid until the digraph
    ///changes.
    AllArcLookUp(const Digraph &g) : ArcLookUp<G>(g), _next(g) {refreshNext();}

    ///Refresh the data structure at a node.

    ///Build up the search database of node \c n.
    ///
    ///It runs in time <em>O(d</em>log<em>d)</em>, where <em>d</em> is
    ///the number of the outgoing arcs of \c n.

    void refresh(Node n)
    {
      ArcLookUp<G>::refresh(n);
      refreshNext(_head[n]);
    }

    ///Refresh the full data structure.

    ///Build up the full search database. In fact, it simply calls
    ///\ref refresh(Node) "refresh(n)" for each node \c n.
    ///
    ///It runs in time <em>O(m</em>log<em>D)</em>, where <em>m</em> is
    ///the number of the arcs of \c n and <em>D</em> is the maximum
    ///out-degree of the digraph.

    void refresh()
    {
      for(NodeIt n(_g);n!=INVALID;++n) refresh(_head[n]);
    }

    ///Find an arc between two nodes.

    ///Find an arc between two nodes.
    ///\param s The source node
    ///\param t The target node
    ///\param prev The previous arc between \c s and \c t. It it is INVALID or
    ///not given, the operator finds the first appropriate arc.
    ///\return An arc from \c s to \c t after \c prev or
    ///\ref INVALID if there is no more.
    ///
    ///For example, you can count the number of arcs from \c u to \c v in the
    ///following way.
    ///\code
    ///AllArcLookUp<ListDigraph> ae(g);
    ///...
    ///int n=0;
    ///for(Arc e=ae(u,v);e!=INVALID;e=ae(u,v,e)) n++;
    ///\endcode
    ///
    ///Finding the first arc take <em>O(</em>log<em>d)</em> time, where
    /// <em>d</em> is the number of outgoing arcs of \c s. Then, the
    ///consecutive arcs are found in constant time.
    ///
    ///\warning If you change the digraph, refresh() must be called before using
    ///this operator. If you change the outgoing arcs of
    ///a single node \c n, then
    ///\ref refresh(Node) "refresh(n)" is enough.
    ///
#ifdef DOXYGEN
    Arc operator()(Node s, Node t, Arc prev=INVALID) const {}
#else
    using ArcLookUp<G>::operator() ;
    Arc operator()(Node s, Node t, Arc prev) const
    {
      return prev==INVALID?(*this)(s,t):_next[prev];
    }
#endif

  };

  /// @}

} //namespace lemon

#endif