package io.kuun.jnfs;

public enum NfsVersion {
    NFS3(3),
    NFS4(4);

    private int version;

    private NfsVersion(int version) {
        this.version = version;
    }

    public int getVersion() {
        return version;
    }    
}
