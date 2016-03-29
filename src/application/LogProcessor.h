/*
 * TimeSeriesProcessor.h
 *
 *  Created on: Mar 13, 2016
 *      Author: Sourav Senapati
 */

#ifndef LOGPROCESSOR_H_
#define LOGPROCESSOR_H_
//-------------------------------------------------------------------------------------------------
// Project Includes
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Includes
//-------------------------------------------------------------------------------------------------
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

namespace LogProcessorApplication {

class LogProcessor {
public:
    typedef std::shared_ptr<LogProcessor> Ptr;
    typedef std::map<std::string, std::map<std::string, int>> RowDataValue;
    typedef std::map<std::string, double> BannerRevenue;

    enum class AdsLogType {
        AD_PLACEMENT,
        AD_DISPLAY
    };

    struct PlacementData {
        std::string id;
        std::string uid;
        std::string banner;
        double price;
    };

    struct DisplayData {
        std::string id;
        std::string uid;
    };

    /*! The factory method for creating LogProcessor class.
     *
     *  \param 	argc  The given number of command line arguments.
     *  \param 	argv  The given command line arguments.
	 *  \return Ptr   object pointer to the LogProcessor class.
     */
    static Ptr Create(int argc, char* argv[]);

    //! Dtor
    virtual ~LogProcessor();

    /*! The method used to run the application.
     */
    bool RunApp();

protected:
    //! Ctor
    LogProcessor(int argc, char* argv[]);

private:
    int                             m_argc;                 //!< Holds the number of command line arguments.
    const char**                    m_argv;                 //!< Holds the given command line arguments.
    std::ifstream                   m_input_file_stream;    //!< Input file stream for reading log file.
    std::ofstream                   m_output_file_stream;   //!< Output file stream.
    RowDataValue                    m_row_data_value;       //!< Container for key-value pair of banner-id and <event-id, occurrences of banner>.
    BannerRevenue                   m_banner_revenue_store; //!< Holds the key-value pair of banner and the revenue it has generated.
    std::set<std::string>           m_unique_banner_list;   //!< A unique list of banners in the log file.
    std::vector<PlacementData>      m_placement_log_store;  //!< Container that stores the data associated with banner-placement logs.
    std::vector<DisplayData>        m_display_log_store;    //!< Container that stores the data associated with banner-display logs.

    /*! Method used for opening data log file.
     *
     * \return true    success in opening log file.
     * \return false   failure in opening file
     */
    bool OpenLogFile();

    /*! Method used for opening the output XML file.
     *
     * \return true    success in output log file.
     * \return false   failure in opening output file
     */
    bool OpenOutputFile();

    /*! Method used for closing the output XML file.
     */
    void CloseOutputFile();

    /*! In this method the data from log file is parsed and
     *  data structures used to store the read values.
     */
    void ParseLogData();

    /*! Once the data is stored, this method arranges the data
     *  in a suitable way, making it available for output file.
     *  Also performs calculation for banner revenues.
     *
     *  \return true    success in setting log valu.s in data structure
     *  \return false	Fail to set data structure.
     */
    bool MakeRow();

    /*! This method is used to finally write the data into a XML
     *  file.
     */
    void WriteRow();
};
} // namespace LogProcessorApplication
#endif // LOGPROCESSOR_H_
