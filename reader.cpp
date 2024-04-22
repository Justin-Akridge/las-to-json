#include <iostream>
#include <fstream>
#include <liblas/liblas.hpp>
#include <json/json.h>

using namespace std;

int main() {
    std::ifstream ifs;
    ifs.open("output_half1.las", std::ios::in | std::ios::binary);

    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);

    liblas::Header const& header = reader.GetHeader();

    std::cout << "Compressed: " << ((header.Compressed() == true) ? "true\n":"false\n");
    std::cout << "Signature: " << header.GetFileSignature() << '\n';
    std::cout << "Points count: " << header.GetPointRecordsCount() << '\n';

    Json::Value root;
    root["version"] = header.GetVersionMinor();

    // Read LAS point data
    int line = 0;
    while (line != 1'000'000 && reader.ReadNextPoint()) {

        liblas::Point const& p = reader.GetPoint();
        // Construct JSON object for each point
        Json::Value point;
        point["X"] = p.GetX();
        point["Y"] = p.GetY();
        point["Z"] = p.GetZ();
        
        std::cout << "Point: " << line << "X: " << point["X"] << "Y: " << point["Y"] << "Z: " << point["Z"] << '\n';
        line++; 
        // Add point to JSON array
        root["points"].append(point);
    }

    // Write JSON to file
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "\t";
    std::ofstream jsonFile("output.json");
    if (jsonFile.is_open()) {
        cout << Json::writeString(builder,root);
        jsonFile << Json::writeString(builder, root);
        jsonFile.close();
        std::cout << "JSON data written to output.json\n";
    } else {
        std::cerr << "Error opening output.json\n";
        return 1;
    }
    return 0;
}

