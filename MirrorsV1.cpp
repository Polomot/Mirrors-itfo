# include <iostream>
# include <string>
# include <vector>
# include <fstream>
//# include <algorithm>
# include <cmath>

bool search_next_LorC_4flux(std::vector<int>& flux,std::vector<int> point,unsigned int t,bool line,bool dir){
// fonction pour chercher si le point donne peut etre inserer dans le flux si oui l'insert et renvoie vrai sinon renvoie faux
	unsigned int LorC=1;
	bool find=false;
	if (line) LorC=0;
	if (flux[t]==point[LorC]){
		LorC=LorC+std::pow(-1,LorC);				
		if(dir){
			if (flux.size()==t+1 and point[LorC]>flux[t-1]){							
				flux.push_back(point[LorC]);
				find=true;							
			}else if(point[LorC]<flux[t+1] and point[LorC]>flux[t-1]){
				flux[t+1]=point[LorC];
				find=true;
			}
//			}else{
//				find=false;
//			}
		}else{
			if (flux.size()==t+1 and point[LorC]<flux[t-1]){
				flux.push_back(point[LorC]);
				find=true;						
			}else if(point[LorC]>flux[t+1] and point[LorC]<flux[t-1]){
				flux[t+1]=point[LorC];
				find=true;
			}
//			}else{
//				find=false;
//			}
		}
	}
	return find;
}

void find_flux_intersection(std::vector<std::vector<int>>& intersection_point,std::vector<int> flux1,std::vector<int> flux2){
	float float4div;
	for (unsigned int i=1;i<flux1.size()-1;i++){
		float4div=i;
		if (float4div/2.!=i/2){
// test des intersection ligne du flux in et colone flux out
			for (unsigned int j=1;j<flux2.size()-1;j++){
				float4div=j;						
				if (float4div/2.==j/2){
					if (flux1[i]<std::max(flux2[j-1],flux2[j+1]) and flux1[i]>std::min(flux2[j-1],flux2[j+1])){
						if (flux2[j]<std::max(flux1[i-1],flux1[i+1]) and flux2[j]>std::min(flux1[i-1],flux1[i+1])){
							intersection_point.push_back({flux1[i],flux2[j]});
						}
					}													
				}						
			}
		}else{
// test des intersection colone du flux in et ligne flux out
			for (unsigned int j=1;j<flux2.size()-1;j++){
				float4div=j;
				if (float4div/2.!=j/2){
					if (flux1[i]<std::max(flux2[j-1],flux2[j+1]) and flux1[i]>std::min(flux2[j-1],flux2[j+1])){
						if (flux2[j]<std::max(flux1[i-1],flux1[i+1]) and flux2[j]>std::min(flux1[i-1],flux1[i+1])){
							intersection_point.push_back({flux2[j],flux1[i]});
						}
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
// Contole du nombre d'argument transmit lors de l'execution
	if (argc < 2 or argc > 3) {
        std::cerr << "commande execution: " << argv[0] << "path/input/file path/input/file";
        return EXIT_FAILURE;
    }
// declaration des variables
	std::string infile(argv[1]);
	std::string outfile;
    std::vector<unsigned int> header;
    std::vector<std::vector<int>> datam;
    std::vector<std::vector<int>> datan;
    std::vector<std::vector<int>> pointits;
    std::vector<int> inter({0,0});
    std::vector<int> fluxin;
    std::vector<int> fluxout;
    std::string sinter;
    std::string line;
    unsigned int tin=0,tout=0 , nbcas=0,u=0;
    size_t pos = 0;    
    bool endin=false,endout=false,linein=true,lineout=true,dirin=true,dirout=false,inmir=true,outmir=true;
// ouverture des fichier    
    std::ifstream input_file(infile);
    if (!input_file.is_open()) {
        std::cerr << "Problème d'ouverture du fichier - '"
             << infile << "'" << std::endl;
        return EXIT_FAILURE;
    }
    if (argc != 2){
		outfile=argv[2];
		
	}
	std::ofstream output_file(outfile);
// dedut du traitement	
    while (getline(input_file, line)){
        pos = 0; 
        if (argc == 2){ 
			std::cout <<"!!!!!!!!!!!Nouveau Cas!!!!!!!!!!!!"<< std::endl;
			std::cout <<"cas N: "<< nbcas+1 << std::endl;
		}
// lecture du header	
		while (pos<line.length()){
			sinter = line.substr(pos,line.find(" ",pos)-pos);
			pos=pos+sinter.length()+1;
			header.push_back(atoi(sinter.c_str()));
		}
// initialisation des flux
		fluxin.push_back({0});
		fluxout.push_back(header[1]+1);
		fluxin.push_back({1});
		fluxout.push_back(header[0]);
// traitement pour remplir les flux
		for (unsigned int i=0;i<header[2]+header[3];i++){
			pos=0;
			u=0; 
// lecture des points
			getline(input_file, line);
			while (pos<line.length()){
				sinter = line.substr(pos,line.find(" ",pos)-pos);
				pos=pos+sinter.length()+1;
				inter[u]=atoi(sinter.c_str());
				u=u+1;	
			}
			if (i<header[2]){
				datam.push_back({inter[0],inter[1]});
			}else{
				datan.push_back({inter[0],inter[1]});
			}
		}
// ecriture des donnees d'entrees
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
// traitement du remplissage des flux
		tin=1;
		tout=1;
		endin=false;endout=false;linein=true;lineout=true;dirin=true;dirout=false;inmir=true;outmir=true;
		while (!endin or !endout){
// recherche des point avec un mirroire "/" flux			
			for (unsigned int i=0;i<datam.size();i++){
					
// flux	dedutant en entree
				if (search_next_LorC_4flux( fluxin, datam[i], tin, linein, dirin)){
					inmir=true;
				}
// flux	dedutant en sortie
				if (search_next_LorC_4flux( fluxout, datam[i], tout, lineout, dirout)){
					outmir=true;
				}
				
			}

// recherche des point avec un mirroire "\" flux		
			for (unsigned int i=0;i<datan.size();i++){
// flux	dedutant en entree	
				if (search_next_LorC_4flux( fluxin, datan[i], tin, linein, dirin)){
					inmir=false;
				}
// flux	dedutant en sortie
				if (search_next_LorC_4flux( fluxout, datan[i], tout, lineout, dirout)){
					outmir=false;
				}
			}
// determination suite traitement
			if (inmir and fluxin.size()>tin+1){				
				dirin=!dirin;
				tin=tin+1;
				linein=!linein;
			}else if (fluxin.size()>tin+1){				
				tin=tin+1;
				linein=!linein;
			}else if(!endin){
				if (dirin){
					if (linein){
						fluxin.push_back(header[0]+1);
					}else{
						fluxin.push_back(header[1]+1);
					}
				}else{
					fluxin.push_back(0);
				}
				endin=true;
			}
			if (outmir and fluxout.size()>tout+1){
				
				dirout=!dirout;
				tout=tout+1;
				lineout=!lineout;
			}else if (fluxout.size()>tout+1){
				
				tout=tout+1;
				lineout=!lineout;
			}else if(!endout) {
				if (dirout){
					if (lineout){
						fluxout.push_back(header[0]+1);
					}else{
						fluxout.push_back(header[1]+1);
					}
				}else{
					fluxout.push_back(0);
				}
					
				endout=true;
			}
		}
// affichage des flux
		if (argc == 2){
			std::cout << "***fluxin***"<<std::endl;
			for (unsigned int h=0;h<fluxin.size();h++)
				std::cout << fluxin[h] << std::endl;
			std::cout << "***fluxout***"<<std::endl;
			for (unsigned int h=0;h<fluxout.size();h++)
				std::cout << fluxout[h] << std::endl;
		}
// recherche de résolution
		if (fluxin[tin]==fluxout[1] and dirin){
			
			if (argc != 2){
				output_file << "Cas "<< nbcas+1 << ": 0" << std::endl;
			}else{
				std::cout << "%%%%%pas besoin de mirroires%%%%%"<< std::endl;
			}
		}else{
			find_flux_intersection(pointits,fluxin,fluxout);
// remplissage des points ou inserer un mirroire pour relier l'entree à la sortie
			if (argc != 2){
				if (pointits.size()>=1){
					inter[0]=pointits[0][0];
					inter[1]=pointits[0][1];
					for (unsigned int h=1;h<pointits.size();h++){
						if (pointits[h][0]<inter[0]){
							inter[0]=pointits[h][0];
							inter[1]=pointits[h][1];
						}else if (pointits[h][1]<inter[1]){
							inter[0]=pointits[h][0];
							inter[1]=pointits[h][1];
						}			
					}					
					output_file << "Cas "<< nbcas+1 << ": "<< pointits.size()<< " "<<inter[0]<< " "<< inter[1]<< std::endl;
				}else{					
					output_file << "Cas "<< nbcas+1 << ": impossible"<< std::endl;
				}
			}
				
		}
// affichage des points ou inserer un mirroire pour relier l'entree à la sortie
		if (argc == 2){
			std::cout<<"???????Point pour insertion mirroires????????"<<std::endl;
			for (unsigned int h=0;h<pointits.size();h++)
				std::cout << pointits[h][0]<< " "<< pointits[h][1] << std::endl;
			std::cout<<"?????????????????????????????????????????????"<<std::endl;
		}
// reinitialisation des variables
		nbcas=nbcas+1;
		pointits.clear();
		fluxin.clear();
		fluxout.clear();
		header.clear();
		datam.clear();
		datan.clear();
		
    }
    if (argc == 2){
		std::cout <<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
		std::cout<<"endcode"<<std::endl;
	}
	output_file.close();
    input_file.close();
    return EXIT_SUCCESS;
}
