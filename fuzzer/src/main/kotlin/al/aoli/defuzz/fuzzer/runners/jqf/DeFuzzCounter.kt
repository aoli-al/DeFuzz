package al.aoli.defuzz.fuzzer.runners.jqf

import edu.berkeley.cs.jqf.fuzz.util.Counter

class DeFuzzCounter: Counter(1) {
    val index = initNative()

    override fun clear() {
        clearNative(index)
    }

    override fun getNonZeroSize(): Int {
        return getNonZeroSizeNative(index)
    }

    fun updateBits(that: DeFuzzCounter): Int {
        return updateBitsNative(index, that.index)
    }

    external fun initNative(): Int

    external fun clearNative(index: Int)
    external fun getNonZeroSizeNative(index: Int): Int
    external fun updateBitsNative(thisIndex: Int, thatIndex: Int): Int
}