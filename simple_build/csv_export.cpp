#include "iex_decoder.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#define Need_QuoteUpdate_Data
#define Need_TradeReport_Data
std::string filename_quotesupdate="export.quotes_update.csv";
std::string filename_tradereport="export.trade_report.csv";

int main(int argc, char* argv[]) {
    std::string filename;
    // Get the input pcap file as an argument.
    if (argc < 2) {
        std::cout << "Usage: iex_pcap_decoder <input_pcap>" << std::endl;
        return 1;
    }

#ifdef Need_QuoteUpdate_Data
    // Open a file stream for writing output to csv.
    std::ofstream quoteupdate_out_stream;
    try {
        quoteupdate_out_stream.open(filename_quotesupdate);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_quotesupdate << std::endl;
        return 1;
    }
    // Add the header.
    quoteupdate_out_stream << "Timestamp,Symbol,BidSize,BidPrice,AskSize,AskPrice" << std::endl;
#endif

#ifdef Need_TradeReport_Data
    std::ofstream tradereport_out_stream;
    try {
        tradereport_out_stream.open(filename_tradereport);
    } catch (...) {
        std::cout << "Exception thrown opening output file:" << filename_tradereport << std::endl;
        return 1;
    }
    // Add the header.
    tradereport_out_stream << "Timestamp,Symbol,Size,Price,TradeId,bit7_F,bit6_T,bit5_I,bit4_8,bit3_X,Flags" << std::endl;
#endif


    // Initialize decoder object with file path.
    std::string input_file(argv[1]);
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
                    << quote_msg->ask_price << std::endl;
            }
        }
#endif

#if 0
//        NoData = 0xFF,
//        StreamHeader = 0x00,
//        SystemEvent = 0x53,
//        SecurityDirectory = 0x44,
//        SecurityEvent = 0x45,
//        TradingStatus = 0x48,
//        OperationalHaltStatus = 0x4f,
//        ShortSalePriceTestStatus = 0x50,
//        QuoteUpdate = 0x51,
//        OfficialPrice = 0x58,
//        TradeBreak = 0x42,
//        AuctionInformation = 0x41,
//        PriceLevelUpdateBuy = 0x38,
//        PriceLevelUpdateSell = 0x35
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
    }
#ifdef Need_QuoteUpdate_Data
    quoteupdate_out_stream.close();
#endif
#ifdef Need_TradeReport_Data
    tradereport_out_stream.close();
#endif
    return 0;
}

