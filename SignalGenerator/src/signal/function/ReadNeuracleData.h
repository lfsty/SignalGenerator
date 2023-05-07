#ifndef READNEURACLEDATA_H
#define READNEURACLEDATA_H

#include <vector>
#include <string>
struct AnnotationsInfo
{
    unsigned int index = 0;
    unsigned long long time_stamp = 0;
    unsigned int duration = 0;
    unsigned int category = 0;
};
class ReadNeuracleData
{
private:
    ReadNeuracleData();
public:
    static const std::vector<std::vector<double>> ReadEEGData(const std::string& file_path);
    static const std::vector<AnnotationsInfo> ReadAnnotationData(const std::string& file_path);
};

#endif
