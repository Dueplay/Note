VFS层结构体定义

1.文件系统信息

```c
typedef struct {
    uint8_t dev_id; // 设备号
    vfs_filesystem_ops_t *fs; // 具体文件系统的操作函数表
    void *fs_specified_data;  // 具体文件系统特地的数据
    UT_hash_handle hh; // hash表维护全部文件系统信息,通过dev_id找到对应的vfs_fsinfo_t
} vfs_fsinfo_t;
```

2. 文件信息

```c
typedef struct {
    const vfs_fsinfo_t *fsinfo; // 指向具体的fs
    void *file_specified_data;  // 底层的文件描述符，FAT对应FIL，littlefs对应lfs_file_t
} vfs_file_t;
```

3.文件系统操作函数表，具体文件系统需要去实现的函数。

```c
typedef struct vfs_filesystem_ops {
    char fs_name[16]; // 具体文件名称
    VFS_RESULT (*open)(const char *path, int flags, vfs_file_t *fileinfo); // 底层的文件句柄放在fileinfo的file_specified_data字段中
    VFS_RESULT (*mkfs)(uint8_t dev_id);
    VFS_RESULT (*mount)(uint8_t dev_id, void ** fs_specified_data); // 底层fs挂载，创建vfs_fsinfo_t文件系统信息，插入hash表。
    int (*read)(const void *info, void *buf, size_t count); // info为底层具体的文件句柄
    int (*write)(const void *info, const void *buf, size_t count);
    VFS_RESULT (*lseek)(const void *info, size_t offset, int wherece);
    VFS_RESULT (*close)(const void *info);
    VFS_RESULT (*mkdir)(const char *path);
    VFS_RESULT (*opendir)(const char *path, vfs_file_t *info); 
    VFS_RESULT (*readdir)(const void *info);
    VFS_RESULT (*closedir)(const void *info);
    UT_hash_handle hh;
} vfs_fiesystem_ops_t;


// littlefs的实现
static vfs_filesystem_ops_t lfs_operations = {
    .fs_name = "littlefs",
    .read = lfs_vfs_read,
    .write = lfs_vfs_write,
    .lseek = lfs_vfs_lseek,
    .open = lfs_vfs_open,
    .close = lfs_vfs_close,
    .mkdir = lfs_vfs_mkdir,
    .opendir = lfs_vfs_opendir,
    .readdir = lfs_vfs_readdir,
    .closedir = lfs_vfs_closedir,
};
```

4.VFS层文件描述符表

```c
// 维护打开的文件信息。
// open时返回给用户int fd，fd对应一个vfs_file_t
struct vfs_fdtable {
    uint32_t maxfd; // 最大的fd
    vfs_file_t **fds; // vfs_file_t *的数组
};

// 从表中找到一个空的vfs_file_t
int fdt_fd_new(struct vfs_fdtable *fdt);
// 通过fd得到对应的vfs_file_t
struct vfs_file *fdt_get_file(struct vfs_fdtable *fdt, int fd);
// 释放fd对应的vfs_file_t
void fdt_fd_release(struct vfs_fdtable *fdt, int fd);
```

5.VFS层函数

```c
// 一个设备对应一个文件系统
VFS_RESULT vfs_mkfs(uint8_t dev_id, const char *fstype);
VFS_RESULT detect_fs(uint8_t dev_id, char **fstype_str);
VFS_RESULT _vfs_mount(uint8_t dev_id, const char *fstype_str, char *ret_key);
VFS_RESULT vfs_mount(uint8_t dev_id, char *ret_key);
VFS_RESULT vfs_umount_by_dev(uint8_t dev_id);
VFS_RESULT vfs_umount(char ret_key);

// 通过path判断出具体的文件系统，fd->fsinfo指向具体文件系统的fs_info
// 然后调用具体文件系统的open
VFS_RESULT vfs_open(vfs_file_t *fd, const char *path, int flags, int mode);

// 通过fd->fsinfo得到具体文件系统，从而调用具体的read
int vfs_read(vfs_file_t *fd, void *buf, size_t count);

// 通过fd->fsinfo得到具体文件系统，从而调用具体的read
int vfs_write(vfs_file_t *fd, const void *buf, size_t count);

// 通过fd->fsinfo得到具体文件系统，从而调用具体的read
VFS_RESULT vfs_close(vfs_file_t *fd);
// only support begin
VFS_RESULT vfs_lseek(vfs_file_t *fd, offset_t offset, int wherece);
```

