package io.kuun.jnfs;

import java.io.Closeable;

import io.kuun.jnfs.exceptions.NfsException;
import io.kuun.jnfs.exceptions.NfsInitException;

public class NfsContext implements Closeable {
    long contextPtr;

    static {
        Utils.loadAllEmbeddedLibraries();
    }

    public NfsContext() {
    }


    public void init() throws NfsInitException {
        contextPtr = NativeNfsContext.init();
        if (contextPtr == 0) {
            throw new NfsInitException();
        }
    }

    /**
     * Init an NFS context, the context should be closed to avoid resource leaked.
     */
    public void close() {
        if (contextPtr != 0) {
            NativeNfsContext.destroy(contextPtr);
        }
    }

    /**
     *  Number of SYNs to send during the session establish
     *  before failing setting up the TCP connection to the
     *  server
     * @param v
     */
    public void setTcpSynCnt(int v) {
        NativeNfsContext.setTcpSynCnt(contextPtr, v);
    }

    public void setUid(int uid) {
        NativeNfsContext.setUid(contextPtr, uid);
    }

    public void setGid(int gid) {
        NativeNfsContext.setGid(contextPtr, gid);
    }


    public void setPageCache(int v) {
        NativeNfsContext.setPageCache(contextPtr, v);
    }

    public void setPageCacheTtl(int v) {
        NativeNfsContext.setPageCacheTtl(contextPtr, v);
    }

    public void setReadhead(int v) {
        NativeNfsContext.setReadAhead(contextPtr, v);
    }


    public void setDebug(int level) {
        NativeNfsContext.setDebug(contextPtr, level);
    }


    public void setDirCache(boolean enabled) {
        int isEnable = 0;

        if (enabled) {
            isEnable = 1;
        }
        NativeNfsContext.setDirCache(contextPtr, isEnable);
    }


    public void setAutoConnect(int numRetries) {
        NativeNfsContext.setAutoReconnect(contextPtr, numRetries);
    }


    /**
     * Set NFS version.
     * @param version Supported versions are 3 (NFS_V3, default) and 4 (NFS_V4)
     */
    public void setVersion(int version) {
        NativeNfsContext.setVersion(contextPtr, version);
    }

    void mount(String server, String exportName) throws NfsException {
        int errno = NativeNfsContext.mount(contextPtr, server, exportName);
        failThenThrow(errno);
    }

    void umount() throws NfsException {
        int errno = NativeNfsContext.umount(contextPtr);
        failThenThrow(errno);
    }

    NfsFileStat stat(String path) throws NfsException {
        NfsFileStat fileStat = new NfsFileStat();
        int errno = NativeNfsContext.stat(contextPtr, path, fileStat);
        failThenThrow(errno);
        return fileStat;
    }

    NfsFile open(String path, int flags) throws NfsException {
        NfsFile file = new NfsFile();
        int errno = NativeNfsContext.open(contextPtr, path, flags, file);
        failThenThrow(errno);
        return file;
    }

    NfsFile open(String path, int flags, int mode) throws NfsException {
        NfsFile file = new NfsFile();
        int errno = NativeNfsContext.open2(contextPtr, path, flags, mode, file);
        failThenThrow(errno);
        return file;
    }

    void truncate(String path, long length) throws NfsException {
        int errno = NativeNfsContext.truncate(contextPtr, path, length);
        failThenThrow(errno);
    }

    void mkdir(String path) throws NfsException {
        int errno = NativeNfsContext.mkdir(contextPtr, path);
        failThenThrow(errno);
    }

    void mkdir(String path, int mode) throws NfsException {
        int errno = NativeNfsContext.mkdir2(contextPtr, path, mode);
        failThenThrow(errno);
    }

    void rmdir(String path) throws NfsException {
        int errno = NativeNfsContext.rmdir(contextPtr, path);
        failThenThrow(errno);  
    }

    NfsFile create(String path, int mode) throws NfsException {
        NfsFile file = new NfsFile();
        int errno = NativeNfsContext.creat(contextPtr, path, mode, file);
        failThenThrow(errno);
        return file;
    }

    NfsFile create(String path, int flags, int mode) throws NfsException {
        NfsFile file = new NfsFile();
        int errno = NativeNfsContext.create(contextPtr, path, flags, mode, file);
        failThenThrow(errno);
        return file;
    }

    void unlink(String path) throws NfsException {
        int errno = NativeNfsContext.unlink(contextPtr, path);
        failThenThrow(errno);  
    }

    NfsDir openDir(String path) throws NfsException {
        NfsDir dir = new NfsDir();
        int errno = NativeNfsContext.openDir(contextPtr, path, dir);
        failThenThrow(errno);
        return dir;
    }

    void chdir(String path) throws NfsException {
        int errno = NativeNfsContext.chdir(contextPtr, path);
        failThenThrow(errno); 
    }

    String getCwd() {
        return NativeNfsContext.getcwd(contextPtr);
    }

    void chmod(String path, int mode) throws NfsException {
        int errno = NativeNfsContext.chmod(contextPtr, path, mode);
        failThenThrow(errno); 
    }

    void chown(String path, int uid, int gid) throws NfsException {
        int errno = NativeNfsContext.chown(contextPtr, path, uid, gid);
        failThenThrow(errno);
    }

    void utime(String path, long sec, long nsec) throws NfsException {
        int errno = NativeNfsContext.utime(contextPtr, path, sec, nsec);
        failThenThrow(errno);
    }

    void rename(String oldPath, String newPath) throws NfsException {
        int errno = NativeNfsContext.rename(contextPtr, oldPath, newPath);
        failThenThrow(errno);
    }

    void setTimeout(int timeout) {
        NativeNfsContext.setTimeout(contextPtr, timeout);;
    }

    void setClientName(String name) {
        NativeNfsContext.setClientName(contextPtr, name);
    }


    private void failThenThrow(int errno) throws NfsException {
        if (errno != 0) {
            String err = NativeNfsContext.getError(contextPtr);
            throw new NfsException(errno, err);
        }
    }
}
