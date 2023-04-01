///////////////////////////////////////////////////////////////////////////////
//
// ZX-ESPectrum-IDF - Sinclair ZX Spectrum emulator for ESP32 / IDF
//
// Copyright (c) 2023 Víctor Iborra [Eremus] and David Crespo [dcrespo3d]
// https://github.com/EremusOne/ZX-ESPectrum-IDF
//
// Based on ZX-ESPectrum-Wiimote
// Copyright (c) 2020, 2021 David Crespo [dcrespo3d]
// https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote
//
// Based on previous work by Ramón Martinez and Jorge Fuertes
// https://github.com/rampa069/ZX-ESPectrum
//
// Original project by Pete Todd
// https://github.com/retrogubbins/paseVGA
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "FileUtils.h"
#include "Config.h"
#include "CPU.h"
#include "MemESP.h"
#include "ESPectrum.h"
#include "hardpins.h"
#include "messages.h"
#include "OSDMain.h"
#include "roms.h"
#include "Video.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

using namespace std;

string FileUtils::MountPoint = MOUNT_POINT_SPIFFS; // Start with SPIFFS
bool FileUtils::SDReady = false;
sdmmc_card_t *FileUtils::card;

void FileUtils::initFileSystem() {

    // Initialize SPIFFS file system
    esp_vfs_spiffs_conf_t config = {
        .base_path = MOUNT_POINT_SPIFFS,
        .partition_label = NULL,
        .max_files = 3,
        .format_if_mount_failed = false,
    };
    if (esp_vfs_spiffs_register(&config) != ESP_OK) {
        return;
    }

    SDReady = mountSDCard();

    vTaskDelay(2);

}

bool FileUtils::mountSDCard() {

    // Init SD Card
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 3,
        .allocation_unit_size = 16 * 1024
    };
    
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    
    ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH1);
    if (ret != ESP_OK) {
        printf("SD Card init: Failed to initialize bus.\n");
        return false;
    }

    sdspi_device_config_t slot_config =  {
    .host_id   = SDSPI_DEFAULT_HOST,
    .gpio_cs   = GPIO_NUM_13,
    .gpio_cd   = SDSPI_SLOT_NO_CD,
    .gpio_wp   = SDSPI_SLOT_NO_WP,
    .gpio_int  = GPIO_NUM_NC, \
    };
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = SPI2_HOST;

    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT_SD, &host, &slot_config, &mount_config, &FileUtils::card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            printf("Failed to mount filesystem.\n");
        } else {
            printf("Failed to initialize the card.\n");
        }
        return false;
    }

    return true;

}

void FileUtils::unmountSDCard() {
    // Unmount partition and disable SPI peripheral
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT_SD, card);
    // //deinitialize the bus after all devices are removed
    spi_bus_free(SPI2_HOST);
}

// String FileUtils::getAllFilesFrom(const String path) {
//     KB_INT_STOP;
//     File root = THE_FS.open("/");
//     File file = root.openNextFile();
//     String listing;

//     while (file) {
//         file = root.openNextFile();
//         String filename = file.name();
//         if (filename.startsWith(path) && !filename.startsWith(path + "/.")) {
//             listing.concat(filename.substring(path.length() + 1));
//             listing.concat("\n");
//         }
//     }
//     vTaskDelay(2);
//     KB_INT_START;
//     return listing;
// }

// void FileUtils::listAllFiles() {
//     KB_INT_STOP;
//     File root = THE_FS.open("/");
//     Serial.println("fs opened");
//     File file = root.openNextFile();
//     Serial.println("fs openednextfile");

//     while (file) {
//         Serial.print("FILE: ");
//         Serial.println(file.name());
//         file = root.openNextFile();
//     }
//     vTaskDelay(2);
//     KB_INT_START;
// }

// void FileUtils::sanitizeFilename(String filename)
// {
//     filename.replace("\n", " ");
//     filename.trim();
// }

// File FileUtils::safeOpenFileRead(String filename)
// {
//     sanitizeFilename(filename);
//     File f;
//     if (Config::slog_on)
//         Serial.printf("%s '%s'\n", MSG_LOADING, filename.c_str());
//     if (!THE_FS.exists(filename.c_str())) {
//         KB_INT_START;
//         OSD::errorHalt((String)ERR_READ_FILE + "\n" + filename);
//     }
//     f = THE_FS.open(filename.c_str(), FILE_READ);
//     vTaskDelay(2);

//     return f;
// }

string FileUtils::getFileEntriesFromDir(string path) {

    string filelist;

    // printf("Getting entries from: '%s'\n", path.c_str());

    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        // OSD::errorHalt(ERR_DIR_OPEN + "\n" + path).cstr());
    }

    struct dirent* de = readdir(dir);
    
    if (!de) {

        printf("No entries found!\n");

    } else {

        int cnt = 0;
        while (true) {
            
            printf("Found file: %s\n", de->d_name);
            
            string filename = de->d_name;

            // printf("readdir filename -> %s\n", filename.c_str());

            if (filename.compare(0,1,".") == 0) {
        //        printf("HIDDEN\n");
            } else if (filename.substr(filename.size()-4) == ".txt") {
        //        printf("IGNORING TXT\n");
            } else if (filename.substr(filename.size()-4) == ".TXT") {
        //        printf("IGNORING TXT\n");
            } else {
        //        printf("ADDING\n");
                filelist += filename + "\n";
                cnt++;
            }
            
            de = readdir(dir);
            if ((!de) || (cnt == 20)) break;
        
        }

    }

    // printf(filelist.c_str());

    closedir(dir);

    return filelist;

}

int IRAM_ATTR FileUtils::DirToFile(string fpath, string fileExts) {

    char fileName[8];

    std::vector<std::string> filenames;
    filenames.reserve(MAX_FNAMES_PER_CHUNK);

    DIR* dir = opendir(fpath.c_str());
    if (dir == NULL) {
        printf("Error opening %s",fpath.c_str());
        return 0;
    }

    struct dirent* de = readdir(dir);
    if (!de) {
        printf("No entries found!\n");
        closedir(dir);
        return 0;
    }

    // Remove previous dir file
    remove((fpath + "/.d").c_str());

    // Read filenames from medium into vector, sort it, and dump into MAX_FNAMES_PER_CHUNK filenames long files
    int cnt = 0;
    int chunk_cnt = 0;

    while (true) {
        string fname = de->d_name;
        if (fname.compare(0,1,".") != 0) {
            if ((fileExts.find(fname.substr(fname.size()-4)) != string::npos)) {
                // printf("%s\n",fname.c_str());
                filenames.push_back(fname.c_str());                    
                cnt++;
                if (cnt == MAX_FNAMES_PER_CHUNK) {
                    // Dump current chunk
                    sort(filenames.begin(),filenames.end()); // Sort vector
                    sprintf(fileName, "/.d%d", chunk_cnt);
                    FILE *f = fopen((fpath + fileName).c_str(), "w");
                    if (f==NULL) {
                        printf("Error opening filelist chunk\n");
                        return 0;
                    }
                    for (int n=0; n < MAX_FNAMES_PER_CHUNK; n++) fputs((filenames[n] + std::string(31 - filenames[n].size(), ' ') + "\n").c_str(),f);
                    fclose(f);
                    filenames.clear();
                    cnt = 0;
                    chunk_cnt++;
                }

            }

        }
        de = readdir(dir);
        if (!de) break;
    }

    if (cnt > 0) { 
        // Dump last chunk
        sort(filenames.begin(),filenames.end()); // Sort vector
        sprintf(fileName, "/.d%d", chunk_cnt);
        FILE *f = fopen((fpath + fileName).c_str(), "w");
        if (f == NULL) {
            printf("Error opening last filelist chunk\n");
            return 0;
        }
        for (int n=0; n < cnt;n++) fputs((filenames[n] + std::string(31 - filenames[n].size(), ' ') + "\n").c_str(),f);
        fclose(f);
    }

    if (chunk_cnt == 0) {
        // Rename unique chunk
        rename((fpath + "/.d0").c_str(),(fpath + "/.d").c_str());
    }

    closedir(dir);

    printf("Sort done.\n");

    return chunk_cnt;

}

void FileUtils::Mergefiles(string fpath, int chunk_cnt) {

    char fileName[8];

    // Merge sort
    FILE *file1,*file2,*fout;
    char fname1[64];
    char fname2[64];

    file1 = fopen((fpath + "/.d0").c_str(), "r");
    file2 = fopen((fpath + "/.d1").c_str(), "r");
    string bufout="";
    int bufcnt = 0;

    int  n = 1;
    while (file2 != NULL) {

        sprintf(fileName, "/.t%d", n);
        printf("Creating %s\n",fileName);
        fout  = fopen((fpath + fileName).c_str(), "w");

        fgets(fname1, sizeof(fname1), file1);
        fgets(fname2, sizeof(fname2), file2);

        while(1) {

            if (feof(file1)) {
                if (feof(file2)) break;
                bufout += fname2;
                fgets(fname2, sizeof(fname2), file2);
            }
            else if (feof(file2)) {
                if (feof(file1)) break;
                bufout += fname1;
                fgets(fname1, sizeof(fname1), file1);
            } else if (strcmp(fname1,fname2)< 0) {
                bufout += fname1;
                fgets(fname1, sizeof(fname1), file1);
            } else {
                bufout += fname2;
                fgets(fname2, sizeof(fname2), file2);
            }

            bufcnt++;

            if (bufcnt == 128) {
                fwrite(bufout.c_str(),sizeof(char),bufout.length(),fout);
                bufout = "";
                bufcnt = 0;
            }

        }

        if (bufcnt) {
            fwrite(bufout.c_str(),sizeof(char),bufout.length(),fout);
        }

        fclose(file1);
        fclose (file2);
        fclose (fout);

        // Next cycle: open t<n> for read
        sprintf(fileName, "/.t%d", n);
        file1 = fopen((fpath + fileName).c_str(), "r");

        n++;

        sprintf(fileName, "/.d%d", n);
        file2 = fopen((fpath + fileName).c_str(), "r");

    }

    fclose(file1);

    // Rename final chunk
    sprintf(fileName, "/.t%d", n - 1);
    rename((fpath + fileName).c_str(),(fpath + "/.d").c_str());

    // Remove temp files
    for (int n = 0; n <= chunk_cnt; n++) {
        sprintf(fileName, "/.d%d", n);
        remove((fpath + fileName).c_str());
        sprintf(fileName, "/.t%d", n);
        remove((fpath + fileName).c_str());
    }

}

bool FileUtils::hasSNAextension(string filename)
{
    
    if (filename.substr(filename.size()-4,4) == ".sna") return true;
    if (filename.substr(filename.size()-4,4) == ".SNA") return true;

    return false;

}

bool FileUtils::hasZ80extension(string filename)
{

    if (filename.substr(filename.size()-4,4) == ".z80") return true;
    if (filename.substr(filename.size()-4,4) == ".Z80") return true;

    return false;

}

// uint16_t FileUtils::countFileEntriesFromDir(String path) {
//     String entries = getFileEntriesFromDir(path);
//     unsigned short count = 0;
//     for (unsigned short i = 0; i < entries.length(); i++) {
//         if (entries.charAt(i) == ASCII_NL) {
//             count++;
//         }
//     }
//     return count;
// }

// Get all sna files sorted alphabetically
string FileUtils::getSortedFileList(string fileDir)
{
    
    // get string of unsorted filenames, separated by newlines
    string entries = getFileEntriesFromDir(fileDir);

    // count filenames (they always end at newline)
    int count = 0;
    for (int i = 0; i < entries.length(); i++) {
        if (entries.at(i) == ASCII_NL) {
            count++;
        }
    }

    std::vector<std::string> filenames;
    filenames.reserve(count);

    // Copy filenames from string to vector
    string fname = "";
    for (int i = 0; i < entries.length(); i++) {
        if (entries.at(i) == ASCII_NL) {
            filenames.push_back(fname.c_str());
            fname = "";
        } else fname += entries.at(i);
    }

    // Sort vector
    sort(filenames.begin(),filenames.end());

    // Copy back filenames from vector to string
    string sortedEntries = "";
    for (int i = 0; i < count; i++) {
        sortedEntries += filenames[i] + '\n';
    }

    return sortedEntries;

}
