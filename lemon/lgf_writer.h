/* -*- C++ -*-
 *
 * This file is a part of LEMON, a generic C++ optimization library
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

///\ingroup lemon_io
///\file
///\brief Lemon Graph Format writer.


#ifndef LEMON_LGF_WRITER_H
#define LEMON_LGF_WRITER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>

#include <vector>
#include <functional>

#include <lemon/assert.h>
#include <lemon/graph_utils.h>

namespace lemon {

  namespace _writer_bits {

    template <typename Value>
    struct DefaultConverter {
      std::string operator()(const Value& value) {
	std::ostringstream os;
	os << value;
	return os.str();
      }
    };

    template <typename T>
    bool operator<(const T&, const T&) {
      throw DataFormatError("Label map is not comparable");
    }

    template <typename _Map>
    class MapLess {
    public:
      typedef _Map Map;
      typedef typename Map::Key Item;

    private:
      const Map& _map;
      
    public:
      MapLess(const Map& map) : _map(map) {}

      bool operator()(const Item& left, const Item& right) {
	return _map[left] < _map[right];
      }
    };

    template <typename _Item>    
    class MapStorageBase {
    public:
      typedef _Item Item;

    public:
      MapStorageBase() {}
      virtual ~MapStorageBase() {}

      virtual std::string get(const Item& item) = 0;
      virtual void sort(std::vector<Item>&) = 0;
    };

    template <typename _Item, typename _Map, 
	      typename _Converter = DefaultConverter<typename _Map::Value> >
    class MapStorage : public MapStorageBase<_Item> {
    public:
      typedef _Map Map;
      typedef _Converter Converter;
      typedef _Item Item;
      
    private:
      const Map& _map;
      Converter _converter;

    public:
      MapStorage(const Map& map, const Converter& converter = Converter()) 
	: _map(map), _converter(converter) {}
      virtual ~MapStorage() {}

      virtual std::string get(const Item& item) {
	return _converter(_map[item]);
      }
      virtual void sort(std::vector<Item>& items) {
	MapLess<Map> less(_map);
	std::sort(items.begin(), items.end(), less);
      }
    };

    class ValueStorageBase {
    public:
      ValueStorageBase() {}
      virtual ~ValueStorageBase() {}

      virtual std::string get() = 0;      
    };

    template <typename _Value, typename _Converter = DefaultConverter<_Value> >
    class ValueStorage : public ValueStorageBase {
    public:
      typedef _Value Value;
      typedef _Converter Converter;

    private:
      const Value& _value;
      Converter _converter;

    public:
      ValueStorage(const Value& value, const Converter& converter = Converter())
 	: _value(value), _converter(converter) {}

      virtual std::string get() {
	return _converter(_value);
      }
    };

    template <typename Value>
    struct MapLookUpConverter {
      const std::map<Value, std::string>& _map;
      
      MapLookUpConverter(const std::map<Value, std::string>& map) 
	: _map(map) {}
      
      std::string operator()(const Value& str) {
	typename std::map<Value, std::string>::const_iterator it = 
	  _map.find(str);
	if (it == _map.end()) {
	  throw DataFormatError("Item not found");
	}
	return it->second;
      }
    };

    bool isWhiteSpace(char c) {
      return c == ' ' || c == '\t' || c == '\v' || 
        c == '\n' || c == '\r' || c == '\f'; 
    }

    bool isEscaped(char c) {
      return c == '\\' || c == '\"' || c == '\'' || 
	c == '\a' || c == '\b';
    }

    static void writeEscape(std::ostream& os, char c) {
      switch (c) {
      case '\\':
	os << "\\\\";
	return;
      case '\"':
	os << "\\\"";
	return;
      case '\a':
	os << "\\a";
	return;
      case '\b':
	os << "\\b";
	return;
      case '\f':
	os << "\\f";
	return;
      case '\r':
	os << "\\r";
	return;
      case '\n':
	os << "\\n";
	return;
      case '\t':
	os << "\\t";
	return;
      case '\v':
	os << "\\v";
	return;
      default:
	if (c < 0x20) {
	  os << '\\' << std::oct << static_cast<int>(c);
	} else {
	  os << c;
	}
	return;
      }     
    }

    bool requireEscape(const std::string& str) {
      std::istringstream is(str);
      char c;
      while (is.get(c)) {
	if (isWhiteSpace(c) || isEscaped(c)) {
	  return true;
	}
      }
      return false;
    }
    
    std::ostream& writeToken(std::ostream& os, const std::string& str) {

      if (requireEscape(str)) {
	os << '\"';
	for (std::string::const_iterator it = str.begin(); 
	     it != str.end(); ++it) {
	  writeEscape(os, *it);
	}	
	os << '\"';
      } else {
	os << str;
      }
      return os;
    }

  }
  
  /// \e
  template <typename _Digraph>
  class DigraphWriter {
  public:

    typedef _Digraph Digraph;
    GRAPH_TYPEDEFS(typename Digraph);
    
  private:


    std::ostream* _os;
    bool local_os;

    Digraph& _digraph;

    std::string _nodes_caption;
    std::string _arcs_caption;
    std::string _attributes_caption;
    
    typedef std::map<Node, std::string> NodeIndex;
    NodeIndex _node_index;
    typedef std::map<Arc, std::string> ArcIndex;
    ArcIndex _arc_index;

    typedef std::vector<std::pair<std::string, 
      _writer_bits::MapStorageBase<Node>* > > NodeMaps;    
    NodeMaps _node_maps; 

    typedef std::vector<std::pair<std::string, 
      _writer_bits::MapStorageBase<Arc>* > >ArcMaps;
    ArcMaps _arc_maps;

    typedef std::vector<std::pair<std::string, 
      _writer_bits::ValueStorageBase*> > Attributes;
    Attributes _attributes;

    bool _skip_nodes;
    bool _skip_arcs;

  public:

    /// \e
    DigraphWriter(std::ostream& is, Digraph& digraph) 
      : _os(&is), local_os(false), _digraph(digraph),
	_skip_nodes(false), _skip_arcs(false) {}

    /// \e
    DigraphWriter(const std::string& fn, Digraph& digraph) 
      : _os(new std::ofstream(fn.c_str())), local_os(true), _digraph(digraph),
	_skip_nodes(false), _skip_arcs(false) {}

    /// \e
    DigraphWriter(const char* fn, Digraph& digraph) 
      : _os(new std::ofstream(fn)), local_os(true), _digraph(digraph),
	_skip_nodes(false), _skip_arcs(false) {}

    DigraphWriter(DigraphWriter& other) 
      : _os(other._os), local_os(other.local_os), _digraph(other._digraph),
	_skip_nodes(other._skip_nodes), _skip_arcs(other._skip_arcs) {

      other.is = 0;
      other.local_os = false;

      _node_index.swap(other._node_index);
      _arc_index.swap(other._arc_index);

      _node_maps.swap(other._node_maps);
      _arc_maps.swap(other._arc_maps);
      _attributes.swap(other._attributes);

      _nodes_caption = other._nodes_caption;
      _arcs_caption = other._arcs_caption;
      _attributes_caption = other._attributes_caption;
    }

    /// \e
    ~DigraphWriter() {
      for (typename NodeMaps::iterator it = _node_maps.begin(); 
	   it != _node_maps.end(); ++it) {
	delete it->second;
      }

      for (typename ArcMaps::iterator it = _arc_maps.begin(); 
	   it != _arc_maps.end(); ++it) {
	delete it->second;
      }

      for (typename Attributes::iterator it = _attributes.begin(); 
	   it != _attributes.end(); ++it) {
	delete it->second;
      }

      if (local_os) {
	delete _os;
      }
    }

  private:
    
    DigraphWriter& operator=(const DigraphWriter&);

  public:

    /// \e
    template <typename Map>
    DigraphWriter& nodeMap(const std::string& caption, const Map& map) {
      checkConcept<concepts::ReadMap<Node, typename Map::Value>, Map>();
      _writer_bits::MapStorageBase<Node>* storage = 
	new _writer_bits::MapStorage<Node, Map>(map);
      _node_maps.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    template <typename Map, typename Converter>
    DigraphWriter& nodeMap(const std::string& caption, const Map& map, 
			   const Converter& converter = Converter()) {
      checkConcept<concepts::ReadMap<Node, typename Map::Value>, Map>();
      _writer_bits::MapStorageBase<Node>* storage = 
	new _writer_bits::MapStorage<Node, Map, Converter>(map, converter);
      _node_maps.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    template <typename Map>
    DigraphWriter& arcMap(const std::string& caption, const Map& map) {
      checkConcept<concepts::ReadMap<Arc, typename Map::Value>, Map>();
      _writer_bits::MapStorageBase<Arc>* storage = 
	new _writer_bits::MapStorage<Arc, Map>(map);
      _arc_maps.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    template <typename Map, typename Converter>
    DigraphWriter& arcMap(const std::string& caption, const Map& map, 
			  const Converter& converter = Converter()) {
      checkConcept<concepts::ReadMap<Arc, typename Map::Value>, Map>();
      _writer_bits::MapStorageBase<Arc>* storage = 
	new _writer_bits::MapStorage<Arc, Map, Converter>(map, converter);
      _arc_maps.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    template <typename Value>
    DigraphWriter& attribute(const std::string& caption, const Value& value) {
      _writer_bits::ValueStorageBase* storage = 
	new _writer_bits::ValueStorage<Value>(value);
      _attributes.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    template <typename Value, typename Converter>
    DigraphWriter& attribute(const std::string& caption, const Value& value, 
			     const Converter& converter = Converter()) {
      _writer_bits::ValueStorageBase* storage = 
	new _writer_bits::ValueStorage<Value, Converter>(value, converter);
      _attributes.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    DigraphWriter& node(const std::string& caption, const Node& node) {
      typedef _writer_bits::MapLookUpConverter<Node> Converter;
      Converter converter(_node_index);
      _writer_bits::ValueStorageBase* storage = 
	new _writer_bits::ValueStorage<Node, Converter>(node, converter);
      _attributes.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    DigraphWriter& arc(const std::string& caption, const Arc& arc) {
      typedef _writer_bits::MapLookUpConverter<Arc> Converter;
      Converter converter(_arc_index);
      _writer_bits::ValueStorageBase* storage = 
	new _writer_bits::ValueStorage<Arc, Converter>(arc, converter);
      _attributes.push_back(std::make_pair(caption, storage));
      return *this;
    }

    /// \e
    DigraphWriter& nodes(const std::string& caption) {
      _nodes_caption = caption;
      return *this;
    }

    /// \e
    DigraphWriter& arcs(const std::string& caption) {
      _arcs_caption = caption;
      return *this;
    }

    /// \e
    DigraphWriter& attributes(const std::string& caption) {
      _attributes_caption = caption;
      return *this;
    }

    DigraphWriter& skipNodes() {
      LEMON_ASSERT(!_skip_nodes, "Multiple usage of skipNodes() member");
      return *this;
    }

    DigraphWriter& skipArcs() {
      LEMON_ASSERT(!_skip_arcs, "Multiple usage of skipArcs() member");
      return *this;
    }

  private:

    void writeNodes() {
      _writer_bits::MapStorageBase<Node>* label = 0;
      for (typename NodeMaps::iterator it = _node_maps.begin();
	   it != _node_maps.end(); ++it) {
        if (it->first == "label") {
	  label = it->second;
	  break;
	}
      }

      *_os << "@nodes";
      if (!_nodes_caption.empty()) {
	*_os << ' ' << _nodes_caption;
      }
      *_os << std::endl;

      if (label == 0) {
	*_os << "label" << '\t';
      }
      for (typename NodeMaps::iterator it = _node_maps.begin();
	   it != _node_maps.end(); ++it) {
	*_os << it->first << '\t';
      }
      *_os << std::endl;

      std::vector<Node> nodes;
      for (NodeIt n(_digraph); n != INVALID; ++n) {
	nodes.push_back(n);
      }
      
      if (label == 0) {
	IdMap<Digraph, Node> id_map(_digraph);
	_writer_bits::MapLess<IdMap<Digraph, Node> > id_less(id_map);
	std::sort(nodes.begin(), nodes.end(), id_less);
      } else {
	label->sort(nodes);
      }

      for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
	Node n = nodes[i];
	if (label == 0) {
	  std::ostringstream os;
	  os << _digraph.id(n);
	  _writer_bits::writeToken(*_os, os.str());
	  *_os << '\t';
	  _node_index.insert(std::make_pair(n, os.str()));
	}
	for (typename NodeMaps::iterator it = _node_maps.begin();
	     it != _node_maps.end(); ++it) {
	  std::string value = it->second->get(n);
	  _writer_bits::writeToken(*_os, value);
	  if (it->first == "label") {
	    _node_index.insert(std::make_pair(n, value));
	  }
	  *_os << '\t';
	}
	*_os << std::endl;
      }
    }

    void writeArcs() {
      _writer_bits::MapStorageBase<Arc>* label = 0;
      for (typename ArcMaps::iterator it = _arc_maps.begin();
	   it != _arc_maps.end(); ++it) {
        if (it->first == "label") {
	  label = it->second;
	  break;
	}
      }

      *_os << "@arcs";
      if (!_arcs_caption.empty()) {
	*_os << ' ' << _arcs_caption;
      }
      *_os << std::endl;

      *_os << '\t' << '\t';
      if (label == 0) {
	*_os << "label" << '\t';
      }
      for (typename ArcMaps::iterator it = _arc_maps.begin();
	   it != _arc_maps.end(); ++it) {
	*_os << it->first << '\t';
      }
      *_os << std::endl;

      std::vector<Arc> arcs;
      for (ArcIt n(_digraph); n != INVALID; ++n) {
	arcs.push_back(n);
      }
      
      if (label == 0) {
	IdMap<Digraph, Arc> id_map(_digraph);
	_writer_bits::MapLess<IdMap<Digraph, Arc> > id_less(id_map);
	std::sort(arcs.begin(), arcs.end(), id_less);
      } else {
	label->sort(arcs);
      }

      for (int i = 0; i < static_cast<int>(arcs.size()); ++i) {
	Arc a = arcs[i];
	_writer_bits::writeToken(*_os, _node_index.
				 find(_digraph.source(a))->second);
	*_os << '\t';
	_writer_bits::writeToken(*_os, _node_index.
				 find(_digraph.target(a))->second);
	*_os << '\t';
	if (label == 0) {
	  std::ostringstream os;
	  os << _digraph.id(a);
	  _writer_bits::writeToken(*_os, os.str());
	  *_os << '\t';
	  _arc_index.insert(std::make_pair(a, os.str()));
	}
	for (typename ArcMaps::iterator it = _arc_maps.begin();
	     it != _arc_maps.end(); ++it) {
	  std::string value = it->second->get(a);
	  _writer_bits::writeToken(*_os, value);
	  if (it->first == "label") {
	    _arc_index.insert(std::make_pair(a, value));
	  }
	  *_os << '\t';
	}
	*_os << std::endl;
      }
    }

    void writeAttributes() {
      if (_attributes.empty()) return;
      *_os << "@attributes";
      if (!_attributes_caption.empty()) {
	*_os << ' ' << _attributes_caption;
      }
      *_os << std::endl;
      for (typename Attributes::iterator it = _attributes.begin();
	   it != _attributes.end(); ++it) {
	*_os << it->first << ' ';
	_writer_bits::writeToken(*_os, it->second->get());
	*_os << std::endl;
      }
    }
    
  public:
    
    /// \e
    void run() {
      if (!_skip_nodes) {
	writeNodes();
      }
      if (!_skip_arcs) {      
	writeArcs();
      }
      writeAttributes();
    }

    /// \e
    std::ostream& stream() {
      return *_os;
    }
  };

  template <typename Digraph>
  DigraphWriter<Digraph> digraphWriter(std::istream& is, Digraph& digraph) {
    return DigraphWriter<Digraph>(is, digraph);
  }

  template <typename Digraph>
  DigraphWriter<Digraph> digraphWriter(const std::string& fn, 
				       Digraph& digraph) {
    return DigraphWriter<Digraph>(fn, digraph);
  }

  template <typename Digraph>
  DigraphWriter<Digraph> digraphWriter(const char* fn, Digraph& digraph) {
    return DigraphWriter<Digraph>(fn, digraph);
  }
}

#endif