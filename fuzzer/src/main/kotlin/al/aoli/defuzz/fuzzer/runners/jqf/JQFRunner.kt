package al.aoli.defuzz.fuzzer.runners.jqf

import al.aoli.defuzz.fuzzer.cli.Options
import edu.berkeley.cs.jqf.fuzz.junit.GuidedFuzzing
import org.junit.runner.JUnitCore
import org.junit.runner.Request
import org.junit.runner.Result
import kotlin.io.path.deleteIfExists
import kotlin.io.path.notExists
import kotlin.system.exitProcess

object JQFRunner {
    fun run(options: Options) {
        options.outputDirectory.deleteRecursively()

        try {
            val guidance = DeFuzzGuidance("${options.testClass}#${options.testMethod}", null,
                options.outputDirectory, emptyArray())
            GuidedFuzzing.run(options.testClass, options.testMethod, guidance, System.out)
        } catch (e: Exception) {
            e.printStackTrace()
            exitProcess(12)
        }
    }

}