package al.aoli.defuzz.fuzzer.runners.jqf

import edu.berkeley.cs.jqf.fuzz.util.Counter
import mu.KotlinLogging
import org.eclipse.collections.api.list.primitive.IntList
import org.eclipse.collections.impl.list.mutable.primitive.IntArrayList


private val logger = KotlinLogging.logger {}
class DeFuzzCounter: Counter(1) {
    val index = initNative()

    init {
        logger.info { "Counter initialized: $index" }
    }


    override fun clear() {
        clearNative(index)
    }

    override fun getNonZeroSize(): Int {
        return getNonZeroIndicesNative(index).size
    }

    override fun getNonZeroIndices(): IntList {
        return IntArrayList.newListWith(*getNonZeroIndicesNative(index))
    }

    fun updateBits(that: DeFuzzCounter): Int {
        return updateBitsNative(index, that.index)
    }

    fun computeNewCoverage(that: DeFuzzCounter): IntList {
        val result = computeNewCoverageNative(index, that.index)
        return IntArrayList.newListWith(*result)
    }

    companion object {
        @JvmStatic
        external fun computeNewCoverageNative(thisIndex: Int, thatIndex: Int): IntArray
        @JvmStatic
        external fun initNative(): Int
        @JvmStatic
        external fun clearNative(index: Int)
        @JvmStatic
        external fun getNonZeroIndicesNative(index: Int): IntArray
        @JvmStatic
        external fun updateBitsNative(thisIndex: Int, thatIndex: Int): Int
    }
}