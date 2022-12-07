#include "OTRizer/Resource.h"

OTRizer::Resource::Resource(std::shared_ptr<Ship::Archive> otrFile, const std::string& outPath, int resType,
                            int gameVersion, int resVersion)
    : OutPath(outPath), ResType(resType), GameVersion(gameVersion), ResVersion(resVersion), otrFile(otrFile) {
    writer = std::make_shared<Ship::BinaryWriter>();
}

OTRizer::Resource::Resource(std::shared_ptr<Ship::Archive> otrFile, const std::string& outPath, int resType)
    : OutPath(outPath), ResType(resType), GameVersion(0), ResVersion(0), otrFile(otrFile) {
    writer = std::make_shared<Ship::BinaryWriter>();
}

void OTRizer::Resource::OTRize() {
    WriteHeader();
    WriteResourceData();
    otrFile->AddFile(OutPath, (uintptr_t)ToVector().data(), (uint32_t)ToVector().size());
}

void OTRizer::Resource::WriteHeader() {
    writer->Write((uint8_t)Ship::Endianness::Native); // 0x00
    writer->Write((uint8_t)0);                        // 0x01
    writer->Write((uint8_t)0);                        // 0x02
    writer->Write((uint8_t)0);                        // 0x03

    writer->Write((uint32_t)ResType); // 0x04
    // writer->Write((uint32_t)MAJOR_VERSION); // 0x08
    writer->Write((uint32_t)GameVersion);        // 0x08
    writer->Write((uint64_t)0xDEADBEEFDEADBEEF); // id, 0x0C
    writer->Write((uint32_t)ResVersion);         // 0x10
    writer->Write((uint64_t)0);                  // ROM CRC, 0x14
    writer->Write((uint32_t)0);                  // ROM Enum, 0x1C

    while (writer->GetBaseAddress() < 0x40) {
        writer->Write((uint32_t)0); // To be used at a later date!
    }
}

std::vector<char> OTRizer::Resource::ToVector() {
    return writer->ToVector();
}