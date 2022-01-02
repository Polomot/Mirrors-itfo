# include <iostream>
# include <string>
# include <vector>
# include <fstream>

// function that return 1 if value = 0 and -1 if value = 1
int ret_inc_or_dec(unsigned int value);

/* function to read input file and buils vectors of mirrors type M and N and the header
 return true if reading well done, otherwise return false */
bool read_file(std::ifstream& input_file,std::vector<std::vector<int>>& mirrors_m,std::vector<std::vector<int>>& mirrors_n,std::vector<unsigned int>& header);

// fonction to find if point could be insert at the end of flow if yes insert it and return true else return false
bool search_next_LorC_4flow(std::vector<int>& flow,std::vector<int> point,unsigned int t,bool line,bool dir);

// function to build flow
void build_flow(std::vector<int>& flow,std::vector<std::vector<int>> mirrors_m,std::vector<std::vector<int>> mirrors_n,bool line,bool dir,int r_max,int c_max);

// function to find intersection between 2 flow and insert them in intersection_points 
void find_flow_intersection(std::vector<std::vector<int>>& intersection_point,std::vector<int> flow1,std::vector<int> flow2);

// function that read the input file find solution for nex data set and return true if well done and pointits is completed with intersection points
bool find_solution(std::vector<std::vector<int>>& pointits,int argc,std::ifstream& input_file,std::ofstream& output_file,unsigned int  nbcas);
