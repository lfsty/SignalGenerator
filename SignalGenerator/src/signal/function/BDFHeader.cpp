#include "BDFHeader.h"
BDFHeader::BDFHeader(const BDFFileMode& file_mode, const std::string& file_path) :
    m_file_mode(file_mode),
    m_file_path(file_path)
{
    m_fs = new std::fstream();
    switch (file_mode)
    {
        case BDFFileMode::IN:
            {
                if (!file_path.empty())
                    OpenBDFFile(file_path);
                break;
            }
        case BDFFileMode::OUT:
            {
                if (!file_path.empty())
                    InitOutBDFHeader(file_path);
                break;
            }
    }

}

BDFHeader::~BDFHeader()
{
    ClearBDFHeader();
    delete m_fs;
}

int BDFHeader::OpenBDFFile(const std::string& file_path)
{
    if (m_file_mode != BDFFileMode::IN)
        return -10;
    m_fs->open(file_path, std::ios::in | std::ios::binary);
    if (!m_fs->is_open())
        return -1;

    m_fs->seekg(0, std::ios::end);
    int _file_size = m_fs->tellg();
    m_fs->seekg(0, std::ios::beg);

    if (_file_size < 8 + 80 + 80 + 8 + 8 + 8)
    {
        m_fs->close();
        return -2;
    }

    // read number of header bytes
    m_fs->seekg(8 + 80 + 80 + 8 + 8, std::ios::beg);
    char* _header_bytes = new char[9];
    m_fs->read(_header_bytes, 8);
    _header_bytes[8] = '\0';
    int _num_header_bytes = atoi(_header_bytes);
    delete[] _header_bytes;
    if (_file_size < _num_header_bytes)
    {
        m_fs->close();
        return -3;
    }
    m_fs->seekg(0, std::ios::beg);

    // read ch number
    m_fs->seekg(8 + 80 * 2 + 8 * 3 + 44 + 8 * 2, std::ios::beg);
    char* _num_ch_bytes = new char[5];
    m_fs->read(_num_ch_bytes, 4);
    _num_ch_bytes[4] = '\0';
    m_number_ch = atoi(_num_ch_bytes);
    m_fs->seekg(0, std::ios::beg);


    ClearBDFHeader();
    m_bdf_header = new _bdf_header;
    // alloc memory
    m_bdf_header->m_ch_labels = new char[m_number_ch * 16];
    m_bdf_header->m_transducer_type = new char[m_number_ch * 80];
    m_bdf_header->m_physical_dimension = new char[m_number_ch * 8];
    m_bdf_header->m_physical_minimum = new char[m_number_ch * 8];
    m_bdf_header->m_physical_maximum = new char[m_number_ch * 8];
    m_bdf_header->m_digital_minimum = new char[m_number_ch * 8];
    m_bdf_header->m_digital_maximum = new char[m_number_ch * 8];
    m_bdf_header->m_prefiltering = new char[m_number_ch * 80];
    m_bdf_header->m_num_samples_in_each_record = new char[m_number_ch * 8];
    m_bdf_header->m_Reserved = new char[m_number_ch * 32];

    m_fs->read(reinterpret_cast<char*>(m_bdf_header), 8 + 80 * 2 + 8 * 3 + 44 + 8 * 2 + 4);
    m_fs->read(m_bdf_header->m_ch_labels, m_number_ch * 16);
    m_fs->read(m_bdf_header->m_transducer_type, m_number_ch * 80);
    m_fs->read(m_bdf_header->m_physical_dimension, m_number_ch * 8);
    m_fs->read(m_bdf_header->m_physical_minimum, m_number_ch * 8);
    m_fs->read(m_bdf_header->m_physical_maximum, m_number_ch * 8);
    m_fs->read(m_bdf_header->m_digital_minimum, m_number_ch * 8);
    m_fs->read(m_bdf_header->m_digital_maximum, m_number_ch * 8);
    m_fs->read(m_bdf_header->m_prefiltering, m_number_ch * 80);
    m_fs->read(m_bdf_header->m_num_samples_in_each_record, m_number_ch * 8);
    m_fs->read(m_bdf_header->m_Reserved, m_number_ch * 32);

    m_fs->close();
}

int BDFHeader::InitOutBDFHeader(const std::string& file_path)
{
    if (m_file_mode != BDFFileMode::OUT)
        return -10;
    m_file_path = file_path;
    ClearBDFHeader();
    m_bdf_header = new _bdf_header;

    // Identification code
    memcpy(m_bdf_header->m_id_code, "BIOSEMI", 7);

    // time init
    time_t _now = time(0);
    tm* _ltm = new tm;
    time_t now = time(0);
    localtime_s(_ltm, &now);

    auto _formattime = [](int _data)
    {
        std::string _tmp_str = std::to_string(_data);
        return std::string(2 - _tmp_str.length(), '0') + _tmp_str;
    };
    std::string _date = _formattime(_ltm->tm_mday) + "." + _formattime(1 + _ltm->tm_mon) + "." + _formattime(_ltm->tm_year % 100);
    std::string _time = _formattime(_ltm->tm_hour) + "." + _formattime(_ltm->tm_min) + "." + _formattime(_ltm->tm_sec);
    delete _ltm;
    memcpy(m_bdf_header->m_start_date, _date.data(), _date.size());
    memcpy(m_bdf_header->m_start_time, _time.data(), _time.size());

    //Version of data format
    memset(m_bdf_header->m_version_data, 32, 44);
    memcpy(m_bdf_header->m_version_data, "24BIT", 5);
    //Number of data records "-1" if unknown
    memset(m_bdf_header->m_num_data_record, 32, 8);
    memcpy(m_bdf_header->m_num_data_record, "-1", 2);
    //Duration of a data record, in seconds
    memset(m_bdf_header->m_duration_record_s, 32, 8);
    memcpy(m_bdf_header->m_duration_record_s, "1", 1);

    return _WriteBDFHeader(m_file_path);
}

int BDFHeader::WriteBDFHeader(
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
    const std::vector<int>& NumberOfSamplesInEachDataRecord)
{
    if (m_file_mode != BDFFileMode::OUT)
        return -10;
    if (m_bdf_header == nullptr)
        throw std::runtime_error("call InitOutBDFHeader first");
    if (SubjectIdentification.size() > 80)
        throw std::invalid_argument("SubjectIdentification size should <= 80");
    if (RecordingIdentification.size() > 80)
        throw std::invalid_argument("RecordingIdentification size should <= 80");
    if (NumberOfChannels > 9999)
        throw std::invalid_argument("NumberOfChannels size should <= 9999");
    if (LabelsOfChannels.size() != NumberOfChannels)
        throw std::invalid_argument("LabelsOfChannels size should == NumberOfChannels");
    if (TransducerType.size() != NumberOfChannels)
        throw std::invalid_argument("TransducerType size should == NumberOfChannels");
    if (PhysicalDimension.size() != NumberOfChannels)
        throw std::invalid_argument("PhysicalDimension size should == NumberOfChannels");
    if (PhysicalMinimum.size() != NumberOfChannels)
        throw std::invalid_argument("PhysicalMinimum size should == NumberOfChannels");
    if (PhysicalMaximum.size() != NumberOfChannels)
        throw std::invalid_argument("PhysicalMaximum size should == NumberOfChannels");
    if (DigitalMinimum.size() != NumberOfChannels)
        throw std::invalid_argument("DigitalMinimum size should == NumberOfChannels");
    if (DigitalMaximum.size() != NumberOfChannels)
        throw std::invalid_argument("DigitalMaximum size should == NumberOfChannels");
    if (Prefiltering.size() != NumberOfChannels)
        throw std::invalid_argument("Prefiltering size should == NumberOfChannels");
    if (NumberOfSamplesInEachDataRecord.size() != NumberOfChannels)
        throw std::invalid_argument("NumberOfSamplesInEachDataRecord size should == NumberOfChannels");
    m_number_ch = NumberOfChannels;

    //static const auto _atoi_appendspace = [](char* data, int origin_dig, int length) {
    //	char* _buffer = new char[length + 1];
    //	int _tmp_len = sprintf_s(_buffer, length + 1, "%d", origin_dig);
    //	if (_tmp_len < length)
    //		for (int i = _tmp_len; i < length; i++)
    //			_buffer[i] = ' ';
    //	memcpy(data, _buffer, length);
    //	delete[] _buffer;
    //};

    char _tmp[9];
    int _tmp_len = 0;
    _itoa_appendspace(m_bdf_header->m_num_byte_header_record, 8 + 80 * 2 + 8 * 3 + 44 + 8 * 2 + 4 + m_number_ch * (16 + 80 + 8 * 5 + 80 + 8 + 32), 8);

    memset(m_bdf_header->m_subject_id, 32, 80);
    memcpy(m_bdf_header->m_subject_id, SubjectIdentification.data(), SubjectIdentification.size());
    memset(m_bdf_header->m_record_id, 32, 80);
    memcpy(m_bdf_header->m_record_id, RecordingIdentification.data(), RecordingIdentification.size());

    _itoa_appendspace(m_bdf_header->m_num_channels, NumberOfChannels, 4);

    {
        m_bdf_header->m_ch_labels = new char[m_number_ch * 16];
        memset(m_bdf_header->m_ch_labels, 32, m_number_ch * 16);
        m_bdf_header->m_transducer_type = new char[m_number_ch * 80];
        memset(m_bdf_header->m_transducer_type, 32, m_number_ch * 80);
        m_bdf_header->m_physical_dimension = new char[m_number_ch * 8];
        memset(m_bdf_header->m_physical_dimension, 32, m_number_ch * 8);
        m_bdf_header->m_physical_minimum = new char[m_number_ch * 8];
        memset(m_bdf_header->m_physical_minimum, 32, m_number_ch * 8);
        m_bdf_header->m_physical_maximum = new char[m_number_ch * 8];
        memset(m_bdf_header->m_physical_maximum, 32, m_number_ch * 8);
        m_bdf_header->m_digital_minimum = new char[m_number_ch * 8];
        memset(m_bdf_header->m_digital_minimum, 32, m_number_ch * 8);
        m_bdf_header->m_digital_maximum = new char[m_number_ch * 8];
        memset(m_bdf_header->m_digital_maximum, 32, m_number_ch * 8);
        m_bdf_header->m_prefiltering = new char[m_number_ch * 80];
        memset(m_bdf_header->m_prefiltering, 32, m_number_ch * 80);
        m_bdf_header->m_num_samples_in_each_record = new char[m_number_ch * 8];
        memset(m_bdf_header->m_num_samples_in_each_record, 32, m_number_ch * 8);
        m_bdf_header->m_Reserved = new char[m_number_ch * 32];
        memset(m_bdf_header->m_Reserved, 32, m_number_ch * 32);
    }

    for (int i = 0; i < NumberOfChannels; i++)
    {
        memcpy(m_bdf_header->m_ch_labels + 16 * i, LabelsOfChannels.at(i).data(), LabelsOfChannels.at(i).size());
        memcpy(m_bdf_header->m_transducer_type + 80 * i, TransducerType.at(i).data(), LabelsOfChannels.at(i).size());
        memcpy(m_bdf_header->m_physical_dimension + 8 * i, PhysicalDimension.at(i).data(), PhysicalDimension.at(i).size());

        _itoa_appendspace(m_bdf_header->m_physical_minimum + 8 * i, PhysicalMinimum.at(i), 8);
        _itoa_appendspace(m_bdf_header->m_physical_maximum + 8 * i, PhysicalMaximum.at(i), 8);

        _itoa_appendspace(m_bdf_header->m_digital_minimum + 8 * i, DigitalMinimum.at(i), 8);
        _itoa_appendspace(m_bdf_header->m_digital_maximum + 8 * i, DigitalMaximum.at(i), 8);

        memcpy(m_bdf_header->m_prefiltering + 80 * i, Prefiltering.at(i).data(), Prefiltering.at(i).size());

        _itoa_appendspace(m_bdf_header->m_num_samples_in_each_record + 8 * i, NumberOfSamplesInEachDataRecord.at(i), 8);
    }


    return _WriteBDFHeader(m_file_path);
}

int BDFHeader::FinishBDFDataWrite(const unsigned int& NumberOfDataRecord)
{
    if (m_file_mode != BDFFileMode::OUT)
        return -10;

    m_fs->open(m_file_path, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_fs->is_open())
        return -1;

    char _pchar_NumberOfDataRecord[8];
    _itoa_appendspace(_pchar_NumberOfDataRecord, NumberOfDataRecord, 8);
    m_fs->seekp(8 + 80 * 2 + 8 * 3 + 44, std::ios::beg);
    m_fs->write(_pchar_NumberOfDataRecord, 8);
    m_fs->close();

    return 0;
}

std::string BDFHeader::GetFileName()
{
    return m_file_path;
}

const std::string BDFHeader::GetSubjectIdentification()
{
    assert(m_bdf_header != nullptr);
    std::string _SubjectIdentification;
    for (int i = 0; i < 80; i++)
    {
        _SubjectIdentification.push_back(*(m_bdf_header->m_subject_id + i));
    }

    return _SubjectIdentification;
}

const std::string BDFHeader::GetRecordingIdentification()
{
    assert(m_bdf_header != nullptr);
    std::string _RecordingIdentification;
    for (int i = 0; i < 80; i++)
    {
        _RecordingIdentification.push_back(*(m_bdf_header->m_record_id + i));
    }

    return _RecordingIdentification;
}

const std::string BDFHeader::GetStartDateOfRecording()
{
    assert(m_bdf_header != nullptr);
    std::string _StartDateOfRecording;
    for (int i = 0; i < 8; i++)
    {
        _StartDateOfRecording.push_back(*(m_bdf_header->m_start_date + i));
    }

    return _StartDateOfRecording;
}

const std::string BDFHeader::GetStartTimeOfRecording()
{
    assert(m_bdf_header != nullptr);
    std::string _StartTimeOfRecording;
    for (int i = 0; i < 8; i++)
    {
        _StartTimeOfRecording.push_back(*(m_bdf_header->m_start_time + i));
    }

    return _StartTimeOfRecording;
}

const unsigned int BDFHeader::GetNumberOfBytesInHeaderRecord()
{
    assert(m_bdf_header != nullptr);
    char _tmp[9] = { 0 };
    memcpy(_tmp, m_bdf_header->m_num_byte_header_record, 8);
    return atoi(_tmp);
}

const unsigned int BDFHeader::GetNumberOfDataRecord()
{
    assert(m_bdf_header != nullptr);
    char _tmp[9] = { 0 };
    memcpy(_tmp, m_bdf_header->m_num_data_record, 8);
    return atoi(_tmp);
}

const unsigned int BDFHeader::GetDurationOfDataRecordInSeconds()
{
    assert(m_bdf_header != nullptr);
    char _tmp[9] = { 0 };
    memcpy(_tmp, m_bdf_header->m_duration_record_s, 8);
    return atoi(_tmp);
}

const unsigned int BDFHeader::GetNumberOfChannels()
{
    return m_number_ch;
}

const std::vector<std::string> BDFHeader::GetLabelsOfChannels()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_ch_labels != nullptr);
    std::vector<std::string> _LabelsOfChannels;
    static const int _num_bytes = 16;
    for (int i = 0; i < m_number_ch; i++)
    {
        std::string _ch_label;
        for (int j = 0; j < _num_bytes; j++)
        {
            _ch_label.push_back(*(m_bdf_header->m_ch_labels + i * _num_bytes + j));
        }
        _LabelsOfChannels.push_back(_ch_label);
    }

    return _LabelsOfChannels;
}

const std::vector<std::string> BDFHeader::GetTransducerType()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_transducer_type != nullptr);
    std::vector<std::string> _TransducerType;
    static const int _num_bytes = 80;
    for (int i = 0; i < m_number_ch; i++)
    {
        std::string _ch_transducer_type;
        for (int j = 0; j < _num_bytes; j++)
        {
            _ch_transducer_type.push_back(*(m_bdf_header->m_transducer_type + i * _num_bytes + j));
        }
        _TransducerType.push_back(_ch_transducer_type);
    }
    return _TransducerType;
}

const std::vector<std::string> BDFHeader::GetPhysicalDimension()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_physical_dimension != nullptr);
    std::vector<std::string> _PhysicalDimension;
    static const int _num_bytes = 8;
    for (int i = 0; i < m_number_ch; i++)
    {
        std::string _ch_physical_dimension;
        for (int j = 0; j < _num_bytes; j++)
        {
            _ch_physical_dimension.push_back(*(m_bdf_header->m_physical_dimension + i * _num_bytes + j));
        }
        _PhysicalDimension.push_back(_ch_physical_dimension);
    }
    return _PhysicalDimension;
}

const std::vector<int> BDFHeader::GetPhysicalMinimum()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_physical_minimum != nullptr);
    std::vector<int> _PhysicalMinimum;
    static const int _num_bytes = 8;
    char _tmp_buf[_num_bytes + 1] = { 0 };
    for (int i = 0; i < m_number_ch; i++)
    {
        memcpy(_tmp_buf, m_bdf_header->m_physical_minimum + _num_bytes * i, _num_bytes);
        _PhysicalMinimum.push_back(atoi(_tmp_buf));
    }
    return _PhysicalMinimum;
}

const std::vector<int> BDFHeader::GetPhysicalMaximum()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_physical_maximum != nullptr);
    std::vector<int> _PhysicalMaximum;
    static const int _num_bytes = 8;
    char _tmp_buf[_num_bytes + 1] = { 0 };
    for (int i = 0; i < m_number_ch; i++)
    {
        memcpy(_tmp_buf, m_bdf_header->m_physical_maximum + _num_bytes * i, _num_bytes);
        _PhysicalMaximum.push_back(atoi(_tmp_buf));
    }
    return _PhysicalMaximum;
}

const std::vector<int> BDFHeader::GetDigitalMinimum()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_digital_minimum != nullptr);
    std::vector<int> _DigitalMinimum;
    static const int _num_bytes = 8;
    char _tmp_buf[_num_bytes + 1] = { 0 };
    for (int i = 0; i < m_number_ch; i++)
    {
        memcpy(_tmp_buf, m_bdf_header->m_digital_minimum + _num_bytes * i, _num_bytes);
        _DigitalMinimum.push_back(atoi(_tmp_buf));
    }
    return _DigitalMinimum;
}

const std::vector<int> BDFHeader::GetDigitalMaximum()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_digital_maximum != nullptr);
    std::vector<int> _DigitalMaximum;
    static const int _num_bytes = 8;
    char _tmp_buf[_num_bytes + 1] = { 0 };
    for (int i = 0; i < m_number_ch; i++)
    {
        memcpy(_tmp_buf, m_bdf_header->m_digital_maximum + _num_bytes * i, _num_bytes);
        _DigitalMaximum.push_back(atoi(_tmp_buf));
    }
    return _DigitalMaximum;
}

const std::vector<std::string> BDFHeader::GetPrefiltering()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_prefiltering != nullptr);
    std::vector<std::string> _Prefiltering;
    static const int _num_bytes = 80;
    for (int i = 0; i < m_number_ch; i++)
    {
        std::string _ch_prefiltering;
        for (int j = 0; j < _num_bytes; j++)
        {
            _ch_prefiltering.push_back(*(m_bdf_header->m_prefiltering + i * _num_bytes + j));
        }
        _Prefiltering.push_back(_ch_prefiltering);
    }
    return _Prefiltering;
}

const std::vector<int> BDFHeader::GetNumberOfSamplesInEachDataRecord()
{
    assert(m_bdf_header != nullptr);
    assert(m_bdf_header->m_num_samples_in_each_record != nullptr);
    std::vector<int> _NumberOfSamplesInEachDataRecord;
    static const int _num_bytes = 8;
    char _tmp_buf[_num_bytes + 1] = { 0 };
    for (int i = 0; i < m_number_ch; i++)
    {
        memcpy(_tmp_buf, m_bdf_header->m_num_samples_in_each_record + _num_bytes * i, _num_bytes);
        _NumberOfSamplesInEachDataRecord.push_back(atoi(_tmp_buf));
    }
    return _NumberOfSamplesInEachDataRecord;
}

void BDFHeader::SafeDel(void* p)
{
    if (p != nullptr)
    {
        delete[] p;
        p = nullptr;
    }
}

void BDFHeader::ClearBDFHeader()
{
    if (m_bdf_header != nullptr)
    {
        SafeDel(m_bdf_header->m_ch_labels);
        SafeDel(m_bdf_header->m_transducer_type);
        SafeDel(m_bdf_header->m_physical_dimension);
        SafeDel(m_bdf_header->m_physical_minimum);
        SafeDel(m_bdf_header->m_physical_maximum);
        SafeDel(m_bdf_header->m_digital_minimum);
        SafeDel(m_bdf_header->m_digital_maximum);
        SafeDel(m_bdf_header->m_prefiltering);
        SafeDel(m_bdf_header->m_num_samples_in_each_record);
        SafeDel(m_bdf_header->m_Reserved);
        delete m_bdf_header;
        m_bdf_header = nullptr;
    }
}

int BDFHeader::_WriteBDFHeader(const std::string& file_path)
{
    m_fs->open(file_path, std::ios::binary | std::ios::trunc | std::ios::out);
    if (!m_fs->is_open())
        return -1;

    assert(m_bdf_header != nullptr);

    m_fs->write(&(m_bdf_header->m_first_byte), 1);
    m_fs->write(m_bdf_header->m_id_code, 7);
    m_fs->write(m_bdf_header->m_subject_id, 80);
    m_fs->write(m_bdf_header->m_record_id, 80);
    m_fs->write(m_bdf_header->m_start_date, 8);
    m_fs->write(m_bdf_header->m_start_time, 8);
    m_fs->write(m_bdf_header->m_num_byte_header_record, 8);
    m_fs->write(m_bdf_header->m_version_data, 44);
    m_fs->write(m_bdf_header->m_num_data_record, 8);
    m_fs->write(m_bdf_header->m_duration_record_s, 8);
    m_fs->write(m_bdf_header->m_num_channels, 4);

    if (m_bdf_header->m_ch_labels != nullptr)
    {
        assert(m_bdf_header->m_transducer_type != nullptr);
        assert(m_bdf_header->m_physical_dimension != nullptr);
        assert(m_bdf_header->m_physical_minimum != nullptr);
        assert(m_bdf_header->m_physical_maximum != nullptr);
        assert(m_bdf_header->m_digital_minimum != nullptr);
        assert(m_bdf_header->m_digital_maximum != nullptr);
        assert(m_bdf_header->m_prefiltering != nullptr);
        assert(m_bdf_header->m_num_samples_in_each_record != nullptr);
        assert(m_bdf_header->m_Reserved != nullptr);

        m_fs->write(m_bdf_header->m_ch_labels, m_number_ch * 16);
        m_fs->write(m_bdf_header->m_transducer_type, m_number_ch * 80);
        m_fs->write(m_bdf_header->m_physical_dimension, m_number_ch * 8);
        m_fs->write(m_bdf_header->m_physical_minimum, m_number_ch * 8);
        m_fs->write(m_bdf_header->m_physical_maximum, m_number_ch * 8);
        m_fs->write(m_bdf_header->m_digital_minimum, m_number_ch * 8);
        m_fs->write(m_bdf_header->m_digital_maximum, m_number_ch * 8);
        m_fs->write(m_bdf_header->m_prefiltering, m_number_ch * 80);
        m_fs->write(m_bdf_header->m_num_samples_in_each_record, m_number_ch * 8);
        m_fs->write(m_bdf_header->m_Reserved, m_number_ch * 32);
    }
    m_fs->close();

}

void BDFHeader::_itoa_appendspace(char* data, const int& origin_dig, const int& length)
{
    char* _buffer = new char[length + 1];
    int _tmp_len = sprintf_s(_buffer, length + 1, "%d", origin_dig);
    if (_tmp_len < length)
        for (int i = _tmp_len; i < length; i++)
            _buffer[i] = ' ';
    memcpy(data, _buffer, length);
    delete[] _buffer;
}
