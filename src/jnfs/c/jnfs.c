#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <nfsc/libnfs.h>

#include "../include/io_kuun_jnfs_NativeNfsContext.h"

static void convert_nfs_stat64(JNIEnv *env, const struct nfs_stat_64 *stat, jobject fileStat);

JNIEXPORT jlong JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_init(JNIEnv *env, jclass clazz) {
    struct nfs_context *context = nfs_init_context();
    return (jlong)context;
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_destroy(JNIEnv *env, jclass clazz, jlong context) {
    nfs_destroy_context((struct nfs_context *)context);
}


JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setTcpSynCnt(JNIEnv *env, jclass clazz, jlong contextPtr, jint v) {
    nfs_set_tcp_syncnt((struct nfs_context *)contextPtr, v);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setUid(JNIEnv *env, jclass clazz, jlong contextPtr, jint uid) {
    nfs_set_uid((struct nfs_context *)contextPtr, uid);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setGid(JNIEnv *env, jclass clazz, jlong contextPtr, jint gid) {
    nfs_set_gid((struct nfs_context *)contextPtr, gid);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setPageCache(JNIEnv *env, jclass clazz, jlong contextPtr, jint v) {
    nfs_set_pagecache((struct nfs_context *)contextPtr, v);
}

JNIEXPORT void 
JNICALL Java_io_kuun_jnfs_NativeNfsContext_setPageCacheTtl (JNIEnv *env, jclass clazz, jlong contextPtr, jint v) {
    nfs_set_pagecache_ttl((struct nfs_context *)contextPtr, v);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setReadAhead(JNIEnv *env, jclass clazz, jlong contextPtr, jint v) {
    nfs_set_readahead((struct nfs_context *)contextPtr, v);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setDebug(JNIEnv *env, jclass clazz, jlong contextPtr, jint level) {
    nfs_set_debug((struct nfs_context *)contextPtr, level);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setDirCache(JNIEnv *env, jclass clazz, jlong contextPtr, jint enabled) {
    nfs_set_dircache((struct nfs_context *)contextPtr, enabled);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setAutoReconnect(JNIEnv * env, jclass clazz, jlong contextPtr, jint numRetries) {
    nfs_set_autoreconnect((struct nfs_context *)contextPtr, numRetries);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setVersion(JNIEnv *env, jclass clazz, jlong contextPtr, jint version) {
    nfs_set_version((struct nfs_context *)contextPtr, version);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setTimeout(JNIEnv *env, jclass clazz, jlong contextPtr, jint timeout) {
    nfs_set_timeout((struct nfs_context *)contextPtr, timeout);
}


JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_setClientName(JNIEnv *env, jclass clazz, jlong contextPtr, jstring name) {
    const char *cname = (*env)->GetStringUTFChars(env, name, NULL);
    nfs4_set_client_name((struct nfs_context *)contextPtr, cname);
    (*env)->ReleaseStringUTFChars(env, name, cname);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_mount(JNIEnv *env, jclass clazz, jlong contextPtr, jstring server, jstring export) {
    const char *cserver = (*env)->GetStringUTFChars(env, server, NULL);
    const char *cexport = (*env)->GetStringUTFChars(env, export, NULL);
    int ret;

    ret = nfs_mount((struct nfs_context *)contextPtr, cserver, cexport);
    (*env)->ReleaseStringUTFChars(env, server, cserver);
    (*env)->ReleaseStringUTFChars(env, export, cexport);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_umount(JNIEnv *env, jclass clazz, jlong contextPtr) {
    return nfs_umount((struct nfs_context *)contextPtr);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_stat(
        JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jobject fileStat) {
    const char *cpath = (*env)->GetStringUTFChars(env, path, NULL);
    struct nfs_stat_64 stat = {0};
    int ret;

    ret = nfs_stat64((struct nfs_context *)contextPtr, cpath, &stat);
    (*env)->ReleaseStringUTFChars(env, path, cpath);

    if (ret != 0) {
        goto RET;
    }

    convert_nfs_stat64(env, &stat, fileStat);
RET:
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_fstat(
        JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr , jobject fileStat) {
    struct nfs_stat_64 stat = {0};
    int ret;

    ret = nfs_fstat64((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, &stat);

    if (ret != 0) {
        goto RET;
    }

    convert_nfs_stat64(env, &stat, fileStat);
RET:
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_open__JLjava_lang_String_2ILio_kuun_jnfs_NfsFile_2(
        JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint flags, jobject file) {
    int ret;
    struct nfsfh *fh = NULL;
    const char *cpath;
    jclass nfsFileClass;
    jfieldID fieldId;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_open((struct nfs_context *)contextPtr, cpath, flags, &fh);
    (*env)->ReleaseStringUTFChars(env, path, cpath);

    if (ret != 0) {
        goto RET;
    }
    
    nfsFileClass = (*env)->GetObjectClass(env, file);
    fieldId = (*env)->GetFieldID(env, nfsFileClass, "filePtr", "J");
    (*env)->SetLongField(env, file, fieldId, (jlong)fh);

RET:
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_open__JLjava_lang_String_2IILio_kuun_jnfs_NfsFile_2(
        JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint flags, jint mode, jobject file) {
    int ret;
    struct nfsfh *fh = NULL;
    const char *cpath;
    jclass nfsFileClass;
    jfieldID fieldId;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_open2((struct nfs_context *)contextPtr, cpath, flags, mode, &fh);
    (*env)->ReleaseStringUTFChars(env, path, cpath);

    if (ret != 0) {
        goto RET;
    }
    
    nfsFileClass = (*env)->GetObjectClass(env, file);
    fieldId = (*env)->GetFieldID(env, nfsFileClass, "filePtr", "J");
    (*env)->SetLongField(env, file, fieldId, (jlong)fh);

RET:
    return ret;    
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_close(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr) {
    return nfs_close((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_pread(JNIEnv *env, jclass clazz, jlong contextPtr, 
        jlong filePtr, jlong offset, jlong count, jbyteArray jbuf) {
    int ret;
    int jbufLen;
    int readLen;
    jbyte *buf;

    jbufLen = (*env)->GetArrayLength(env, jbuf);
    readLen = count < jbufLen ? count : jbufLen;
    buf = (*env)->GetByteArrayElements(env, jbuf, NULL);
    
    ret = nfs_pread((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, offset, readLen, buf);
    (*env)->ReleaseByteArrayElements(env, jbuf, buf, 0);

    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_read(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jlong count, jbyteArray jbuf) {
    int ret;
    int jbufLen;
    int readLen;
    jbyte *buf;

    jbufLen = (*env)->GetArrayLength(env, jbuf);
    readLen = count < jbufLen ? count : jbufLen;
    buf = (*env)->GetByteArrayElements(env, jbuf, NULL);
    
    ret = nfs_read((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, readLen, buf);
    (*env)->ReleaseByteArrayElements(env, jbuf, buf, 0);

    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_pwrite(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jlong offset, jlong count, jbyteArray jbuf) {
    int ret;
    int jbufLen;
    int readLen;
    jbyte *buf;

    jbufLen = (*env)->GetArrayLength(env, jbuf);
    readLen = count < jbufLen ? count : jbufLen;
    buf = (*env)->GetByteArrayElements(env, jbuf, NULL);
    
    ret = nfs_pwrite((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, offset, readLen, buf);
    (*env)->ReleaseByteArrayElements(env, jbuf, buf, 0);

    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_write(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jlong count, jbyteArray jbuf) {
        int ret;
    int jbufLen;
    int readLen;
    jbyte *buf;

    jbufLen = (*env)->GetArrayLength(env, jbuf);
    readLen = count < jbufLen ? count : jbufLen;
    buf = (*env)->GetByteArrayElements(env, jbuf, NULL);
    
    ret = nfs_write((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, readLen, buf);
    (*env)->ReleaseByteArrayElements(env, jbuf, buf, 0);

    return ret;
}

JNIEXPORT jlong JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_lseek(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jlong offset, jint whence) {
    uint64_t curOffset = 0;
    int ret;

    ret = nfs_lseek((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, offset, whence, &curOffset);
    if (ret < 0) {
        return ret;
    }
    return curOffset;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_fsync(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr) {
    return nfs_fsync((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_truncate(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jlong length) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_truncate((struct nfs_context *)contextPtr, cpath, length);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_ftruncate(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jlong length) {
     return nfs_ftruncate((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, length);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_mkdir(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_mkdir((struct nfs_context *)contextPtr, cpath);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_mkdir2(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint mode) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_mkdir2((struct nfs_context *)contextPtr, cpath, mode);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_rmdir(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_rmdir((struct nfs_context *)contextPtr, cpath);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_creat(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint mode, jobject file) {
    int ret;
    const char *cpath;
    jclass nfsFileClazz;
    jfieldID fieldId;
    struct nfsfh *fh = NULL;
    
    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_creat((struct nfs_context *)contextPtr, cpath, mode, &fh);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    if (ret != 0) {
        goto RET;
    }

    nfsFileClazz = (*env)->GetObjectClass(env, file);
    fieldId = (*env)->GetFieldID(env, nfsFileClazz, "filePtr", "J");
    (*env)->SetLongField(env, file, fieldId, (jlong)fh);

RET:
    return ret;
}


JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_create(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint flags, jint mode, jobject file) {
    int ret;
    const char *cpath;
    jclass nfsFileClazz;
    jfieldID fieldId;
    struct nfsfh *fh = NULL;
    
    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_create((struct nfs_context *)contextPtr, cpath, flags, mode, &fh);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    if (ret != 0) {
        goto RET;
    }

    nfsFileClazz = (*env)->GetObjectClass(env, file);
    fieldId = (*env)->GetFieldID(env, nfsFileClazz, "filePtr", "J");
    (*env)->SetLongField(env, file, fieldId, (jlong)fh);

RET:
    return ret; 
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_unlink(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_unlink((struct nfs_context *)contextPtr, cpath);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_openDir(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jobject dir) {
    int ret;
    const char *cpath;
    jclass nfsDirClazz;
    jfieldID fieldId;
    struct nfsdir *nfsdir;
    
    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_opendir((struct nfs_context *)contextPtr, cpath, &nfsdir);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    if (ret != 0) {
        goto RET;
    }

    nfsDirClazz = (*env)->GetObjectClass(env, dir);
    fieldId = (*env)->GetFieldID(env, dir, "dirPtr", "J");
    (*env)->SetLongField(env, dir, fieldId, (jlong)nfsdir);

RET:
    return ret; 
}

JNIEXPORT jobject JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_readDir(JNIEnv *env, jclass clazz, jlong contextPtr, jlong dirPtr) {
    struct nfsdirent *dirent = NULL;
    jclass nfsDirEntClazz;
    jmethodID methodId;
    jobject jdirent;
    jfieldID fieldId;

    dirent = nfs_readdir((struct nfs_context *)contextPtr, (struct nfsdir *)dirPtr);
    
    nfsDirEntClazz = (*env)->FindClass(env, "io/kuun/jnfs/NfsDirEnt");
    methodId = (*env)->GetMethodID(env, nfsDirEntClazz, "<init>", "()V");
    jdirent = (*env)->NewObject(env, nfsDirEntClazz, methodId);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "name", "Ljava/lang/String;");
    jstring name = (*env)->NewStringUTF(env, dirent->name);
    (*env)->SetObjectField(env, jdirent, fieldId, name);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "type", "I");
    (*env)->SetIntField(env, jdirent, fieldId, dirent->type);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "mode", "I");
    (*env)->SetIntField(env, jdirent, fieldId, dirent->mode);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "uid", "I");
    (*env)->SetIntField(env, jdirent, fieldId, dirent->uid);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "gid", "I");
    (*env)->SetIntField(env, jdirent, fieldId, dirent->gid);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "size", "J");
    (*env)->SetLongField(env, jdirent, fieldId, dirent->size);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "atime", "J");
    (*env)->SetLongField(env, jdirent, fieldId, dirent->atime.tv_sec);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "mtime", "J");
    (*env)->SetLongField(env, jdirent, fieldId, dirent->mtime.tv_sec);

    fieldId = (*env)->GetFieldID(env, nfsDirEntClazz, "ctime", "J");
    (*env)->SetLongField(env, jdirent, fieldId, dirent->ctime.tv_sec);

    return jdirent;
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_seekDir(JNIEnv *env, jclass clazz, jlong contextPtr, jlong dirPtr, jlong loc) {
    nfs_seekdir((struct nfs_context *)contextPtr, (struct nfsdir *)dirPtr, loc);
}

JNIEXPORT jlong JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_tellDir(JNIEnv *env, jclass clazz, jlong contextPtr, jlong dirPtr) {
    return nfs_telldir((struct nfs_context *)contextPtr, (struct nfsdir *)dirPtr);
}

JNIEXPORT void JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_rewindDir(JNIEnv *env, jclass clazz, jlong contextPtr, jlong dirPtr) {
    nfs_rewinddir((struct nfs_context *)contextPtr, (struct nfsdir *)dirPtr);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_closeDir(JNIEnv *env, jclass clazz, jlong contextPtr, jlong dirPtr) {
    return nfs_close((struct nfs_context *)contextPtr, (struct nfsdir *)dirPtr);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_chdir(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_chdir((struct nfs_context *)contextPtr, cpath);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jstring JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_getcwd(JNIEnv *env, jclass clazz, jlong contextPtr) {
    char *cwd = NULL;

    nfs_getcwd((struct nfs_context *)contextPtr, &cwd);
    return (*env)->NewStringUTF(env, cwd);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_chmod(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint mode) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_chmod((struct nfs_context *)contextPtr, cpath, mode);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_fchmod(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jint mode) {
    return nfs_fchmod((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, mode);
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_chown(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jint uid, jint gid) {
    int ret;
    const char *cpath;

    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_chown((struct nfs_context *)contextPtr, cpath, uid, gid);
    (*env)->ReleaseStringUTFChars(env, path, cpath);
    
    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_fchown(JNIEnv *env, jclass clazz, jlong contextPtr, jlong filePtr, jint uid, jint gid) {
    return nfs_fchown((struct nfs_context *)contextPtr, (struct nfsfh *)filePtr, uid, gid);
}

JNIEXPORT jint JNICALL Java_io_kuun_jnfs_NativeNfsContext_utime(JNIEnv *env, jclass clazz, jlong contextPtr, jstring path, jlong seconds, jlong useconds) {
    int ret;
    struct timeval times = {0};
    const char *cpath;

    times.tv_sec = seconds;
    times.tv_usec = useconds;
    cpath = (*env)->GetStringUTFChars(env, path, NULL);
    ret = nfs_utime((struct nfs_context *)contextPtr, cpath, &times);
    (*env)->ReleaseStringUTFChars(env, path, cpath);

    return ret;
}

JNIEXPORT jint JNICALL 
Java_io_kuun_jnfs_NativeNfsContext_rename(JNIEnv *env, jclass clazz, jlong contextPtr, jstring oldpath, jstring newpath) {
    int ret;
    const char *coldpath;
    const char *cnewpath;

    coldpath = (*env)->GetStringUTFChars(env, oldpath, NULL);
    cnewpath = (*env)->GetStringUTFChars(env, newpath, NULL);
    ret = nfs_rename((struct nfs_context *)contextPtr, coldpath, cnewpath);
    (*env)->ReleaseStringUTFChars(env, oldpath, cnewpath);
    (*env)->ReleaseStringUTFChars(env, newpath, cnewpath);
    
    return ret;
}

JNIEXPORT jstring JNICALL
Java_io_kuun_jnfs_NativeNfsContext_getError(JNIEnv *env, jclass clazz, jlong contextPtr) {
    const char *msg = nfs_get_error((struct nfs_context *)contextPtr);
    return (*env)->NewStringUTF(env, msg);
}

static void
convert_nfs_stat64(JNIEnv *env, const struct nfs_stat_64 *stat, jobject fileStat) {
    jclass nfsFileStatClazz;
    jfieldID fieldId;

    nfsFileStatClazz = (*env)->GetObjectClass(env, fileStat);
    
    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "uid", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_uid);

    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "gid", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_gid);

    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "mode", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_mode);

    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "size", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_size);

    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "atime", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_atime);

    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "mtime", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_mtime);

    fieldId = (*env)->GetFieldID(env, nfsFileStatClazz, "ctime", "J");
    (*env)->SetLongField(env, fileStat, fieldId, stat->nfs_ctime);
}