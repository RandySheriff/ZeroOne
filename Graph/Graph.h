#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <set>
#include <cstdlib>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>
#define FOR_FRM_0(I,T) for(size_t I=0;I<T;++I)
#define FOR(I,F,T) for(size_t I=F;I<T;++I)

template<typename T>
class Graph
{
public:

    using Edge = std::pair<size_t, T>;

    class Node
    {
    public:
        friend class Graph;
        virtual ~Node() {};

    private:

        std::vector<Edge> __edges;
    };

    virtual ~Graph()
    {
        delete [] __nodes;
    }

    void PrintGraph(const T& defaultValue) const
    {
        FOR_FRM_0 (i,__nodeCount)
        {
            auto* curFrmNode = &__nodes[i];

            size_t index = 0;

            for (auto& edge: curFrmNode->__edges)
            {
                while (index++ < edge.first)
                {
                    std::cout << defaultValue << " ";
                }

                std::cout << edge.second << " ";
            }

            while (index++ < __nodeCount)
            {
                std::cout << defaultValue << " ";
            }

            std::cout << std::endl;
        }
    }

    // f = g + h
    T FindShortestPathByAStarSearch(size_t frm, size_t to, const T& notReachable,
                                    std::function<bool(const T&,const T&)> isSmaller /*, std::vector<size_t>& path*/) const
    {
        if (!__isValidNodeIndex(frm) || !__isValidNodeIndex(to) || frm == to)
        {
            return notReachable;
        }

        T* closedDistArray = new T[__nodeCount];

        FOR_FRM_0 (i, __nodeCount)
        {
            closedDistArray[i] = notReachable;
        }

        struct SetNode
        {
           size_t idx;
           T g;
           T h;
           // T f() const { return g + h; }
           T f() const { return g; }
           bool operator < (const SetNode& node) const {return f() < node.f();}
           bool operator > (const SetNode& node) const {return f() > node.f();}
           void Print() const
           {
               std::cout << "(" << idx << "," << g << "," << h << ") ";
           }
        };

        // use std set as priority queue
        std::multiset<SetNode> priorityQueue;

        for (auto& edge: __nodes[frm].__edges)
        {
            SetNode nextNode = {edge.first, edge.second, __calcH(edge.first, to)};

            priorityQueue.insert(nextNode);

            closedDistArray[edge.first] = nextNode.f();
        }

        /*
        std::cout << " ------------- " << std::endl;

        auto iter = priorityQueue.begin();
        while (iter != priorityQueue.end())
        {
            iter->Print();
            ++iter;
        }

        std::cout << std::endl << " ------------- " << std::endl;
        */

        while (!priorityQueue.empty())
        {
            auto curNode = *priorityQueue.begin();

            priorityQueue.erase(priorityQueue.begin());

            std::cout << "Dequeue:" << std::endl;
            curNode.Print();
            std::cout << std::endl;

            if (curNode.idx == to)
            {
                return curNode.g;
            }
            else
            {
                for (auto& edge: __nodes[curNode.idx].__edges)
                {
                    auto g = curNode.g + edge.second;
                    auto h = __calcH(edge.first, to);
                    SetNode nextNode = {edge.first, g, h};
                    if (closedDistArray[edge.first] == notReachable ||
                        !isSmaller(closedDistArray[edge.first], nextNode.f()))
                    {
                        // closedDistArray[edge.first] = nextNode.f();
                        priorityQueue.insert(nextNode);
                        std::cout << "Enqueue:" << std::endl;
                        nextNode.Print();
                        std::cout << std::endl;
                    }
                }
            }

            closedDistArray[curNode.idx] = curNode.f();
        }

        return notReachable;
    }

    static Graph<T>* CreateFromArray(void* addr, size_t sz, const T& notReachable, std::function<bool(const T&)> hasEdge)
    {
        auto graph = new Graph<T>();
        graph->__nodes = new Node[sz];
        graph->__nodeCount = sz;

        FOR_FRM_0 (i,sz)
        {
            auto* curFrmNode = &graph->__nodes[i];

            FOR_FRM_0 (j, sz)
            {
                if (i == j) { continue; }

                auto t = *((T*)addr + sz * i + j);

                if (t != notReachable && hasEdge(t))
                {
                    curFrmNode->__edges.push_back(std::make_pair(j,t));
                }
            }
        }

        return graph;
    }

private:

    T __calcH(size_t frm, size_t to) const
    {
        T xDist = (frm % __nodeCount) > (to % __nodeCount) ? (frm % __nodeCount) - (to % __nodeCount) : (to % __nodeCount) - (frm % __nodeCount);
        T yDist = (frm / __nodeCount) > (to / __nodeCount) ? (frm / __nodeCount) - (to / __nodeCount) : (to / __nodeCount) - (frm / __nodeCount);
        return xDist + yDist;
    }

    bool    __isValidNodeIndex (size_t idx) const { return idx < __nodeCount; }
    Node*   __nodes = nullptr;
    size_t  __nodeCount = 0;
    Graph() {};
};
#endif // GRAPH_H_INCLUDED
