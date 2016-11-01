#include <string>
#include <vector>
#include <fstream>
#include "QueryProcessor/QueryProjector.h"

QueryProjector::QueryProjector() {}

QueryProjector::~QueryProjector() {}

void QueryProjector::projectResult(std::list<std::string>& results, std::unordered_map<std::string, bool> resultsInfo, ResultList resultList) {
    //Check resultList
    /*std::ofstream output;
    output.open("tests/result.txt");

    output << "selected synonyms \n" << "-------------" << "\n";
    for (std::string s : resultList.first) {
        output << s << " ";
    }
    output << "\n\n";
    output << "results \n" << "-------------" << "\n";
    for (auto tuple : resultList.second) {
        for (std::string s : tuple) {
            output << s << " ";
        }
        output << "\n";
    }
    output.close();*/

    results.clear();

    std::vector<std::string> selectList = resultList.first;
    std::vector<std::vector<std::string>> tupleList = resultList.second;
    std::string result;

    if (selectList[0] == SYMBOL_BOOLEAN) {
        result = tupleList[0][0];
        results.emplace_back(result);
    }
    else {
        std::set<int> positionToConvert;

        for (int i = 0; i < selectList.size(); i++) {
            if (resultsInfo.find(selectList[i])->second == true) {
                positionToConvert.insert(i);
            }
        }

        for (std::vector<std::string> tuple : tupleList) {
            result = "";

            for (int i = 0; i < tuple.size(); i++) {
                if (positionToConvert.find(i) != positionToConvert.end()) {
                    result += PKB::GetCallStmtProcedureName(stoi(tuple[i]), "");
                }
                else {
                    result += tuple[i];
                }
                if (i != tuple.size() - 1) {
                    result += " ";
                }
            }

            results.emplace_back(result);
        }

    }
}