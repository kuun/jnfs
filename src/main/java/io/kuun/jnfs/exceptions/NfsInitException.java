package io.kuun.jnfs.exceptions;

public class NfsInitException extends Exception {
    public NfsInitException() {
        super("Can not init nfs context");
    }
}