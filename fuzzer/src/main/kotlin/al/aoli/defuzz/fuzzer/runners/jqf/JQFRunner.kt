package al.aoli.defuzz.fuzzer.runners.jqf

import al.aoli.defuzz.fuzzer.cli.Options
import org.junit.runner.JUnitCore
import org.junit.runner.Request
import org.junit.runner.Result

class JQFRunner(val options: Options) {
    fun run(): Result? {
        val clazz = try {
            Class.forName(options.testClass)
        } catch (e: ClassNotFoundException) {
            return null
        }
        val request = Request.method(clazz, options.testMethod)
        val junit = JUnitCore()
        return junit.run(request.runner)
    }

}