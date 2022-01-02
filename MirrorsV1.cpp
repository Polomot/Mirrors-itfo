#include "mirror_module.h"

int main(int argc, char **argv)
{
// check given arguments 
	if (argc < 2 or argc > 3) {
        std::cerr << "commande execution: " << argv[0] << "path/input/file path/output/file";
        return EXIT_FAILURE;
    }
// variables declaration
	std::string infile(argv[1]);
	std::string outfile;
	unsigned int  nbcas=0;
	std::vector<std::vector<int>> pointits;

// open file   
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


	while (find_solution(pointits,argc,input_file,output_file,nbcas)){
// display intersections points
		if (argc == 2){
			std::cout<<"???????Point pour insertion mirroires????????"<<std::endl;
			for (unsigned int h=0;h<pointits.size();h++)
				std::cout << pointits[h][0]<< " "<< pointits[h][1] << std::endl;
			std::cout<<"?????????????????????????????????????????????"<<std::endl;
		}
// clearing points
		nbcas=nbcas+1;
		pointits.clear();		
    }
    if (argc == 2){
		std::cout <<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
		std::cout<<"endcode"<<std::endl;
	}
	output_file.close();
    input_file.close();
    return EXIT_SUCCESS;
}
