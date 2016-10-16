#include <string>
#include <vector>
#include "QueryProcessor/QueryProjector.h"

QueryProjector::QueryProjector() {}

QueryProjector::~QueryProjector() {}

//ResultList type std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>

void QueryProjector::projectResult(std::list<std::string>& results, std::unordered_map<std::string, bool> resultsInfo, ResultList resultList) {
    results.clear();

    std::vector<std::string> selectList = resultList.first;
    std::vector<std::vector<std::string>> tupleList = resultList.second;
    std::string result;

    if (selectList[0] == "BOOLEAN") {
        result = tupleList[0][0];
        for (int i = 0; i < result.length(); i++) {
            result[i] = toupper(result[i]);
        }
        results.emplace_back(result);
    }
    else {
        for (std::vector<std::string> tuple : tupleList) {
            result = "";
            
            //Check for call.procName in Selected synonyms and convert to procName if found
            for (int i = 0; i < tuple.size(); i++) {
                if (resultsInfo.find(selectList[i])->second == true) {
                    result += PKB::GetCallProcedureName(stoi(tuple[i])) + " ";
                }
                else {
                    result += tuple[i] + " ";
                }
            }

            results.emplace_back(result.substr(0, result.length() - 1));
        }
        
    }
}