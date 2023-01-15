package al.aoli.defuzz.fuzzer.runtime

object DeFuzzRuntime {

    @JvmStatic
    fun classLoad(byteArray: ByteArray) {
        val classReader = BreakPointClassReader(byteArray)
        classReader.accept(BreakPointClassVisitor(classReader), 0)
    }
}