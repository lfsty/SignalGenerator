#ifndef BDFHEADER_H
#define BDFHEADER_H

#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <assert.h>
#include <exception>

struct BDF_INFO
{
    int srate = 0;
};

// http://www.biosemi.com/faq/file_format.htm
enum BDFFileMode
{
    IN,
    OUT
};
class BDFHeader
{
private:
    struct _bdf_header
    {
        char m_first_byte = 255;
        char m_id_code[7] = { 0 };
        char m_subject_id[80] = { 0 };
        char m_record_id[80] = { 0 };
        char m_start_date[8] = { 0 };
        char m_start_time[8] = { 0 };
        char m_num_byte_header_record[8] = { 0 };
        char m_version_data[44] = { 0 };
        char m_num_data_record[8] = { 0 };
        char m_duration_record_s[8] = { 0 };
        char m_num_channels[4] = { 0 };
        // N*16
        char* m_ch_labels = nullptr;
        // N*80
        char* m_transducer_type = nullptr;
        // N*8
        char* m_physical_dimension = nullptr;
        // N*8
        char* m_physical_minimum = nullptr;
        // N*8
        char* m_physical_maximum = nullptr;
        // N*8
        char* m_digital_minimum = nullptr;
        // N*8
        char* m_digital_maximum = nullptr;
        // N*80
        char* m_prefiltering = nullptr;
        // N*8
        char* m_num_samples_in_each_record = nullptr;
        // N*32
        char* m_Reserved = nullptr;
    };

public:
    BDFHeader(const BDFFileMode& file_mode, const std::string& file_path = "");
    ~BDFHeader();
    int OpenBDFFile(const std::string& file_path);
    int InitOutBDFHeader(const std::string& file_path);
    int WriteBDFHeader(
        const std::string& SubjectIdentification,
        const std::string& RecordingIdentification,
        const unsigned int& NumberOfChannels,
        const std::vector<std::string>& LabelsOfChannels,
        const std::vector<std::string>& TransducerType,
        const std::vector<std::string>& PhysicalDimension,
        const std::vector<int>& PhysicalMinimum,
        const std::vector<int>& PhysicalMaximum,
        const std::vector<int>& DigitalMinimum,
        const std::vector<int>& DigitalMaximum,
        const std::vector<std::string>& Prefiltering,
        const std::vector<int>& NumberOfSamplesInEachDataRecord
    );
    int FinishBDFDataWrite(const unsigned int& NumberOfDataRecord);
    std::string GetFileName();
public:
    const std::string GetSubjectIdentification();
    const std::string GetRecordingIdentification();
    const std::string GetStartDateOfRecording();
    const std::string GetStartTimeOfRecording();
    const unsigned int GetNumberOfBytesInHeaderRecord();
    const unsigned int GetNumberOfDataRecord();
    const unsigned int GetDurationOfDataRecordInSeconds();
    const unsigned int GetNumberOfChannels();

    const std::vector<std::string> GetLabelsOfChannels();
    const std::vector<std::string> GetTransducerType();
    const std::vector<std::string> GetPhysicalDimension();
    const std::vector<int> GetPhysicalMinimum();
    const std::vector<int> GetPhysicalMaximum();
    const std::vector<int> GetDigitalMinimum();
    const std::vector<int> GetDigitalMaximum();
    const std::vector<std::string> GetPrefiltering();
    const std::vector<int> GetNumberOfSamplesInEachDataRecord();
private:
    void SafeDel(void* p);
    void ClearBDFHeader();
    int _WriteBDFHeader(const std::string& file_path);
    void _itoa_appendspace(char* data, const int& origin_dig, const int& length);
private:
    BDFFileMode m_file_mode;
    std::string m_file_path;
    _bdf_header* m_bdf_header = nullptr;
    int m_number_ch = 0;
    std::fstream* m_fs = nullptr;
};

#endif
