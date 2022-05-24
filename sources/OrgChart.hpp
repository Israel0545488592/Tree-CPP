#include <vector>
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

/* Organistion-Chart. Main class of the program, ment to represant a tree*/

namespace ariel{

    class OrgChart{

    private:

        struct Node{        // the units that make upthe structure

            string val;
            vector <Node*> children;

            // de/constractors
            Node(const string& val) : val(val) {}
            Node(const Node&);


            // I dont know what those mean
            Node (const Node&& other) noexcept;
            Node& operator=(Node&& other) noexcept;

            Node& operator= (const Node&) noexcept;
            ~Node();

            void addChild(Node*);
        };

        // members
        Node *root;
        size_t size; 

        // private method, is this name already in the structure
        Node* find(const string&) const;
        Node* find(const string&, Node*) const;    // recursion

        enum class Method{BFS, RBFS, pre};      // iteration methods

    public:

        // de/constractors
        OrgChart() : size(0), root(nullptr) {}
        OrgChart(const OrgChart& other) : size(0), root(other.root) {}

        //I dont know what those mean
        OrgChart (const OrgChart&& other) noexcept;
        OrgChart& operator= (const OrgChart&) noexcept;

        OrgChart& operator=(OrgChart&&) noexcept;
        ~OrgChart();

        class Iterator{

        private:

            size_t cur;             // cursur
            Method method;          // iteration method
            vector <Node*> order;   // order of iteration defined by iteration method

        public:

            // Iteration Methods
            //Based on: https://he.wikipedia.org/wiki/%D7%90%D7%9C%D7%92%D7%95%D7%A8%D7%99%D7%AA%D7%9D_%D7%97%D7%99%D7%A4%D7%95%D7%A9_%D7%9C%D7%A8%D7%95%D7%97%D7%91

            void BFS(Node* cur){

                if (cur == nullptr){ return;}

                queue <Node*> Q;
                Q.push(cur);

                while (! Q.empty()){

                    cur = Q.front();
                    Q.pop();
                    order.push_back(cur);

                    for (Node* node : cur -> children){ Q.push(node); }
                }
            }

            void RBFS(Node* cur){

                if (cur == nullptr){ return;}

                stack <Node*> stk;
                queue <Node*> Q;
                Q.push(cur);

                while (! Q.empty()){

                    cur = Q.front();
                    Q.pop();

                    for (size_t i = 0; i < cur -> children.size(); i++){    // inverted order
                        Q.push(cur -> children.at(cur -> children.size() -1 -i));
                    }

                    stk.push(cur);
                }

                while (! stk.empty()){

                    order.push_back(stk.top());
                    stk.pop();
                }
            }

            void pre(Node* cur){

                if (cur == nullptr){ return;}

                stack <Node*> stk;
                stk.push(cur);

                while (! stk.empty()){

                    cur = stk.top();
                    stk.pop();
                    order.push_back(cur);

                    for (size_t i = 0; i < cur -> children.size(); i++){    // inverted order
                        stk.push(cur -> children.at(cur -> children.size() -1 -i));
                    }
                }
            }

            inline Iterator(Node *root, Method method, size_t size, size_t cur) : cur(cur), method(method) {

                order = vector <Node*> ();

                if (cur != 0){ return;}      // an end iterator

                switch (method){

                case Method::BFS:
                    
                    BFS(root);
                    break;

                case Method::RBFS:
                    
                    RBFS(root);
                    break;

                case Method::pre:
                    
                    pre(root);
                    break;
                
                default:
                    break;
                }
            }

            // iterator operators for fluent cpp code

            bool operator!=(const Iterator& it) const { return cur != it.cur; }

            void operator++(){ 

                cur++;
                if (cur > order.size()){ throw out_of_range("u have reached the end of the chart");}
            }
            void operator++(int){ 

                cur++;
                if (cur > order.size()){ throw out_of_range("u have reached the end of the chart");}
            }

            const string& operator*() const { return order[cur] -> val;}

            const string* operator->() const { return &order[cur] -> val;}
        };


    // methods for instancing an iterator for fluent cpp code

        Iterator begin()
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(root, Method::BFS, size, 0);
        }
        Iterator end() const
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(nullptr, Method::BFS, 0, size);
        }
        Iterator begin_level_order()
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(root, Method::BFS, size, 0);
        }
        Iterator end_level_order() const
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(nullptr, Method::BFS, 0, size);
        }
        Iterator begin_reverse_order()
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(root, Method::RBFS, size, 0);
        }
        Iterator reverse_order() const
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(root ,Method::RBFS, 0, size);
        }
        Iterator begin_preorder()
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(root, Method::pre, size, 0);
        }
        Iterator end_preorder() const
        {
            if (size == 0){ throw runtime_error("chart is empty"); }
            return Iterator(nullptr, Method::pre, 0, size);
        }

        // class methods

        OrgChart& add_root(const string&);                       //set root
        OrgChart& add_sub(const string&, const string&);         //add node
        friend ostream& operator<< (ostream&, const Node*);      //to-string
        friend ostream& operator<< (ostream&, const OrgChart&);  //to-string
    };
}