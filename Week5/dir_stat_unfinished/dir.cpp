#define NDEBUG

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#include<stdio.h>
#include<string>
#include<deque>

#include<assert.h>

//    https://man7.org/linux/man-pages/man3/readdir.3.html
//    struct dirent {
//        ino_t          d_ino;        Inode number
//        off_t          d_off;        Not an offset;
//        unsigned short d_reclen;     Length of this record
//        unsigned char  d_type;       Type of file; not supported
//                                     by all filesystem types 
//        char           d_name[256];  Null-terminated filename 
//    };

//    https://man7.org/linux/man-pages/man2/lstat.2.html
//    struct stat {
//        dev_t     st_dev;         /* ID of device containing file */
//        ino_t     st_ino;         /* Inode number */
//        mode_t    st_mode;        /* File type and mode */
//        nlink_t   st_nlink;       /* Number of hard links */
//        uid_t     st_uid;         /* User ID of owner */
//        gid_t     st_gid;         /* Group ID of owner */
//        dev_t     st_rdev;        /* Device ID (if special file) */
//        off_t     st_size;        /* Total size, in bytes */
//        blksize_t st_blksize;     /* Block size for filesystem I/O */
//        blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
//        struct timespec st_atim;  /* Time of last access */
//        struct timespec st_mtim;  /* Time of last modification */
//        struct timespec st_ctim;  /* Time of last status change */

int main(int argc, char * argv[]){
    if(argc!=2){
        printf("Usage: ./dir <path>\n");
        return 1;
    }    

    std::deque<std::string> files;
    std::deque<std::string> directories;

    std::string root = argv[1];

    directories.push_back(root);
    while(!directories.empty()){
        std::string dirpath = directories.front();
        directories.pop_front();
        DIR * dir = opendir(dirpath.c_str());
        if(dir){
            while(1){
                assert(0==1);
                dirent * file = readdir(dir);
                if(!file){
                    break;
                }
                std::string name = file->d_name;
                if(name.compare(".")==0 || name.compare("..")==0){
                    continue; 
                }
                std::string path = dirpath + "/" + name;
                // printf("%s\n", path.c_str());   
                directories.push_back(path);
            }
            closedir(dir);
        }
        else{
            files.push_back(dirpath);
        }
    }

    // DIR * rootdir = opendir(argv[1]);
    // if(rootdir){
    //     while(1){
    //         dirent * file = readdir(rootdir);
    //         if(!file){
    //             break;
    //         }
    //         std::string name = file->d_name;
    //         if(name.compare(".")==0 || name.compare("..")==0){
    //             continue; 
    //         }
    //         std::string path = root + "/" + name;
    //         printf("%s\n", path.c_str());   
    //         files.push_back(path);
    //     }
     
    //     closedir(rootdir);
    // }
    // else{
    //     printf("Error not a directory!\n");
    //     return 1;
    // }

    for(auto file:files){
        struct stat s;
        stat(file.c_str(), &s);
        int64_t size = (int64_t) s.st_size;
        printf("%s %ld\n", file.c_str(),size);
    }

    return 0;

} 