package al.aoli.defuzz.fuzzer.cli

import com.github.ajalt.clikt.core.CliktCommand
import com.github.ajalt.clikt.parameters.arguments.argument
import com.github.ajalt.clikt.parameters.options.option
import com.github.ajalt.clikt.parameters.options.prompt

class Options: CliktCommand() {
    val testClass: String by argument(help="Test class name")
    val testMethod: String by argument(help="Test method name")

    override fun run() {

    }
}

fun main(args: Array<String>) = Options().main(args)