/*

A* grid based planning

*/

#include "a_star.h"

std::vector<Node> A_STAR::a_star(void *grid, int n, Node start, Node goal){
  int (*p_grid)[n][n] = (int (*)[n][n]) grid;
  double cost_grid[n][n];
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      cost_grid[i][j] = n*n;
    }
  }

  std::vector<Node> motion = get_motion(n);
  std::priority_queue<Node, std::vector<Node>, compare_cost> points_list;
  points_list.push(start);
  std::vector<Node> path_vector;
  path_vector.push_back(start);

  cost_grid[start.x][start.y] = 0;
  while(!points_list.empty()){
    Node current = points_list.top();
    current.id = current.x * n + current.y;
    points_list.pop();
    if(current == goal){
      return path_vector;
    }
    if((*p_grid)[current.x][current.y]!=0){
      continue; // Point already opened and
                // points around it added to points list
    }
    (*p_grid)[current.x][current.y] = 2; // Point opened
    int current_cost = current.cost;
    for(auto it = motion.begin(); it!=motion.end(); ++it){
      Node new_point;
      new_point = current + *it;
      new_point.h_cost = abs(new_point.x - goal.x) + abs(new_point.y - goal.y);
      if(new_point.x < 0 || new_point.y < 0
          || new_point.x >= n || new_point.y >= n) continue; // Check boundaries
      if(cost_grid[new_point.x][new_point.y] > new_point.cost + new_point.h_cost){//=1 && (*p_grid)[new_point.x][new_point.y]!=2){
        new_point.pid = current.id;
        new_point.id = new_point.x * n + new_point.y;
        points_list.push(new_point);
        std::vector<Node>::iterator it_v;
        it_v = find (path_vector.begin(), path_vector.end(), new_point);
        if (it_v != path_vector.end()) *it_v = new_point;
        else path_vector.push_back(new_point);
        cost_grid[new_point.x][new_point.y] = new_point.cost + new_point.h_cost;
      }
    }
  }
  path_vector.clear();
  Node no_path_node(-1,-1,-1,-1,-1,-1);
  path_vector.push_back(no_path_node);
  return path_vector;
}

#ifdef BUILD_INDIVIDUAL
int main(){
  int n = 3;
  int num_points = n*n;


  n = 6;
  int grid[n][n] = {
                     { 0 , 0 , 0 , 0 , 0, 0 },
                     { 0 , 1 , 0 , 0 , 0, 0 },
                     { 0 , 1 , 0 , 0 , 1, 0 },
                     { 0 , 1 , 0 , 0 , 1, 0 },
                     { 0 , 1 , 1 , 1 , 1, 0 },
                     { 0 , 0 , 0 , 0 , 0, 0 }
                   } ;

  //int grid[n][n];
  //make_grid(grid, n);

  //NOTE:
  // x = row index, y = column index.

  std::cout << "Grid:" << std::endl;
  std::cout << "1. Points not considered ---> 0" << std::endl;
  std::cout << "2. Obstacles             ---> 1" << std::endl;
  print_grid(grid, n);

  //Make sure start and goal not obstacles and their ids are correctly assigned.
  Node start(0,1,0,0,0,0);
  start.id = start.x * n + start.y;
  start.pid = start.x * n + start.y;
  Node goal(n-1,n-1,0,0,0,0);
  goal.id = goal.x * n + goal.y;
  start.h_cost = abs(goal.x - start.x) + abs(goal.y - start.y);

  grid[start.x][start.y] = 0;
  grid[goal.x][goal.y] = 0;
  std::map<Node, Node, compare_id> path;
  A_STAR new_a_star;
  std::vector<Node> path_vector = new_a_star.a_star(grid, n, start, goal);

  print_path(path_vector, start, goal, grid, n);

  return 0;
}
#endif
