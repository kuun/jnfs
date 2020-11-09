package io.kuun.jnfs;

public class NfsFile {
    long filePtr;
    NfsContext context;

    NfsFileStat stat() {
        return null;
    }

    void close() {

    }

    int pread(long offset, long count, byte[] buf) {
        return 0;
    }

    int read(NfsFile file, long offset, long count, byte[] buf) {
        return 0;
    }

    int pwrite(NfsFile file, long count, byte[] buf) {
        return 0;
    }

    int lseek(NfsFile file, long offset, int whence) {
        return 0;
    }

    void lock(NfsFile file, int lockOp, long count) {

    }

    void sync(NfsFile file) {

    }

    void truncate(long length) {

    }

    void chmod(int mode) {

    }

    void chown(int uid, int gid) {

    }
}
