#include "agentProperties.h"
#include "stringUtil.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace FastPlume
{

    void agentProperties::printData() const
    {
        std::cout << "Agent Properties:\n";
        for(int i=0; i<m_AgentNames.size(); i++)
        {
            std::cout << "Agent: " << m_AgentNames[i] << "\n";
            //std::cout << "  Cutoff Concentration: " << m_cutoffConcentrations[i] << "\n";
            std::cout << "  Molecular Weight: " << m_MolecularWeights[i] << "\n";
            std::cout << "  Molecular Volume: " << m_MolecularVolumes[i] << "\n";
            std::cout << "  Antoine A: " << m_AntoineAs[i] << "\n";
            std::cout << "  Antoine B: " << m_AntoineBs[i] << "\n";
            std::cout << "  Antoine C: " << m_AntoineCs[i] << "\n";
            std::cout << "  Boiling Point: " << m_BoilingPoints[i] << "\n";
            std::cout << "  Freezing Point: " << m_FreezingPoints[i] << "\n";
            //std::cout << "  Aerosol Present: " << m_bAerosolPresentFlags[i] << "\n";
            //std::cout << "  Skin Factor: " << m_dSkinFactors[i] << "\n";
            std::cout << "----------------\n";
        }
    }

    agentProperty agentProperties::getAgentProperty(const std::string &agentName) const
    {
        auto it = std::find(m_AgentNames.begin(), m_AgentNames.end(), agentName);
        if (it == m_AgentNames.end())
        {
            throw std::runtime_error("Agent not found: " + agentName);
        }
        int index = std::distance(m_AgentNames.begin(), it);
        //printf("agentName = %s\n", agentName.c_str());
        //printf("index = %d\n", index);
        return agentProperty{m_AgentNames[index], m_MolecularWeights[index], m_MolecularVolumes[index], m_AntoineAs[index], m_AntoineBs[index], m_AntoineCs[index], m_BoilingPoints[index], m_FreezingPoints[index]};
    }

    void agentProperties::parseCSV(const std::string &filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filePath);
        }

        std::string line;
        std::getline(file, line); // Skip header line
        line = trim(line);  
        // Identify column positions for required columns
        std::unordered_map<std::string, int> column_map;
        std::istringstream headerStream(line);
         std::string column;
        int columnIndex = 0;

        //std::cout << "Reading agent properties header line:" << line << std::endl;
        while (std::getline(headerStream, column, ','))
        {

           // std::cout << "Column: " << column << " at index: " << columnIndex << std::endl;


            column_map[column] = columnIndex++;
        }

        // Check if all required columns are present
        const std::vector<std::string> requiredColumns = {"name","molecular_weight",
                       "molecular_volume",
                       "boiling_point",
                       "freezing_point",
                       "antoine_a",
                       "antoine_b",
                       "antoine_c"};
        for (const auto &required : requiredColumns)
        {
            if (column_map.find(required) == column_map.end())
            {
                throw std::runtime_error("Missing required column: " + required);
            }
        }

        // Read data lines
        while(std::getline(file, line))
        {
            line = trim(line);
            line = removeCommasInsideQuotes(line);
            std::istringstream ss(line);
            std::string value;

            //std::cout << "Reading agent properties line:" << line << std::endl;
           // Parse line based on column positions
            int currentColumn = 0;
            while (std::getline(ss, value, ','))
            {
                value = trim(value);
                //std::cout << "Value: " << value << " at column: " << currentColumn << std::endl;
                if (currentColumn == column_map["name"])
                {
                    m_AgentNames.push_back(value);
                }
                else if (currentColumn == column_map["molecular_weight"])
                {
                    m_MolecularWeights.push_back(safeConvertToDouble(value));
                }
                else if (currentColumn == column_map["molecular_volume"])
                {
                    m_MolecularVolumes.push_back(safeConvertToDouble(value));
                }
                else if (currentColumn == column_map["antoine_a"])
                {
                    m_AntoineAs.push_back(safeConvertToDouble(value));
                }
                else if (currentColumn == column_map["antoine_b"])
                {
                    m_AntoineBs.push_back(safeConvertToDouble(value));
                }
                else if (currentColumn == column_map["antoine_c"])
                {
                    m_AntoineCs.push_back(safeConvertToDouble(value));
                }
                else if (currentColumn == column_map["boiling_point"])
                {
                    m_BoilingPoints.push_back(safeConvertToDouble(value));
                }
                else if (currentColumn == column_map["freezing_point"])
                {
                    m_FreezingPoints.push_back(safeConvertToDouble(value));
                }
                currentColumn++;
            }
        }
        file.close();
    }

}