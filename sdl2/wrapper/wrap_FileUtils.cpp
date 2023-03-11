#include "FileUtils.h"

string FileUtils::MountPoint = "/data"; // MOUNT_POINT_SPIFFS; // Start with SPIFFS
bool FileUtils::SDReady = false;
sdmmc_card_t* FileUtils::card;

void FileUtils::initFileSystem() {

}

// Get all sna files sorted alphabetically
string FileUtils::getSortedFileList(string fileDir)
{
    return "";
}

int FileUtils::DirToFile(string fpath, string fileExts)
{
    return 0;
}

void FileUtils::Mergefiles(string fpath, int chunk_cnt)
{
}

bool FileUtils::hasSNAextension(string filename)
{

    if (filename.substr(filename.size() - 4, 4) == ".sna") return true;
    if (filename.substr(filename.size() - 4, 4) == ".SNA") return true;

    return false;

}

bool FileUtils::hasZ80extension(string filename)
{

    if (filename.substr(filename.size() - 4, 4) == ".z80") return true;
    if (filename.substr(filename.size() - 4, 4) == ".Z80") return true;

    return false;

}
