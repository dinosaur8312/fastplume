#ifndef AGENTPROPERTIES_H
#define AGENTPROPERTIES_H

#include <iostream>
#include <vector>


namespace FastPlume {

struct agentProperty
{
    std::string m_AgentName;
    //double m_cutoffConcentration;
    double m_MolecularWeight;//
    double m_MolecularVolume;//
    double m_AntoineA;//
    double m_AntoineB;//
    double m_AntoineC;//
    double m_BoilingPoint;//
    double m_FreezingPoint;//
    //bool m_bAerosolPresentFlag;
    //double m_dSkinFactor;
};

class agentProperties {
public:
    // Agent parameters
    std::vector<std::string> m_AgentNames;
    //std::vector<double> m_cutoffConcentrations;
    std::vector<double> m_MolecularWeights;
    std::vector<double> m_MolecularVolumes;
    std::vector<double> m_AntoineAs;
    std::vector<double> m_AntoineBs;
    std::vector<double> m_AntoineCs;
    std::vector<double> m_BoilingPoints;
    std::vector<double> m_FreezingPoints;
    //std::vector<bool> m_bAerosolPresentFlags;
    //std::vector<double> m_dSkinFactors;

   //default constructor
    agentProperties() {}

    // Constructor to parse CSV and initialize data
    agentProperties(const std::string &filePath) {
        parseCSV(filePath);
    }

    void parseCSV(const std::string &filePath);
    
    // Method to print data
    void printData() const;

    agentProperty getAgentProperty(const std::string &agentName) const;

};




} // namespace FastPlume

#endif // AGENTPROPERTIES_H