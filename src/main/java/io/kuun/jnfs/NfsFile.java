package io.kuun.jnfs;

import io.kuun.jnfs.exceptions.NfsException;

public class NfsFile {
    long filePtr;
    NfsContext context;

    NfsFileStat stat() {
        return null;
    }

    void close() throws NfsException {
        int ret = NativeNfsContext.close(context.contextPtr, filePtr);
        failThenThrow(ret);
    }

    int pread(long offset, long count, byte[] buf) {
        return NativeNfsContext.pread(context.contextPtr, filePtr, offset, count, buf);
    }

    int read(NfsFile file, long offset, long count, byte[] buf) {
        return NativeNfsContext.read(context.contextPtr, filePtr, count, buf);
    }

    int pwrite(long offset, long count, byte[] buf) {
        return NativeNfsContext.pwrite(context.contextPtr, filePtr, offset, count, buf);
    }

    int write(long count, byte[] buf) {
        return NativeNfsContext.write(context.contextPtr, filePtr, count, buf);
    }

    long lseek(long offset, int whence) {
        return NativeNfsContext.lseek(context.contextPtr, filePtr, offset, whence);
    }

    void sync() {
        NativeNfsContext.fsync(context.contextPtr, filePtr);
    }

    void truncate(long length) throws NfsException {
        int ret = NativeNfsContext.ftruncate(context.contextPtr, filePtr, length);
        failThenThrow(ret);
    }

    void chmod(int mode) throws NfsException {
        int ret = NativeNfsContext.fchmod(context.contextPtr, filePtr, mode);
        failThenThrow(ret);
    }

    void chown(int uid, int gid) throws NfsException {
        int ret = NativeNfsContext.fchown(context.contextPtr, filePtr, uid, gid);
        failThenThrow(ret);
    }

    private void failThenThrow(int errno) throws NfsException {
        if (errno != 0) {
            String err = NativeNfsContext.getError(context.contextPtr);
            throw new NfsException(errno, err);
        }
    }
}
