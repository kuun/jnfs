package io.kuun.jnfs;

public class NfsFileStat {
    private long uid;
    private long gid;
    private long mode;
    private long size;
    private long atime;
    private long mtime;
    private long ctime;

    public long getUid() {
        return uid;
    }

    public long getGid() {
        return gid;
    }

    public void setGid(long gid) {
        this.gid = gid;
    }

    public long getMode() {
        return mode;
    }

    public void setMode(long mode) {
        this.mode = mode;
    }

    public long getSize() {
        return size;
    }

    public void setSize(long size) {
        this.size = size;
    }

    public long getAtime() {
        return atime;
    }

    public void setAtime(long atime) {
        this.atime = atime;
    }

    public long getMtime() {
        return mtime;
    }

    public void setMtime(long mtime) {
        this.mtime = mtime;
    }

    public long getCtime() {
        return ctime;
    }

    public void setCtime(long ctime) {
        this.ctime = ctime;
    }

    public long getPermissions() {
        return mode & FileMode.S_PERM_MASK;
    }

    public long getFileMode() {
        return mode & FileMode.S_MODE_MASK;
    }

    @Override
    public String toString() {
        return "NfsFileStat [atime=" + atime + ", ctime=" + ctime + ", gid=" + gid + ", mode=" + mode + ", mtime="
                + mtime + ", size=" + size + ", uid=" + uid + "]";
    }

}
