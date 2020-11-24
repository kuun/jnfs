package io.kuun.jnfs;

public class FileMode {
    public static int S_ISUID = 04000;     // set-user-id bit
    public static int S_ISGID = 02000;     // set-group-id bit
    public static int S_ISVTX = 01000;     // sticky bit
    // for user permissions
    public static int S_IRWXU = 00700;     // owner has read, write, and execute permission
    public static int S_IRUSR = 00400;     // owner has read permission
    public static int S_IWUSR = 00200;     // owner has write permission
    public static int S_IXUSR = 00100;     // owner has execute permission
    // for group permissions
    public static int S_IRWXG = 00070;     // group has read, write, and execute permission
    public static int S_IRGRP = 00040;     // group has read permission
    public static int S_IWGRP = 00020;     // group has write permission
    public static int S_IXGRP = 00010;     // group has execute permission
    // for other permissions
    public static int S_IRWXO = 00007;     // others (not in group) have read, write, and execute permission
    public static int S_IROTH = 00004;     // others have read permission
    public static int S_IWOTH = 00002;     // others have write permission
    public static int S_IXOTH = 00001;     // others have execute permission

    public static int S_IFMT = 0170000;    // bit mask for the file type bit field
    public static int S_IFSOCK = 0140000;  // is unix socket
    public static int S_IFLNK = 0120000;   // is symbolic link
    public static int S_IFREG = 0100000;   // is regular file
    public static int S_IFBLK = 0060000;   // is block device
    public static int S_IFDIR = 0040000;   // is directory
    public static int S_IFCHR = 0020000;   // is character device
    public static int S_IFIFO = 0010000;   // is FIFO

    public static int S_MODE_MASK = 0007777;
    public static int S_PERM_MASK = 0000777;

    public static boolean isReg(long mode) {
        return (mode & S_IFMT) == S_IFREG;
    }

    public static boolean isSock(long mode) {
        return (mode & S_IFMT) == S_IFSOCK;
    }

    public static boolean isLink(long mode) {
        return (mode & S_IFMT) == S_IFLNK;
    }

    public static boolean isBlock(long mode) {
        return (mode & S_IFMT) == S_IFBLK;
    } 

    public static boolean isDir(long mode) {
        return (mode & S_IFMT) == S_IFDIR;
    }

    public static boolean isChar(long mode) {
        return (mode & S_IFMT) == S_IFCHR;
    }

    public static boolean isFifo(long mode) {
        return (mode & S_IFMT) == S_IFIFO;
    }

}
