package al.aoli.defuzz.fuzzer.runtime

import al.aoli.defuzz.runtime.BreakPoint
import org.objectweb.asm.ClassVisitor
import org.objectweb.asm.MethodVisitor
import org.objectweb.asm.Opcodes

class BreakPointClassVisitor(val classReader: BreakPointClassReader): ClassVisitor(Opcodes.ASM9) {
    var className = ""
    val breakPoints = mutableListOf<BreakPoint>()
    override fun visit(
        version: Int,
        access: Int,
        name: String,
        signature: String?,
        superName: String?,
        interfaces: Array<out String>?
    ) {
        super.visit(version, access, name, signature, superName, interfaces)
        className = name
    }
    override fun visitMethod(
        access: Int,
        name: String,
        descriptor: String,
        signature: String?,
        exceptions: Array<out String>?
    ): MethodVisitor {
        val isStatic = (access and Opcodes.ACC_STATIC) != 0
        return BreakPointMethodVisitor(classReader, className, name, descriptor, isStatic, breakPoints)
    }
}