/*
 * TimeSeriesProcessor.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: Sourav Senapati
 */
//-------------------------------------------------------------------------------------------------
// Project Includes
//-------------------------------------------------------------------------------------------------
#include "LogProcessor.h"

//-------------------------------------------------------------------------------------------------
// Global Includes
//-------------------------------------------------------------------------------------------------
#include <exception>

namespace LogProcessorApplication {
//-------------------------------------------------------------------------------------------------
// Variable definitions
//-------------------------------------------------------------------------------------------------
#define OUTPUT_FILE_NAME    "output.xml"
#define LOG_ENTRY           std::cout << " "<< __FUNCTION__ << ": ENTRY" << std::endl   // Logger for method entry
#define LOG_EXIT            std::cout << " "<< __FUNCTION__ << ": EXIT" << std::endl    // Logger for method exit

// this makes parsing log data consistent, when an additional table is introduced to the log data at later point
static std::map<LogProcessor::AdsLogType,std::string> cWriters = {
        { LogProcessor::AdsLogType::AD_PLACEMENT,"sel" },
        { LogProcessor::AdsLogType::AD_DISPLAY,"cnt" }
};
/*
 *
 */
LogProcessor::Ptr LogProcessor::Create(int argc, char* argv[]) {
	LogProcessor::Ptr retVal = LogProcessor::Ptr( new LogProcessor(argc, argv));
  return retVal;
}

LogProcessor::LogProcessor(int argc, char* argv[])
: m_argc(argc)
, m_argv((const char**)argv)
, m_input_file_stream(0)
, m_output_file_stream()
, m_row_data_value()
, m_banner_revenue_store()
, m_unique_banner_list()
, m_placement_log_store()
, m_display_log_store() {
	std::cout << "LogProcessor created ..." << std::endl;
}

LogProcessor::~LogProcessor() {
	std::cout << "LogProcessor deleted ..." << std::endl;
}

bool LogProcessor::RunApp() {
    LOG_ENTRY;

    bool ret_value =  true;
    if( OpenLogFile()) {
        ParseLogData();
        if (! OpenOutputFile()) {
            ret_value = false;
        }
        if (! MakeRow()) {
            std::cout << "Error while collecting data. Aborting the application!" << std::endl;
        	ret_value = false;
        }

    }
    else {
    	ret_value = false;
    }
    LOG_EXIT;
    return ret_value;
}

//-------------------------------------------------------------------------------------------------
// Private implementation
//-------------------------------------------------------------------------------------------------
bool LogProcessor::OpenLogFile() {
    LOG_ENTRY;
    std::string given_file_name(m_argv[1]);
    m_input_file_stream.open(given_file_name.c_str(), std::ios::in);
    if(! (m_input_file_stream.good())) {
        std::cout << "   Given file '" << given_file_name << "' could not be opened !!" << std::endl;
        LOG_EXIT;
        return false;
    }
    LOG_EXIT;
    return true;
}

bool LogProcessor::OpenOutputFile() {
    LOG_ENTRY;
    std::string outputFileName = OUTPUT_FILE_NAME;
    m_output_file_stream.open(outputFileName,std::ios_base::trunc);
    if(!m_output_file_stream.good()) {
        std::cout << "   Failed to open " << outputFileName << " : " << std::endl;
        return false;
    }
    m_output_file_stream << " <Banners>" << std::endl;
    LOG_EXIT;
    return true;
}

void LogProcessor::CloseOutputFile() {
    LOG_ENTRY;
    m_output_file_stream << " </Banners>" << std::endl;
    m_output_file_stream.close();
    std::cout << "   The data has been written to '"<<OUTPUT_FILE_NAME<<"'"<< std::endl;
    LOG_EXIT;
}

void LogProcessor::ParseLogData() {
    LOG_ENTRY;
    m_unique_banner_list.clear();
    m_placement_log_store.clear();
    m_display_log_store.clear();

    std::string read_log_entry;
    std::cout << "   Parsing data from log file." << std::endl;
    while( std::getline(m_input_file_stream, read_log_entry)) {

        // the log consists of two sets of data (sel and cnt), storing them separately
        if (read_log_entry.find(cWriters[LogProcessor::AdsLogType::AD_PLACEMENT]) == 0){
            std::istringstream myline(read_log_entry);
            int csvItemCounter = 0;
            std::string csvItem;
            PlacementData sel_data; // container for placement data (sel data)
            while(std::getline(myline, csvItem, ',')) {
                switch(csvItemCounter)
                {
                    case 0: sel_data.id = csvItem.substr(csvItem.find(":") + 1); break;
                    case 1: sel_data.uid = csvItem; break ;
                    case 2: sel_data.banner = csvItem; m_unique_banner_list.insert(csvItem); break ;
                    case 3: sel_data.price = std::stod(csvItem); break ;
                }
                csvItemCounter++;
            }
            m_placement_log_store.push_back(sel_data);
        }

        if (read_log_entry.find(cWriters[LogProcessor::AdsLogType::AD_DISPLAY]) == 0){
            std::istringstream myline(read_log_entry);
            int csvItemCounter = 0;
            std::string csvItem;
            DisplayData cnt_data; // container for display data (cnt data)
            while(std::getline(myline, csvItem, ',')) {
                switch(csvItemCounter)
                {
                    case 0: cnt_data.id = csvItem.substr(csvItem.find(":") + 1); break;
                    case 1: cnt_data.uid = csvItem; break ;
                }
                csvItemCounter++;
            }
            m_display_log_store.push_back(cnt_data);
        }
    }

    LOG_EXIT;
    return;
}

bool LogProcessor::MakeRow() {
    LOG_ENTRY;

    try {
        m_row_data_value.clear();
        // under each banner, get the relevant data
        for (auto banner : m_unique_banner_list) {
            m_banner_revenue_store[banner] = 0;  // revenue for the corresponding banner
            for (auto sel_log_entry : m_placement_log_store) {
                if (sel_log_entry.banner == banner) {
                    if (m_row_data_value.count(sel_log_entry.banner)) {
                        // banner exists in sel table
                        // check for id, if id exists increment the counter for current banner
                        if (m_row_data_value[sel_log_entry.banner].count(sel_log_entry.id)) {
                            m_row_data_value[sel_log_entry.banner][sel_log_entry.id]++;
                        }
                        else {
                            // otherwise, store the new id under this banner
                            m_row_data_value[sel_log_entry.banner].insert(std::pair<std::string, int>(sel_log_entry.id, 1));
                        }
                    }
                    else {
                        m_row_data_value[sel_log_entry.banner][sel_log_entry.id] = 1;
                    }
                    // now comparing uid in cnt table for counting occurrence of new id
                    for (auto cnt_log_entry : m_display_log_store) {
                        if (! sel_log_entry.uid.compare(cnt_log_entry.uid)) {
                            // banner with corresponding uid
                            // check for id, if id exists increment the counter for current banner
                            if (m_row_data_value[sel_log_entry.banner].count(cnt_log_entry.id)) {
                                m_row_data_value[sel_log_entry.banner][cnt_log_entry.id]++;
                            }
                            else {
                                // otherwise, store the new id under this banner
                                m_row_data_value[sel_log_entry.banner].insert(std::pair<std::string, int>(cnt_log_entry.id, 1));
                            }
                            // update the price for displayed banner
                            if (cnt_log_entry.id == "1") {
                                m_banner_revenue_store[banner] += sel_log_entry.price;
                            }
                        }
                    }
                }
            }
        }
    }
    catch (const std::out_of_range& e)
    {
    	std::cerr << "Out of range error! : " << e.what() << std::endl;
    }
    catch(const std::exception& e) {
    	std::cerr << "Error caused while processesing log data :" << e.what();
        return false;
    }
    catch(...) {
        std::cerr << "Unknown error while processesing log data";
        return false;
    }

    WriteRow();
    LOG_EXIT;
    return true;
}

void LogProcessor::WriteRow() {
    LOG_ENTRY;
    // finally write data from container to the output file
    for(auto entry : m_row_data_value) {
        double revenue = 0;
        if (m_banner_revenue_store.count(entry.first)) revenue = m_banner_revenue_store[entry.first];
            m_output_file_stream
                << "        <Banner id=\""<< entry.first <<"\"  revenues=\"" << revenue <<"\">" << std::endl
                << "            <Events>" << std::endl ;
                for(auto ev : entry.second) {
                    m_output_file_stream	<< "                <Event id=\"" << ev.first <<"\">" << ev.second << "</event>" << std::endl;
                }
            m_output_file_stream
                <<"             </Events>" << std::endl
                << "        </Banner>" << std::endl;
    }
    CloseOutputFile();
    LOG_EXIT;
}
} // namespace LogProcessorApplication

//-------------------------------------------------------------------------------------------------
// main
//-------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if( argc < 2) {
        std::cout << "Missing file name as argument " << std::endl;
        std::cout << " Usage    : " << argv[0] << " filename" << std::endl;
        return 1;
    }
    // create the LogProcessorApplication::LogProcessor class using a factory method, that returns an object pointer
    LogProcessorApplication::LogProcessor::Ptr log_processor_app = LogProcessorApplication::LogProcessor::Create(argc, argv);
    if (! log_processor_app->RunApp()) {
        return 1;
    }
    return 0;
}

