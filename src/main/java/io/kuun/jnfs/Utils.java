package io.kuun.jnfs;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.net.URL;

class Utils {
    static String getCurrentPlatformIdentifier() {
        String osName = System.getProperty("os.name");
        if (osName.toLowerCase().contains("windows")) {
            osName = "Windows";
        } else if (osName.toLowerCase().contains("mac")) {
            osName = "Mac";
        } else {
            osName = osName.replaceAll("\\s+", "_");
        }
        return osName + "/" + System.getProperty("os.arch");
    }

    static void loadLibrary(String lib) {
        String url = "/io/kuun/jnfs/native/" + getCurrentPlatformIdentifier() + "/" + lib + "." + getLibSuffix();
        URL nativeLibraryUrl = NfsContext.class.getResource(url);
        if (nativeLibraryUrl == null) {
            throw new RuntimeException("can not load native library " + lib);
        }
        // native library found within JAR, extract and load
        try {

            final File libfile = File.createTempFile(lib, ".lib");
            libfile.deleteOnExit(); // just in case

            final InputStream in = nativeLibraryUrl.openStream();
            final OutputStream out = new BufferedOutputStream(new FileOutputStream(libfile));

            int len = 0;
            byte[] buffer = new byte[8192];
            while ((len = in.read(buffer)) > -1)
                out.write(buffer, 0, len);
            out.close();
            in.close();
            System.load(libfile.getAbsolutePath());

        } catch (IOException e) {
            throw new RuntimeException("can not load native library " + lib, e);
        }
    }

    static String getLibSuffix() {
        String platformIdentifier = getCurrentPlatformIdentifier();
        if (platformIdentifier.startsWith("Windows")) {
            return "dll";
        } else if (platformIdentifier.startsWith("Mac")) {
            return "dylib";
        } else {
            return "so";
        }
    }

    static void loadAllEmbeddedLibraries() throws RuntimeException {
        loadLibrary("libnfs");
        loadLibrary("libjnfs");
    }

    static String getStackTrace(Throwable e) {
        StringWriter sw = new StringWriter();
        PrintWriter writer = new PrintWriter(sw);
        try {
            e.printStackTrace(writer);
            writer.close();
            String stack = sw.toString();
            sw.close();
            return stack;
        } catch (Exception x) {
            x.printStackTrace();
        }
        return null;
    }
}
