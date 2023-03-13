#include "FileUtils.h"

#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>

string FileUtils::MountPoint = MOUNT_POINT_SPIFFS; // Start with SPIFFS
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
    char fileName[8];

    std::vector<std::string> filenames;
    filenames.reserve(MAX_FNAMES_PER_CHUNK);

    DIR* dir = opendir(fpath.c_str());
    if (dir == NULL) {
        printf("Error opening %s", fpath.c_str());
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
        if (fname.compare(0, 1, ".") != 0) {
            if ((fileExts.find(fname.substr(fname.size() - 4)) != string::npos)) {
                // printf("%s\n",fname.c_str());
                filenames.push_back(fname.c_str());
                cnt++;
                if (cnt == MAX_FNAMES_PER_CHUNK) {
                    // Dump current chunk
                    sort(filenames.begin(), filenames.end()); // Sort vector
                    sprintf(fileName, "/.d%d", chunk_cnt);
                    FILE* f = fopen((fpath + fileName).c_str(), "w");
                    if (f == NULL) {
                        printf("Error opening filelist chunk\n");
                        return 0;
                    }
                    for (int n = 0; n < MAX_FNAMES_PER_CHUNK; n++) fputs((filenames[n] + std::string(127 - filenames[n].size(), ' ') + "\n").c_str(), f);
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
        sort(filenames.begin(), filenames.end()); // Sort vector
        sprintf(fileName, "/.d%d", chunk_cnt);
        FILE* f = fopen((fpath + fileName).c_str(), "w");
        if (f == NULL) {
            printf("Error opening last filelist chunk\n");
            return 0;
        }
        for (int n = 0; n < cnt; n++) fputs((filenames[n] + std::string(127 - filenames[n].size(), ' ') + "\n").c_str(), f);
        fclose(f);
    }

    if (chunk_cnt == 0) {
        // Rename unique chunk
        rename((fpath + "/.d0").c_str(), (fpath + "/.d").c_str());
    }

    closedir(dir);

    printf("Sort done.\n");

    return chunk_cnt;

}

void FileUtils::Mergefiles(string fpath, int chunk_cnt)
{
    char fileName[8];

    // Merge sort
    FILE* file1, * file2, * fout;
    char fname1[64];
    char fname2[64];

    file1 = fopen((fpath + "/.d0").c_str(), "r");
    file2 = fopen((fpath + "/.d1").c_str(), "r");
    string bufout = "";
    int bufcnt = 0;

    int  n = 1;
    while (file2 != NULL) {

        sprintf(fileName, "/.t%d", n);
        printf("Creating %s\n", fileName);
        fout = fopen((fpath + fileName).c_str(), "w");

        fgets(fname1, sizeof(fname1), file1);
        fgets(fname2, sizeof(fname2), file2);

        while (1) {

            if (feof(file1)) {
                if (feof(file2)) break;
                bufout += fname2;
                fgets(fname2, sizeof(fname2), file2);
            }
            else if (feof(file2)) {
                if (feof(file1)) break;
                bufout += fname1;
                fgets(fname1, sizeof(fname1), file1);
            }
            else if (strcmp(fname1, fname2) < 0) {
                bufout += fname1;
                fgets(fname1, sizeof(fname1), file1);
            }
            else {
                bufout += fname2;
                fgets(fname2, sizeof(fname2), file2);
            }

            bufcnt++;

            if (bufcnt == 128) {
                fwrite(bufout.c_str(), sizeof(char), bufout.length(), fout);
                bufout = "";
                bufcnt = 0;
            }

        }

        if (bufcnt) {
            fwrite(bufout.c_str(), sizeof(char), bufout.length(), fout);
        }

        fclose(file1);
        fclose(file2);
        fclose(fout);

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
    rename((fpath + fileName).c_str(), (fpath + "/.d").c_str());

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
