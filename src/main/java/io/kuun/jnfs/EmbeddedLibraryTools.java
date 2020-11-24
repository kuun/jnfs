package io.kuun.jnfs;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class EmbeddedLibraryTools {
    public static final boolean LOADED_EMBEDDED_LIBRARY;
    private Logger log = LoggerFactory.getLogger(EmbeddedLibraryTools.class);

    static {
        LOADED_EMBEDDED_LIBRARY = loadEmbeddedLibrary();
    }

    private EmbeddedLibraryTools() {
    }

    public static String getCurrentPlatformIdentifier() {
        String osName = System.getProperty("os.name");
        if (osName.toLowerCase().contains("windows")) {
            osName = "Windows";
        } else if (osName.toLowerCase().contains("mac os x")) {
            osName = "Mac OS X";
        } else {
            osName = osName.replaceAll("\\s+", "_");
        }
        return System.getProperty("os.arch") + "/" + osName;
    }

    private static boolean loadEmbeddedLibrary() {

        boolean usingEmbedded = false;

        // attempt to locate embedded native library within JAR at following location:
        // /NATIVE/${os.arch}/${os.name}/libjnfs.[so|dylib|dll]
        String[] allowedExtensions = new String[]{"so", "dylib", "dll"};
        String[] libs = new String[]{"nfs", "jnfs"};
        StringBuilder url = new StringBuilder();
        url.append("/NATIVE/");
        url.append(getCurrentPlatformIdentifier()).append("/");
        for (String lib : libs) {
            URL nativeLibraryUrl = null;
            // loop through extensions, stopping after finding first one
            for (String ext : allowedExtensions) {
                nativeLibraryUrl = NfsContext.class.getResource(url.toString() + lib + "." + ext);
                if (nativeLibraryUrl != null)
                    break;
            }

            if (nativeLibraryUrl != null) {
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

                    usingEmbedded = true;

                } catch (IOException x) {
                    // mission failed, do nothing
                }

            } // nativeLibraryUrl exists
        }
        return usingEmbedded;
    }
}
