//
//  scheduling.cpp
//  hlsyn
//
//  Created by Finn Willard on 11/19/20.
//

#include "scheduling.hpp"

Graph Scheduler::force_directed(Graph g, int lambda){
    //create list of unscheduled nodes
    std::map<int, std::shared_ptr<vertex>> unscheduled = g.graph;
    
    unscheduled.erase(unscheduled.begin());
    unscheduled.erase(prev(unscheduled.end()));
    int unscheduled_count = unscheduled.size();
    int i = 0;
    while(unscheduled_count > 0){
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
        
        for(auto v : g.graph){
            //calc time frames for each operation
            v.second->time_frame = std::make_pair(g_asap.graph[v.first]->cycle, g_alap.graph[v.first]->cycle);
        }
    
//        //calc min force cycle for each vertex
        for(auto v : unscheduled){
            std::shared_ptr<vertex> u = v.second;
//
            u->total_force = std::numeric_limits<double>::max();
                for(int i = u->time_frame.first; i <= u->time_frame.second; i++){
                    double self_force = g.calc_self_force(i, u);
                    double pred_force = g.calc_predecessor_force(i, u);
                    double succ_force = g.calc_successor_force(i, u);
                    double total_force = self_force + pred_force + succ_force;
//
                    if(total_force < u->total_force){
                        u->total_force = total_force;
                        u->min_force_cycle = i;
                    }
                }
            }

        //schedule minimum force node
        std::shared_ptr<vertex> min_vertex = unscheduled.begin()->second;
        for(auto v : unscheduled){
            if(v.second->total_force < min_vertex->total_force){
                min_vertex = v.second;
            }
        }

        min_vertex->cycle = min_vertex->min_force_cycle;
        unscheduled.erase(min_vertex->id);
        unscheduled_count--;
        
        std::cout << "Force Directed schedule (iteration " << ++i << "): " << std::endl;
        print_schedule(g);
        std::cout << std::endl;
    }
    return g;
}

Graph Scheduler::asap(Graph g){
    g.graph.begin()->second->cycle = 0;
    
    while(g.graph.rbegin()->second->cycle == -1){
        std::shared_ptr<vertex> v = find_unscheduled_asap(&g);
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
        std::shared_ptr<vertex> v = find_unscheduled_alap(&g);
        int min = INT_MAX;
        for(auto u : v->outputs){
            int temp = u->cycle - v->delay;
            if(temp < min){
                min = temp;
            }
        }
        if(min < 0){
            throw ScheduleException("Cannot schedule ALAP with requested latency");
        }
        v->cycle = min;
        
    }
    return g;
}

std::shared_ptr<vertex> Scheduler::find_unscheduled_asap(Graph *g){
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
    throw ScheduleException("No nodes able to be scheduled");
}

std::shared_ptr<vertex> Scheduler::find_unscheduled_alap(Graph *g){
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
    throw ScheduleException("No nodes able to be scheduled");
}

void Scheduler::print_schedule(Graph g){
    int time = -1;
    while(!g.graph.empty()){
        std::cout << "Time " << time << ": ";
        std::map<int, std::shared_ptr<vertex>>::iterator it=g.graph.begin();
        while(it!=g.graph.end()){
            std::shared_ptr<vertex> v = it->second;
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
