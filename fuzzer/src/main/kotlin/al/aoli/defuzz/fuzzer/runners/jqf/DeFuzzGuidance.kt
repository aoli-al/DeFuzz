package al.aoli.defuzz.fuzzer.runners.jqf

import al.aoli.defuzz.fuzzer.runtime.DeFuzzRuntime
import edu.berkeley.cs.jqf.fuzz.ei.ZestGuidance
import edu.berkeley.cs.jqf.fuzz.guidance.Result
import mu.KotlinLogging
import java.io.File
import java.time.Duration

private val logger = KotlinLogging.logger {}
class DeFuzzGuidance(testName: String, duration: Duration?, outputDirectory: File, seedFiles: Array<File>):
    ZestGuidance(testName, duration, outputDirectory, seedFiles) {

    init {
        val cov = DeFuzzCoverage()
        runCoverage = cov
        totalCoverage = DeFuzzCoverage()
        validCoverage = DeFuzzCoverage()

        DeFuzzRuntime.registerCounter(cov.counter.index)
    }

    override fun handleResult(result: Result?, error: Throwable?) {
        super.handleResult(result, error)
    }

}