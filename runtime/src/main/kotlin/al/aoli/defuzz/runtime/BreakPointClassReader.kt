package al.aoli.defuzz.fuzzer.runtime

import org.objectweb.asm.ClassReader

class BreakPointClassReader: ClassReader {
    constructor(byteCode: ByteArray) : super(byteCode)
    constructor(className: String) : super(className)

    var lastReadConstIndex = -1
    var lastUnsignedShortOffset = -1

    override fun readConst(constantPoolEntryIndex: Int, charBuffer: CharArray?): Any {
        lastReadConstIndex = constantPoolEntryIndex
        return super.readConst(constantPoolEntryIndex, charBuffer)
    }

    override fun readUnsignedShort(offset: Int): Int {
        lastUnsignedShortOffset = offset
        return super.readUnsignedShort(offset)
    }
}