package al.aoli.defuzz.fuzzer.runtime

import al.aoli.defuzz.runtime.BreakPoint
import al.aoli.defuzz.runtime.BreakPointResponse
import org.objectweb.asm.ConstantDynamic
import org.objectweb.asm.Handle
import org.objectweb.asm.Label
import org.objectweb.asm.MethodVisitor
import org.objectweb.asm.Opcodes
import org.objectweb.asm.Opcodes.ASM9

class BreakPointMethodVisitor(val classReader: BreakPointClassReader, val className: String, val name: String,
                              val descriptor: String, val isStatic: Boolean, val breakPoints: MutableList<BreakPoint>):
    MethodVisitor(ASM9) {
    var byteCodeOffset = 0
    var labelCount = 0

    override fun visitInsn(opcode: Int) {
        super.visitInsn(opcode)
        byteCodeOffset += 1
    }

    override fun visitVarInsn(opcode: Int, varIndex: Int) {
        super.visitVarInsn(opcode, varIndex)
        byteCodeOffset +=
            if (varIndex < 4 && opcode != Opcodes.RET) {
                1
            } else if (varIndex > 256) {
                4
            } else {
                2
            }
    }

    override fun visitJumpInsn(opcode: Int, label: Label?) {
        super.visitJumpInsn(opcode, label)
        byteCodeOffset += 3
    }

    override fun visitTableSwitchInsn(min: Int, max: Int, dflt: Label, vararg labels: Label) {
        super.visitTableSwitchInsn(min, max, dflt, *labels)
        byteCodeOffset += 4 - (byteCodeOffset and 3)
        byteCodeOffset += 12 + 4 * labels.size
    }

    override fun visitLookupSwitchInsn(dflt: Label, keys: IntArray, labels: Array<out Label>) {
        super.visitLookupSwitchInsn(dflt, keys, labels)
        byteCodeOffset += 4 - (byteCodeOffset and 3)
        byteCodeOffset += 8 + 8 * labels.size
    }

    override fun visitFieldInsn(opcode: Int, owner: String, name: String, descriptor: String) {
        super.visitFieldInsn(opcode, owner, name, descriptor)
        byteCodeOffset += 3
    }

    override fun visitMethodInsn(
        opcodeAndSource: Int,
        owner: String?,
        name: String?,
        descriptor: String?,
        isInterface: Boolean
    ) {
        super.visitMethodInsn(opcodeAndSource, owner, name, descriptor, isInterface)
        val opcode = opcodeAndSource and Opcodes.SOURCE_MASK.inv()
        byteCodeOffset +=
            if (opcode == Opcodes.INVOKEINTERFACE) {
                5
            } else {
                3
            }
    }

    override fun visitInvokeDynamicInsn(
        name: String?,
        descriptor: String?,
        bootstrapMethodHandle: Handle?,
        vararg bootstrapMethodArguments: Any?
    ) {
        super.visitInvokeDynamicInsn(name, descriptor, bootstrapMethodHandle, *bootstrapMethodArguments)
        byteCodeOffset += 5
    }

    override fun visitTypeInsn(opcode: Int, type: String?) {
        super.visitTypeInsn(opcode, type)
        byteCodeOffset += 3
    }

    override fun visitIincInsn(varIndex: Int, increment: Int) {
        super.visitIincInsn(varIndex, increment)
        byteCodeOffset +=
            if (varIndex > 255 || increment > 127 || increment < -128) {
                6
            } else {
                3
            }
    }

    override fun visitMultiANewArrayInsn(descriptor: String?, numDimensions: Int) {
        super.visitMultiANewArrayInsn(descriptor, numDimensions)
        byteCodeOffset += 4
    }

    override fun visitIntInsn(opcode: Int, operand: Int) {
        super.visitIntInsn(opcode, operand)
        byteCodeOffset +=
            when (opcode) {
                Opcodes.BIPUSH,
                Opcodes.NEWARRAY -> 2
                else -> 3
            }
    }

    override fun visitLdcInsn(value: Any) {
        super.visitLdcInsn(value)
        byteCodeOffset +=
            if (value is Long ||
                value is Double ||
                value is ConstantDynamic && value.size == 2 ||
                classReader.lastReadConstIndex > 255
            ) {
                3
            } else {
                2
            }
    }

    override fun visitLabel(label: Label) {
        super.visitLabel(label)
        breakPoints.add(BreakPoint.newBuilder()
            .setClassName(className)
            .setMethodName(name)
            .setMethodDescriptor(descriptor)
            .setLocation(byteCodeOffset)
            .setIsStatic(isStatic)
            .build()
        )
    }
}