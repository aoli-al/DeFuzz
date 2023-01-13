package al.aoli.defuzz.fuzzer.runners.jqf

import edu.berkeley.cs.jqf.fuzz.ei.ZestGuidance
import java.io.File
import java.time.Duration

class DeFuzzGuidance(testName: String, duration: Duration, outputDirectory: File, seedFiles: Array<File>): ZestGuidance(testName, duration, outputDirectory, seedFiles) {

}