package io.kuun.jnfs;

import io.kuun.jnfs.exceptions.NfsException;
import io.kuun.jnfs.exceptions.NfsInitException;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.parallel.Execution;
import org.junit.jupiter.api.parallel.ExecutionMode;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Calendar;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertEquals;

@Execution(ExecutionMode.SAME_THREAD)
public class FileOperationTest {
    private NfsContext context;
    private static String server;
    private static String export;

    private String testFileName = "/test";
    private int testFileMode = FileMode.S_IWUSR | FileMode.S_IRUSR | FileMode.S_IXUSR;
    private long testFileModTime;
    private static Logger log = LoggerFactory.getLogger(FileOperationTest.class);

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
    void setup() throws NfsInitException, NfsException {
        context = new NfsContext();
        context.init();
        context.setUid(65534);
        context.setGid(65534);

        Calendar calendar = Calendar.getInstance();
        calendar.set(2020, Calendar.JANUARY, 1, 0, 0, 0);
        testFileModTime =  calendar.getTimeInMillis();
        log.debug("setup, test: {}", this);
    }

    @AfterEach
    void cleanup() throws NfsException {
        //context.unlink(testFileName);
        context.umount();
        context.close();
        log.debug("cleanup, test: {}", this);
    }


    @ParameterizedTest
    @ValueSource(ints = {3})
    void stat(int version) throws NfsException {
        context.setVersion(version);
        context.mount(server, export);
        createTestFile();
        NfsFileStat stat = context.stat(testFileName);
        log.error("stat: {}", stat);
        assertEquals(testFileMode, stat.getFileMode());
    }

    void createTestFile() throws NfsException {
        context.create(testFileName, testFileMode);
        context.utime(testFileName, testFileModTime, 0);
    }
}
