package io.kuun.jnfs;

import io.kuun.jnfs.exceptions.NfsException;
import io.kuun.jnfs.exceptions.NfsInitException;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Calendar;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

class NfsContextTest {
    private NfsContext context;
    private static String server;
    private static String export;


    @BeforeAll
    static void init() throws IOException {
        URL url = NfsContextTest.class.getResource("/test.properties");
        InputStream inputStream = url.openStream();
        Properties properties = new Properties();
        properties.load(inputStream);

        server = properties.getProperty("server");
        export = properties.getProperty("export");

    }

    @BeforeEach
    void setup() throws NfsInitException {
        context = new NfsContext();
        context.init();
    }

    @AfterEach
    void cleanup() throws NfsException {
        context.close();
    }

    @Test
    void testSetOptions() {
        context.setClientName("test");
        context.setTimeout(10 * 1000);
        context.setAutoConnect(3);
        context.setDebug(1);
        context.setDirCache(true);
        context.setPageCache(10 * 1024 * 1024);
        context.setPageCacheTtl(10);
        context.setReadhead(10 * 1024 * 1024);
        context.setTcpSynCnt(3);
        context.setUid(1000);
        context.setGid(1000);
        context.setVersion(4);
    }

    @ParameterizedTest
    @ValueSource(ints = {3, 4})
    void mountNfs(int version) {
        assertDoesNotThrow(() -> {
            // context.setDebug(5);
            context.setVersion(version);
            context.mount(server, export);
        });
    }

    @ParameterizedTest
    @ValueSource(ints = {3, 4})
    void umountNfs(int version) {
        assertDoesNotThrow(() -> {
            context.setVersion(version);
            context.mount(server, export);
            context.umount();
        });
    }
}