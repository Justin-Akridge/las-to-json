#include <iostream>
#include <liblas/liblas.hpp>

int main() {
    // Open input LAS file
    std::ifstream ifs("input.las", std::ios::in | std::ios::binary);
    if (!ifs) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    // Create LAS reader
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);

    // Get total number of points
    uint32_t totalPoints = reader.GetHeader().GetPointRecordsCount();

    // Open output LAS files for halves
    std::ofstream ofs1("output_half1.las", std::ios::out | std::ios::binary);
    std::ofstream ofs2("output_half2.las", std::ios::out | std::ios::binary);
    if (!ofs1 || !ofs2) {
        std::cerr << "Error: Unable to create output files." << std::endl;
        return 1;
    }

    // Create LAS writers for halves
    liblas::WriterFactory writerFactory;
    liblas::Header header = reader.GetHeader();
    liblas::Writer writer1(ofs1, header);
    liblas::Writer writer2(ofs2, header);

    // Copy points to halves
    uint32_t halfPointCount = totalPoints / 2;
    uint32_t pointsWritten1 = 0;
    uint32_t pointsWritten2 = 0;
    while (reader.ReadNextPoint()) {
        if (pointsWritten1 < halfPointCount) {
            writer1.WritePoint(reader.GetPoint());
            ++pointsWritten1;
        } else {
            writer2.WritePoint(reader.GetPoint());
            ++pointsWritten2;
        }
    }

    std::cout << "First half: " << pointsWritten1 << " points" << std::endl;
    std::cout << "Second half: " << pointsWritten2 << " points" << std::endl;

    return 0;
}

