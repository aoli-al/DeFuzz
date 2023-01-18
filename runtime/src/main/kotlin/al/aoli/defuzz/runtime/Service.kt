package al.aoli.defuzz.runtime

import io.grpc.ServerBuilder
import mu.KotlinLogging

private val logger = KotlinLogging.logger {}
class Service(val port: Int) {
    val server = ServerBuilder.forPort(port).addService(BreakPointAnalyzerService()).build()

    fun start() {
        server.start()

        Runtime.getRuntime().addShutdownHook(
            Thread {
                stop()
            }
        )
    }

    fun stop() {
        server.shutdown()
    }

    fun blockUntilShutdown() {
        server.awaitTermination()
    }


    internal class BreakPointAnalyzerService(): BreakPointAnalyzerGrpcKt.BreakPointAnalyzerCoroutineImplBase() {
        override suspend fun getBreakPoints(request: BreakPointRequest): BreakPointResponse {
            logger.info { "Breakpoint Request received." }
            val classReader = BreakPointClassReader(request.classData.toByteArray())
            val classVisitor = BreakPointClassVisitor(classReader);
            classReader.accept(classVisitor, 0)
            logger.info { "Breakpoints identified ---- ${classVisitor.className}: ${classVisitor.breakPoints.size}" }
            return BreakPointResponse.newBuilder()
                .addAllBreakpoints(classVisitor.breakPoints)
                .build()
        }
    }

}

fun main() {
    val port = 8980
    val server = Service(port)
    server.start()
    server.blockUntilShutdown()
}
