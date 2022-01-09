#include <iostream>
#include <fstream>
#include "DAG.hpp"
#include "convert.hpp"
#include "json.hpp"


using json = nlohmann::json;

//#define DEBUG
int main(int argc, char** argv)
{
#ifdef DEBUG
    QuadExp E;
    DAG D;

    std::ifstream in("in02.txt");
    while (in.peek() != EOF)
    {
        std::string tmp;
        std::getline(in, tmp);
        E = convert(strip(strip(tmp, ' '), '"'));
        D.readQuad(E);
    }

    std::cout << D.print_DAG();
    std::vector<std::string> o = { "A", "I", "J", "X", "Y" };
    auto s = D.genOptimizedCode(o);
#endif

    std::string infilename = "quick_ext.json";
    std::string outfilename = "blkopt.json";
    std::string DAGfilename = "DAG.txt";
    if (argc == 3)
    {
        infilename  = std::string{ argv[1] };
        outfilename = std::string{ argv[2] };
    }

    std::ifstream jfile(infilename);
    std::ofstream jout(outfilename);
    std::ofstream DAGout(DAGfilename);

    json j;
    jfile >> j;
    size_t total = j["summary"]["total_blocks"];


    for (size_t i = 0; i < total; ++i)
    {
        std::vector<std::string> codes = j["blocks"][std::to_string(i)]["code"], out = j["blocks"][std::to_string(i)]["out"];
        std::vector<std::string> activeVars;

        for (auto&& var : out)
            activeVars.emplace_back(strip(strip(var, '"'), ' '));

        DAG D;

        for (auto&& code : codes)
        {
            QuadExp E = convert(strip(strip(code, '"'), ' '));
            D.readQuad(E);
        }

        std::vector<QuadExp> optcd = D.genOptimizedCode(activeVars);
        j["blocks"][std::to_string(i)]["code"].clear();

        for (size_t k = 0; k < optcd.size(); ++k)
            j["blocks"][std::to_string(i)]["code"][k] = convert2tri(optcd[k]);


        DAGout << "BLOCK" << i << ": " << std::endl;
        DAGout << D.print_DAG();

        DAGout << "**************************************************" << std::endl << std::endl;
    }

    jout << j.dump(4);


    return 0;

}