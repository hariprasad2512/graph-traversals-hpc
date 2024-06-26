#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "euler_tour.h"

using namespace std;

vector<pair<int,int>> edges2;

void postorder(vector<int>& weight, EulerTour et){
    vector<int> post(et.n,0);
    
    // populating post[v] for all v
    for(int i=0;i<et.num_edges;i++){
        int u,v;
        u=edges2[i].first;
        v=edges2[i].second;
        int j=et.reverse_map[v*MAX_FACTOR+u];
        // parent of u is v
        if(et.dist[et.tour_order[i]]>=et.dist[j]){
            post[u]=weight[i];
        }
    }
    post[et.root]=et.n;

    // printing postorder traversal
    cout<<"Postorder traversal:\n";
    for(int i=0;i<et.n;i++){
        cout<<(i+1)<<" => "<<(post[i])<<endl;
    }
}

void prefix_sum(vector<int>& weight, EulerTour et){
    int limit=(int) (log2(et.num_edges)+1);

    vector<int> weight2(et.num_edges,-1);

    // initialization of weight2
    for(int i=0;i<et.num_edges;i++){
        weight2[i]=weight[i];
    }

    for(int i=0;i<limit;i++){
        for(int j=0;j<et.num_edges;j++){
            if(j<(int) pow(2,i)){
                weight2[j]=weight[j];
            }
            else{
                weight2[j]=weight[j]+weight[j-(int) pow(2,i)];
            }
        }

        for(int j=0;j<et.num_edges;j++){
            weight[j]=weight2[j];
        }
    }
}

void assign_weights(vector<int>& weight, EulerTour et){
    weight.resize(et.num_edges,-1);
    edges2.resize(et.num_edges,{-1,-1});

    /*
    * changing the original order of et.edges
    * according to the order of Euler tour
    */
    for(int i=0;i<et.num_edges;i++){
        edges2[i]=et.edges[et.tour_order[i]];
    }

    // assigning weights to edges
    for(int i=0;i<et.num_edges;i++){
        int u,v;
        u=edges2[i].first;
        v=edges2[i].second;
        int j=et.reverse_map[v*MAX_FACTOR+u];
        // parent of v is u
        if(et.dist[et.tour_order[i]]<et.dist[j]){
            weight[i]=0;
        }
        // parent of u is v
        else{
            weight[i]=1;
        }
    }
}

int main(){
    // performing Euler tour
    EulerTour et;
    et.find_successors();
    et.list_ranking();
    et.store_order();
    
    // performing postorder traversal
    vector<int> weight;
    assign_weights(weight,et);
    prefix_sum(weight,et);
    postorder(weight,et);
    return 0;
}
