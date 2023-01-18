package al.aoli.defuzz.fuzzer.cli

import al.aoli.defuzz.fuzzer.runners.jqf.JQFRunner
import com.github.ajalt.clikt.core.CliktCommand
import com.github.ajalt.clikt.parameters.arguments.argument
import com.github.ajalt.clikt.parameters.types.file
import com.github.ajalt.clikt.parameters.types.path
import java.io.File
import java.nio.file.Path

class Options: CliktCommand() {
    val testClass: String by argument(help="Test class name")
    val testMethod: String by argument(help="Test method name")
    val outputDirectory: File by argument(help = "Output location").file(canBeFile = false)

    override fun run() {
        fuzzerStarted()
        JQFRunner.run(this)
    }

    external fun fuzzerStarted()
}

fun main(args: Array<String>) = Options().main(args)