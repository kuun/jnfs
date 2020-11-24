package io.kuun.jnfs;

class NativeNfsContext {
    static native long init();

    static native void destroy(long contextPtr);

    static native void setTcpSynCnt(long contextPtr, int cnt);

    static native void setUid(long contextPtr, int uid);

    static native void setGid(long contextPtr, int gid);

    static native void setPageCache(long contextPtr, int v);

    static native void setPageCacheTtl(long contextPtr, int v);

    static native void setReadAhead(long contextPtr, int v);

    static native void setDebug(long contextPtr, int level);

    static native void setDirCache(long contextPtr, int enabled);

    static native void setAutoReconnect(long contextPtr, int numRetries);

    static native void setVersion(long contextPtr, int version);

    static native void setTimeout(long contextPtr, int milliseconds);

    static native void setClientName(long contextPtr, String name);

    static native int mount(long contextPtr, String server, String export);

    static native int umount(long contextPtr);

    static native int stat(long contextPtr, String path, NfsFileStat fileStat);

    static native int fstat(long contextPtr, long filePtr, NfsFileStat fileStat);

    static native int open(long contextPtr, String path, int flags, NfsFile file);

    static native int open2(long contextPtr, String path, int flags, int mode, NfsFile file);

    static native int close(long contextPtr, long filePtr);

    static native int pread(long contextPtr, long filePtr, long offset, long count, byte[] buf);

    static native int read(long contextPtr, long filePtr, long count, byte[] buf);

    static native int pwrite(long contextPtr, long filePtr, long offset, long count, byte[] buf);

    static native int write(long contextPtr, long filePtr, long count, byte[] buf);

    static native long lseek(long contextPtr, long filePtr, long offset, int whence);

    static native int fsync(long contextPtr, long filePtr);

    static native int truncate(long contextPtr, String path, long length);

    static native int ftruncate(long contextPtr, long filePtr, long length);

    static native int mkdir(long contextPtr, String path);

    static native int mkdir2(long contextPtr, String path, int mode);

    static native int rmdir(long contextPtr, String path);

    static native int creat(long contextPtr, String path, int mode, NfsFile file);

    static native int create(long contextPtr, String path, int flags, int mode, NfsFile file);

    static native int unlink(long contextPtr, String path);

    static native int openDir(long contextPtr, String path, NfsDir dir);

    static native NfsDirEnt readDir(long contextPtr, long dirPtr);

    static native void seekDir(long contextPtr, long dirPtr, long loc);

    static native long tellDir(long contextPtr, long dirPtr);

    static native void rewindDir(long contextPtr, long dirPtr);

    static native int closeDir(long contextPtr, long dirPtr);

    static native int chdir(long contextPtr, String path);

    static native String getcwd(long contextPtr);

    static native int chmod(long contextPtr, String path, int mode);

    static native int fchmod(long contextPtr, long filePtr, int mode);

    static native int chown(long contextPtr, String path, int uid, int gid);

    static native int fchown(long contextPtr, long filePtr, int uid, int gid);

    static native int utime(long contextPtr, String path, long sec, long nsec);

    static native int rename(long contextPtr, String oldPath, String newPath);

    static native String getError(long contextPtr);
}
