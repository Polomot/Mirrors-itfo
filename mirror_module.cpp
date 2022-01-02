#include "mirror_module.h"

int ret_inc_or_dec(unsigned int value){
	if (value==0) return 1;
	return -1;
} 

bool read_file(std::ifstream& input_file,std::vector<std::vector<int>>& mirrors_m,std::vector<std::vector<int>>& mirrors_n,std::vector<unsigned int>& header){
	std::string sinter;
    std::string line;
    size_t pos = 0; 
    std::vector<int> inter({0,0});
    unsigned int u=0;
	if(!getline(input_file, line)) return false;
	while (pos<line.length()){
		sinter = line.substr(pos,line.find(" ",pos)-pos);
		pos=pos+sinter.length()+1;
		header.push_back(atoi(sinter.c_str()));
	}
	if (header.size()!=4){
		std::cout<<"file format incorrect: wrong header, please respect format"<<std::endl;
		std::cout<<"header line: 'NB_rows NB_columns NB_mirrors_m(/) NB_mirrors_n(\\)'"<<std::endl;
		std::cout<<"NB_mirrors_m lines: 'rows_indice columns_indice'"<<std::endl;
		std::cout<<"NB_mirrors_n lines: 'rows_indice columns_indice'"<<std::endl;
		return false;
	}
	for (unsigned int i=0;i<header[2]+header[3];i++){
		pos=0;
		u=0; 
// reading points
		if(!getline(input_file, line)){
			std::cout<<"file format incorrect: wrong NB_mirrors_m or NB_mirrors_n, please respect format"<<std::endl;
			std::cout<<"header line: 'NB_rows NB_columns NB_mirrors_m(/) NB_mirrors_n(\\)'"<<std::endl;
			std::cout<<"NB_mirrors_m lines: 'rows_indice columns_indice'"<<std::endl;
			std::cout<<"NB_mirrors_n lines: 'rows_indice columns_indice'"<<std::endl;
			return false;
		}
		while (pos<line.length()){
			sinter = line.substr(pos,line.find(" ",pos)-pos);
			pos=pos+sinter.length()+1;
			inter[u]=atoi(sinter.c_str());
			u=u+1;	
		}
		if (i<header[2]){
			mirrors_m.push_back({inter[0],inter[1]});
		}else{
			mirrors_n.push_back({inter[0],inter[1]});
		}
	}
	return true;
	
}
bool search_next_LorC_4flow(std::vector<int>& flow,std::vector<int> point,unsigned int t,bool line,bool dir){
	unsigned int LorC=1;
	bool find=false;
	if (line) LorC=0;
	if (flow[t]==point[LorC]){
		LorC=LorC+ret_inc_or_dec(LorC);				
		if(dir){
			if (flow.size()==t+1 and point[LorC]>flow[t-1]){							
				flow.push_back(point[LorC]);
				find=true;							
			}else if(point[LorC]<flow[t+1] and point[LorC]>flow[t-1]){
				flow[t+1]=point[LorC];
				find=true;
			}
		}else{
			if (flow.size()==t+1 and point[LorC]<flow[t-1]){
				flow.push_back(point[LorC]);
				find=true;						
			}else if(point[LorC]>flow[t+1] and point[LorC]<flow[t-1]){
				flow[t+1]=point[LorC];
				find=true;
			}
		}
	}
	return find;
}

void build_flow(std::vector<int>& flow,std::vector<std::vector<int>> mirrors_m,std::vector<std::vector<int>> mirrors_n,bool line,bool dir,int r_max,int c_max){
	bool end=false,mirrors=true;
	unsigned int t=1;
	while (!end){
// search if next mirror is type M "/" and insert it in flow			
		for (unsigned int i=0;i<mirrors_m.size();i++){
					
			if (search_next_LorC_4flow( flow, mirrors_m[i], t, line, dir)){
				mirrors=true;
			}
		}
// search if next mirror is type N "/" and insert it in flow		
		for (unsigned int i=0;i<mirrors_n.size();i++){

			if (search_next_LorC_4flow( flow, mirrors_n[i], t, line, dir)){
				mirrors=false;
			}
		}
// test and update parameters for next mirror
		if (mirrors and flow.size()>t+1){				
			dir=!dir;
			t=t+1;
			line=!line;
		}else if (flow.size()>t+1){				
			t=t+1;
			line=!line;
		}else if(!end){
			if (dir){
				if (line){
					flow.push_back(r_max+1);
				}else{
					flow.push_back(c_max+1);
				}
			}else{
				flow.push_back(0);
			}
			end=true;
		}
	}
}

void find_flow_intersection(std::vector<std::vector<int>>& intersection_point,std::vector<int> flow1,std::vector<int> flow2){
	float float4div; 
	for (unsigned int i=1;i<flow1.size()-1;i++){
		float4div=i;
		if (float4div/2.!=i/2){
// test of intersection row of flow in and culumn of flow out
			for (unsigned int j=1;j<flow2.size()-1;j++){
				float4div=j;						
				if (float4div/2.==j/2){
					if (flow1[i]<std::max(flow2[j-1],flow2[j+1]) and flow1[i]>std::min(flow2[j-1],flow2[j+1])){
						if (flow2[j]<std::max(flow1[i-1],flow1[i+1]) and flow2[j]>std::min(flow1[i-1],flow1[i+1])){
							intersection_point.push_back({flow1[i],flow2[j]});
							if (intersection_point.size()>1){
								if( !(flow1[i]<intersection_point[intersection_point.size()-2][0]) and !(flow1[i]==intersection_point[intersection_point.size()-2][0] and flow2[j]<intersection_point[intersection_point.size()-2][1])){
									intersection_point[intersection_point.size()-1]=intersection_point[intersection_point.size()-2];
									intersection_point[intersection_point.size()-2]={flow1[i],flow2[j]};
								}
							}	
						}
					}													
				}						
			}
		}else{
// test of intersection culumn of flow in and row of flow out
			for (unsigned int j=1;j<flow2.size()-1;j++){
				float4div=j;
				if (float4div/2.!=j/2){
					if (flow1[i]<std::max(flow2[j-1],flow2[j+1]) and flow1[i]>std::min(flow2[j-1],flow2[j+1])){
						if (flow2[j]<std::max(flow1[i-1],flow1[i+1]) and flow2[j]>std::min(flow1[i-1],flow1[i+1])){
							intersection_point.push_back({flow2[j],flow1[i]});
							if (intersection_point.size()>1){
								if( !(flow2[j]<intersection_point[intersection_point.size()-2][0]) and !(flow2[j]==intersection_point[intersection_point.size()-2][0] and flow1[i]<intersection_point[intersection_point.size()-2][1])){
									intersection_point[intersection_point.size()-1]=intersection_point[intersection_point.size()-2];
									intersection_point[intersection_point.size()-2]={flow2[j],flow1[i]};
								}
							}
						}
					}
				}
			}
		}
	}
}

bool find_solution(std::vector<std::vector<int>>& pointits,int argc,std::ifstream& input_file,std::ofstream& output_file,unsigned int  nbcas){
    std::vector<unsigned int> header;
    std::vector<std::vector<int>> datam;
    std::vector<std::vector<int>> datan;
    std::vector<int> flowin;
    std::vector<int> flowout;
    bool linein=true,lineout=true,dirin=true,dirout=false,ok=true;

	ok=read_file(input_file,datam,datan,header);
	if (!ok) return ok;
    if (argc == 2){ 
		std::cout <<"!!!!!!!!!!!Nouveau Cas!!!!!!!!!!!!"<< std::endl;
		std::cout <<"cas N: "<< nbcas+1 << std::endl;
	}
// initialisation of flow
	flowin.push_back({0});
	flowout.push_back(header[1]+1);
	flowin.push_back({1});
	flowout.push_back(header[0]);
// write input data
	if (argc == 2){
		std::cout <<"header :"<< std::endl;
		for (unsigned int j=0;j<header.size();j++)
			std::cout << header[j] << std::endl;
		std::cout <<"datam :"<< std::endl;
		for (unsigned int h=0;h<datam.size();h++)
			std::cout << datam[h][0]<< " "<< datam[h][1] << std::endl;
		std::cout <<"datan :"<< std::endl;
		for (unsigned int k=0;k<datan.size();k++)
			std::cout << datan[k][0]<<" "<< datan[k][1] << std::endl;
		std::cout <<"--------------------"<< std::endl;
	}
// build flow
	linein=true;lineout=true;dirin=true;dirout=false;
	build_flow(flowin, datam, datan,linein,dirin,header[0],header[1]);
	build_flow(flowout, datam,datan,lineout,dirout,header[0],header[1]);
	header.clear();
	datam.clear();
	datan.clear();
// display flow
	if (argc == 2){
		std::cout << "***flowin***"<<std::endl;
		for (unsigned int h=0;h<flowin.size();h++)
			std::cout << flowin[h] << std::endl;
		std::cout << "***flowout***"<<std::endl;
		for (unsigned int h=0;h<flowout.size();h++)
			std::cout << flowout[h] << std::endl;
	}
// search and write solution
	if (flowin[flowin.size()-2]==flowout[1] and (unsigned int) flowin.back()==header[1]+1){
		if (argc != 2){
			output_file << "Cas "<< nbcas+1 << ": 0" << std::endl;
		}else{
			std::cout << "%%%%%pas besoin de mirroires%%%%%"<< std::endl;
		}
	}else{
		find_flow_intersection(pointits,flowin,flowout);
		if (argc != 2){
			if (pointits.size()>=1){				
				output_file << "Cas "<< nbcas+1 << ": "<< pointits.size()<< " "<<pointits.back()[0]<< " "<< pointits.back()[1]<< std::endl;
			}else{					
				output_file << "Cas "<< nbcas+1 << ": impossible"<< std::endl;
			}
		}
	}
	return ok;
}
