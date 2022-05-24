#include "OrgChart.hpp"

using namespace std;

namespace ariel{

    OrgChart::Node::~Node(){

        for (Node* child : children){
            
            child -> ~Node();
        }
    }

    OrgChart::~OrgChart(){

        if (root == nullptr){ return;}

        root -> ~Node();
    }

    OrgChart::Node& OrgChart::Node::operator= (const Node& other) noexcept{

        *this = OrgChart::Node(other);

        return *this;
    }

    OrgChart& OrgChart::operator= (const OrgChart& other) noexcept{

        *this = OrgChart(other);

        return *this;
    }

    OrgChart::Node::Node(const Node& node) : val(node.val){

        for (Node* child : node.children){

            Node copy_child = Node(*child);

            children.push_back(&copy_child);
        }
    }

    // I dont know what those mean:

    OrgChart::Node::Node (const OrgChart::Node&& other) noexcept{

        val = other.val;
        children = other.children;        
    }

    OrgChart::Node& OrgChart::Node::operator=(Node&& other) noexcept{

        return *this;
    }

    OrgChart::OrgChart (const OrgChart&& other) noexcept{

        size = other.size;
        root = other.root;
    }

    OrgChart& OrgChart::operator=(OrgChart&& other) noexcept{

        return *this;
    }

    // end, from now on I do know what I am doing

    void OrgChart::Node::addChild(Node* child){ this -> children.push_back(child);}

    OrgChart::Node* OrgChart::find(const string& name) const{

        if (root == nullptr) {return nullptr;}
        
        return find(name, root);
    }

    OrgChart::Node* OrgChart::find(const string& name, Node* node) const{

        // halting terms
        if (node -> val == name){ return node;}
        if (node -> children.empty()){ return nullptr;}

        for (Node* child : node -> children){
            
            Node* desendant = find(name, child);

            if (desendant != nullptr){ return desendant;}
        }
        return nullptr;
    }

    OrgChart& OrgChart::add_root(const string& name){ 

        if (root != nullptr) {

            root -> val = name;
        }
        else{

            root = new Node{ name };
            size++;
        }
        return *this;
     }

    OrgChart& OrgChart::add_sub(const string& supirior, const string& infirior){

        Node* father = find(supirior);

        if (father != nullptr){

            father -> addChild(new Node(infirior));
            size++;

        }else{ throw invalid_argument("suprior does not present in the chart"); }

        return *this;
    }

    ostream& operator<< (ostream& out, const OrgChart::Node* node){

        if (node == nullptr){ return out;}

        out << node -> val << "  ";

        for (OrgChart::Node* child : node -> children){

            out << child;
        }

        return out;
    }

    ostream& operator<< (ostream& out, const OrgChart& chart){

        OrgChart::Node* curr = chart.root;

        if (curr == nullptr){ return out;}

        out << curr -> val << "  ";

        for (OrgChart::Node* child : curr -> children){

            out << child;
        }

        return out;
    }
}