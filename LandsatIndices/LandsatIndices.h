//***********************************************************************

#include <bitset>
#include <deque>
#include "Basic/UtilStd.h"
#include "geomatic/LandsatDataset2.h"

namespace WBSF
{
	
	typedef std::deque<std::deque<std::vector<__int16>>> OutputData;

	class CLandsatIndicesOption : public CBaseOptions
	{
	public:


		enum TFilePath		{ INPUT_FILE_PATH, OUTPUT_FILE_PATH, NB_FILE_PATH };


		CLandsatIndicesOption();
		virtual ERMsg ParseOption(int argc, char* argv[]);
		virtual ERMsg ProcessOption(int& i, int argc, char* argv[]);

		std::deque<Landsat2::TIndices> m_despike;
		
		std::bitset<Landsat2::NB_INDICES> m_indices;
		std::array<size_t, 2> m_scenes;
		bool m_bVirtual;
		size_t m_rings;
		std::vector<double> m_weight;
		std::string m_weight_str;
	};


	class CLandsatIndices
	{
	public:

		ERMsg Execute();

		std::string GetDescription() { return  std::string("LandsatIndices version ") + VERSION + " (" + __DATE__ + ")"; }

//		ERMsg OpenAll(Landsat2::CLandsatDataset& inputDS, CGDALDatasetEx& maskDS, CGDALDatasetEx& outputDS);
		//void ReadBlock(int xBlock, int yBlock, CBandsHolder& bandHolder);
		//void ProcessBlock(int xBlock, int yBlock, CBandsHolder& bandHolder, OutputData& outputData);
		//void WriteBlock(int xBlock, int yBlock, CBandsHolder& bandHolder, CGDALDatasetEx& outputDS, OutputData& outputData);
		//void CloseAll(CGDALDatasetEx& inputDS, CGDALDatasetEx& maskDS, CGDALDatasetEx& outputDS);
		ERMsg OpenAll(Landsat2::CLandsatDataset& inputDS, CGDALDatasetEx& maskDS, Landsat2::CLandsatDataset& outputDS);
		void ReadBlock(Landsat2::CLandsatDataset& inputDS, CGDALDatasetEx& validityDS, int xBlock, int yBlock, Landsat2::CLandsatWindow& bandHolder);
		void ProcessBlock(int xBlock, int yBlock, const Landsat2::CLandsatWindow& bandHolder, OutputData& outputData);
		void WriteBlock(int xBlock, int yBlock, CGDALDatasetEx& outputDS, CGDALDatasetEx& breaksDS, OutputData& outputData);
		void CloseAll(CGDALDatasetEx& inputDS, CGDALDatasetEx& maskDS, CGDALDatasetEx& outputDS, CGDALDatasetEx& breaksDS);

		ERMsg CreateVirtual(Landsat2::CLandsatDataset& inputDS);


		CLandsatIndicesOption m_options;

		static const char* VERSION;
		static const int NB_THREAD_PROCESS;
	};
}