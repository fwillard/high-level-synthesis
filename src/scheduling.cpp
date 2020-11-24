//
//  scheduling.cpp
//  hlsyn
//
//  Created by Finn Willard on 11/19/20.
//

#include "scheduling.hpp"

void Scheduler::force_directed(Graph g, int lambda){
    //generate asap schedule
    Graph g_asap = asap(g);
    
    std::cout << "ASAP schedule: " << std::endl;
    print_schedule(g_asap);
    std::cout << std::endl;
    
    //generate alap schedule
    Graph g_alap = alap(g, lambda);
    
    std::cout << "ALAP schedule: " << std::endl;
    print_schedule(g_alap);
    std::cout << std::endl;
    
    //calc time frames for each operation
    for(auto v : g.graph){
        v.second->time_frame = std::make_pair(g_asap.graph[v.first]->cycle, g_alap.graph[v.first]->cycle);
    }
}

Graph Scheduler::asap(Graph g){
    g.graph.begin()->second->cycle = 0;
    
    while(g.graph.rbegin()->second->cycle == -1){
        vertex *v = find_unscheduled_asap(&g);
        int max = 0;
        for(auto u : v->inputs){
            int temp = u->cycle + u->delay;
            if(temp > max){
                max = temp;
            }
        }
        v->cycle = max;
    }
    return g;
}

Graph Scheduler::alap(Graph g, int lambda){
    g.graph.rbegin()->second->cycle = lambda + 1;
    while(g.graph.begin()->second->cycle == -1){
        vertex *v = find_unscheduled_alap(&g);
        int min = INT_MAX;
        for(auto u : v->outputs){
            int temp = u->cycle - v->delay;
            if(temp < min){
                min = temp;
            }
        }
        if(min < 0){
            throw "Cannot schedule ALAP with requested latency";
        }
        v->cycle = min;
        
    }
    return g;
}

vertex* Scheduler::find_unscheduled_asap(Graph *g){
    for(auto v : g->graph){
        bool scheduled = false;
        for(auto u : v.second->inputs){
            if(u->cycle == -1){
                continue;
            }
            scheduled = true;
        }
        if(scheduled && v.second->cycle == -1){
            return v.second;
        }
    }
    throw "No nodes able to be scheduled";
}

vertex* Scheduler::find_unscheduled_alap(Graph *g){
    for(auto v : g->graph){
        bool scheduled = true;
        for(auto u : v.second->outputs){
            if(u->cycle == -1){
                scheduled = false;
            }
        }
        if(scheduled && v.second->cycle == -1){
            return v.second;
        }
    }
    throw "No nodes able to be scheduled";
}

void Scheduler::print_schedule(Graph g){
    int time = 0;
    while(!g.graph.empty()){
        std::cout << "Time " << time << ": ";
        std::map<int, vertex*>::iterator it=g.graph.begin();
        while(it!=g.graph.end()){
            vertex* v = it->second;
            if(v->cycle == time){
                std::cout << v->id << " ";
                g.graph.erase(it);
                it = g.graph.begin();
            }
            else{
                it++;
            }
        }
        std::cout << std::endl;
        std::cout << "=========================" << std::endl;
        time++;
    }
}
