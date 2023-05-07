#include "ReadNeuracleData.h"
#include "BDFHeader.h"
#include <fstream>
const std::vector<std::vector<double>> ReadNeuracleData::ReadEEGData(const std::string& file_path)
{
	BDFHeader* _bdf_header = new BDFHeader(BDFFileMode::IN, file_path);
	unsigned int _num_header_bytes = _bdf_header->GetNumberOfBytesInHeaderRecord();
	unsigned int _num_data_records = _bdf_header->GetNumberOfDataRecord();
	unsigned int _num_channels = _bdf_header->GetNumberOfChannels();
	const std::vector<int> _PhysicalMinimum = _bdf_header->GetPhysicalMinimum();
	const std::vector<int> _PhysicalMaximum = _bdf_header->GetPhysicalMaximum();
	const std::vector<int> _DigitalMinimum = _bdf_header->GetDigitalMinimum();
	const std::vector<int> _DigitalMaximum = _bdf_header->GetDigitalMaximum();
	const std::vector<int> _NumberOfSamplesInEachDataRecord = _bdf_header->GetNumberOfSamplesInEachDataRecord();
	delete _bdf_header;

	std::vector<double> _scale_factor;
	std::vector<double> _offset;
	for (int i = 0; i < _num_channels; i++) {
		double phys_min = _PhysicalMinimum.at(i);
		double phys_max = _PhysicalMaximum.at(i);
		double dig_min = _DigitalMinimum.at(i);
		double dig_max = _DigitalMaximum.at(i);

		_scale_factor.push_back((phys_max - phys_min) / (dig_max - dig_min));
		_offset.push_back(phys_max - _scale_factor.at(i) * dig_max);
	}

	static auto convert_sample_to_phys_value = [](char* sample_bytes, double offset, double scale_factor) {
		//小端
		int sample_value = (sample_bytes[2] << 16 & 0x00ff0000) | (sample_bytes[1] << 8 & 0x0000ff00) | (sample_bytes[0] & 0x000000ff);
		//判断正负
		if (sample_value & 0x00800000)
		{
			sample_value |= 0xFF000000;
		}
		return offset + scale_factor * double(sample_value);
	};

	std::vector<std::vector<double>> ret_data;
	ret_data.resize(_num_channels);
	for (int i = 0; i < ret_data.size(); i++) {
		ret_data[i].resize(_num_data_records * _NumberOfSamplesInEachDataRecord.at(i));
	}

	std::ifstream _ifs(file_path, std::ios::binary);
	_ifs.seekg(_num_header_bytes, std::ios::beg);
	char* _buf = new char[3 * _NumberOfSamplesInEachDataRecord.at(0)];

	for (int i = 0; i < _num_data_records; i++) {
		for (int j = 0; j < _num_channels; j++) {
			_ifs.read(_buf, 3 * _NumberOfSamplesInEachDataRecord.at(j));
			for (int k = 0; k < _NumberOfSamplesInEachDataRecord.at(j); k++) {
				ret_data[j][i * _NumberOfSamplesInEachDataRecord.at(j) + k] = convert_sample_to_phys_value(_buf + 3 * k, _offset.at(j), _scale_factor.at(j));
			}
		}
	}

	delete[] _buf;

	return ret_data;
}
const std::vector<AnnotationsInfo> ReadNeuracleData::ReadAnnotationData(const std::string& file_path)
{
	BDFHeader* _bdf_header = new BDFHeader(BDFFileMode::IN, file_path);
	unsigned int _num_header_bytes = _bdf_header->GetNumberOfBytesInHeaderRecord();
	unsigned int _num_annotations = _bdf_header->GetNumberOfDataRecord();
	std::vector<int> _NumberOfSamplesInEachDataRecord = _bdf_header->GetNumberOfSamplesInEachDataRecord();
	delete _bdf_header;

	std::ifstream _ifs(file_path, std::ios::binary);
	_ifs.seekg(_num_header_bytes, std::ios::beg);
	char* _buf = new char[3 * _NumberOfSamplesInEachDataRecord.at(1)];

	std::vector<AnnotationsInfo> ret_data;
	for (unsigned int i = 0; i < _num_annotations; i++) {
		_ifs.seekg(3 * _NumberOfSamplesInEachDataRecord.at(0), _ifs.cur);
		_ifs.read(_buf, 3 * _NumberOfSamplesInEachDataRecord.at(1));
		char* p = _buf;
		//格式化字符串
		for (int i = 0; i < 3 * _NumberOfSamplesInEachDataRecord.at(1); i++) {
			if (*(p + i) == 0x15 || *(p + i) == 0x14)
				*(p + i) = 0x00;
		}

		AnnotationsInfo _tmp_info;
		//index
		_tmp_info.index = atoi(p);
		p += strlen(p) + 3;
		//time_stamp
		_tmp_info.time_stamp = int(atof(p) * 1000);
		p += strlen(p) + 1;
		//duration
		_tmp_info.duration = atoi(p);
		p += strlen(p) + 1;
		//category
		_tmp_info.category = atoi(p);
		ret_data.push_back(_tmp_info);
	}
	delete[] _buf;

	return ret_data;
}
