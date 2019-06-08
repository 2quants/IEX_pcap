#include "iex_decoder.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <string>
#include <vector>
#include <iostream>

#if defined _MSC_VER
    #include <direct.h>
    #include <io.h> 
    #define access    _access_s
#elif defined __GNUC__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

void createDir(const std::string& dir) {
#if defined _MSC_VER
    _mkdir(dir.data());
#elif defined __GNUC__
    mkdir(dir.data(), 0777);
#endif
}

bool FileExists(const std::string &Filename)
{
    return access(Filename.c_str(), 0) == 0;
}


void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length()) {
        v.push_back(s.substr(pos1));
    }
}

//#define Need_AllMsgType_Data

#define Need_QuoteUpdate_Data
#define Need_TradeReport_Data
#define Need_OfficialPrice_Data
#define Need_TradeBreak_Data
#define Need_AuctionInformation_Data
#define Need_PriceLevelUpdate_Data
#define Need_SystemEventMessage_Data
#define Need_SecurityDirectoryMessage_Data
#define Need_SecurityEventMessage_Data
#define Need_TradingStatusMessage_Data
#define Need_OperationalHaltStatusMessage_Data
#define Need_ShortSalePriceTestStatusMessage_Data

const char szfilename_all_msgtype[]="export.all.msgtype.txt";

const char szfilename_quotesupdate[]="export.quotes_update.csv";
const char szfilename_tradereport[]="export.trade_report.csv";
const char szfilename_officialprice[]="export.official_price.csv";
const char szfilename_tradebreak[]="export.trade_break.csv";
const char szfilename_auctioninfo[]="export.auctioninfo.csv";
const char szfilename_pricelevelupdate[]="export.pricelevelupdate.csv";
const char szfilename_systemEventMessage[]="export.system_event.csv";
const char szfilename_securityDirectoryMessage[]="export.security_directory.csv"; 
const char szfilename_securityEventMessage[]="export.security_event.csv"; 
const char szfilename_tradingStatusMessage[]="export.trading_status.csv"; 
const char szfilename_operationalHaltStatusMessage[]="export.operational_halt_status.csv"; 
const char szfilename_shortSalePriceTestStatusMessage[]="export.shortsaleprice_test_status.csv"; 


std::string filename_all_msgtype;

std::string filename_quotesupdate;
std::string filename_tradereport;
std::string filename_officialprice;
std::string filename_tradebreak;
std::string filename_auctioninfo;
std::string filename_pricelevelupdate;
std::string filename_systemEventMessage;
std::string filename_securityDirectoryMessage;
std::string filename_securityEventMessage;
std::string filename_tradingStatusMessage;
std::string filename_operationalHaltStatusMessage;
std::string filename_shortSalePriceTestStatusMessage;

void make_file_name_with_date_path(std::string & strFilename, const std::string & date_as_path, const char* szfilename)
{
    strFilename= "./";
    strFilename+= date_as_path;
    strFilename+=+"/";
    strFilename+=szfilename;   
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        std::cout << "Usage: iex_pcap_decoder <input_pcap>" << std::endl;
        return -1;
    }

    std::string input_file(argv[1]);

    std::vector<std::string>  vector_paths;
    SplitString(input_file,vector_paths,"/");
    std::string filename=vector_paths[vector_paths.size()-1];

    std::vector<std::string>  vector_patterns;
    SplitString(filename,vector_patterns,"_");
    if (vector_patterns.size() <= 3) {
        std::cout << "file name not contain date part :" << filename << std::endl;
        return -2;
    }
    std::string date_as_path = vector_patterns[3];


    if (FileExists(date_as_path)) {
        std::cout << date_as_path << "  " << "first check dir exists" << std::endl;
    }
    else {
        std::cout << date_as_path << "  " << "dir need create" << std::endl;
        createDir(date_as_path);
        if (FileExists(date_as_path)) {
            std::cout << date_as_path << "  " << "after create dir exists" << std::endl;
        }
    }

    make_file_name_with_date_path(filename_all_msgtype,date_as_path,szfilename_all_msgtype);

    make_file_name_with_date_path(filename_quotesupdate,date_as_path,szfilename_quotesupdate);
    make_file_name_with_date_path(filename_tradereport,date_as_path,szfilename_tradereport);
    make_file_name_with_date_path(filename_officialprice,date_as_path,szfilename_officialprice);
    make_file_name_with_date_path(filename_tradebreak,date_as_path,szfilename_tradebreak);
    make_file_name_with_date_path(filename_auctioninfo,date_as_path,szfilename_auctioninfo);
    make_file_name_with_date_path(filename_pricelevelupdate,date_as_path,szfilename_pricelevelupdate);
    make_file_name_with_date_path(filename_systemEventMessage,date_as_path,szfilename_systemEventMessage);
    make_file_name_with_date_path(filename_securityDirectoryMessage,date_as_path,szfilename_securityDirectoryMessage);
    make_file_name_with_date_path(filename_securityEventMessage,date_as_path,szfilename_securityEventMessage);
    make_file_name_with_date_path(filename_tradingStatusMessage,date_as_path,szfilename_tradingStatusMessage);
    make_file_name_with_date_path(filename_operationalHaltStatusMessage,date_as_path,szfilename_operationalHaltStatusMessage);
    make_file_name_with_date_path(filename_shortSalePriceTestStatusMessage,date_as_path,szfilename_shortSalePriceTestStatusMessage);


#ifdef Need_AllMsgType_Data
    std::ofstream allmsgtype_out_stream;
    try {
        allmsgtype_out_stream.open(filename_all_msgtype);
    } 
    catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_all_msgtype << std::endl;
        return 1;
    }
#endif  

#ifdef Need_QuoteUpdate_Data
    // Open a file stream for writing output to csv.
    std::ofstream quoteupdate_out_stream;
    try {
        quoteupdate_out_stream.open(filename_quotesupdate);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_quotesupdate << std::endl;
        return 1;
    }

    quoteupdate_out_stream << "Timestamp,Symbol,BidSize,BidPrice,AskSize,AskPrice,bit7_A,bit6_P,Flags" << std::endl;
#endif

#ifdef Need_TradeReport_Data
    std::ofstream tradereport_out_stream;
    try {
        tradereport_out_stream.open(filename_tradereport);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradereport << std::endl;
        return 1;
    }

    tradereport_out_stream << "Timestamp,Symbol,Size,Price,TradeId,bit7_F,bit6_T,bit5_I,bit4_8,bit3_X,Flags" << std::endl;
#endif

#ifdef Need_OfficialPrice_Data
    std::ofstream officialprice_out_stream;
    try {
        officialprice_out_stream.open(filename_officialprice);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradereport << std::endl;
        return 1;
    }

    officialprice_out_stream << "Timestamp,Symbol,Type,Price" << std::endl;
#endif

#ifdef Need_TradeBreak_Data
    std::ofstream tradebreak_out_stream;
    try {
        tradebreak_out_stream.open(filename_tradebreak);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradebreak << std::endl;
        return 1;
    }

    tradebreak_out_stream << "Timestamp,Symbol,Size,Price,TradeId,bit7_F,bit6_T,bit5_I,bit4_8,bit3_X,Flags" << std::endl;
#endif


#ifdef Need_AuctionInformation_Data
    std::ofstream auctioninfo_out_stream;
    try {
        auctioninfo_out_stream.open(filename_auctioninfo);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradebreak << std::endl;
        return 1;
    }
    auctioninfo_out_stream<< "Timestamp,Symbol,AuctionType,PairedShares,ReferencePrice,IndicativeClear,ImbalanceShares,ImbalanceSide,ExtensionNumber,SchdAuctionTime,BookClearPrice,CollarRefPrice,LwrAuctionCollar,UprAuctionCollar" << std::endl;
#endif

#ifdef Need_PriceLevelUpdate_Data
    std::ofstream pricelevelupdate_out_stream;
    try {
        pricelevelupdate_out_stream.open(filename_pricelevelupdate);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradebreak << std::endl;
        return 1;
    }
    pricelevelupdate_out_stream<< "Timestamp,Symbol,Type,Flags,Size,Price" << std::endl;
  
#endif

#ifdef Need_SystemEventMessage_Data
    std::ofstream systemevent_out_stream;
    try {
        systemevent_out_stream.open(filename_systemEventMessage);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_systemEventMessage << std::endl;
        return 1;
    }
    systemevent_out_stream<< "Timestamp,SystemEvent" << std::endl;
#endif  

#ifdef Need_SecurityDirectoryMessage_Data
    std::ofstream securitydirectory_out_stream;
    try {
        securitydirectory_out_stream.open(filename_securityDirectoryMessage);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_securityDirectoryMessage<< std::endl;
        return 1;
    }
    securitydirectory_out_stream<<"Timestamp,Symbol,RoundLotSize,AdjustPOCPrice,LULDTier,bit7_T,bit6_W,bit5_E,Flags" << std::endl;

#endif


#ifdef Need_SecurityEventMessage_Data
    std::ofstream securityevent_out_stream;
    try {
        securityevent_out_stream.open(filename_securityEventMessage);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_securityEventMessage<< std::endl;
        return 1;
    }
    securityevent_out_stream <<"Timestamp,Symbol,Event" << std::endl;
#endif


#ifdef Need_TradingStatusMessage_Data
    std::ofstream tradingstatus_out_stream;
    try {
        tradingstatus_out_stream.open(filename_tradingStatusMessage);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradingStatusMessage<< std::endl;
        return 1;
    }
    tradingstatus_out_stream <<  "Timestamp,Symbol,Status,Reason" << std::endl;
#endif

#ifdef Need_OperationalHaltStatusMessage_Data
    std::ofstream operationalhaltstatus_out_stream;
    try {
        operationalhaltstatus_out_stream.open(filename_operationalHaltStatusMessage);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_operationalHaltStatusMessage << std::endl;
        return 1;
    }
    operationalhaltstatus_out_stream << "Timestamp,Symbol,OperationalHaltStatus" << std::endl;
#endif

#ifdef Need_ShortSalePriceTestStatusMessage_Data
    std::ofstream shortsalepriceteststatus_out_stream;
    try {
        shortsalepriceteststatus_out_stream.open(filename_shortSalePriceTestStatusMessage);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_shortSalePriceTestStatusMessage << std::endl;
        return 1;
    }
    shortsalepriceteststatus_out_stream << "Timestamp,Symbol,Status,Detail" << std::endl;
#endif

    IEXDecoder decoder;
    if (!decoder.OpenFileForDecoding(input_file)) {
        std::cout << "Failed to open file '" << input_file << "'." << std::endl;
        return 1;
    }

    // Get the first message from the pcap file.
    std::unique_ptr<IEXMessageBase> msg_ptr;
    auto ret_code = decoder.GetNextMessage(msg_ptr);

    // Main loop to loop through all messages.
    for (; ret_code == ReturnCode::Success; ret_code = decoder.GetNextMessage(msg_ptr)) {

#ifdef Need_AllMsgType_Data
        allmsgtype_out_stream << "0x" << std::hex << ((uint16_t)msg_ptr->GetMessageType()) << std::dec << std::endl;
#endif     

        // For quick message introspection:
        // msg_ptr->Print();
        // Uncommenting this will completely dominate your terminal with output.

        // There are many different message types. Here we just look for quote update (L1 tick).
#ifdef Need_QuoteUpdate_Data
        if (msg_ptr->GetMessageType() == MessageType::QuoteUpdate) {

            // Cast it to the derived type.
            auto quote_msg = dynamic_cast<QuoteUpdateMessage*>(msg_ptr.get());

            // Check the pointer and write all L1 ticks for ticker 'AMD' to file.
            //if (quote_msg && quote_msg->symbol == "AMD")
            if (quote_msg) {
                quoteupdate_out_stream << quote_msg->timestamp << ","
                    << quote_msg->symbol    << ","
                    << quote_msg->bid_size  << ","
                    << quote_msg->bid_price << ","
                    << quote_msg->ask_size  << ","
                    << quote_msg->ask_price << ","
                    << (uint16_t)((quote_msg->flags & 0x80)/0x80) << "," 
                    << (uint16_t)((quote_msg->flags & 0x40)/0x40) << "," 
                    << (uint16_t)quote_msg->flags << std::endl;
            }
        }
#endif


#ifdef Need_TradeReport_Data
        if (msg_ptr->GetMessageType() == MessageType::TradeReport) {
            auto report_msg = dynamic_cast<TradeReportMessage*>(msg_ptr.get());
            if (report_msg) {
                tradereport_out_stream 
                    << report_msg->timestamp << ","
                    << report_msg->symbol << ","
                    << report_msg->size << ","
                    << report_msg->price << ","
                    << report_msg->trade_id << ","
                    << (uint16_t)((report_msg->flags & 0x80)/0x80) << "," 
                    << (uint16_t)((report_msg->flags & 0x40)/0x40) << "," 
                    << (uint16_t)((report_msg->flags & 0x20)/0x20) << "," 
                    << (uint16_t)((report_msg->flags & 0x10)/0x10) << "," 
                    << (uint16_t)((report_msg->flags & 0x08)/0x08) << "," 
                    << (uint16_t)report_msg->flags << std::endl;
            }
        }
#endif

#ifdef Need_OfficialPrice_Data
        if (msg_ptr->GetMessageType() == MessageType::OfficialPrice) {
            auto price_msg = dynamic_cast<OfficialPriceMessage*>(msg_ptr.get());
            if (price_msg) {
                officialprice_out_stream 
                    << price_msg->timestamp << ","
                    << price_msg->symbol << ","
                    << ((uint8_t)price_msg->price_type) << ","
                    << price_msg->price << std::endl;
            }
        }
#endif


#ifdef Need_TradeBreak_Data
        if (msg_ptr->GetMessageType() == MessageType::TradeBreak) {
            auto break_msg = dynamic_cast<TradeReportMessage*>(msg_ptr.get());
            if (break_msg) {
                tradebreak_out_stream 
                    << break_msg->timestamp << ","
                    << break_msg->symbol << ","
                    << break_msg->size << ","
                    << break_msg->price << ","
                    << break_msg->trade_id << ","
                    << (uint16_t)((break_msg->flags & 0x80)/0x80) << "," 
                    << (uint16_t)((break_msg->flags & 0x40)/0x40) << "," 
                    << (uint16_t)((break_msg->flags & 0x20)/0x20) << "," 
                    << (uint16_t)((break_msg->flags & 0x10)/0x10) << "," 
                    << (uint16_t)((break_msg->flags & 0x08)/0x08) << "," 
                    << (uint16_t)break_msg->flags << std::endl;
            }
        }
#endif


#ifdef Need_AuctionInformation_Data
        if (msg_ptr->GetMessageType() == MessageType::AuctionInformation) {
            auto auctioninfo_msg = dynamic_cast<AuctionInformationMessage*>(msg_ptr.get());
            if (auctioninfo_msg) {
                auctioninfo_out_stream 
                    << auctioninfo_msg->timestamp << ","
                    << auctioninfo_msg->symbol << ","
                    << static_cast<char>(auctioninfo_msg->auction_type) << ","
                    << auctioninfo_msg->paired_shares << ","
                    << auctioninfo_msg->reference_price << ","
                    << auctioninfo_msg->indicative_clearing_price << ","
                    << auctioninfo_msg->imbalance_shares << ","
                    << static_cast<char>(auctioninfo_msg->imbalance_side) << ","
                    << auctioninfo_msg->extension_number << ","
                    << auctioninfo_msg->scheduled_auction_time << ","
                    << auctioninfo_msg->auction_book_clearing_price << ","
                    << auctioninfo_msg->collar_reference_price << ","
                    << auctioninfo_msg->lower_auction_collar << ","
                    << auctioninfo_msg->upper_auction_collar << std::endl;
            }
        }
#endif


#ifdef Need_PriceLevelUpdate_Data
        if (msg_ptr->GetMessageType() == MessageType::PriceLevelUpdateBuy
             || msg_ptr->GetMessageType() == MessageType::PriceLevelUpdateSell) {
            auto priceLevelUpdate_msg = dynamic_cast<PriceLevelUpdateMessage*>(msg_ptr.get());
            if (priceLevelUpdate_msg) {
                pricelevelupdate_out_stream 
                    << priceLevelUpdate_msg->timestamp << ","
                    << priceLevelUpdate_msg->symbol << ","
                    << ((uint16_t)msg_ptr->GetMessageType()) << ","
                    << ((uint16_t)priceLevelUpdate_msg->flags) << ","
                    << priceLevelUpdate_msg->size << ","
                    << priceLevelUpdate_msg->price << std::endl;
            }
        }
#endif

#ifdef Need_SystemEventMessage_Data
        if (msg_ptr->GetMessageType() == MessageType::SystemEvent) {
            auto systemEventMessage_msg = dynamic_cast<SystemEventMessage*>(msg_ptr.get());
            if (systemEventMessage_msg) {
                systemevent_out_stream 
                    << systemEventMessage_msg->timestamp << ","
                    << static_cast<char> (systemEventMessage_msg->system_event) << std::endl;
            }
        }
#endif

#ifdef Need_SecurityDirectoryMessage_Data
        if (msg_ptr->GetMessageType() == MessageType::SecurityDirectory) {
            auto securityDirectoryMessage_msg = dynamic_cast<SecurityDirectoryMessage*>(msg_ptr.get());
            if (securityDirectoryMessage_msg) {
                securitydirectory_out_stream 
                    << securityDirectoryMessage_msg->timestamp << ","
                    << securityDirectoryMessage_msg->symbol << ","
                    << securityDirectoryMessage_msg->round_lot_size << ","
                    << securityDirectoryMessage_msg->adjusted_POC_price << ","
                    << static_cast<uint16_t>(securityDirectoryMessage_msg->LULD_tier) << ","
                    << (uint16_t)((securityDirectoryMessage_msg->flags & 0x80)/0x80) << "," 
                    << (uint16_t)((securityDirectoryMessage_msg->flags & 0x40)/0x40) << "," 
                    << (uint16_t)((securityDirectoryMessage_msg->flags & 0x20)/0x20) << "," 
                    << (uint16_t)securityDirectoryMessage_msg->flags << std::endl; 
            }
        }
#endif


#ifdef Need_SecurityEventMessage_Data
        if (msg_ptr->GetMessageType() == MessageType::SecurityEvent) {
            auto securityEventMessage_msg = dynamic_cast<SecurityEventMessage*>(msg_ptr.get());
            if (securityEventMessage_msg) {
                securityevent_out_stream 
                    << securityEventMessage_msg->timestamp << ","
                    << securityEventMessage_msg->symbol << ","
                    << static_cast<char>(securityEventMessage_msg->security_event) << std::endl;
            }
        }
#endif


#ifdef Need_TradingStatusMessage_Data
        if (msg_ptr->GetMessageType() == MessageType::TradingStatus) {
            auto tradingStatusMessage_msg = dynamic_cast<TradingStatusMessage*>(msg_ptr.get());
            if (tradingStatusMessage_msg) {
                tradingstatus_out_stream 
                    << tradingStatusMessage_msg->timestamp << ","
                    << tradingStatusMessage_msg->symbol << ","
                    << static_cast<char>(tradingStatusMessage_msg->trading_status)  << ","
                    << tradingStatusMessage_msg->reason << std::endl;
            }
        }
#endif


#ifdef Need_OperationalHaltStatusMessage_Data
        if (msg_ptr->GetMessageType() == MessageType::OperationalHaltStatus) {
            auto operationalHaltStatusMessage_msg = dynamic_cast<OperationalHaltStatusMessage*>(msg_ptr.get());
            if (operationalHaltStatusMessage_msg) {
                operationalhaltstatus_out_stream 
                    << operationalHaltStatusMessage_msg->timestamp << ","
                    << operationalHaltStatusMessage_msg->symbol << ","
                    << static_cast<char>(operationalHaltStatusMessage_msg->operational_halt_status)  << std::endl;
            }
        }
#endif

#ifdef Need_ShortSalePriceTestStatusMessage_Data
        if (msg_ptr->GetMessageType() == MessageType::ShortSalePriceTestStatus) {
            auto shortSalePriceTestStatusMessage_msg = dynamic_cast<ShortSalePriceTestStatusMessage*>(msg_ptr.get());
            if (shortSalePriceTestStatusMessage_msg) {
               shortsalepriceteststatus_out_stream 
                    << shortSalePriceTestStatusMessage_msg->timestamp << ","
                    << shortSalePriceTestStatusMessage_msg->symbol << ","
                    << static_cast<uint16_t>(shortSalePriceTestStatusMessage_msg->short_sale_test_in_effect) << ","
                    << static_cast<char>(shortSalePriceTestStatusMessage_msg->detail) << std::endl;
            }
        }
#endif


    }
#ifdef Need_AllMsgType_Data
    allmsgtype_out_stream.close();
#endif     

#ifdef Need_QuoteUpdate_Data
    quoteupdate_out_stream.close();
#endif

#ifdef Need_TradeReport_Data
    tradereport_out_stream.close();
#endif

#ifdef Need_OfficialPrice_Data
    officialprice_out_stream.close();
#endif

#ifdef Need_TradeBreak_Data
    tradebreak_out_stream.close();
#endif

#ifdef Need_AuctionInformation_Data
    auctioninfo_out_stream.close();
#endif

#ifdef Need_PriceLevelUpdate_Data
    pricelevelupdate_out_stream.close();
#endif

#ifdef Need_SystemEventMessage_Data
    systemevent_out_stream.close();
#endif

#ifdef Need_SecurityDirectoryMessage_Data
    securitydirectory_out_stream.close();
#endif

#ifdef Need_SecurityEventMessage_Data
    securityevent_out_stream.close(); 
#endif

#ifdef Need_TradingStatusMessage_Data
    tradingstatus_out_stream.close();
#endif

#ifdef Need_OperationalHaltStatusMessage_Data
    operationalhaltstatus_out_stream.close(); 
#endif

#ifdef Need_ShortSalePriceTestStatusMessage_Data
    shortsalepriceteststatus_out_stream.close();
#endif

    return 0;
}

