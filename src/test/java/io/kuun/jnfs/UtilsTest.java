package io.kuun.jnfs;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import static org.junit.jupiter.api.Assertions.*;

class UtilsTest {

    @Test
    void getLibSuffix() {
        String osName = System.getProperty("os.name").toLowerCase();
        String suffix = Utils.getLibSuffix();
        if (osName.contains("windows")) {
            assertEquals("dll", suffix);
        } else if (osName.contains("mac")) {
            assertEquals("dylib", suffix);
        } else {
            assertEquals("so", suffix);
        }
    }

    @ParameterizedTest
    @ValueSource(strings = {"libnfs", "libjnfs"})
    void loadLibrary(String lib) {
        assertDoesNotThrow(() -> {
            Utils.loadLibrary(lib);
        });
    }

    @Test
    void getCurrentPlatformIdentifier() {
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.contains("windows")) {
            // TODO:
        } else if (osName.contains("mac")) {
            // TODO
        } else {
            assertEquals("Linux/amd64", Utils.getCurrentPlatformIdentifier());
        }
    }
}