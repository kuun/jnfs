package io.kuun.jnfs.exceptions;

public class NfsException extends Exception {
    private int errno;

    public NfsException(int errno, String msg) {
        super(msg);
        this.errno = errno;
    }

    public int getErrno() {
        return errno;
    }

    public void setErrno(int errno) {
        this.errno = errno;
    }

    @Override
    public String toString() {
        return "NfsException [errno=" + errno + ", message=" + getMessage() + "]";
    }    
}
