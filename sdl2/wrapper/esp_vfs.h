#ifndef _wrap_esp_vfs_h
#define _wrap_esp_vfs_h

struct DIR {};
struct dirent {
    const char* d_name;
};

inline DIR* opendir(const char*) { return 0; }
inline dirent* readdir(DIR*) { return 0; }
inline void closedir(DIR*) {}

#endif